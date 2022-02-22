#define _CRT_SECURE_NO_WARNINGS 1

#include"string.h"


int main()
{
	ck::string s1;
	cout << s1.size() << endl;


	ck::string s2("helloworld");
	s2.resize(15);
	s2[14] = 'x';
	// 0  resize  截断
	//resize capacity  填充字符
	//capacity 无穷  填充字符  扩容
	s2.reserve(30);
	cout << s2.c_str() << endl;
	
	ck::string s3("helloworld");
	ck::string s4(s3);//定义s4进入拷贝构造函数前会提前进行一次自动的初始化构造
	cout << s3.size() << endl;
	return 0;
}








