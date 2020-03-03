

#ifndef sql_server_h
#define sql_server_h
#include "Database.h"
class sql_server
{
private:
	map<string, Database> db;                        //����map<string, Database> db����Ӧ�����ݿ�����ֺ����ݿ�
	map<string, Database>::iterator current;         //������ָ��ǰ�����ݿ�����Ӧ��pair
public:
	void start();
	void start(string a);                                   //��ʼ����ں���
	void restart(string a);
	void operate(SQL& sql);                         //�Դ����sql��������Ӧ����
};

#endif /* sql_server_h */
