#pragma once


#include <string>
#include "RBTree.h"

namespace ck
{
	template<class K,class V>
	class map
	{
		
	public:
		
		//红黑树模板的第三个参数，利用仿函数得到比较的值
		struct MapKeyOfT
		{
			const K& operator()(const pair<const K,V>& kv)
			{
				return kv.first;
			}
		};
		typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::iterator iterator;//有些给常量的值最好就不要给变量了 不然容易导致各种奇怪的报错...
		iterator begin()
		{
			return _rbt.begin();
		}
		iterator end()
		{
			return _rbt.end();
		}
		pair<iterator, bool> insert(const pair<const K, V>& kv)
		{
			return _rbt.Insert(kv);
		}

		size_t erase(pair<const K,V>kv)
		{
			return _rbt.Erase(kv);
		}
		V& operator[](const K& key)
		{
			pair<iterator, bool> ret = insert(make_pair(key, V()));
			return (ret.first)->second;

		}

	
	private:
		//红黑树通过第二个参数来间接知道值的类型
		RBTree<K,pair<const K,V>,MapKeyOfT> _rbt;
	};
	void test_map()
	{
		
		map<string, string>m;
		m.insert(make_pair("string", "字符串"));
		m.insert(make_pair("hello", "你好"));
		m.insert(make_pair("haha", "哈哈"));
		m["one"] = "一";
		
		m.erase(make_pair("string", "字符串"));
		m.erase(make_pair("haha", "哈哈"));
		m.erase(make_pair("hello", "你好"));

		auto it = m.begin();
		while (it != m.end())
		{
			cout << it->first << " " << it->second << endl;
			it++;
		}
	}
}









