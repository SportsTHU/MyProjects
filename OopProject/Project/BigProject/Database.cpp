

#include "Header.h"
#include "Database.h"

void Database::operate(SQL& sql)                            //���ܶ�Ӧ��SQL��䲢ִ����Ӧ����
{
	if (sql[0] == "CREATE" && sql[1] == "TABLE")                   //�����create table����
	{
		Table newt(sql);                                    //�½�table newt
		tables[sql[2]] = newt;                                //��map�����pair:(sql[2], new table)
	}
	else if (sql[0] == "INSERT" && sql[1] == "INTO")               //�����insert info����
	{
		auto it = tables.find(sql[2]);                        //�ҵ���Ϊsql[2]�ı�
		it->second.insert_into(sql);                        //���ø�table����ĳ�Ա����insert_into��ִ����Ӧ�������
	}
	else if (sql[0] == "DELETE" && sql[1] == "FROM")               //�����delete from�Ĳ���
	{
		auto it = tables.find(sql[2]);                        //�ҵ���Ϊsql[2]�ı�
		it->second.delete_from(sql);                        //���ø�table����ĳ�Ա����delete_from��ִ����Ӧ����
	}
	else if (sql[0] == "UPDATE")                               //�����update����
	{
		auto it = tables.find(sql[1]);                        //�ҵ���Ϊsql[1]�ı�
		it->second.update(sql);                             //���ø�table����ĳ�Ա����update��ִ����Ӧ����
	}
	else if (sql[0] == "SELECT")                               //�����selete����
	{
		int i;
		for (i = 0; i != sql.get_size(); i++)                      //�ҵ�from�ؼ��֣�֮��ľ��Ǳ���
		{
			if (sql[i] == "FROM")
			{
				break;
			}
		}
		auto it = tables.find(sql[i + 1]);                      //�ҵ���Ϊsql[i + 1]�ı�
		it->second.select(sql);                             //���ø�table����ĳ�Ա����selete��ִ����Ӧ����
	}
	else if (sql[0] == "DROP" && sql[1] == "TABLE")                //�����drop table����
	{
		tables.erase(sql[2]);                               //��<table name, table>map�н�����Ϊsql[2]��pairɾ��
	}
	else if (sql[0] == "SHOW" && sql[1] == "COLUMNS")              //�����show columns����
	{
		tables[sql[3]].show_columns();                      //������Ϊsql[3]�ı����ĳ�Ա����show_columns��ִ����Ӧ����
	}
	//balabalabala	
	else if (sql[0] == "LOAD") {
		auto it = tables.find(sql[6]);
		it->second.load(sql);
	}
	//balabalabala	
}
void Database::show_tables()                                //�����database�е����б�
{

	for (auto it = tables.begin(); it != tables.end(); it++)       //���������������
	{
		cout << it->first << endl;
	}
}
