#define _CRT_SECURE_NO_WARNINGS
#include "Header.h"
#include "sql_server.h"
#include "SQL.h"
#include "Database.h"
using namespace std;
void addtime(string a, int n) {
	int hour, min, sec;
	hour = stoi(a.substr(0, 2));
	min = stoi(a.substr(3, 2));
	sec = stoi(a.substr(6, 2));
	//cout << hour << " " << min << " " << sec << endl;
	if (n < 0||min>60||sec>60) {
		cout << "Invalid" << endl;
		return;
	}
	else if (n == 0) {
		cout << a << endl;
		return;
	}
	while (n > 0) {
		while (n >= 3600) {
			hour++;
			n -= 3600;
		}
		while (n >= 60) {
			min++;
			n -= 60;
			if (min == 60) {
				hour++;
				min -= 60;
			}
		}
		if (n >= 60 - sec) {
			sec = n + sec - 60;
			min++;
			n = 0;
			if (min == 60) {
				hour++;
				min -= 60;
			}
		}
		else {
			sec += n;
			n = 0;
		}
	}
	cout << hour << ":" << min << ":" << sec << endl;
}

void adddate(string a, int n) {
	if (a[0] == '-') {
		cout << "Invalid" << endl;
		return;
	}
	int year, month, day;
	year=stoi(a.substr(0,4));
	month = stoi(a.substr(5,2));
	day=stoi(a.substr(8,9));
	if (n < 0) {
		cout << "Invalid" << endl;
		return;
	}
	else if (n == 0) {
		cout << a << endl;
		return;
	}
	while (n > 0) {
		if (month > 13) {
			cout << "Invalid" << endl;
			return;
		}
		else {
			bool run = false;
			if ((year / 4 == 0 && year / 100 != 0) || (year / 400 == 0)) run = true;
			if (month == 1||month==3||month==5|| month == 7|| month == 8|| month == 10|| month == 12) {
				if (day > 31) {
					cout << "Invalid" << endl;
					return;
				}
				if (n <= 31 - day) {
					day += n;
					cout << year << "-" << month << "-" << day << endl;
					return;
				}
				n -= (31 - day);
				day = 0;
				month++;
				if (month == 13) {
					month -= 12;
					year++;
					if ((year / 4 == 0 && year / 100 != 0) || (year / 400 == 0)) {
						run = true;
					}
					else run = false;
				}
			}
			else if (month == 4 || month == 6 || month == 9 || month == 11) {
				if (day > 30) {
					cout << "Invalid" << endl;
					return;
				}
				if (n <= 30 - day) {
					day += n;
					cout << year << "-" << month << "-" << day << endl;
					return;
				}
				n -= (30 - day);
				day = 0;
				month++;
			}
			else {
				if (run == true) {
					if (day > 29) {
						cout << "Invalid" << endl;
						return;
					}
					if (n <= 29 - day) {
						day += n;
						cout << year << "-" << month << "-" << day << endl;
						return;
					}
					n -= (29 - day);
					day = 0;
					month++;
				}
				else {
					if (day > 28) {
						cout << "Invalid" << endl;
						return;
					}
					if (n <= 28 - day) {
						day += n;
						cout << year << "-" << month << "-" << day << endl;
						return;
					}
					n -= (28 - day);
					day = 0;
					month++;
				}
				
			}
		}
	}
}
void sql_server::start() 
{
	//Testģʽר�ú���
	
	
	string temp;
	while (true) { //�ж��ļ��Ƿ�ĩβ ���������ص�bool�����
		getline(cin, temp);
		if (temp == "quit" || temp == "")                  //��Ϊquit
		{
			return;                                 //�������
		}
		else                                        //����
		{
			SQL sql(temp);                          //����sql���󣬶�ָ����д���
			operate(sql);                           //�Դ�����sqlָ�������Ӧ����
		}
	}
}
void sql_server::start(string a)
{

	while (true)
	{
		ifstream ifs(a);
		if (!ifs) {
			sql_server::restart(a);
			return;
		}
		string tmp;
		getline(ifs, tmp);
		if (ifs.eof()) {
			sql_server::restart(a);
			return;
		}
		while (!ifs.eof()) { //�ж��ļ��Ƿ�ĩβ ���������ص�bool�����
			string temp;
			getline(ifs, temp);
			if (temp == "quit" || temp == "")                  //��Ϊquit
			{
				sql_server::restart(a);
				return;                                 //�������
			}
			else                                        //����
			{
				SQL sql(temp);                          //����sql���󣬶�ָ����д���
				operate(sql);                           //�Դ�����sqlָ�������Ӧ����
			}
		}
		ifs.close();
	}
}

void sql_server::restart(string a) {
	ofstream ofs(a,ios::app);
	while (true)
	{
		//cout<<"mysql>";
		string temp;
		getline(cin, temp); 
		if (temp == "quit" || temp == "")        
		{
			return;             
		}
		else                             
		{
			SQL sql(temp);
			if (sql[0] != "SELECT" && sql[0] != "SHOW") {
				ofs << temp <<"\n";
			}
			operate(sql);            
		}
	}
}

void sql_server::operate(SQL& sql)
{
	if (sql[0] == "USE")                               //�����use��������������ָ�����ݿ�����Ϊsql[1]��pair
	{
		current = db.find(sql[1]);
	}
	else if (sql[1] == "DATABASE")                     //�����database����
	{
		if (sql[0] == "CREATE")                        //�����create����
		{
			Database temp;                          //�����µ����ݿ�
			db[sql[2]] = temp;                        //��pair:(sql[2], new database)����db map
			current = db.find(sql[2]);                //��������currentָ��new database
			//cout<<"create database "<<sql[2]<<endl;
		}
		else if (sql[0] == "DROP")                     //�����drop����
		{
			//cout<<"drop database "<<sql[2]<<endl;
			db.erase(sql[2]);                       //��map��ɾ����Ϊsql[2]��database
		}
	}
	else if (sql[0] == "SHOW" && sql[1] == "DATABASES")    //�����show databases�Ĳ���
	{
		cout << "Database\n";
		for (auto it = db.begin(); it != db.end(); it++)   //����db
		{
			cout << it->first << endl;                  //������е�databases��name
		}
	}
	else if (sql[0] == "SHOW" && sql[1] == "TABLES")       //�����show table�Ĳ���
	{
		cout << "Tables_in_" << current->first << endl;
		current->second.show_tables();              //����show_tables()���current��ָ��database������tables
	}
	else if (sql[0] == "SELECT" && sql[1] == "LN") {
		cout << log(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "SQRT") {
		cout << sqrt(stoi(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "ABS") {
		cout << fabs(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "SIN") {
		cout << sin(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "TAN") {
		cout << tan(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "COS") {
		cout << cos(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "ASIN") {
		cout << asin(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "COT") {
		cout << cos(stod(sql[2])) * 1.0 / sin(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "CEILING") {
		cout << ceil(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "ACOS") {
		cout << acos(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "ATAN") {
		cout << atan(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "POWER") {
		cout << pow(stod(sql[2]), stod(sql[3])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "SPACE") {
		for (int i = 0; i < stoi(sql[2]); i++) {
			cout << " ";
		}
		cout << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "ROUND") {
		cout << round(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "FLOOR") {
		cout << floor(stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "PI") {
		cout << pi << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "DEGREES") {
		cout << stod(sql[2]) * 180.0 / pi << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "EXP") {
		cout << pow(e, stod(sql[2])) << endl;
	}
	else if (sql[0] == "SELECT" && sql[2] == "DIV") {
		cout << stod(sql[1]) / stod(sql[3]) << endl;
	}
	else if (sql[0] == "SELECT" && sql[1] == "ADDDATE") {
		string a = sql[2];
		a = a.substr(1, a.length() - 2);
		int n = stoi(sql[4]);
		adddate(a, n);
	}
	else if (sql[0] == "SELECT" && sql[1] == "ADDTIME") {
		string a = sql[2];
		//string b = sql[3];
		a = a.substr(1, a.length() - 2);
		//b = b.substr(0, b.length() - 1);
		int n = stoi(sql[3]);
		addtime(a, n);
	}
	else if (sql[2] == "FROM" && sql[4] == "UNION") {
		//��������select�Ķ����Լ�order by��Ҳ��ͬһ��attribute name
		if (sql[5] == "ALL") {
			//��ɾ���ظ�����
			vector<string> all;
			int flag1 = current->second.tables[sql[3]].pri();
			int flag2 = current->second.tables[sql[9]].pri();//����
			int sign1 = current->second.tables[sql[3]].show_loca(sql[1]);
			int sign2 = current->second.tables[sql[9]].show_loca(sql[7]);//Ҫ�ҵ�
			for (int i = 0; i < current->second.tables[sql[3]].rnum_f(); i++) {
				all.push_back(current->second.tables[sql[3]].record_f()[i * current->second.tables[sql[3]].cnum_f() + sign1]);
			}
			for (int i = 0; i < current->second.tables[sql[9]].rnum_f(); i++) {
				all.push_back(current->second.tables[sql[9]].record_f()[i * current->second.tables[sql[9]].cnum_f() + sign2]);
			}
			if (all.size() == 0) {
				return;
			}
			sort(all.begin(), all.end());
			cout << sql[1] << endl;
			for (auto it : all) cout << it << endl;
		}
		else if (sql[5] == "SELECT") {
			//ɾ���ظ�����
			int flag1 = current->second.tables[sql[3]].pri();
			int flag2 = current->second.tables[sql[8]].pri();
			int sign1 = current->second.tables[sql[3]].show_loca(sql[1]);
			int sign2 = current->second.tables[sql[8]].show_loca(sql[6]);
			vector<string> all;
			for (int i = 0; i < current->second.tables[sql[3]].rnum_f(); i++) {
				all.push_back(current->second.tables[sql[3]].record_f()[i * current->second.tables[sql[3]].cnum_f() + sign1]);
			}
			for (int i = 0; i < current->second.tables[sql[8]].rnum_f(); i++) {
				all.push_back(current->second.tables[sql[8]].record_f()[i * current->second.tables[sql[8]].cnum_f() + sign2]);
			}
			if (all.size() == 0) {
				return;
			}
			sort(all.begin(), all.end());
			cout << sql[1] << endl << all[0] << endl;
			for (int i = 1; i < all.size(); i++) {
				if (all[i] != all[i - 1]) cout << all[i] << endl;
			}
		}
	}
	else
	{
		current->second.operate(sql);               //���򣬲���database����������sql��䴫��current��ָ��pair�е�database���operate(SQL&)������
	}
}