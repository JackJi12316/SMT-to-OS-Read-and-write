#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <ostream>
#include <istream>
#include <thread>

using namespace std;


class WRer
{
public:
	WRer(int id, int runt, int t);
	int getID() {
		return ID;
	}
	int getRun() {
		return runtime;
	}
	int getitem() {
		return item;
	}
private:
	int ID;
	int runtime;
	int item;
};

struct process {
	WRer WR;
	process* next;
};



const int read = 0;
const int write = 1;


int sign = 1;
int signal_Reader = 20;

int rid = 0;//读者ID
int wid = 0;//写者ID



void waitRead();
void waitSign();
void creatWRer();
void Read(WRer er);
void inRead(WRer er);
void Write(WRer er);
//void dataWrite(WRer er);


process* head, * rwmove, * creat;


//等待读写信号量
void waitSign() {
	while (sign == 0);
}

//等待读者信号量
void waitRead() {
	while (signal_Reader == 0);
}

//监听函数
void Listing() {
	char ch = ' ';
	if (_kbhit())//检查当前是否有键盘输入，若有则返回一个非0值，否则返回0
	{
		ch = _getch();
		if ((ch == 'S') || (ch == 's'))
		{
			printf("\t\t已暂停，任意键继续...\n添加新的进程输入a\n");
			ch = _getch();
		}
		if (ch == 'a') {
			creatWRer();
		}
	}
}

WRer::WRer(int id, int runt, int t) {
	ID = id;
	runtime = runt;
	item = t;
}

//建立进程
void creatWRer() {
	int _item, _id;
	int _runtime;
	cout << "please input read->0 or write->1  :";
	cin >> _item;
	switch (_item) {
	case 0: {
		_item = read;
		_id = rid;
		rid++;
		break;
	}
	case 1: {
		_item = write;
		_id = wid;
		wid++;
		break;
	}
	default:
		printf("wrong input");
		cin >> _item;
	}
	cout << "运行时间:" << endl;
	scanf("%d", &_runtime);
	if (_runtime < 0) {
		cout << "wrong";
		return;
	}

	WRer er = WRer(_id, _runtime, _item);
	creat = (process*)malloc(sizeof(process));
	creat->WR = er;
	creat->next = NULL;
	rwmove->next = creat;
	rwmove = rwmove->next;

}

//打印信息
void show(WRer er, int time) {
	cout << "\tID\t\t需要时间\t\t类型\t\t已运行时间" << endl;
	if (er.getitem() == read) {
		cout << " \t RID:" << er.getID() << "  \t  " << er.getRun() << "  \t\t      读者  \t\t" << time
			<< endl;
		cout << endl;
	}
	else {
		cout << " \t WID:" << er.getID() << "  \t  " << er.getRun() << "  \t\t      写者  \t\t" << time
			<< endl;
		cout << endl;
	}
}

//读者操作判断函数
void Read(WRer er) {
	if (sign == 0)
		if (signal_Reader != 20)
			if (signal_Reader != 0)
				inRead(er);
			else {
				waitRead();
				inRead(er);
			}
		else {
			waitSign();
			inRead(er);
		}
	else {
		sign--;
		inRead(er);
	}
}

//读操作具体函数
void inRead(WRer er) {
	int i = 0;
	signal_Reader--;
	while (er.getRun() > i) {
		Listing();
		Sleep(1000);
		i++;
		show(er, i);
	}
	signal_Reader++;//运行结束，释放位置
	//dataWrite(er);
	head = head->next;
	if (signal_Reader == 20)
		sign++;

}

//写者运行
void Write(WRer er) {
	waitSign();
	sign--;

	int i = 0;
	while (er.getRun() > i) {
		Listing();
		Sleep(1000);
		i++;
		show(er, i);
	}
	//dataWrite(er);
	head = head->next;
	sign++;
}

//向txt中写信息
/*void dataWrite(WRer er) {
	ofstream out();
	out.open("../log.txt", ios::out | ios::app);
	if (!out.is_open())
	{
		cerr << "cannot open the file";
	}

	time_t nowTime;
	time(&nowTime); //获取当前时间
	out << "  进程测试时间  " << ctime(&nowTime);

	if (er.getitem() == read)
		out << " ID  " << "  运行时间  " << " 类型:读者\n";
	else
		out << " ID  " << "  运行时间  " << " 类型:写者\n";

	out << "  " << er.getID() << "    " << er.getRun() << "\n";

	out.close();
}
*/
//建立线程与运行
void run() {
	process* run;
	run = head->next;
	thread myThread[50];
	int i = 0;
	while (run != NULL) {

		if (run->WR.getitem() == read) {
			myThread[i] = thread(Read, run->WR);
			i++;
			run = run->next;
		}

		else {

			if (i != 0) {
				for (int j = 0; j < i; j++) 
					myThread[j].join();
				for (int j = 0;j < i;j++) 
					myThread[j].~thread();
				i = 0;
			}

			if (run->WR.getitem() == write) {
				Write(run->WR);
				run = run->next;
			}

		}

	}

	if (i != 0) {
		for (int j = 0; j < i; j++)
			myThread[j].join();
	}

}

//主函数
int menu() {
	system("cls");
	cout << "\t\t******读写平等系统******" << endl;
	cout << "\t1.批量输入进程" << endl;
	cout << "\t2.运行并展示进程运行情况" << endl;
	cout << "\t3.展示进程" << endl;
	cout << "\t4.退出" << endl;
	int _chose;
	cin >> _chose;
	switch (_chose) {
	case 1: {
		int num;
		system("cls");
		cout << "输入进程个数" << endl;
		cin >> num;
		for (int j = 0;j < num;j++) {
			cout << "第 " << j + 1 << " 个 进程信息" << endl;
			creatWRer();
		}
		menu();
		break;
	}
	case 2: {
		system("cls");
		run();
		cout << "\t\t\t按任意键继续" << endl;
		_getch();
		menu();
		break;
	}
	case 3: {
		process* check;
		check = head->next;
		if (head->next == nullptr)
			cout << "empty" << endl;
		else
			while (check != nullptr) {
				show(check->WR, 0);
				check = check->next;
			}
		_getch();
		menu();
		break;
	}
	case 4:
		exit(0);
	default: {
		system("cls");
		cout << "\n\n\n\t\t\twrong input(按任意键继续)" << endl;
		_getch();
		menu();
	}
	}
}

int main() {
	creat = (process*)malloc(sizeof(process));
	creat->next = NULL;
	head = creat;
	rwmove = head;
	menu();
	return 0;
}
