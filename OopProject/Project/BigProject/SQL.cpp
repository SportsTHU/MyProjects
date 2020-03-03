
#include "SQL.h"
#include <sstream>
#include <cctype>
#include <set>
void SQL::parser()                                                          //��str����Ϊ���ַ�������words
{
	static set<string> keywords                                             //set������װ��SQL���Ĺؼ���(��Ϊ��д)
	{
		"CREATE","DATABASE","DATABASES","USE","WHERE","SHOW","TABLE","UNION"
		"TABLES","INT","DOUBLE","CHAR","NOT","NULL","INSERT","INTO","ALL",
		"VALUES","DELETE","FROM","UPDATE","SET","SELECT","COLUMNS","ADDDATE","ADDTIME",
		"DROP","AND","OR","LOAD","DATA","INFILE","OUTFILE","GROUP","LIKE"
		"BY","DIV","EXP","DEGREES","PI","FLOOR","ROUND","SPACE","POWER",
		"ATAN","ACOS","CEILING","COT","ASIN","COS","TAN","SIN","ABS","SQRT","LN","TIME","DATE"
	};
	for (auto it = str.begin(); it != str.end(); it++)                             //����str��ÿ���ַ�������',''('')'';'��תΪ�ո�
	{
		if (*it == ',' || *it == '(' || *it == ')' || *it == ';')
		{
			*it = ' ';
		}
		else if (*it == '\'')
		{
			*it = '\"';
		}
	}
	stringstream ss(str);                                                   //�ô�����str��ʼ��һ��stringstream
	string temp;                                                            //�ݴ������ַ���
	while (ss >> temp)
	{
		if (isalpha(temp[0]))                                                //�жϣ������һλ����ĸ
		{
			string copy = temp;                                               //copy��temp�ĸ���
			for (auto it = temp.begin(); it != temp.end(); it++)                   //��temp��ÿ����ĸ����ɴ�д
			{
				*it = toupper(*it);
			}
			if (keywords.find(temp) != keywords.end())                         //����ǹؼ���
			{
				words.push_back(temp);                                      //�Ͱ����ɵ�ȫΪ��д�Ĺؼ��ַ���vector<string>words��
			}
			else
			{
				words.push_back(copy);                                      //������ǹؼ��֣���ô��ԭ�ͷ���vector<string>words��
			}

		}
		else
			words.push_back(temp);                                          //�����һλ������ĸ����϶����ǹؼ��֣���temp����vector<string>words��
	}
}

string SQL::operator[](int n)const                                          //��������أ�����words�ĵ�n������
{
	if (n >= (int)words.size())                                                //���Խ�磬�����"operator[] RE!\n"�����ؿ��ַ���;
	{
		//cout<<"operator[] RE!\n";
		return "";
	}
	else
	{
		return words[n];
	}
}

