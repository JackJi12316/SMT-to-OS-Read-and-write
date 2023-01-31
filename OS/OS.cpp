#include <iostream>
#include <string>
#include <mysql.h>
#include <Windows.h>

using namespace std;
// Writer.ID(0~9)
// Reader.ID(10~19)

static int sign = 1;
static int signal_Reader = 20;

const int r = 0;
const int w = 1;

static int rid = 0;
static int wid = 0;

static int reader_i = 1;
static int wirter_i = 1;

string writer_path = "/wirter.xlsx";
string reader_path = "/reader.xlsx";


class WRer
{
public:
	WRer(int in,int out,int t);

	void doe();
	void Read();
	void inread();
	void Write();

	void waitRead();
	void waitSign();

	void rdataWrite();
	void wdataWrite();
	
	WRer creat(int t);
	WRer creatReader();
	WRer creatWriter();
	
	int getOut() {
		return outtime;
	}
	int getIn() {
		return intime;
	}
	
	int getItem() {
		return item;
	}
	void setintime(int in) {
		intime = in;
	}
	void setouttime(int out) {
		outtime = out;
	}
private:
	int ID;
	float intime;
	float outtime;
	int item;
};

WRer::WRer(int in, int out, int t) {
	item = t;
	setintime(in);
	setouttime(out);



	if (t == r) {
		ID = rid;
		rid++;
	}
	else {
		ID = wid;
		wid++;
	}
}


int setin() {
	int intime;
	cout << "input intime";
	cin >> intime;
	return intime;
}
int setout() {
	int outtime;
	cout << "input outtime";
	cin >> outtime;
	return outtime;
}

WRer WRer::creat(int t) {
	int in = setin();
	int out = setout();
	if (out < in) {
		printf("wrong");
		return;
	}
	WRer r = WRer(in,out,t);
	return r;
}



void WRer::doe() {
	if (getItem() == r)
		Read();
	else
		Write();
}

void WRer::Read() {
	if (sign == 0)
		if (signal_Reader != 20)
			if (signal_Reader != 0)
				inread();
			else {
				waitRead();
				inread();
			}
		else {
			waitSign();
			inread();
		}
	else
		inread();
}

void WRer::inread() {
	if (sign == 1) {
		sign--;
		signal_Reader--;
		rdataWrite();
		int time = int(outtime) - int(intime);
		Sleep(time);
		signal_Reader++;
		if (signal_Reader == 20)
			sign++;
	}
}

WRer WRer::creatReader() {
	WRer rer = creat(r);
	rid++;
	return rer;
}

WRer WRer::creatWriter() {
	WRer wer = creat(w);
	wid++;
	return wer;
}


void WRer::Write() {
	if (sign == 0)
		waitsign();
	sign--;
	wdata_write();
	int time = getOut() - getIn();
	Sleep(time);
	sign++;
}
void waitsign() {
	while (sign == 0)
		Sleep(0.1);
}

void waitRead() {
	while (signal_Reader == 0)
		Sleep(0.1);
}
void rdata_write() {
	/*workbook = xlrd.open_workbook(reader_path)
	sheet = workbook.sheet_by_index(0)
		j = 0
		sheet.write(reader_i, j, self.ID)
		j = j + 1
		sheet.write(reader_i, j, self.intime)
		j = j + 1
		sheet.write(reader_i, j, self.outtime)
		reader_i = reader_i + 1
		workbook.save(reader_path)
*/
}
void wdata_write() {
	/*workbook = xlrd.open_workbook(writer_path)
		sheet = workbook.sheet_by_index(0)
		j = 0
		sheet.write(wirter_i, j, self.ID)
		j = j + 1
		sheet.write(wirter_i, j, self.intime)
		j = j + 1
		sheet.write(wirter_i, j, self.outtime)
		wirter_i = wirter_i + 1
		workbook.save(writer_path)
}
		void creat(self, it) {
		if it == r :
		creatReader(self)
		else :
		creatWriter(self)
			return WRer

*/
}
int main() {

}