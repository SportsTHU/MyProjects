

#ifndef Database_h
#define Database_h
#include "Table.h"
class Database
{
public:
	map<string, Table> tables;                       //database�е�table name��table���map<string, Table> tables
	void operate(SQL& sql);                         //���ܶ�Ӧ��table����SQL��䣬ִ����Ӧ����
	void show_tables();                             //�����database������table
};
#endif /* Database_h */
