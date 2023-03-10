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

int rid = 0;//读者ID
int wid = 0;//写者ID


const string filePath = "file.txt";



void waitRead();
void waitSign();
void creatWRer();
void Read(WRer er);
void inread(WRer er);
void Write(WRer er);
void dataWrite(WRer er);


process* head, * rwmove, * creat, * reaD;


//等待读写信号量
void waitSign() {
    while (sign == 0)
        Sleep(1000);
}

//等待读者信号量
void waitRead() {
    while (signal_Reader == 0)
        Sleep(1000);
}

void Listing() {
    char ch = ' ';
    if (kbhit())//检查当前是否有键盘输入，若有则返回一个非0值，否则返回0
    {
        ch = getch();
        if ((ch == 'S') || (ch == 's'))
        {
            printf("\t\t已暂停，任意键继续...\n添加新的进程输入a\n");
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

//建立进程
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
    cout << "输入进入时间和结束时间" << endl;
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
    cout << "\tID\t进入时间\t结束时间\t已运行时间\t类型" << endl;
    cout << " \t " << er.ID << "  \t  " << er.inTime << "  \t  " << er.outTime << "  \t  " << time << "\t  " << er.item << endl;
    cout << endl;
}

//读者操作判断函数
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

//读操作具体函数
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
    signal_Reader++;//运行结束，释放位置
    dataWrite(er);
    head = head->next;
    if (signal_Reader == 20)
        sign++;

}

//写者运行
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

//向txt中写信息
void dataWrite(WRer er) {
    fstream fout;
    fout.open(filePath, ios::out | ios::app);
    if (!fout.is_open())
    {
        cerr << "cannot open the file";
    }
    time_t nowTime;
    time(&nowTime); //获取当前时间
    fout << "  进程测试时间  " << ctime(&nowTime);
    if (er.item == read)
        fout << " ID  " << "  起始时间  " << "  结束时间  " << " 类型:读者";
    else
        fout << " ID  " << "  起始时间  " << "  结束时间  " << " 类型:写者";

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
    cout << "\t\t******读写平等系统******" << endl;
    cout << "\t1.批量输入进程" << endl;
    cout << "\t2.运行并展示进程运行情况" << endl;
    cout << "\t3.展示进程" << endl;
    cout << "\t4.退出" << endl;
    int _chose;
    cin >> _chose;
    switch (_chose) {
    case 1: {
        int i;
        system("cls");
        cout << "输入进程个数" << endl;
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
        cout << "\n\n\n\t\t\twrong input(按任意键继续)" << endl;
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
