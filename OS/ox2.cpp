#include <iostream>
#include <string>
#include <thread>
#include <Windows.h>
#include <conio.h>
#include <fstream>

using namespace std;


class WRer
{
public:
    WRer(int id, int in, int out, int t);

    int ID;
    int inTime;
    int outTime;
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


const string filePath = "file.txt";



void waitRead();
void waitSign();
void creatWRer();
void Read(WRer er);
void inread(WRer er);
void Write(WRer er);
void dataWrite(WRer er);


process* head, * rwmove, * creat, * reaD;


//�ȴ���д�ź���
void waitSign() {
    while (sign == 0)
        Sleep(1000);
}

//�ȴ������ź���
void waitRead() {
    while (signal_Reader == 0)
        Sleep(1000);
}

void Listing() {
    char ch = ' ';
    if (kbhit())//��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0
    {
        ch = getch();
        if ((ch == 'S') || (ch == 's'))
        {
            printf("\t\t����ͣ�����������...\n����µĽ�������a\n");
            ch = getch();
        }
        if (ch == 'a') {
            creatWRer();
        }
    }
}

WRer::WRer(int id, int in, int out, int t) {
    ID = id;
    inTime = in;
    outTime = out;
    item = t;
}

//��������
void creatWRer() {
    int _item, _id;
    int _intime, _outtime;
    cout << "please input read->0 or write->1";
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
    }
    cout << "�������ʱ��ͽ���ʱ��" << endl;
    scanf("%d %d", &_intime, &_outtime);
    if (_outtime < _intime) {
        cout << "wrong";
        return;
    }
    WRer er = WRer(_id, _intime, _outtime, _item);
    creat = (process*)malloc(sizeof(process));
    creat->WR = er;
    creat->next = nullptr;
    rwmove->next = creat;
    rwmove = rwmove->next;

}

void show(WRer er, int time) {
    cout << "\tID\t����ʱ��\t����ʱ��\t������ʱ��\t����" << endl;
    cout << " \t " << er.ID << "  \t  " << er.inTime << "  \t  " << er.outTime << "  \t  " << time << "\t  " << er.item << endl;
    cout << endl;
}

//���߲����жϺ���
void TRead(WRer er) {
    if (sign == 0)
        if (signal_Reader != 20)
            if (signal_Reader != 0)
                inread(er);
            else {
                waitRead();
                inread(er);
            }
        else {
            waitSign();
            inread(er);
        }
    else {
        sign--;
        inread(er);
    }
}

//���������庯��
void inread(WRer er) {
    signal_Reader--;
    int time = er.outTime - er.inTime;
    int i = 0;

    while (time > i) {
        Listing();
        Sleep(1000);
        i++;
        show(er, i);
    }
    signal_Reader++;//���н������ͷ�λ��
    dataWrite(er);
    head = head->next;
    if (signal_Reader == 20)
        sign++;

}

//д������
void Write(WRer er) {
    waitSign();
    sign--;

    int time = er.outTime - er.inTime;
    int i = 0;
    while (time > i) {
        Sleep(1000);
        i++;
        show(er, i);
    }
    dataWrite(er);
    head = head->next;
    sign++;
}

//��txt��д��Ϣ
void dataWrite(WRer er) {
    fstream fout;
    fout.open(filePath, ios::out | ios::app);
    if (!fout.is_open())
    {
        cerr << "cannot open the file";
    }
    time_t nowTime;
    time(&nowTime); //��ȡ��ǰʱ��
    fout << "  ���̲���ʱ��  " << ctime(&nowTime);
    if (er.item == read)
        fout << " ID  " << "  ��ʼʱ��  " << "  ����ʱ��  " << " ����:����";
    else
        fout << " ID  " << "  ��ʼʱ��  " << "  ����ʱ��  " << " ����:д��";

    fout << er.ID << "  " << er.inTime << "  " << er.outTime;

    fout.close();
}
/*int readList(process *p){
    process* temp;
    int k=thi;
    temp=p;
    temp=temp->next;
    p->next;
    thi++;
    reaD=temp;
    while(p!= nullptr&&p->WR.item==read){
        temp=p;
        temp=temp->next;
        p=p->next;
        thi++;
    }
    for(int j=k;j<thi-k;j++){

        thread th[j](read,reaD->WR);
        reaD=reaD->next;
    }
}
*/
void run() {
    process* run;
    run = head->next;
    while (run != nullptr) {
        if (run->WR.item == read) {
            thread th(TRead, run->WR);
            th.detach();
        }
        else {
            Write(run->WR);
        }
        run = run->next;
    }
}

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
        int i;
        system("cls");
        cout << "������̸���" << endl;
        cin >> i;
        for (int j = 0;j < i;j++)
            creatWRer();
        menu();
        break;
    }
    case 2: {
        system("cls");
        run();
        getch();
        break;
    }
    case 3: {
        process* check;
        check = head->next;
        while (check != nullptr) {
            show(check->WR, 0);
            check = check->next;
        }
        getch();
        menu();
        break;
    }
    case 4:
        exit(0);
    default: {
        system("cls");
        cout << "\n\n\n\t\t\twrong input(�����������)" << endl;
        getch();
        menu();
    }
    }
}

int main() {
    creat = (process*)malloc(sizeof(process));
    creat->next = nullptr;
    head = creat;
    rwmove = head;
    menu();
    return 0;
}
