#include "Header.h"
#include "Table.h"
#include <stack>
#include <sstream>
#include <set>
#include <iomanip>
int Table::show_loca(string a) {
	return Table::columns[a].order;
}
int Table::pri() {
	return Table::primary;
}
Table::Table(SQL& sql)
{
	int i = 3;                                    //
	int counter = 0;                              //����ͳ��
	while (true)                                 //һֱѭ��������������
	{
		if (sql[i] == "")                          //���sql����ÿ�����ʶ��ѱ�����ϣ�������ѭ��
		{
			break;
		}
		else if (sql[i] == "PRIMARY")              //����������ؼ���
		{
			auto pri = columns.find(sql[i + 2]);    //sql[i + 2]������������pri����������Ӧ��pair
			primary = pri->second.order;          //primary��������Ӧ���е���λ��
			pri_type = pri->second.type;          //pri_type��������Ӧ���е��е���������
			i = i + 3;                              //���������ʣ���ȡ������һ���е���Ϣ
			continue;
		}
		col_info newc;                          //�����µ�����Ϣnewc
		newc.order = counter;                     //�е�˳����Ŀǰ��counter
		counter++;                              //�������к�counter��1
		string cname = sql[i++];                  //������sql[i]��Ȼ���ȡ��һ����
		col_name.push_back(cname);              //��¼����˳��
		newc.type = sql[i++];                     //�е�����������sql[i]��Ȼ���ȡ��һ����
		if (sql[i] == "NOT" && sql[i + 1] == "NULL")     //�������������NOT NULL
		{
			newc.not_null = true;                 //���о���Ϊnot null
			i += 2;                               //������������
		}
		else                                    //��֮
		newc.not_null = false;                //���в���not null
		columns[cname] = newc;                    //��(cname, newc)�����е�map
	}
	cnum = (int)columns.size();                   //�е���Ŀ��columns��size
	rnum = 0;                                     //����Ϊ��
}

void Table::insert_into(SQL& sql)               //ִ��insert into����
{
	int value_pos = 0;                            //��¼value��λ��
	for (int i = 3; i < sql.get_size(); i++)           //����sql��ÿһ������
	{
		if (sql[i] == "VALUES")                    //����ҵ���VALUES
		{
			value_pos = i;                        //�Ͱ�value_pos��Ϊ��VALUE��λ��
			break;                              //����ѭ��
		}
	}
	string* temp = new string[cnum];              //�½�cnum(����)��string
	for (int i = 0; i < cnum; i++) {                    //��ʼ��Ϊ��
		temp[i] = "NULL";
	}
	for (int i = 3; i < value_pos; i++)                //�ӵ������ʿ�ʼ������VALUE֮ǰ�����д�
	{
		int c_pos = columns[sql[i]].order;        //c_pos����Ϊsql[i]���е�λ��
		temp[c_pos] = sql[i + value_pos - 2];         //��temp[c_pos]��ֵΪvalue֮���Ӧ�Ĳ�������
	}
	for (int i = 0; i < cnum; i++)                     //�������е�temp<string>����������������ݼ�¼��
	{
		record.push_back(temp[i]);
	}
	rnum++;                                     //������һ
	delete[]temp;                              //ɾ��Ϊtemp������ڴ�
}

bool Table::judge(string& str, int r) {          //r�ǵڼ��У�����ÿ�����ʽ����ȷ��
	string op;                                  //������ȡ�����'>''<''='
	for (int i = 0; i < str.size(); i++) {      //����str������'<'��'>'��'='���ͣ��
		if (str[i] == '<' || str[i] == '=' || str[i] == '>') {
			op = str[i];
			str[i] = ' ';
			break;
		}
	}
	stringstream ss(str);                       //��str��ʼ��ss stringstream
	string name, value;                         //Ҫ�Ƚϵ���������ֵ
	ss >> name >> value;                        //����Ҫ�Ƚϵ���������ֵ
	int c = columns[name].order;                //c������Ϊname���е���˳��
	if (columns[name].type == "INT" || columns[name].type == "DOUBLE") {                 //���������int��double
		if (op == "<") {                         //�����'<'
			if (atof(record[r * cnum + c].c_str()) < atof(value.c_str())) return true;    //С��Ϊtrue����֮Ϊfalse
			else return false;
		}
		else if (op == "=") {                   //�����'='
			if (atof(record[r * cnum + c].c_str()) == atof(value.c_str())) return true;   //����Ϊtrue����֮Ϊfalse
			else return false;
		}
		else {                                  //�����'>'
			if (atof(record[r * cnum + c].c_str()) > atof(value.c_str())) return true;    //����Ϊtrue����֮Ϊfalse
			else return false;
		}
	}
	else {                                      //������CHAR
		if (op == "<") {                        //�����'<'
			if (record[r * cnum + c] < value) return true;    //С��Ϊtrue����֮Ϊfalse
			else return false;
		}
		else if (op == "=") {                   //�����'='
			if (record[r * cnum + c] == value) return true;   //����Ϊtrue����֮Ϊfalse
			else return false;
		}
		else {                                  //�����'>'
			if (record[r * cnum + c] > value) return true;    //����Ϊtrue����֮Ϊfalse
			else return false;
		}
	}
}

void Table::where_clause(SQL & sql) {
	int n;
	bool like_bool = false;
	int flag = -1;
	string col;
	string like;
	for (int i = 0; i < sql.get_size(); i++) {
		if (sql[i] == "LIKE") {
			like_bool = true;
			col = sql[i - 1];
			like = sql[i + 1].substr(1, sql[i + 1].size() - 2);
			for (int j = 0; j < cnum; j++) {
				if (col == col_name[j]) {
					flag = j;
				}
			}
		}
	}
	if (like_bool) {
		if (like[0] == '%' && like[like.size() - 1] == '%') {
			like = like.substr(1, like.size() - 2);
			for (int i = 0; i < rnum; i++) {

				if (record[i * cnum + flag].find(like)== string::npos) {
					pick[i] = false;
				}
			}
			return;
		}
		else if (like[0] == '%') {
			
			like = like.substr(1, like.size() - 1);
			for (int i = 0; i < rnum; i++) {
				string tmp = record[i * cnum + flag].substr(record[i * cnum + flag].size()-like.size()-1, like.size());
				if (tmp != like) {
					pick[i] = false;
				}
			}
			return;

		}
		else if (like[like.size() - 1] == '%'){
			like = like.substr(0, like.size() - 1);
			for (int i = 0; i < rnum; i++) {
				string tmp = record[i * cnum + flag].substr(1, like.size());
				if ( tmp != like) {
					pick[i] = false;
				}
			}
			return;
		}
		return;
	}
	for (n = 0; n < sql.get_size(); n++)               //��where��λ��
	{
		if (sql[n] == "WHERE")
		{
			break;
		}
	}
	if (n == sql.get_size())                       //���û��where
	{
		pick.clear();                           //��ô���pick
		for (int lp = 0; lp < rnum; lp++)              //�������ж���Ϊture
		{
			pick.push_back(true);
		}
		return;
	}
	string suff;                                    //suffΪת�ɵĺ�׺ʽ
	map<string, int>p = { {"AND",1},{"OR",0} };     //map p��AND��Ӧ1��OR��Ӧ0
	stack<string> s;                                //ת��׺ʽʱ�õ���ջs
	for (int i = n + 1; i < sql.get_size(); i++) {  //��where��ĵ�һ���ʿ�ʼ��������е�ÿһ����
		if (sql[i] == "AND" || sql[i] == "OR") {    //����������AND��OR
			if (s.empty()) {                        //��ջΪ��
				s.push(sql[i]);                     //��AND��OR����ջ��
			}
			else {                                  //��ջ��Ϊ��
				while (!s.empty() && p[s.top()] > p[sql[i]]) {    //��ջ��Ϊ���Ҷ������������ȼ����ڵ�ǰ���������ȼ�
					suff += s.top();                //��ջ���Ĵʷ������ɵĺ�׺ʽ
					suff += " ";                    //��һ���ո�
					s.pop();                        //��ջ����pop����
				}
				s.push(sql[i]);                     //ֱ��ջΪ�ջ���Ŀǰ����������ȼ���ջ���󣬾Ͱѵ�ǰ���������ջ��
			}
		}
		else {                                      //������������
			suff += sql[i];                         //��ֱ�ӰѸôʷ����ת��׺ʽ
			suff += " ";                            //�ӿո�
		}
	}
	while (!s.empty()) {                            //��ջ����ʱ
		suff += s.top();                            //�����е�ջ�еĴʷ����ת��׺ʽ��
		suff += " ";                                //�ӿո�
		s.pop();
	}
	//cout << suff << endl;//��׺ʽ
	pick.clear();                                   //���ԭpick
	for (int i = 0; i < rnum; i++) {                //�������е���
		stringstream ss(suff);                      //��ת�ɵĺ�׺ʽ����ʼ��ss stringstream
		string temp;                                //������ȡ��ǰ�Ĵ�
		stack<bool> cal;                            //
		while (ss >> temp) {                        //��ssû�н�����ʱ��
			if (temp == "AND" || temp == "OR") {    //��temp��AND��OR
				bool t2 = cal.top(); cal.pop();     //t1��cal��ջ��
				bool t1 = cal.top(); cal.pop();     //t2��cal�Ķ����ڶ���
				if (temp == "AND") {                //���������AND
					if (t1 && t2) {                   //���ǰ������Ϊtrue
						cal.push(true);             //����cal�з���true
					}
					else cal.push(false);           //�������false
				}
				else {                              //����������OR
					if (t1 || t2) {                 //���t1��t2��һΪtrue
						cal.push(true);             //��ջ�з���true
					}
					else cal.push(false);           //�������false
				}
			}
			else {
				cal.push(judge(temp, i));           //��i��
				//cout<<"judge "<<(int)cal.top()<<endl;
			}
		}
		bool res = cal.top(); cal.pop();            //ֱ����׺ʽ���꣬��ȡcalջ���ı���������ջ��
		pick.push_back(res);                        //��calջ������pick�У������Ƿ�ѡ����һ��
		//cout<<"push_back "<<(int)res<<endl;
	}
	/*for (int i = 0; i < rnum; i++) {
		if (pick[i]) cout <<"yes" << " ";
		else cout << "no" << " ";
	}
	cout << endl;*/
}


void Table::delete_from(SQL & sql)                   //ִ��delete from����
{
	where_clause(sql);                              //����where_clause��������sql�������pick
	int pos = 0;                                      //pos��¼��ǰ��������������(ȫ������)
	for (int i = 0; i < rnum; i++)                         //�������е���
	{
		if (pick[i])                                 //�����Ӧ��bool����Ϊ��
		{
			for (int j = 0; j < cnum; j++)                 //�������е��У��Ѹ��е�������ɾȥ
			{
				record.erase(record.begin() + pos);
				/*for(int i=0;i<(int)record.size();i++)
				{
					cout<<record[i]<<' ';
				}
				cout<<endl;*/
			}
		}
		else                                        //�����Ӧ��bool����Ϊ�٣����ƶ�����һ�еĿ�ͷ
			pos += cnum;
	}
	rnum--;                                         //ɾ��һ�к�����-1
}

void Table::update(SQL & sql)                        //ִ��update����
{
	where_clause(sql);                              //����where_clause�����������Ӧ��pick
	int pos = 3;                                      //�ӵ������ʿ�ʼ
	while (sql[pos] != "WHERE")                        //�������WHERE�ؼ���
	{
		string cname;                               //����
		int i = 0;
		while (sql[pos][i] != '=')                     //������в���'='
		{
			cname += sql[pos][i];                     //���ַ�����cnameֱ������'='
			i++;                                    //
		}
		string value;                               //����
		i++;                                        //����'='
		while (i != sql[pos].size())                   //��'='��ʼ�������е���ĸ
		{
			value += sql[pos][i];                     //����value
			i++;
		}
		int c = columns[cname].order;                 //c�Ǹ��е����
		for (int r = 0; r < rnum; r++)                     //�������е���
		{
			if (pick[r])                             //�����е�pickΪtrue
			{
				record[r * cnum + c] = value;             //�Ѹ��ж�Ӧ���иĵ�
			}
		}
		pos++;                                      //ֱ���ؼ���WHERE
	}
}
struct group {
	string name;
	int cnt = 0;
};
bool cmp(const group& a1, const group& a2) {
	return a1.cnt < a2.cnt;
}
void Table::select(SQL& sql)                        //ִ��select����
{
	/*for (int i = 0; i < record.size(); i++) {
		cout << record[i] << " " << endl;
	}*/
	where_clause(sql);                              //��where_clause��������sql��䣬����pick��ֵ
	bool has_result = false;                          //�ж��Ƿ��в�ѯ�������ʼΪfalse
	for (int i = 0; i < rnum; i++)                         //�������е��У�����н������has_result��Ϊtrue
	{
//		pick[i] = false;
		if (pick[i])
			has_result = true;
	}
	if (!has_result)                                 //���û�н����ֱ�ӷ���
		return;
	////////////////////////////////���е�count�������������ˡ�
	if (sql[1] == "COUNT") {/////////////����ǽ�count����,only��false
		if (sql[2] == "*") {
			cout << "COUNT(*)\n" << rnum << endl;
		}
		else {
			int pin = -1;
			for (int i = 0; i < cnum; i++) {
				if (sql[2] == col_name[i]) {
					pin = i;
					break;
				}
			}
			vector<string> contain;
			contain.push_back(record[pin]);
			for (int i = 1; i < rnum; i++) {
				bool check = true;
				for (int j = 0; j < contain.size(); j++) {
					if (contain[j] == record[i * cnum + pin]) {
						check = false;
						break;
					}
				}
				if (check)
					contain.push_back(record[i * cnum + pin]);
			}
			int size = contain.size();
			for (auto it : contain) {
				if (it == "NULL") {
					size--;
					break;
				}
			}
			cout << "COUNT(" << sql[2] << ")" << endl << size << endl;
		}
	}
	else if (sql[2] == "COUNT") {////////////////////������Ƿ����������䣬only��true,��count()
		if (sql.get_size() >= 9) {
			if (sql[3] == "*") {
				int pin = -1;
				for (int i = 0; i < cnum; i++) {
					if (sql[8] == col_name[i]) {
						pin = i;
						break;
					}
				}
				vector<string> contain;
				vector<group> group_by;
				for (int i = 0; i < rnum; i++) {
					bool check = true;
					for (int j = 0; j < contain.size(); j++) {
						if (contain[j] == record[i * cnum + pin]) {
							check = false;
							break;
						}
					}
					if (check) {
						contain.push_back(record[i * cnum + pin]);
						group tmp;
						tmp.name = record[i * cnum + pin];
						group_by.push_back(tmp);
					}
				}
				for (int i = 0; i < rnum; i++) {
					for (int j = 0; j < group_by.size(); j++) {
						if (record[i * cnum + pin] == group_by[j].name) {
							group_by[j].cnt++;
						}
					}
				}
				cout << sql[pin] << "\t" << "COUNT(*)" << endl;
				if (sql.get_size() == 9) {/////////////////////////////�������
					for (int i = 0; i < group_by.size(); i++) {
						if(group_by[i].name[0]=='"')
							cout << group_by[i].name.substr(1, group_by[i].name.size()-2) << "\t" << group_by[i].cnt << endl;
						else
							cout << group_by[i].name << "\t" << group_by[i].cnt << endl;
					}
				}
				else {////////////////////�������Ļ����Ͻ�������
					sort(group_by.begin(),group_by.end(), cmp);
					for (int i = 0; i < group_by.size(); i++) {
						if (group_by[i].name[0] == '"')
							cout << group_by[i].name.substr(1, group_by[i].name.size() - 2) << "\t" << group_by[i].cnt << endl;
						else
							cout << group_by[i].name << "\t" << group_by[i].cnt << endl;
					}
				}
			}
			else {
				//���ܴ���������ͬcount(expression),������ȷ
			}
		}
	}
	////////////////////////��count�ҷ�union������ÿһ����׼�������
	else {
		int pos = 1;                                      //
		vector<bool> output;                            //�ж϶�Ӧ�����Ƿ����
		for (int lp = 0; lp < cnum; lp++)                      //����������
		{
			output.push_back(false);                    //��ʼ��Ϊ���ж�Ϊfalse
		}
		while (sql[pos] != "FROM")                         //����sql�����FROM֮ǰ�����д�
		{
			if (sql[pos] == "*")                           //�����'*'�����ж�Ϊtrue�����ٲ��ң�ֱ������
			{
				for (int lp = 0; lp < cnum; lp++)
				{
					output[lp] = true;
				}
				break;
			}
			else                                        //����Ѷ�Ӧ������Ϊtrue
			{
				output[columns[sql[pos]].order] = true;
			}
			pos++;                                      //������һ����
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool outfile = false;
		string filename;
		if (sql[3] == "OUTFILE") {
			filename = sql[4].substr(1, sql[4].length() - 2);
			outfile = true;
			ifstream fin(filename);
			if (fin) {
				cout << "The File Already exists!" << endl;
				return;
			}
			else {
				std::ofstream log(filename);
				std::streambuf* oldbuf = std::cout.rdbuf(log.rdbuf());
				/*for (int c = 0; c < cnum; c++)                         //�������е��У��������Ҫ����������������'\t'
				{
					if (output[c])
						cout << col_name[c] << '\t';
				}
				cout << endl;                                     //���к����������*/

				if (pri_type == "CHAR" || pri_type == "DATE" || pri_type == "TIME") {                           //�������������char
					set<string> pri;                                //������
					for (int r = 0; r < rnum; r++)                         //���������У�������ж�ӦΪtrue������Ҫ���������
					{
						if (pick[r])
						{
							pri.insert(record[r * cnum + primary]);
						}//��Ҫ�������������
					}
					for (auto it = pri.begin(); it != pri.end(); it++) {    //�������е�����
						for (int r = 0; r < rnum; r++) {                    //�������е���
							if (*it == record[r * cnum + primary]) {        //���Ҫ�����r��
								for (int c = 0; c < cnum; c++) {            //�������е���
									if (output[c]) {                  //�������Ҫ���
										if (columns[col_name[c]].type == "CHAR") {          //�����char��
											if (record[r * cnum + c] == "NULL") {               //�����NULL�����"NULL"
												cout << "NULL\t";
											}
											else {
												string temp;                           //����
												if (record[r * cnum + c][0] != '"') {
													for (int i = 0; i < record[r * cnum + c].size(); i++)
													{
														temp += record[r * cnum + c][i];
													}
												}
												else {
													for (int i = 1; i < record[r * cnum + c].size() - 1; i++)
													{
														temp += record[r * cnum + c][i];
													}
												}
												cout << temp << '\t';
											}
										}
										else if (columns[col_name[c]].type == "DOUBLE") {   //�����double��
											if (record[r * cnum + c] == "NULL") {               //�����NULL�����"NULL"
												cout << "NULL\t";
											}
											else {
												cout << fixed << setprecision(4);          //�������x��x�Ƕ�Ӧ���µ�����
												double x = atof(record[r * cnum + c].c_str());
												cout << x << '\t';
												//std::cout << std::defaultfloat;
											}
										}
										else cout << record[r * cnum + c] << '\t';             //������int�ͣ�ֱ�����
									}
								}
								cout << endl;                             //����
								break;
							}
						}
					}
				}
				else {                                           //����������int��double
					set<double> pri;                                //pri�������
					for (int r = 0; r < rnum; r++)                         //���������У���Ҫ�������������pri
					{
						if (pick[r])
						{
							pri.insert(atof(record[r * cnum + primary].c_str()));
						}//��Ҫ�������������
					}
					for (auto it = pri.begin(); it != pri.end(); it++) {                        //������������
						for (int r = 0; r < rnum; r++) {                    //�������е��У��ҵ�Ҫ�������
							if (*it == atof(record[r * cnum + primary].c_str())) {              //���Ҫ�����r��
								for (int c = 0; c < cnum; c++) {            //�������е���
									if (output[c]) {                  //������Ҫ���
										if (columns[col_name[c]].type == "CHAR") {          //�����char��
											if (record[r * cnum + c] == "NULL") {               //�����NULL�����"NULL"
												cout << "NULL\t";
											}
											else {
												string temp;       //��֮������������temp�У���temp
												if (record[r * cnum + c][0] != '"') {
													for (int i = 0; i < record[r * cnum + c].size(); i++)
													{
														temp += record[r * cnum + c][i];
													}
												}
												else {
													for (int i = 1; i < record[r * cnum + c].size() - 1; i++)
													{
														temp += record[r * cnum + c][i];
													}
												}
												cout << temp << '\t';
											}
										}
										else if (columns[col_name[c]].type == "DOUBLE") {   //�����double��
											if (record[r * cnum + c] == "NULL") {               //�����NULL�����"NULL"
												cout << "NULL\t";
											}
											else {
												cout << fixed << setprecision(4);          //��֮�������(ת���ɸ�����)
												double x = atof(record[r * cnum + c].c_str());
												cout << x << '\t';
												//std::cout << std::defaultfloat;
											}
										}
										else cout << record[r * cnum + c] << '\t';             //�����int�ͣ�ֱ����������
									}
								}
								cout << endl;                             //����
								break;
							}
						}
					}
				}
				std::cout.rdbuf(oldbuf);
			}
		}

		if (!outfile) {
			for (int c = 0; c < cnum; c++)                         //�������е��У��������Ҫ����������������'\t'
			{
				if (output[c])
					cout << col_name[c] << '\t';
			}
			cout << endl;                                     //���к����������

			if (pri_type == "CHAR"||pri_type=="DATE"||pri_type=="TIME") {                           //�������������char
				set<string> pri;                                //������
				for (int r = 0; r < rnum; r++)                         //���������У�������ж�ӦΪtrue������Ҫ���������
				{
					if (pick[r])
					{
						pri.insert(record[r * cnum + primary]);
					}//��Ҫ�������������
				}
				for (auto it = pri.begin(); it != pri.end(); it++) {    //�������е�����
					for (int r = 0; r < rnum; r++) {                    //�������е���
						if (*it == record[r * cnum + primary]) {        //���Ҫ�����r��
							for (int c = 0; c < cnum; c++) {            //�������е���
								if (output[c]) {                  //�������Ҫ���
									if (columns[col_name[c]].type == "CHAR") {          //�����char��
										if (record[r * cnum + c] == "NULL") {               //�����NULL�����"NULL"
											cout << "NULL\t";
										}
										else {
											string temp;                           //����
											if (record[r * cnum + c][0] != '"') {
												for (int i = 0; i < record[r * cnum + c].size(); i++)
												{
													temp += record[r * cnum + c][i];
												}
											}
											else {
												for (int i = 1; i < record[r * cnum + c].size() - 1; i++)
												{
													temp += record[r * cnum + c][i];
												}
											}
											cout << temp << '\t';
										}
									}
									else if (columns[col_name[c]].type == "DOUBLE") {   //�����double��
										if (record[r * cnum + c] == "NULL") {               //�����NULL�����"NULL"
											cout << "NULL\t";
										}
										else {
											cout << fixed << setprecision(4);          //�������x��x�Ƕ�Ӧ���µ�����
											double x = atof(record[r * cnum + c].c_str());
											cout << x << '\t';
											//std::cout << std::defaultfloat;
										}
									}
									else cout << record[r * cnum + c] << '\t';             //������int�ͣ�ֱ�����
								}
							}
							cout << endl;                             //����
							break;
						}
					}
				}
			}
			else {                                           //����������int��double
				set<double> pri;                                //pri�������
				for (int r = 0; r < rnum; r++)                         //���������У���Ҫ�������������pri
				{
					if (pick[r])
					{
						pri.insert(atof(record[r * cnum + primary].c_str()));
					}//��Ҫ�������������
				}
				for (auto it = pri.begin(); it != pri.end(); it++) {                        //������������
					for (int r = 0; r < rnum; r++) {                    //�������е��У��ҵ�Ҫ�������
						if (*it == atof(record[r * cnum + primary].c_str())) {              //���Ҫ�����r��
							for (int c = 0; c < cnum; c++) {            //�������е���
								if (output[c]) {                  //������Ҫ���
									if (columns[col_name[c]].type == "CHAR") {          //�����char��
										if (record[r * cnum + c] == "NULL") {               //�����NULL�����"NULL"
											cout << "NULL\t";
										}
										else {
											string temp;       //��֮������������temp�У���temp���
											if (record[r * cnum + c][0] != '"') {
												for (int i = 0; i < record[r * cnum + c].size(); i++)
												{
													temp += record[r * cnum + c][i];
												}
											}
											else {
												for (int i = 1; i < record[r * cnum + c].size() - 1; i++)
												{
													temp += record[r * cnum + c][i];
												}
											}
											cout << temp << '\t';
										}
									}
									else if (columns[col_name[c]].type == "DOUBLE") {   //�����double��
										if (record[r * cnum + c] == "NULL") {               //�����NULL�����"NULL"
											cout << "NULL\t";
										}
										else {
											cout << fixed << setprecision(4);          //��֮�������(ת���ɸ�����)
											double x = atof(record[r * cnum + c].c_str());
											cout << x << '\t';
											//std::cout << std::defaultfloat;
										}
									}
									else cout << record[r * cnum + c] << '\t';             //�����int�ͣ�ֱ����������
								}
							}
							cout << endl;                             //����
							break;
						}
					}
				}
			}
		}
	}
}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////balabalabalabala
void Table::load(SQL& sql) {
	string a = sql[3].substr(1, sql[3].length() - 2);
	ifstream fin(a);
	if (!fin) {
		cout << "No input file>_<" << endl;
	}
	while (!fin.eof()) {
		string inp;
		getline(fin,inp);
		if (inp == "")break;
		stringstream  in(inp);
		vector<string> puzzle;
		while (in >> a) {
			puzzle.push_back(a);
		}
		int len = sql.get_size() - 7;
		while (len--) {
			bool check = false;
			for (int i = 7; i < sql.get_size();i++) {
				if (sql[i] == col_name[sql.get_size()-8-len]) {
					record.push_back(puzzle[i - 7]);
					check = true;
					break;
				}
			}
			if (check == false) {
				record.push_back("NULL");
			}
		}
		rnum++;
		puzzle.clear();
	}
}
///////////////////balabalabalabala
void Table::show_columns()                      //�����
{
	cout << "Field\tType\tNull\tKey\tDefault\tExtra\n";
	for (int i = 0; i < cnum; i++)                     //����������
	{
		auto it = columns.find(col_name[i]);      //�ҵ���Ӧ��������Ӧ����
		cout << it->first << '\t';                  //�������
		if (it->second.type == "INT")              //�������������int��
		{
			cout << "int(11)\t";                  //���"int(11)\t"
		}
		else if (it->second.type == "CHAR")        //�������������char��
		{
			cout << "char(1)\t";                  //���"char(1)\t"
		}
		else                                    //�������������double��
			cout << "double\t";                   //���"double\t"
		if (it->second.not_null)                 //������ݲ�ΪNULL
		{
			cout << "NO" << '\t';                   //���"NO"
		}
		else                                    //��֮���"YES"
			cout << "YES" << '\t';
		if (it->second.order == primary)           //����������������"PRI"
		{
			cout << "PRI" << '\t';
		}
		else                                    //���������������'\t'
			cout << '\t';
		cout << "NULL" << endl;                     //���"NULL"������
	}
}
