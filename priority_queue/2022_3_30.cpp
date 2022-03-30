#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include <functional>
#include "stack.h"
#include "queue.h"
#include "priority_queue.h"
#include <queue>
using namespace std;

//template<class T>
//class Greater
//{
//public:
//	bool operator()(const T&x, const T& y)
//	{
//		return x > y;
//	}
//};
//
//template<class T>
//class Less
//{
//public:
//	bool operator()(const T&x, const T&y)
//	{
//		return x < y;
//	}
//};
//void func()
//{
//	//测试仿函数
//	int a[] = { 5, 3, 6, 1, 2, 4 };
//
//	std::sort(a, a + 6, std::greater<int>());
//	for (auto e : a)
//	{
//		cout << e << " ";
//	}
//	cout << endl;
//}

//void test_priority()
//{
//	priority_queue<int,vector<int>,greater<int>>pq;
//	//默认建立大堆 greater建立大堆
//	pq.push(4);
//	pq.push(6);
//	pq.push(5);
//	while (!pq.empty())
//	{
//		cout << pq.top() << " ";
//		pq.pop();
//	}
//	cout << endl;
//}

int main()
{
	//test1();
	//func();
	//test_priority();

	ck::test2();
	return 0;
}







