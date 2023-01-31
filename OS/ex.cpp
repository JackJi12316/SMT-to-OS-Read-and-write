#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "windows.h"


int Wmutex = 1;//�����д���ź���
int readcount = 0;  //������Ŀ
void input();
void main();

struct process
{
	int ID; //�������
	char type; //�������(�ж��Ƕ��߻���д��)
	int starttime; //���̿�ʼʱ��
	int needtime; //���̶�д��Ҫ��ʱ��
	int runtime;  //�������ڴ��������е�ʱ��
	struct process* next;
};

process* h1 = NULL, * h2 = NULL, * h3 = NULL;

void choose()//ѡ��
{
	int a;
	process* p, * q;
	q = h1 = (process*)malloc(sizeof(process));
	FILE* fp;
	scanf_s("%d", &a);
	switch (a)
	{
	case 1:                                 //�ֶ�����
		int i, j;
		printf("\t\t���������:");
		fp = fopen("file.txt", "w+");
		scanf_s("%d", &i);
		fprintf(fp, "%d\n", i);
		for (j = 1;i > 0;i--, j++)
		{
			p = (process*)malloc(sizeof(process));
			q->next = p;
			printf("\t\t��%d������:\n", j);
			printf("\t\t�������\t����д\t\t��ʼʱ��\tִ��ʱ��\n\t\t");
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
	case 2:                               //�ļ�����
		if ((fp = fopen("file.txt", "r")) == NULL)
		{
			printf("�ļ���ʧ��!\n");
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
			printf("�ļ���ʧ��!\n");
			system("pause");
			system("cls");
			main();
		}
		printf("\t\t�������\t����д\t\t��ʼʱ��\tִ��ʱ��\n");
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
		printf("\t\t������Ŀ:");
		printf("\t%d\n", k);
		printf("\t\tд����Ŀ:");
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
		printf("\t\t��������д�������������:\n");
		system("pause");
		system("cls");
		main();
	}
}


void input()     //���뺯��
{
	printf("\t\t***********��дƽ�Ȳ���*********\n");
	printf("\t\t\t1.�����������Ϣ\n");
	printf("\t\t\t2.�ļ����������Ϣ\n");
	printf("\t\t\t3.�鿴������Ϣ\n");
	printf("\t\t\t4.�˳�\n");
	printf("\t\t************************************\n");
	printf("\t\t��ѡ��:");
	choose();
}


void ready(int i)   //�����������
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


void reader()  //��дƽ���µĶ�����Ϣ
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


void writer()   //��дƽ���µ�д����Ϣ
{
	if ((wait(Wmutex) == 1) && (h3 == NULL))
	{
		h3 = h2;
		h2 = h2->next;
		h3->next = NULL;
	}
	else Wmutex++;
}


void add(int i)   //��̬����
{
	process* p, * q;
	int a;
	p = h1;
	q = (process*)malloc(sizeof(process));
	printf("\t\t�������:");
	scanf("%d", &q->ID);

	printf("\t\t\t����д:");
	fflush(stdin);
	scanf("%c", &q->type);

	printf("\t\t��ʼʱ��:");
	scanf("%d", &a);
	q->starttime = a + i;

	printf("\t\tִ��ʱ��:");
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


void print(int i)    //�������
{
	process* p;
	p = h3;
	while (p != NULL)
	{
		p->runtime++;
		p = p->next;
	}

	printf("\n\t\tִ�� %d :\n", i);
	printf("\t\tִ�ж���: ");
	p = h3;
	if (p == NULL)
		printf("<��>");
	else
		while (p != NULL)
		{
			printf("%d ", p->ID);
			p = p->next;
		}
	printf("\n\t\t�ȴ�����: ");
	p = h2;
	if (p == NULL)
		printf("<��>");
	else
		while (p != NULL)
		{
			printf("%d", p->ID);
			p = p->next;
		}
	printf("\n");
}


void leave()   //�뿪ִ�ж���
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
		Sleep(1000);  //�����߳�ռ��CPUʱ��һ����
		fflush(stdin);//��ջ�����
		char ch = ' ';
		
		if (kbhit() == 1)//��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0 
		{
			ch = getch();
			if ((ch == 'S') || (ch == 's'))
			{
				printf("\t\t����ͣ�����������...\n����µĽ�������a\n");
				ch = getchar();
			}
		}

		if (ch == 'a')
			add(i);
	}

	printf("\n\n\t\tִ�����\n");
	system("pause");
	system("cls");
	Wmutex = 1;
	readcount = 0;
	main();
	while (1);
}
