
#ifndef Table_h
#define Table_h
#include "SQL.h"
struct col_info                                     //��������ÿ�е���Ϣ
{
	int order;                                      //�ڱ��е�˳��
	string type;                                    //���е���������
	bool not_null;                                  //�Ƿ��Ƿǿ�
};
class Table                                         //Table��
{
private:
	int cnum;                                       //�������
	int rnum;                                       //�������
	int primary;                                    //����λ��
	string pri_type;                                //��������
	vector <string> record;                         //��������
	vector <string> col_name;                       //��������
	map<string, col_info> columns;                   //����Ϣ������������Ϣ��Ӧ
	vector<bool> pick;                              //WhereClause���ɸѡ
public:
	Table() {}                                       //Ĭ�Ϲ��캯��
	Table(SQL& sql);                                //��������Table���캯��
	void insert_into(SQL& sql);                     //��SQL��䴫�룬ִ��insert into����
	void show_columns();                            //��SQL��䴫�룬ִ��show columns����
	void delete_from(SQL& sql);                     //��SQL��䴫�룬ִ��delete from����
	void update(SQL& sql);                          //��SQL��䴫�룬ִ��update����
	void select(SQL& sql);                          //��SQL��䴫�룬ִ��selete����
	void load(SQL& sql);				//balabalabala	
	void where_clause(SQL& sql);                    //��SQL��䴫�룬ʵ�ֶ�����whereClause�Ӿ�
	bool judge(string& str, int r);
	int pri();
	int show_loca(string a);
	int cnum_f(){ return cnum; }                                       //�������
	int rnum_f(){ return rnum; }                                     //�������
	int primary_f(){ return primary; }                                    //����λ��
	string pri_type_f(){ return pri_type; }                                //��������
	vector <string> record_f(){ return record; }                         //��������
	vector <string> col_name_f(){ return col_name; }                       //��������
	map<string, col_info> columns_f(){ return columns; }                   //����Ϣ������������Ϣ��Ӧ
};


#endif /* Table_h */
