#define _CRT_SECURE_NO_WARNINGS 1

#include"string.h"


int main()
{
	//ck::string s1;
	//cout << s1.size() << endl;


	//ck::string s2("helloworld");
	//s2.resize(15);
	//s2[14] = 'x';
	//// 0  resize  截断
	////resize capacity  填充字符
	////capacity 无穷  填充字符  扩容
	//s2.reserve(30);
	//cout << s2.c_str() << endl;
	//
	//ck::string s3("helloworld");
	//ck::string s4(s3);//定义s4进入拷贝构造函数前会提前进行一次自动的初始化构造
	//cout << s3.size() << endl;

	//ck::string s5;
	//s5.push_back('a');
	//cout << s5.c_str() << endl;

	//ck::string s6("helloworld");
	//s6.append("abcd");
	//cout << s6.c_str() << endl;

	//ck::string::iterator  it= s6.begin();
	//ck::string::iterator  it2 = s6.end();
	//cout << *it << " " << *(it2-1) << endl;

	//const ck::string s7("hello");
	//const ck::string::iterator it3 = s7.begin();
	//cout<<s7.size()<<endl;

	ck::string s8;
	//s8.insert(0, 'a');
	//s8.insert(0, 'b');
	//s8.insert(0, 'c');
	//s8.insert(0, 'd');
	//cout << s8.c_str() << endl;
	//s8.clear();
  	s8.insert(0, "abcd");
	s8.insert(0, "1234");
	s8.push_back('x');
	s8.append(" iloveyou");
	cout << s8.c_str() << endl;


	return 0;
}








