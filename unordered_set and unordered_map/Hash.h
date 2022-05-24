#pragma once


#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;

namespace ck
{

	template<class K>
	struct HashFunc
	{
		const K& operator()(const K& key)
		{
			return key;
		}
	};
	template<>
	struct HashFunc <string>
	{
		size_t operator()(const string& key)
		{
			size_t value = 0;
			for (auto e : key)
			{
				value = value * 13 + (size_t)e;//*131��BKDR�������ַ�����ϣ�㷨��*131����Ч�ʸ���
			}
			return value;
		}
	};
	namespace close_hash
	{

		enum Status//����һ�½���״̬���������
		{
			EMPTY,
			EXIST,
			DELETE
		};

		template<class K, class V>
		struct HashData
		{
			pair<K, V> _kv;
			Status _status = EMPTY;
		};

		//�ر���string

		template<class K, class V, class Hash = HashFunc<K>>
		class HashTable
		{
		public:
			HashData<K, V>* Find(const K& key)
			{
				if (_tables.size() == 0)
				{
					return nullptr;
				}

				Hash hash;
				size_t start = hash(key) % _tables.size();
				size_t i = 0;
				size_t index = start + i;
				while (_tables[index]._status != EMPTY)
				{
					if (_tables[index]._kv.first == key
						&& _tables[index]._status == EXIST)
					{
						return &_tables[index];
					}
					else
					{
						++i;
						//index = start + i; // ����̽��
						index = start + i * i; // ����̽��
						index %= _tables.size();
					}
				}

				return nullptr;
			}
			bool Insert(const pair<K, V>& kv)
			{
				Hash hash;
				if (Find(kv.first))//ԭ���ı��д���
				{
					return false;
				}
				if (_tables.size() == 0 || _n * 10 / _tables.size() >= 7)
					//����������0.7��ʱ������ ��Ȼ���ܵ�����ѭ��
				{
					size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;

					HashTable<K, V, Hash>NewHT;//��һ��vector�������𣿿��ԣ�����hashtable������
					NewHT._tables.resize(newsize);
					for (auto& e : _tables)//���ִ����� �������õö࿽��һ��
					{
						NewHT.Insert(e._kv);
					}
					_tables.swap(NewHT._tables);//����֮��NewHT�ͱ�������
				}
				size_t start = hash(kv.first) % _tables.size();
				size_t i = 0;
				size_t index = start + i;
				//��������̽������̽��
				while (_tables[index]._status == EXIST)//�����ڿվ�һֱ������
				{
					//index++;//���ڸ���������0.7ʱ���������Բ�����ѭ��.
					i++;
					//index = start + i ����̽��
					index = start + i * i;
					index %= _tables.size();
				}
				//�ҵ�λ����

				_tables[index]._status = EXIST;
				_tables[index]._kv = kv;
				_n++;
				return true;
			}
			bool Erase(const K& key)
			{

				/*	Hash hs;
					size_t index = hs(key) % _tables.size();
					while (_tables[index]._status==EXIST)
					{
						if (_tables[index]._kv.first == key)
						{
							_tables[index]._status = DELETE;
							return true;
						}
						index++;
						index %= _tables.size();
					}
					return false;*/
				HashData<K, V>* ret = Find(key);
				if (ret == nullptr)
				{
					return false;
				}
				else
				{
					ret->_status = DELETE;
					return true;
				}
			}

		private:
			vector<HashData<K, V>>_tables;
			size_t _n = 0;
		};
		void test1()
		{
			//int a[] = { 5, 3, 100, 9999, 333, 14, 26, 34, 5};
			int a[] = { 5, 7, 28, 35, 48,1,2,3,4,5,6 };
			HashTable<int, int> ht;
			for (auto e : a)
			{
				ht.Insert(make_pair(e, e));
			}

			ht.Erase(3);
			cout << ht.Find(3) << endl;
		}
		void test2()
		{
			HashTable<string, string, HashFunc<string>>ht;
			ht.Insert(make_pair("sort", "����"));
			ht.Insert(make_pair("entity", "ʵ��"));
			ht.Insert(make_pair("object", "����"));
			ht.Insert(make_pair("buffer", "����"));
			ht.Erase("sort");
			cout << ht.Find("sort") << endl;

		}

	}

	//namespace open_hash
	namespace HashBucket
	{
		template<class T>
		struct HashNode
		{
			T _data;
			HashNode* _next;
			HashNode(const T&data)
				:_data(data)
				, _next(nullptr)
			{}
		};
		template<class K, class T, class KeyOfT, class Hash>
		class HashTable;

		template<class K, class T, class KeyOfT, class Hash>
		struct HTIterator
		{
			typedef HTIterator<K, T, KeyOfT, Hash> Self;//��������
			typedef HashNode<T> Node;
			typedef HashTable<K, T, KeyOfT, Hash> HT;

			Node* _node;//ͨ��Node*ȥ�������� �����Զ�������++���ܷ��ʵ���һ��Ԫ�أ�������Ҫ��װ
			HT* _ht;

			HTIterator(Node* node, HT* ht)//������Ҫ֪��ָ��Ľ�㣬����++��Ҫ����һ��Ͱ��������Ҫ��ϣ������ڵĹ�ϣ��
				:_node(node)
				, _ht(ht)
			{}
			Self& operator++()//�ҵ�Ͱ����һ��Ԫ��
			{
				Hash hash;
				KeyOfT kot;
				//const K& key = kot(_node->_data);//��¼�����Ϊ��Ԫ�ص�key  ���������Ͳ�ƥ�䵼�½��յ���key�ǿմ�
				Node* tmp = _node;
				_node = _node->_next;//��ǰԪ�ؿ϶���Ϊ�� ���Բ����п�ָ�����õ�����	
				//�����һ��Ϊ�գ�������һ����Ϊ�յ�Ͱ
				if (_node == nullptr)//��һ��Ԫ��Ϊ��
				{
					//����һ����Ϊ�յ�Ͱ��������Ҫ�������ű�
					size_t index = hash(kot(tmp->_data)) % (_ht->_tables.size());
					index++;
					while (index < _ht->_tables.size() && _ht->_tables[index] == nullptr)//һֱ������
					{
						index++;
					}
					if (index == _ht->_tables.size())//�ҵ����һ��Ԫ������Ȼû�ҵ���˵����ǰ�Ѿ������һ��Ԫ����
					{
						_node = nullptr;
					}
					else
					{
						_node = _ht->_tables[index];
						
					}
					return *this;
				}
				else//��һ��Ԫ�ز�Ϊ��
				{
					return *this;
				}

			}

		
			T* operator->()//auto it=m.begin()  ��it->�� ȥ�������ݳ�Ա���Է���ֵ��T*
			{
				return &(_node->_data);
			}
			T& operator*()
			{
				return _node->_data;
			}
			bool operator!= (const Self& s)const
			{
				return s._node != _node;
			}
		};

		template<class K, class T,  class KeyOfT, class Hash>
		class HashTable
		{
			typedef HashNode<T> Node;
		public:
			template<class K, class T, class KeyOfT, class Hash>
			friend struct HTIterator;
			Node* Find(const K& key)//Find��������ֵһ�㶼��ָ�룬ͨ��ָ���������Զ������͵ĳ�Ա
			{
				Hash hash;
				KeyOfT kot;
				if (_tables.size() == 0)//��Ĵ�СΪ0����ֹȡ��0
				{
					return nullptr;
				}
				size_t index = hash(key) % _tables.size();//�ҵ�Ͱ��
				Node* cur = _tables[index];
				while (cur)
				{
					if (kot(cur->_data) == key)
					{
						return cur;
					}
					else
					{
						cur = cur->_next;
					}
				}
				return nullptr;
			}
			size_t GetNextPrime(size_t prime)
			{
				const int PRIMECOUNT = 28;
				static const size_t primeList[PRIMECOUNT] =
				{
					53ul, 97ul, 193ul, 389ul, 769ul,
					1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
					49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
					1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
					50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
					1610612741ul, 3221225473ul, 4294967291ul
				};
				//ul��ʾunsigned long
				size_t i = 0;
				for (; i < PRIMECOUNT; ++i)
				{
					if (primeList[i] > prime)
						return primeList[i];
				}

				return primeList[i];
			}
			typedef HTIterator<K,T,KeyOfT,Hash> iterator;
			iterator begin()
			{
				
				for (size_t i = 0; i < _tables.size(); i++)
				{
					if (_tables[i])
					{
						return iterator(_tables[i], this);
					}
				}
				return iterator(nullptr, this);
			}
			iterator end()
			{
				return iterator(nullptr, this);//�ڶ���ָ������Լ�
			}
			pair<iterator,bool> Insert(const T& data)
			{
				KeyOfT kot;
				Node* tmp = Find(kot(data));
				if (tmp)//����ͬ��keyֱ�ӷ���false
				{
					return make_pair(iterator(tmp, this), false);
				}
				//if(_n==0||_n==_tables.size())
				Hash hash;
				if (_n == _tables.size())//�ʼ_nΪ0����_tables.size()ҲΪ0���Կ��Լ�Ϊһ�д���
				{
					//����
					//size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;
					size_t newSize = GetNextPrime(_tables.size());
					vector<Node*>newTables;
					newTables.resize(newSize, nullptr);
					for (int i = 0; i < _tables.size(); i++)
					{
						Node* cur = _tables[i];
						
						while (cur)
						{
							Node* next = cur->_next;//��¼��һ��λ��
							size_t index = hash(kot(cur->_data)) % newTables.size();
							cur->_next = newTables[index];//cur��ͷ
							newTables[index] = cur;//����vector���ͷ
							cur = next;
						}
					}
					_tables.swap(newTables);//���±�����ݷ���ɱ���
				}

				size_t index = hash(kot(data)) % _tables.size();//���Ͱ��
				//ͷ��
				Node* newNode = new Node(data);
				newNode->_next = _tables[index];
				_tables[index] = newNode;
				++_n;//�����Ǹ�����Ч���ݵĸ���
				return make_pair(iterator(newNode, this), true);
			}
			bool Erase(const K& key)
			{
				//if (!Find(key))//�Ҳ������Ԫ�� 
				// ��ôдҲ���ԣ����Ǻ���ɾ���Ĺ����л�˳����������Ͱ
				//{
				//	return false;
				//}

				if (_tables.size() == 0)//��ϣ��Ϊ��
				{
					return false;
				}
				Hash hash;
				KeyOfT kot;
				size_t index = hash(key) % _tables.size();
				Node* cur = _tables[index];
				Node* prev = nullptr;//��¼ǰһ��λ��
				while (cur)
				{
					if (kot(cur->_data) == key)//�ҵ����Ԫ����
					{
						if (cur == _tables[index])//Ԫ����ͷ���
						{
							_tables[index] = cur->_next;

						}
						else//����ͷ���
						{
							prev->_next = cur->_next;
						}
						delete cur;
						cur = nullptr;
						_n--;
						return true;
					}
					else
					{
						prev = cur;
						cur = cur->_next;
					}
				}
				return false;
			}
			~HashTable()//��ϣͰ���õ�����ṹ ��Ҫ�ͷ�ÿ������
			{
				for (int i = 0; i < _tables.size(); i++)
				{
					Node* cur = _tables[i];
					if (cur == nullptr)
					{
						continue;
					}
					else
					{
						cur = cur->_next;
					}
					while (cur)
					{
						Node* next = cur->_next;
						delete cur;
						cur = next;
					}
					_tables[i] = nullptr;
				}
				_n = 0;
			}
			HashTable() {};

		private:
			vector<Node*>_tables;//�����������Ԫ�ص�ָ��
			size_t _n = 0;//��Ч����
		};
		//void TestHashTable1()
		//{
		//	HashTable<int, int> ht;
		//	int a[] = { 4, 44, 14, 5, 2, 22, 12, 5, 8, 10, 15 };
		//	for (auto e : a)
		//	{
		//		ht.Insert(make_pair(e, e));
		//	}
		//	ht.Insert(make_pair(11, 11));
		//	ht.Erase(22);
		//	ht.Erase(2);
		//	ht.Erase(100);
		//	HashTable<int, int> ht2(ht);
		//}
	}

}