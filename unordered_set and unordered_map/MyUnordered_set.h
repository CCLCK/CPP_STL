#pragma once

#include "Hash.h"

namespace ck
{
	template<class K,class Hash=HashFunc<K>>
	class unordered_set
	{
		struct SetKeyOfT
		{
			const K& operator()(const K& key)
			{
				return key;
			}
		};
	public:
		typedef typename HashBucket::HashTable<K, K, SetKeyOfT, Hash>::iterator iterator;
	public:
		iterator begin()
		{
			return _ht.begin();
		}
		iterator end()
		{
			return  _ht.end();
		}
		pair<iterator, bool> insert(const K& kv)
		{
			return _ht.Insert(kv);
		}
		bool erase(const K& key)
		{
			return _ht.Erase(key);
		}
		bool find(const K& key)
		{
			return _ht.Find(key);
		}
	

	private:
		HashBucket::HashTable<K, K, SetKeyOfT, Hash> _ht;
	};
	void test_unordered_set()
	{
		unordered_set<int> s;
		s.insert(3);
		s.insert(4);
		s.insert(5);
		s.insert(3);
		s.insert(1);
		s.insert(2);
		s.insert(6);
		s.insert(16);
		s.insert(46);
		s.insert(26);
		s.insert(36);
		s.insert(1136);

		unordered_set<int>::iterator it = s.begin();
		while (it != s.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;
	}
}
