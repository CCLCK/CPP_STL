#pragma once


#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <string>
#include <memory.h>//��memcpy���ֽڿ��� ����ɾ�����滻�� pair��ֱ�Ӹ���Ī������
using namespace std;

enum Color
{
	RED,
	BLACK
};
//template<class K, class V>
template<class T>
struct RBTreeNode
{
	RBTreeNode* _left;
	RBTreeNode* _right;
	RBTreeNode* _parent;

	//pair<K, V>_data;
	T _data;
	int _col;

	//RBTreeNode(const pair<K, V>& data)
	RBTreeNode(const T& data)//���������const T&����Ϊ�������Ĳ�����const,������ﲻ��const �͵�ͬ�ڰѳ���(ʵ��)�����������βΣ�
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		//��һ���� ����ĳ�data ģ������ĳ�T������KV
		,_data(data)
		, _col(RED)
	{}
	//~RBTreeNode()
	//{
	//	_left = nullptr;
	//	_right = nullptr;
	//	_parent = nullptr;
	//	_col = BLACK;
	//}
};

template<class T,class Ref,class Ptr>
//������һ�����÷���Ŀ����Ϊ������const����������ͨ������
struct RBTreeIterator
{
	typedef RBTreeNode<T> Node;
	typedef RBTreeIterator<T, Ref, Ptr> self;
	Node* _node;//���ָ��
	RBTreeIterator(Node* node=nullptr)//ȫȱʡ
		:_node(node)
	{}

	//const T& operator*()
	// T& operator*() ͨ��ģ��ĵڶ���������
	Ref operator*()
	{
		return _node->_data;
	}
	Ptr operator->()
	{
		return &(_node->_data);
	}
	self& operator++()//ǰ��++
	{
		Node* cur = _node;
		if (cur->_right)
		{
			//��������Ϊ��,����ֱ�Ӱ��������ĸ���_node
			//��Ϊ����������ߵ�Ҷ�Ӳ�����һ��Ӧ�÷��ʵĽ��
			Node* right = cur->_right;
			while (right&&right->_left)
			{
				right = right->_left;
			}
			_node = right;
			return *this;
		}
		else//������Ϊ��
			//������
		{
			Node* parent = cur->_parent;
			while (parent&&parent->_right == cur)
			{
				cur = parent;
				parent = cur->_parent;
			}
			if (parent == nullptr)
			{
				_node = nullptr;
			}
			else
			{
				_node = parent;
			}
			
			return *this;
		}
	}
	self& operator--()
	{
		//ǰ��--
		/*	������Ϊ��������ȥ��
			������cur�Ǹ��׵��ұ�ֱ�ӷ��ʸ���
			cur�Ǹ��׵���ߴ������������
			����ϣ����ϵݹ�
			��������Ϊ�շ������������ұߵĽ��
		*/
		Node* cur = _node;
		if (cur->_left)//��������Ϊ��
		{
			Node* left = cur->_left;//������ ȡ���ĳ�subRight��һ��
			while (left&&left->_right)
			{
				left = left->_right;
			}
			_node = left;
		}
		else//������Ϊ��
		{
			Node* parent = cur->_parent;
			while (parent&&parent->_left==cur)
			{
				cur = parent;
				parent = cur->_parent;
			}

			_node = parent;
		}
		return *this;
	}
	self operator++(int)
	{
		Node* tmp = _node;
		++(*this);
		return self(tmp);
	}
	self operator--(int)
	{
		Node* tmp = _node;
		--(*this);
		return self(tmp);
	}
	bool operator==(const self& s)const 
	{
		return _node == s._node;//ע�������s�Ǹ����� ���Ƕ���ָ��
	}
	bool operator!=(const self& s) const
	{
		return _node != s._node;
	}
};

template<class K,class T,class KeyOfT>
//�ڶ�������֮ǰ��kv �������������ܵ����ṩ�º�������
class RBTree
{
	typedef RBTreeNode<T> Node;

public:
	typedef RBTreeIterator<T,T&,T*> iterator;
	typedef RBTreeIterator<T, const T&, const T*>const_iterator;
	RBTree()
		:_root(nullptr)
	{}
	iterator begin()
	{
		Node* left = _root;
		while (left&&left->_left)
		{
			left = left->_left;
		}
		return iterator(left);//left��Node* ���ǿתһ��
	}
	iterator end()
	{
		return iterator(nullptr);
	}
	iterator find(const T& data)
	{
		Node* cur;
		KeyOfT kot;
		while (cur)
		{
			if (kot(data) < kot(cur->_data))
			{
				cur = cur->_left;
			}
			else if (kot(data) > kot(cur->_data))
			{
				cur = cur->_right;
			}
			else
			{
				return iterator(cur);
			}
		}
		return end();
	}
	pair<iterator,bool> Insert(const T& data)
	{
		if (_root == nullptr)
		{
			_root = new Node(data);
			_root->_col = BLACK;
			return make_pair(_root,true);
		}
		Node* cur = _root;
		Node* parent = nullptr;
		KeyOfT kot;
		//��mapΪ������������kot���÷º�������data��first
		//kot�����ø��ݴ������Ĳ�����ͬ����ͬ
		while (cur)
		{
			
			if ((kot(data) < kot(cur->_data)))
			{
				parent = cur;
				cur = cur->_left;
			}
			else if ((kot(data) > kot(cur->_data)))
			{
				parent = cur;
				cur = cur->_right;
			}
			else//������ֵͬ
			{
				return make_pair(cur,false);
			}
		}
		cur = new Node(data);//
		Node* newnode = cur;
		if ( kot(cur->_data)< kot(parent->_data))
		{
			parent->_left = cur;
		}
		else
		{
			parent->_right = cur;
		}
		cur->_parent = parent;

		//��ʼ����������
		while (parent && parent->_col == RED)
		{
			_root->_col = BLACK;
			Node* grandfather = parent->_parent;
			if (parent == grandfather->_left)//���������� Ϊ���ж���ֱ�߻�������  һ�������ֿ���
			{
				Node* uncle = grandfather->_right;
				if (uncle && uncle->_col == RED)//uncle������Ϊ��
				{
					grandfather->_col = RED;
					parent->_col = uncle->_col = BLACK;
					cur = grandfather;
					parent = cur->_parent;
				}
				else //uncle�����ڻ���uncleΪ��
				{
					if (parent->_left == cur)//ֱ��
					{
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else//����
					{
						RotateL(parent);
						RotateR(grandfather);
						grandfather->_col = RED;
						cur->_col = BLACK;
					}
					break;
				}
			}
			else//�������Һ���
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;
					cur = grandfather;
					parent = cur->_parent;//����
				}
				else//uncle�����ڻ�uncleΪ��
				{
					if (parent->_right == cur)
					{
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else//����
					{
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}
					break;
				}
			}
		}
		_root->_col = BLACK;
		return make_pair(newnode,true);
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		parent->_left = subLR;
		if (subLR)
		{
			subLR->_parent = parent;
		}
		Node* ppNode = parent->_parent;
		subL->_right = parent;//subL������Ϊ�� ��תʱƽ�����ӿ϶�Ϊ2��-2 ���ýڵ�����һ��һ������
		parent->_parent = subL;
		if (parent == _root)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subL;
			}
			else
			{
				ppNode->_right = subL;
			}
			subL->_parent = ppNode;
		}

	}
	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}
		Node* ppNode = parent->_parent;
		subR->_left = parent;
		parent->_parent = subR;
		if (parent == _root)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subR;
			}
			else
			{
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}

	}
	void _InOrder(Node* root)
	{
		KeyOfT kot;
		if (root == nullptr)
		{
			return;
		}
		_InOrder(root->_left);
		cout << kot((root->_data)) << " ";
		_InOrder(root->_right);
	}
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	bool CheckRED(Node* root)
	{
		if (root == nullptr)
		{
			return true;
		}

		//���Ӳ�һ�����ڣ����Լ���ɫ���ĸ����Ƿ�Ϊ��
		if (root->_col == RED)
		{
			if ((root->_parent)->_col == RED)
			{
				cout << "�����ĺ�ɫ���" << endl;
				return false;
			}
			//���ﲻ��ֱ��return  true ��Ϊ��Ҫ���������������Ƿ���������ĺ���
		}

		return CheckRED(root->_left)
			&& CheckRED(root->_right);

	}

	bool CheckBlackNums(Node* root, int blackNums, int benchmark)
	{
		if (root == nullptr)
		{
			if (blackNums == benchmark)
			{
				return true;
			}
			else
			{
				cout << "ÿ��·���Ϻ�ɫ�ڵ㲻һ��" << endl;
				return false;
			}

		}
		if (root->_col == BLACK)
		{
			blackNums++;
		}
		return CheckBlackNums(root->_left, blackNums, benchmark) && CheckBlackNums(root->_right, blackNums, benchmark);

	}

	bool IsBalance()
	{
		if (_root == nullptr)
		{
			return true;
		}
		if (_root->_col == RED)
		{
			cout << "���ڵ�Ϊ��ɫ" << endl;
			return false;
		}
		int benchmark = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_col == BLACK)
			{
				benchmark++;
			}
			cur = cur->_left;
		}
		int blacknum = 0;
		return CheckRED(_root)
			&& CheckBlackNums(_root, blacknum, benchmark);
	}

	bool Erase(const T& data)
	{
		Node* cur = _root;
		KeyOfT kot;
		while (cur)
		{
			
			if (kot(cur->_data) > kot(data))
			{
				cur = cur->_left;
			}
			else if (kot(cur->_data) < kot(data))
			{
				cur = cur->_right;
			}
			else//�ҵ���
			{
				break;
			}
		}
		if (cur == nullptr)
		{
			return false;
		}
		if (cur == _root && cur->_right == nullptr && cur->_left != nullptr)//ֱ��ɾ���� ��������Ϊ��
		{
			_root = cur->_left;
			_root->_col = BLACK;
			_root->_parent = nullptr;
			delete cur;
			cur = nullptr;
			return true;
		}
		if (cur == _root && cur->_right != nullptr && cur->_left == nullptr)//ֱ��ɾ���� ��������Ϊ��
		{
			_root = cur->_right;
			_root->_col = BLACK;
			_root->_parent = nullptr;
			delete cur;
			cur = nullptr;
			return true;
		}

		Node* realDel = nullptr;
		Node* parent = cur->_parent;

		if (cur->_right == nullptr && cur->_left == nullptr)//Ҷ��
		{
			if (cur->_col == RED)//��ɫҶ��ֱ��ɾ
			{
				if (parent->_left == cur)
				{
					parent->_left = nullptr;
				}
				else
				{
					parent->_right = nullptr;
				}
				
				delete cur;
				cur = nullptr;
			}
			else//��ɫҶ��
			{
				realDel = cur;//��¼����������
			}
		}
		else if (cur->_right == nullptr && cur->_left != nullptr)//ֻ��������
		{
			if (cur->_col == BLACK)//�Ϻ��º�ĺ�ɫ��֧
			{

				if (parent->_left == cur)
				{
					parent->_left = cur->_left;
					cur->_left->_parent = parent;
				}
				else
				{
					parent->_right = cur->_left;
					cur->_left->_parent = parent;

				}
				cur->_left->_col = BLACK;
				delete cur;
				cur = nullptr;
			}
			else//����Ϊ������������� ����оʹ�ӡ
			{
				cout << "������ֻ���������ĵ�֧�����������ɫ���Ǻ�ɫ" << endl;
			}

		}
		else if (cur->_left == nullptr && cur->_right != nullptr)//ֻ��������
		{
			if (cur->_col == BLACK)
			{
				if (parent->_left == cur)
				{
					parent->_left = cur->_right;
					cur->_right->_parent = parent;
				}
				else
				{
					parent->_right = cur->_right;
					cur->_right->_parent = parent;
				}
				cur->_right->_col = BLACK;

				delete cur;
				cur = nullptr;
			}
			else//����Ϊ�������������
			{
				cout << "������ֻ���������ĵ�֧�����������ɫ���Ǻ�ɫ" << endl;
			}
		}
		else//����������
		{
			Node* minRight = cur->_right;
			Node* minRightParent = cur;
			while (minRight->_left)
			{
				minRightParent = minRight;
				minRight = minRight->_left;
			}
			memcpy(&cur->_data, &minRight->_data,sizeof(cur->_data));
			//cur->_data = minRight->_data;//��TΪpair��ʱ�򱨴��˲�֪��ԭ��  ��������������ɾ���ĺ���
			if (minRight->_col == RED && minRight->_right == nullptr)//û�����������滻������Ǻ�ɫҶ��
			{
				if (minRightParent->_left == minRight)
				{
					minRightParent->_left = nullptr;
				}
				else
				{
					minRightParent->_right = nullptr;
				}
				delete minRight;
				minRight = nullptr;
			}
			else if (minRight->_col == BLACK && minRight->_right != nullptr)//�����������Ҳ���Ҷ��
			{
				if (minRightParent->_left == minRight)
				{
					minRightParent->_left = minRight->_right;
					minRight->_right->_parent = minRightParent;
				}
				else
				{
					minRightParent->_right = minRight->_right;
					minRight->_right->_parent = minRightParent;
				}
				minRight->_right->_col = BLACK;
				delete minRight;
				minRight = nullptr;
			}
			else if (minRight->_col == BLACK && minRight->_right == nullptr)//��ɫҶ��
			{
				realDel = minRight;//��¼����������
			}
			else//����Ϊ�������������
			{
				cout << "�����˲�����Ҫ������" << endl;
			}
		}
		if (realDel == nullptr)
		{
			return true;
		}
		//ɾ����ɫҶ��
		/*
			1.��ɫҶ��Ϊ��ʱ
			2.�ֵ�ֶ��ȫ��  --> ������
			3.�ֵ�Ϊ�� ֶ�Ӳ�ȫ��-->��ֱ��/����
			4.�ֵ�Ϊ�� ��ת�����2
		*/
		if (realDel == _root)
		{
			cout << "��ɾ����" << endl;
			_root = nullptr;
			return true;
		}
		//���Ǹ��ڵ� ��ɫҶ�ӿ϶��и���
		Node* del = realDel;//��¼��ɾ���Ľ��  
		while (realDel != _root && realDel->_col == BLACK)
		{
			Node* DelParent = realDel->_parent;
			Node* sibling;
			if (DelParent->_left == realDel)
			{
				sibling = DelParent->_right;
			}
			else
			{
				sibling = DelParent->_left;
			}
			if (sibling == nullptr)
			{
				sibling = new Node(T());
				sibling->_col = BLACK;
			}
			Node* nephew_left = sibling->_left;
			Node* nephew_right = sibling->_right;
			if (nephew_left == nullptr)
			{
				nephew_left = new Node(T());
				nephew_left->_col = BLACK;
			}
			if (nephew_right == nullptr)
			{
				nephew_right = new Node(T());
				nephew_right->_col = BLACK;
			}
			if (sibling->_col == BLACK && nephew_right->_col == BLACK && nephew_left->_col == BLACK)
			{
				//�ֵ�ֶ��ȫ��
				if (DelParent->_col == RED)
				{
					//����Ϊ��
					DelParent->_col = BLACK;
					sibling->_col = RED;
					break;
				}
				else
				{
					//����Ϊ��
					sibling->_col = RED;
					realDel = realDel->_parent;
				}

			}
			else if (sibling->_col == BLACK && (!(nephew_right->_col == BLACK && nephew_left->_col == BLACK)))
			{
				//�ֵ�Ϊ�� ֶ�Ӳ�ȫ��

				if (DelParent->_right == sibling && nephew_right->_col == RED)
				{
					//ֱ��
					sibling->_col = DelParent->_col;
					DelParent->_col = BLACK;
					RotateL(DelParent);
					nephew_right->_col = BLACK;

				}
				else if (DelParent->_left == sibling && nephew_left->_col == RED)
				{
					//ֱ��
					sibling->_col = DelParent->_col;
					DelParent->_col = BLACK;
					RotateR(DelParent);
					nephew_left->_col = BLACK;
				}
				else if (DelParent->_left == sibling && nephew_right->_col == RED)
				{
					//����
					RotateL(sibling);
					nephew_right->_col = BLACK;
					sibling->_col = RED;
					nephew_left = sibling;
					sibling = nephew_right;
					RotateR(DelParent);
					sibling->_col = DelParent->_col;
					DelParent->_col = BLACK;
					nephew_left->_col = BLACK;
				}
				else if (DelParent->_right == sibling && nephew_left->_col == RED)
				{
					//����
					RotateR(sibling);
					nephew_left->_col = BLACK;
					sibling->_col = RED;
					nephew_right = sibling;
					sibling = nephew_left;
					RotateL(DelParent);
					sibling->_col = DelParent->_col;
					DelParent->_col = BLACK;
					nephew_right->_col = BLACK;
				}
				else
				{
					assert(false);
				}
				break;
			}
			else if (sibling->_col == RED)
			{
				//�ֵ�Ϊ��
				if (DelParent->_left == sibling && sibling->_col == RED)
				{
					RotateR(DelParent);
					sibling->_col = BLACK;
					DelParent->_col = RED;
				}
				else if (DelParent->_right == sibling && sibling->_col == RED)
				{
					RotateL(DelParent);
					sibling->_col = BLACK;
					DelParent->_col = RED;
				}
			}
			else
			{
				assert(false);
			}
		}
		Node* delParent = del->_parent;

		if (delParent->_left == del)//������Ķ���Ҷ��
		{
			delParent->_left = del->_right;

			/*if (del->_right)
			{
				cout << "del->_right�ǿ�" << endl;
				del->_right->_parent = delParent;
			}*/
			delete del;
			del = nullptr;
		}
		else
		{
			delParent->_right = del->_right;

			/*	if (del->_right)
				{
					cout << "del->_right�ǿ�" << endl;
					del->_right->_parent = delParent;
				}*/

			delete del;
			del = nullptr;
		}
		return true;
	}

private:
	Node* _root;
};

template<class K>
struct setKeyOfT
{
	const K& operator()(const K& k)//���ﲻ����������
		//����û�����ã�k����һ���βΣ�return��k���ǿ�ռ�ͱ������ˣ�����ֵ������ò���k
	{
		return k;
	}
};
void test_rbtree()
{
	RBTree<int, int, setKeyOfT<int>> rbt;
	rbt.Insert(1);
	rbt.Insert(2);
	rbt.Insert(3);

	//rbt.InOrder();

	rbt.Erase(1);
	rbt.Erase(2);
	rbt.InOrder();
}








