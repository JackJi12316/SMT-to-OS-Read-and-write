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

int rid = 0;//����ID
int wid = 0;//д��ID



void waitRead();
void waitSign();
void creatWRer();
void Read(WRer er);
void inRead(WRer er);
void Write(WRer er);
//void dataWrite(WRer er);


process* head, * rwmove, * creat;


//�ȴ���д�ź���
void waitSign() {
	while (sign == 0);
}

//�ȴ������ź���
void waitRead() {
	while (signal_Reader == 0);
}

//��������
void Listing() {
	char ch = ' ';
	if (_kbhit())//��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0
	{
		ch = _getch();
		if ((ch == 'S') || (ch == 's'))
		{
			printf("\t\t����ͣ�����������...\n����µĽ�������a\n");
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

//��������
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
	cout << "����ʱ��:" << endl;
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

//��ӡ��Ϣ
void show(WRer er, int time) {
	cout << "\tID\t\t��Ҫʱ��\t\t����\t\t������ʱ��" << endl;
	if (er.getitem() == read) {
		cout << " \t RID:" << er.getID() << "  \t  " << er.getRun() << "  \t\t      ����  \t\t" << time
			<< endl;
		cout << endl;
	}
	else {
		cout << " \t WID:" << er.getID() << "  \t  " << er.getRun() << "  \t\t      д��  \t\t" << time
			<< endl;
		cout << endl;
	}
}

//���߲����жϺ���
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

//���������庯��
void inRead(WRer er) {
	int i = 0;
	signal_Reader--;
	while (er.getRun() > i) {
		Listing();
		Sleep(1000);
		i++;
		show(er, i);
	}
	signal_Reader++;//���н������ͷ�λ��
	//dataWrite(er);
	head = head->next;
	if (signal_Reader == 20)
		sign++;

}

//д������
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

//��txt��д��Ϣ
/*void dataWrite(WRer er) {
	ofstream out();
	out.open("../log.txt", ios::out | ios::app);
	if (!out.is_open())
	{
		cerr << "cannot open the file";
	}

	time_t nowTime;
	time(&nowTime); //��ȡ��ǰʱ��
	out << "  ���̲���ʱ��  " << ctime(&nowTime);

	if (er.getitem() == read)
		out << " ID  " << "  ����ʱ��  " << " ����:����\n";
	else
		out << " ID  " << "  ����ʱ��  " << " ����:д��\n";

	out << "  " << er.getID() << "    " << er.getRun() << "\n";

	out.close();
}
*/
//�����߳�������
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

//������
int menu() {
	system("cls");
	cout << "\t\t******��дƽ��ϵͳ******" << endl;
	cout << "\t1.�����������" << endl;
	cout << "\t2.���в�չʾ�����������" << endl;
	cout << "\t3.չʾ����" << endl;
	cout << "\t4.�˳�" << endl;
	int _chose;
	cin >> _chose;
	switch (_chose) {
	case 1: {
		int num;
		system("cls");
		cout << "������̸���" << endl;
		cin >> num;
		for (int j = 0;j < num;j++) {
			cout << "�� " << j + 1 << " �� ������Ϣ" << endl;
			creatWRer();
		}
		menu();
		break;
	}
	case 2: {
		system("cls");
		run();
		cout << "\t\t\t�����������" << endl;
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
		cout << "\n\n\n\t\t\twrong input(�����������)" << endl;
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
