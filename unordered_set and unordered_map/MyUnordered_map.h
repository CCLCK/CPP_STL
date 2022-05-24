#pragma once

#include "Hash.h"

namespace ck
{
	template<class K,class V,class Hash=HashFunc<K>>
	class unordered_map
	{
		struct MapKeyOfT
		{
			const K& operator()(const pair< K, V>& kv) const
			{
				return kv.first;
			}
		};
		typedef typename HashBucket::HashTable<K, pair<K, V>, MapKeyOfT, Hash>::iterator iterator;
	public:
		iterator begin()
		{
			return _ht.begin();
		}
		iterator end()
		{
			return  _ht.end();
		}
		pair<iterator, bool> insert(const pair<const K,V>& kv)
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
		V& operator[](const K& key)
		{
			auto it = insert(make_pair(key, V()));
			return (it.first)->second;
		}
	
		
	private:
		HashBucket::HashTable<K, pair<K, V>, MapKeyOfT,Hash> _ht;
	};

	void test_unordered_map()
	{
		unordered_map<string, string> dict;
		dict.insert(make_pair("sort", "ÅÅÐò"));
		dict.insert(make_pair("string", "×Ö·û´®"));
		dict.insert(make_pair("map", "µØÍ¼"));
		dict["end"] = "×îºó";
		dict["map"] += ",,,,";

		auto it = dict.begin();
		
		while (it != dict.end())
		{
			cout << it->first << ":" << it->second << endl;
			++it;
		}
		cout << endl;

	}
}

