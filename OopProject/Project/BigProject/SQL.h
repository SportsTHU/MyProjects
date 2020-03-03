
#ifndef SQL_h
#define SQL_h
#include "Header.h"
class SQL
{
private:
	void parser();                                          //��str����Ϊ���ַ�������words
	string str;                                             //str��ʼ��Ϊ��ǰSQL����ָ��
	vector<string> words;                                   //words�洢��str�����Ķ��ַ���
public:
	SQL(string& s) :str(s) { parser(); }                        //���캯���ڳ�ʼ��SQL����ʱ������parser()������str����Ϊ���ַ�������words
	string operator[](int n) const;                         //�±����أ�����words�еĵ�n������
	int get_size() { return (int)words.size(); }               //get_size��������words�е��ַ�������
};

#endif /* SQL_h */
