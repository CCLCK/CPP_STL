#define _CRT_SECURE_NO_WARNINGS 1

#include"string.h"


int main()
{
	ck::string s1;
	cout << s1.size() << endl;


	ck::string s2("helloworld");
	s2.resize(15);
	s2[14] = 'x';
	// 0  resize  �ض�
	//resize capacity  ����ַ�
	//capacity ����  ����ַ�  ����
	s2.reserve(30);
	cout << s2.c_str() << endl;
	
	ck::string s3("helloworld");
	ck::string s4(s3);//����s4���뿽�����캯��ǰ����ǰ����һ���Զ��ĳ�ʼ������
	cout << s3.size() << endl;
	return 0;
}








