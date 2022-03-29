#include<iostream>
#include<cmath>
#include<algorithm>
#include<string>
#include <assert.h>
using namespace std;

namespace ck
{
	template<class T>
	struct _list_node
	{
		_list_node(T x=T())
		: _prev(nullptr)
		, _next(nullptr)
		, _data(x)
		{}
		_list_node* _prev;
		_list_node* _next;
		T _data;
	};
	template<class T,class Ref,class Ptr>
	struct _list_iterator
	{
		typedef _list_iterator<T, Ref, Ptr> self;
		typedef _list_node<T> Node;
		Node* _node;
		_list_iterator(Node* node)
			:_node(node)
		{}
		Ref operator*()
		{
			return _node->_data;
		}
		self& operator++()
		{
			_node = _node->_next;
			return *this;
		}
		self operator++(int)
		{
			self tmp(*this);
			_node = _node->_next;
			return tmp;
		}
		self operator--()
		{
			_node = _node->_prev;
			return *this;
		}
		self operator--(int)
		{
			self tmp(*this);
			_node = _node->_prev;
			return tmp;
		}
		bool operator!=(const self& it)const
		{
			return (it._node != _node);
		}
		bool operator==(const self&it) const
		{
			return (_node == it._node);
		}
	};

	//const����������ͨд��,  ���Կ����кܶ��ظ��Ĵ���
	//template<class T>
	//struct _list_const_iterator
	//{
	//	typedef _list_const_iterator<T> self;
	//	typedef _list_node Node;
	//	Node* _node;
	//	_list_const_iterator(Node* node)
	//		:_node(node)
	//	{}
	//	const T& operator*()
	//	{
	//		return _node->data;
	//	}
	//	self& operator++()
	//	{
	//		_node = _node->next;
	//		return *this;
	//	}
	//	self operator++(int)
	//	{
	//		self tmp(*this);
	//		_node = _node->next;
	//		return tmp;
	//	}
	//	self operator--()
	//	{
	//		_node = _node->_prev;
	//		return *this;
	//	}
	//	self operator--(int)
	//	{
	//		self tmp(*this);
	//		_node = _node->_prev;
	//		return tmp;
	//	}
	//	bool operator!=(const self& it)const
	//	{
	//		return (it._node != _node);
	//	}
	//	bool operator==(const self&it) const
	//	{
	//		return (_node == it._node);
	//	}
	//};
	
	template<class T>
	class list
	{
	private:
		typedef _list_node<T> Node;
		Node* _head;
	public:
		typedef _list_iterator<T,T&,T*> iterator;
		typedef _list_iterator<T,const T&,const T*> const_iterator;
	//��������ʵҲ���Կ��� Ref��Ptr���������������ĸ�����
		iterator begin()
		{
			return iterator(_head->_next);
		}
		iterator end()
		{
			return iterator(_head);
		}
		const_iterator begin() const
		{
			return const_iterator(_head->_next);
		}
		const_iterator end() const
		{
			return const_iterator(_head);
		}
		list()
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
		}
		//��ͳд��
		/*list(const list<T>&lt)
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
			for (const auto& e : lt)
			{
				push_back(e);
			}
		}*/
		/*
		//ǳ����
		list(const list<T>&lt)
		{
			_head = lt.end()._node;
		}*/
		template<class InputIterator>
		list(InputIterator first, InputIterator last)
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
			while (first != last)
			{
				push_back(*first);
				first++;
			}
		}
		list(const list<T>&lt)
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
			list<T>tmp(lt.begin(), lt.end());
			std::swap(_head, tmp._head);
		}
		void push_back(const T&x)
		{
			insert(end(), x);
		}
		void push_front(const T& x)
		{
			insert(begin(), x);
		}
		void pop_back()
		{
			erase(--end());
		}
		void pop_front()
		{
			erase(begin());
		}
		void clear()
		{
			iterator cur = begin();
			while (cur != end())
			{
				cur = erase(cur);//��ָ����ܻᵼ��һЩbug
				//ԭ����ָ��ɾ���ڵ��ͬʱ�ñ�֤ѭ�����ϣ�Ҳ����erase
				//���ָ��ָ����� size()�ͳ�������
			}
		}
		~list()
		{
			clear();
			delete _head;
			_head = nullptr;
		}
		//list<T>& operator=(list<T>& lt)
		//{
		//	if (this != &lt)
		//	{
		//		clear();
		//		for (const auto& e:lt)
		//		{
		//			push_back(e);
		//		}
		//	}
		//	return *this;
		//}
		list<T>& operator=(list<T> lt)
		{
			std::swap(lt._head, _head);
			//_head = lt._head; ������ôд ��Ȼ����ָ��ָ��ͬһ��ռ�
			//����������lt���ٵ�ͬʱҲ���_head���ٵ�
			return *this;
		}
		iterator insert(iterator pos, const T&x)
		{
			
			Node* cur = pos._node;
			Node* prev = cur->_prev;
			
			Node* newnode = new Node(x);
			prev->_next = newnode;
			newnode->_next = cur;
			cur->_prev = newnode;
			newnode->_prev = prev;
			
			return iterator(newnode);
		}

		iterator erase(iterator pos)
		{
			assert(pos != end());//����ɾ����� �߼������λ�ò��������� ����������ѭ�� �������Ǵ������λ�õ�
			Node* cur = pos._node;
			Node* next = cur->_next;
			Node* prev = cur->_prev;
			delete cur;
			next->_prev = prev;
			prev->_next = next;
			return iterator(next);
		}
		bool empty()
		{
			return begin() == end();
		}
		size_t size()
		{
			size_t cnt = 0;
			for (iterator it = begin();it != end(); it++)
			{
				cnt++;
			}
			return cnt;
		}
	};
}
template<class T>
void print(ck::list<T>&l)
{
	for (ck::list<T>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}
//void test1()
//{
//	ck::list<int>l;
//	l.insert(l.begin(), 1);
//	l.insert(l.begin(), 1);
//	l.insert(l.begin(), 1);
//	l.insert(l.begin(), 1);
//	l.insert(l.begin(), 1);
//	l.erase(l.begin());
//	l.erase(l.begin());
//	l.erase(l.begin());
//	l.erase(l.begin());
//	l.erase(l.begin());
//	print(l);
//}

//void test2()
//{
//	ck::list<int>l;
//	l.push_back(1);
//	l.push_front(0);
//	l.push_back(2);
//	l.pop_back();
//	l.pop_back();
//	l.pop_front();
//	print(l);
//}

//void test3()
//{
//	ck::list<int>l;
//	
//	l.push_back(1);
//	l.push_back(1);
//	l.push_back(1);
//	ck::list<int>l2(l);
//	l.clear();
//	cout << l.empty() << endl;
//	cout << l.size() << endl;
//	print(l2);
//}

void test4()
{
	ck::list<int>l1;
	l1.push_back(1);
	l1.push_back(1);
	l1.push_back(1);
	ck::list<int>l2(l1);
	ck::list<int>l3;
	l3.push_back(0);
	l3 = l1;
	print(l3);

}