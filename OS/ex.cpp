#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "windows.h"


int Wmutex = 1;//互斥读写的信号量
int readcount = 0;  //读者数目
void input();
void main();

struct process
{
	int ID; //进程序号
	char type; //进程类别(判断是读者还是写者)
	int starttime; //进程开始时间
	int needtime; //进程读写需要的时间
	int runtime;  //进程在内存中已运行的时间
	struct process* next;
};

process* h1 = NULL, * h2 = NULL, * h3 = NULL;

void choose()//选择
{
	int a;
	process* p, * q;
	q = h1 = (process*)malloc(sizeof(process));
	FILE* fp;
	scanf_s("%d", &a);
	switch (a)
	{
	case 1:                                 //手动输入
		int i, j;
		printf("\t\t输入进程数:");
		fp = fopen("file.txt", "w+");
		scanf_s("%d", &i);
		fprintf(fp, "%d\n", i);
		for (j = 1;i > 0;i--, j++)
		{
			p = (process*)malloc(sizeof(process));
			q->next = p;
			printf("\t\t第%d个进程:\n", j);
			printf("\t\t进程序号\t读或写\t\t开始时间\t执行时间\n\t\t");
			scanf("%d %c %d %d", &p->ID, &p->type, &p->starttime, &p->needtime);
			fprintf(fp, "%d %c %d %d\n", p->ID, p->type, p->starttime, p->needtime);
			printf("\n");
			p->runtime = 0;
			q = q->next;
			p->next = NULL;
		}
		fclose(fp);
		p = h1;
		h1 = h1->next;
		p->next = NULL;
		free(p);
		break;
	case 2:                               //文件读入
		if ((fp = fopen("file.txt", "r")) == NULL)
		{
			printf("文件打开失败!\n");
			system("pause");
			system("cls");
			main();
		}
		fscanf(fp, "%d", &i);
		for (j = 1;i > 0;i--, j++)
		{
			p = (process*)malloc(sizeof(process));
			q->next = p;
			fscanf(fp, "%d %c %d %d", &p->ID, &p->type, &p->starttime, &p->needtime);
			p->runtime = 0;
			q = q->next;
			p->next = NULL;
		}
		fclose(fp);
		p = h1;
		h1 = h1->next;
		p->next = NULL;
		free(p);
		break;
	case 3:
		int k;
		if ((fp = fopen("file.txt", "r")) == NULL)
		{
			printf("文件打开失败!\n");
			system("pause");
			system("cls");
			main();
		}
		printf("\t\t进程序号\t读或写\t\t开始时间\t执行时间\n");
		fscanf(fp, "%d", &i);
		k = 0;
		for (j = 0;i > 0;i--, j++)
		{
			p = (process*)malloc(sizeof(process));
			q->next = p;
			fscanf(fp, "%d %c %d %d", &p->ID, &p->type, &p->starttime, &p->needtime);
			if (p->type == 'r' || p->type == 'R')
			{
				k++;
			}
			printf("\t\t%d\t\t%c\t\t%d\t\t%d\n", p->ID, p->type, p->starttime, p->needtime);
			p->runtime = 0;
			q = q->next;
			p->next = NULL;
		}
		j = j - k;
		printf("\t\t读者数目:");
		printf("\t%d\n", k);
		printf("\t\t写者数目:");
		printf("\t%d\n", j);
		fclose(fp);
		p = h1;
		h1 = h1->next;
		p->next = NULL;
		free(p);
		system("pause");
		system("cls");
		main();
		break;
	case 4:
		exit(0);
	default:
		printf("\t\t您输入的有错误，请重新输入:\n");
		system("pause");
		system("cls");
		main();
	}
}


void input()     //输入函数
{
	printf("\t\t***********读写平等策略*********\n");
	printf("\t\t\t1.请输入进程信息\n");
	printf("\t\t\t2.文件载入进程信息\n");
	printf("\t\t\t3.查看进程信息\n");
	printf("\t\t\t4.退出\n");
	printf("\t\t************************************\n");
	printf("\t\t请选择:");
	choose();
}


void ready(int i)   //进入就绪队列
{
	process* p, * q, * j, * k;
	p = h1;
	q = h2;
	int t = 0;
	if (h2 == NULL)
	{
		q = h2 = (process*)malloc(sizeof(process));
		q->next = NULL;
		t = 1;
	}
	else
	{
		while (q->next != NULL)
		{
			q = q->next;
		}
	}

	j = (process*)malloc(sizeof(process));
	j->next = h1;
	while (h1->starttime == i)
	{
		q->next = h1;
		h1 = h1->next;
		q = q->next;
		q->next = NULL;
		j->next = h1;
		if (h1 == NULL)
			break;
	}
	p = h1;
	while (p != NULL)
	{
		if (p->starttime == i)
		{
			k = j;
			while (k->next != p)
			{
				k = k->next;
			}
			k->next = p->next;
			q->next = p;
			q = q->next;
			p = p->next;
			q->next = NULL;
		}
		else
		{
			p = p->next;
		}
	}
	h1 = j->next;
	j->next = NULL;
	free(j);
	if (t == 1)
	{
		p = h2;
		h2 = h2->next;
		p->next = NULL;
		free(p);
	}
}


int wait(int& a)
{
	a--;
	if (a < 0)
	{
		return 0;
	}
	return 1;
}


void signal(int& a)
{
	a++;
}


void reader()  //读写平等下的读者信息
{
	process* p;
	int t = 0;
	p = h3;
	if (h3 == NULL)
	{
		p = h3 = (process*)malloc(sizeof(process));
		p->next = NULL;
		t = 1;
	}
	else
	{
		while (p->next != NULL)
		{
			p = p->next;
		}
	}
	if (readcount > 0)
	{
		p->next = h2;
		h2 = h2->next;
		p = p->next;
		p->next = NULL;
		readcount++;
	}
	if ((readcount == 0) && (wait(Wmutex) == 1))
	{
		p->next = h2;
		h2 = h2->next;
		p = p->next;
		p->next = NULL;
		readcount++;
	}
	else Wmutex++;

	if (t == 1)
	{
		p = h3;
		h3 = h3->next;
		p->next = NULL;
		free(p);
	}
}


void writer()   //读写平等下的写者信息
{
	if ((wait(Wmutex) == 1) && (h3 == NULL))
	{
		h3 = h2;
		h2 = h2->next;
		h3->next = NULL;
	}
	else Wmutex++;
}


void add(int i)   //动态增加
{
	process* p, * q;
	int a;
	p = h1;
	q = (process*)malloc(sizeof(process));
	printf("\t\t进程序号:");
	scanf("%d", &q->ID);

	printf("\t\t\t读或写:");
	fflush(stdin);
	scanf("%c", &q->type);

	printf("\t\t开始时间:");
	scanf("%d", &a);
	q->starttime = a + i;

	printf("\t\t执行时间:");
	scanf("%d", &q->needtime);
	q->runtime = 0;
	q->next = NULL;
	if (h1 != NULL)
	{
		while (p->next != NULL)
			p = p->next;
		p->next = q;
	}
	else
		h1 = q;
}


void print(int i)    //输出函数
{
	process* p;
	p = h3;
	while (p != NULL)
	{
		p->runtime++;
		p = p->next;
	}

	printf("\n\t\t执行 %d :\n", i);
	printf("\t\t执行队列: ");
	p = h3;
	if (p == NULL)
		printf("<空>");
	else
		while (p != NULL)
		{
			printf("%d ", p->ID);
			p = p->next;
		}
	printf("\n\t\t等待队列: ");
	p = h2;
	if (p == NULL)
		printf("<空>");
	else
		while (p != NULL)
		{
			printf("%d", p->ID);
			p = p->next;
		}
	printf("\n");
}


void leave()   //离开执行队列
{
	process* p, * q;
	p = q = (process*)malloc(sizeof(process));
	p->next = NULL;
	while (h3 != NULL)
	{
		if (h3->needtime != h3->runtime)
		{
			p->next = h3;
			p = p->next;
		}
		else
		{
			if ((h3->type == 'r') || (h3->type == 'R'))
			{
				readcount--;
				if (readcount == 0)
					Wmutex = 1;
			}
			else
				signal(Wmutex);
		}
		h3 = h3->next;
		p->next = NULL;
	}
	h3 = q->next;
	q->next = NULL;
	free(q);
}


void main(){
	int i = 0;
	input();
	while ((h1 != NULL) || (h2 != NULL) || (h3 != NULL))
	{
		i++;
		if (h1 != NULL)
			ready(i);
		if (h2 != NULL)
			while (h2->type == 'r')
			{
				reader();
				if (h3 != NULL)
					if (h3->type == 'w')
						break;
				if (h2 == NULL)
					break;
			}
		if (h2 != NULL)
			if (h2->type == 'w')
				writer();
			

		print(i);
		leave();
		Sleep(1000);  //交出线程占用CPU时间一秒钟
		fflush(stdin);//清空缓冲区
		char ch = ' ';
		
		if (kbhit() == 1)//检查当前是否有键盘输入，若有则返回一个非0值，否则返回0 
		{
			ch = getch();
			if ((ch == 'S') || (ch == 's'))
			{
				printf("\t\t已暂停，任意键继续...\n添加新的进程输入a\n");
				ch = getchar();
			}
		}

		if (ch == 'a')
			add(i);
	}

	printf("\n\n\t\t执行完毕\n");
	system("pause");
	system("cls");
	Wmutex = 1;
	readcount = 0;
	main();
	while (1);
}
