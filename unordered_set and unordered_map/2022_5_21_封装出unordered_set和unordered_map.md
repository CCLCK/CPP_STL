[toc]

# 🍭哈希表封装出unordered_set/unordered_map

> 默认你已经实现了哈希表（开散列）

## 🍬封装前的哈希代码

```c++
namespace HashBucket
{
	template<class K,class V>
	struct HashNode
	{
		pair<K, V> _kv;
		HashNode* _next;
		HashNode(const pair<K, V>& kv)
			:_kv(kv)
			, _next(nullptr)
		{}
	};
	template<class K,class V,class Hash=HashFunc<K>>
	class HashTable
	{
		typedef HashNode<K,V> Node;
	public:

		Node* Find(const K& key)//Find函数返回值一般都是指针，通过指针访问这个自定义类型的成员
		{
			Hash hash;
			if (_tables.size() == 0)//表的大小为0，防止取余0
			{
				return nullptr;
			}
			size_t index = hash(key) % _tables.size();//找到桶号
			Node* cur = _tables[index];
			while (cur)
			{
				if (cur->_kv.first == key)
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
			//ul表示unsigned long
			size_t i = 0;
			for (; i < PRIMECOUNT; ++i)
			{
				if (primeList[i] > prime)
					return primeList[i];
			}

			return primeList[i];
		}

		bool Insert(const pair<K, V>& kv)
		{
			if (Find(kv.first))//有相同的key直接返回false
			{
				return false;
			}
			//if(_n==0||_n==_tables.size())
			Hash hash;
			if (_n == _tables.size())//最开始_n为0，而_tables.size()也为0所以可以简化为一行代码
			{
				//增容
				//size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;
				size_t newSize = GetNextPrime(_tables.size());
				vector<Node*>newTables;
				newTables.resize(newSize, nullptr);
				for (int i = 0; i < _tables.size(); i++)
				{
					Node* cur = _tables[i];
					while (cur)
					{
						Node* next = cur->_next;//记录下一个位置
						size_t index = hash(cur->_kv.first) % newTables.size();
						cur->_next = newTables[index];//cur当头
						newTables[index] = cur;//更新vector里的头
						cur = next;
					}
				}
				_tables.swap(newTables);//把新表的数据放入旧表中
			}

			size_t index = hash(kv.first) % _tables.size();//算出桶号
			//头插
			Node* newNode = new Node(kv);
			newNode->_next = _tables[index];
			_tables[index]=newNode;
			++_n;//别忘记更新有效数据的个数
			return true;
		}
		bool Erase(const K& key)
		{ 
			//if (!Find(key))//找不到这个元素 
			// 这么写也可以，但是后面删除的过程中会顺带遍历整个桶
			//{
			//	return false;
			//}
			if (_tables.size() == 0)//哈希表为空
			{
				return false;
			}
			Hash hash;
			size_t index = hash(key) % _tables.size();
			Node* cur = _tables[index];
			Node* prev = nullptr;//记录前一个位置
			while (cur)
			{
				if (cur->_kv.first == key)//找到这个元素了
				{
					if(cur==_tables[index])//元素是头结点
					{
						_tables[index] = cur->_next;
						
					}
					else//不是头结点
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
		~HashTable()//哈希桶采用的链表结构 需要释放每个链表
		{
			for (int i=0;i<_tables.size();i++)
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
		vector<Node*>_tables;//存的是链表首元素的指针
		size_t _n=0;//有效数据
	};
```



## 🍬泛型

封装时想直接搭出unordered_set/unordered_map的结构,发现行不通

于是从哈希表的结构入手，先把一些类型改成泛型

```c++
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
```

> 结点的KV结构改成T ,改变结点的类型后HashTable里的结点类型也需要更改
>
> typedef HashNode<K,V>的模板也需要改为typedef HashNode<T> Node;

## 🍬获取key

> **获取key后可以查找也可以算出桶号**

封装前哈希结点的类型是pair<K,V>，现在是T。

> pair<K,V>kv , 可以通过kv.first来获取key。
>
> 下面默认int、double、string等类型的key就是其本身。

类型T既可能是pair也可能是一个int类型等等，那应该怎么得到类型T的key？**借助模板+仿函数**。

> 以unordered_map为例
>
> unordered_map类中实现仿函数
>
> ![image-20220526161638671](https://pic-1304888003.cos.ap-guangzhou.myqcloud.com/img/image-20220526161638671.png)
>
> <br>**哈希表中增加一个模板参数KeyOfT来获取T类型的Key**
>
> ![image-20220526161849266](https://pic-1304888003.cos.ap-guangzhou.myqcloud.com/img/image-20220526161849266.png)
>
> 同理unordered_set里仿函数的实现
>
> ![image-20220526162643419](https://pic-1304888003.cos.ap-guangzhou.myqcloud.com/img/image-20220526162643419.png)

之后把所有与.first有关的都用模板实例化的kot来获取key

## 🍬自定义哈希规则

去掉哈希表模板参数里哈希函数的默认值  在unordered_set/unordered_map加上第三个模板参数Hash自定义哈希规则

封装前的哈希表

```c++
template<class K,class V,class Hash=HashFunc<K>>
class HashTable{};
```

现在的哈希表

```c++
template<class K, class T, class KeyOfT, class Hash>
//去掉哈希表的默认值，哈希函数由unordered_map传入
class HashTable{};

template<class K,class V,class Hash=HashFunc<K>>
class unordered_map{
    private:
		HashBucket::HashTable<K, pair<K, V>, MapKeyOfT,Hash> _ht;
};
```

![image-20220526163618308](https://pic-1304888003.cos.ap-guangzhou.myqcloud.com/img/image-20220526163618308.png)

> 所以我们使用时可以自己写一个类传入达到自定义哈希规则的效果。

## 🍬哈希表模板参数解释

```c++
template<class K, class T,  class KeyOfT, class Hash>
```

看完上面的对这四个参数应该有大概的了解了。这里一齐解释一下为什么这么写。

第一个参数K：K表示key的类型，查找函数是根据key来查找的，所以需要K。

第二个参数T：哈希表结点存储的数据类型。比如int，double，pair,string等。

第三个参数KeyOfT：拿到结点数据的key。

第四个参数Hash：表示哈希函数类，也即哈希规则。

> ```c++
> //哈希函数
> template<class K>
> struct HashFunc
> {
> 	const K& operator()(const K& key)
> 	{
> 		return key;
> 	}
> };
> template<>//针对string的模板特化
> struct HashFunc <string>
> {
> 	size_t operator()(const string& key)
> 	{
> 		size_t value = 0;
> 		for (auto e : key)
> 		{
> 			value = value * 13 + (size_t)e;//*131是BKDR发明的字符串哈希算法，*131等数效率更高
> 		}
> 		return value;
> 	}
> };
> ```
> HashFunc（Kot(T)） 取出这个类型的key的映射值

## 🍬迭代器

unordered_set/unordered_map的迭代器是单向迭代器

> 迭代器只能++，不能--

### 🍺结构

![image-20220526165535577](https://pic-1304888003.cos.ap-guangzhou.myqcloud.com/img/image-20220526165535577.png)

> Self表示自己

### 🍺operator++()

前置++

实现思路：如果当前结点的下一个不为空 直接访问即可

如果下一个结点为空，就得找下一个桶  怎么找？根据当前指向的数据算出桶号，再把桶号+1，一直往后面找，直到找到一个桶不为空，或者找完了整个容器都没找到，就返回空

```c++
Self& operator++()//找到桶的下一个元素
{
	Hash hash;
	KeyOfT kot;
	Node* tmp = _node;//记录当前位置，用来计算当前桶号
	_node = _node->_next;//当前元素肯定不为空 所以不会有空指针引用的问题	
	//如果下一个为空，就找下一个不为空的桶
	if (_node == nullptr)//下一个元素为空
	{
		//找下一个不为空的桶，所以需要传入这张表
		size_t index = hash(kot(tmp->_data)) % (_ht->_tables.size());
		index++;
		while (index < _ht->_tables.size() && _ht->_tables[index] == nullptr)//一直往后找
		{
			index++;
		}
		if (index == _ht->_tables.size())//找到最后一个元素了仍然没找到，说明当前已经是最后一个元素了
		{
			_node = nullptr;
		}
		else
		{
			_node = _ht->_tables[index];
						
		}
		return *this;
	}
	else//下一个元素不为空
	{
		return *this;
	}
}
```

### 🍺构造函数

> 构造函数得到结点所在的哈希表

```c++
HTIterator(Node* node, HT* ht)//不仅需要知道指向的结点，由于++需要找下一个桶，所以需要哈希结点所在的哈希表
	:_node(node)
	, _ht(ht)	
{}
```



### 🍺重载运算符

> 重载除了++以外的一些运算符

```c++
T* operator->()//auto it=m.begin()  *it可以拿到数据,所以返回值是T*
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
```

> T为pair时可以通过it->first拿到key。

## 🍬小问题

1. 你会发现这样一个现象，迭代器里面用了哈希表，哈希表里用了迭代器，也即两个类互相引用

如果迭代器写在哈希表前面，那么编译时编译器就会发现哈希表是无定义的（编译器只会往前/上找标识符）。

如果哈希表写在迭代器前面，那么编译时编译器就会发现迭代器是无定义的。

为了解决这个问题，得用一个**前置声明**解决，即在迭代器和哈希表的定义前加一个类的声明。

```c++
template<class K, class T, class KeyOfT, class Hash>
class HashTable;//模板需要也需要进行声明

template<class K, class T, class KeyOfT, class Hash>
struct HTIterator{};
...
template<class K, class T, class KeyOfT, class Hash>
class HashTable{};//具体实现
```

2. 迭代器里借助一个指针访问了哈希表的数据。但是哈希表的数据被private修饰，所以在类外不能访问，用友元解决。

在哈希表里面声明迭代器友元类（表示迭代器是哈希表的朋友，可以访问哈希表所有的数据）

3. const pair<const K,V>!=const pair<K,V>

> 写代码时的一个bug
>
> 相关的例子
>
> ![image-20220526172340057](https://pic-1304888003.cos.ap-guangzhou.myqcloud.com/img/image-20220526172340057.png)
>
> 解释：调试看了一下地址，传进仿函数的时候参数用的引用接收，但是因为类型不同，所以仿函数参数接收时进行了一次拷贝才拿到了sort和排序两个字符串，但也因此那个参数成临时变量了，所以返回了一块被销毁的空间的引用 为什么变成空串？因为string析构后那块空间成空串了
>
> 简单来说 仿函数没有拿到真实的那块空间 而是拷贝后形参的空间

4. 不能识别迭代器是类型还是成员报错，加上typename解决。

> typedef typename HashBucket::HashTable<K, K, SetKeyOfT, Hash>::iterator iterator;

typename是告诉编译器这是一个类型 等这个类实例化了再去找里面的东西

## 🍬代码汇总

### 🍵Hash.h

```c++
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
				value = value * 13 + (size_t)e;//*131是BKDR发明的字符串哈希算法，*131等数效率更高
			}
			return value;
		}
	};
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
			typedef HTIterator<K, T, KeyOfT, Hash> Self;//自身
			typedef HashNode<T> Node;
			typedef HashTable<K, T, KeyOfT, Hash> HT;

			Node* _node;//通过Node*去访问数据 不过自定义类型++不能访问到下一个元素，所以需要封装
			HT* _ht;

			HTIterator(Node* node, HT* ht)//不仅需要知道指向的结点，由于++需要找下一个桶，所以需要哈希结点所在的哈希表
				:_node(node)
				, _ht(ht)
			{}
			Self& operator++()//找到桶的下一个元素
			{
				Hash hash;
				KeyOfT kot;
				//const K& key = kot(_node->_data);//记录这个不为空元素的key  有问题类型不匹配导致接收到的key是空串
				Node* tmp = _node;
				_node = _node->_next;//当前元素肯定不为空 所以不会有空指针引用的问题	
				//如果下一个为空，就找下一个不为空的桶
				if (_node == nullptr)//下一个元素为空
				{
					//找下一个不为空的桶，所以需要传入这张表
					size_t index = hash(kot(tmp->_data)) % (_ht->_tables.size());
					index++;
					while (index < _ht->_tables.size() && _ht->_tables[index] == nullptr)//一直往后找
					{
						index++;
					}
					if (index == _ht->_tables.size())//找到最后一个元素了仍然没找到，说明当前已经是最后一个元素了
					{
						_node = nullptr;
					}
					else
					{
						_node = _ht->_tables[index];
						
					}
					return *this;
				}
				else//下一个元素不为空
				{
					return *this;
				}

			}

		
			T* operator->()//auto it=m.begin()  ‘it->’ 去访问数据成员所以返回值是T*
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
			Node* Find(const K& key)//Find函数返回值一般都是指针，通过指针访问这个自定义类型的成员
			{
				Hash hash;
				KeyOfT kot;
				if (_tables.size() == 0)//表的大小为0，防止取余0
				{
					return nullptr;
				}
				size_t index = hash(key) % _tables.size();//找到桶号
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
				//ul表示unsigned long
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
				return iterator(nullptr, this);//第二个指针就是自己
			}
			pair<iterator,bool> Insert(const T& data)
			{
				KeyOfT kot;
				Node* tmp = Find(kot(data));
				if (tmp)//有相同的key直接返回false
				{
					return make_pair(iterator(tmp, this), false);
				}
				//if(_n==0||_n==_tables.size())
				Hash hash;
				if (_n == _tables.size())//最开始_n为0，而_tables.size()也为0所以可以简化为一行代码
				{
					//增容
					//size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;
					size_t newSize = GetNextPrime(_tables.size());
					vector<Node*>newTables;
					newTables.resize(newSize, nullptr);
					for (int i = 0; i < _tables.size(); i++)
					{
						Node* cur = _tables[i];
						
						while (cur)
						{
							Node* next = cur->_next;//记录下一个位置
							size_t index = hash(kot(cur->_data)) % newTables.size();
							cur->_next = newTables[index];//cur当头
							newTables[index] = cur;//更新vector里的头
							cur = next;
						}
					}
					_tables.swap(newTables);//把新表的数据放入旧表中
				}

				size_t index = hash(kot(data)) % _tables.size();//算出桶号
				//头插
				Node* newNode = new Node(data);
				newNode->_next = _tables[index];
				_tables[index] = newNode;
				++_n;//别忘记更新有效数据的个数
				return make_pair(iterator(newNode, this), true);
			}
			bool Erase(const K& key)
			{
				//if (!Find(key))//找不到这个元素 
				// 这么写也可以，但是后面删除的过程中会顺带遍历整个桶
				//{
				//	return false;
				//}
				if (_tables.size() == 0)//哈希表为空
				{
					return false;
				}
				Hash hash;
				KeyOfT kot;
				size_t index = hash(key) % _tables.size();
				Node* cur = _tables[index];
				Node* prev = nullptr;//记录前一个位置
				while (cur)
				{
					if (kot(cur->_data) == key)//找到这个元素了
					{
						if (cur == _tables[index])//元素是头结点
						{
							_tables[index] = cur->_next;

						}
						else//不是头结点
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
			~HashTable()//哈希桶采用的链表结构 需要释放每个链表
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
			vector<Node*>_tables;//存的是链表首元素的指针
			size_t _n = 0;//有效数据
		};
	}
}
```

### 🍵MyUnordered_map.h

```c++
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
}

```

### 🍵MyUnordered_set.h

```c++

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
}
```

