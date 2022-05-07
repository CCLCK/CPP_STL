#pragma once



#include "RBTree.h"


namespace ck
{
	template<class K>
	class set
	{

	public:
		
		struct setKeyOfT
		{
			const K& operator()(const K& k)//这里不能忘了引用
				//假如没有引用，k就是一个形参，return的k的那块空间就被销毁了，返回值那里就拿不到k
			{
				return k;
			}
		};
		typedef typename RBTree<K, K, setKeyOfT>::iterator iterator;
		iterator begin()
		{
			return _rbt.begin();
		}
		iterator end()
		{
			return _rbt.end();
		}
		pair<iterator, bool> insert(const K& key)
		{
			return _rbt.Insert(key);
		}
		size_t erase(const K& key)
		{
			return _rbt.Erase(key);
		}
	
	private:
		RBTree<K, K, setKeyOfT> _rbt;//编译时为什么可以知道setKeyOfT的类型？假如传入的是int，因为公用一个模板参数K，所以setKeyOfT这个类被唯一确定了
	};
	void test_set1()
	{
		set<int>s;
		srand(time(0));
		s.insert(2);
		s.insert(1);
		s.insert(3);
		s.insert(4);
		s.insert(6);
		s.insert(5);
		s.insert(7);
		s.insert(8);
		for (auto e : s)
		{
			cout << e << endl;
		}
		s.erase(1);
		s.erase(2);
		s.erase(3);
		s.erase(4);
		s.erase(5);
		s.erase(6);
		s.erase(7);
		s.erase(8);
		for (auto e : s)
		{
			cout << e << endl;
		}
	}
	void test_set2()
	{
		set<int>s;
		srand(time(0));
		const int n = 100;
		for (int i = 0; i < n; i++)
		{
			s.insert(rand());
		}
		/*for (auto e : s)
		{
			不能这么测试会出现迭代器失效的情况
			s.erase(e);
		}*/
		for (auto e : s)
		{
			cout << e << endl;
		}
	}

}



