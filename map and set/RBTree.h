#pragma once


#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <string>
#include <memory.h>//用memcpy逐字节拷贝 用在删除的替换那 pair的直接复制莫名报错
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
	RBTreeNode(const T& data)//这里必须是const T&，因为传进来的参数是const,如果这里不加const 就等同于把常量(实参)传给变量（形参）
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		//第一个点 这里改成data 模板参数改成T而不是KV
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
//和链表一样的用法，目的是为了区分const迭代器和普通迭代器
struct RBTreeIterator
{
	typedef RBTreeNode<T> Node;
	typedef RBTreeIterator<T, Ref, Ptr> self;
	Node* _node;//结点指针
	RBTreeIterator(Node* node=nullptr)//全缺省
		:_node(node)
	{}

	//const T& operator*()
	// T& operator*() 通过模板的第二个参数简化
	Ref operator*()
	{
		return _node->_data;
	}
	Ptr operator->()
	{
		return &(_node->_data);
	}
	self& operator++()//前置++
	{
		Node* cur = _node;
		if (cur->_right)
		{
			//右子树不为空,不能直接把右子树的根给_node
			//因为右子树最左边的叶子才是下一个应该访问的结点
			Node* right = cur->_right;
			while (right&&right->_left)
			{
				right = right->_left;
			}
			_node = right;
			return *this;
		}
		else//右子树为空
			//看父亲
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
		//前置--
		/*	左子树为空往上面去找
			看父亲cur是父亲的右边直接访问父亲
			cur是父亲的左边代表这颗子树访
			问完毕，往上递归
			左子树不为空访问左子树最右边的结点
		*/
		Node* cur = _node;
		if (cur->_left)//左子树不为空
		{
			Node* left = cur->_left;//左子树 取名改成subRight好一点
			while (left&&left->_right)
			{
				left = left->_right;
			}
			_node = left;
		}
		else//左子树为空
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
		return _node == s._node;//注意这里的s是个对象 不是对象指针
	}
	bool operator!=(const self& s) const
	{
		return _node != s._node;
	}
};

template<class K,class T,class KeyOfT>
//第二个则是之前的kv 第三个参数接受的是提供仿函数的类
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
		return iterator(left);//left是Node* 最好强转一下
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
		//以map为例，传进来后kot调用仿函数返回data的first
		//kot的作用根据传进来的参数不同而不同
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
			else//存在相同值
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

		//开始处理红黑因子
		while (parent && parent->_col == RED)
		{
			_root->_col = BLACK;
			Node* grandfather = parent->_parent;
			if (parent == grandfather->_left)//父亲是左孩子 为了判断是直线还是折线  一共有四种可能
			{
				Node* uncle = grandfather->_right;
				if (uncle && uncle->_col == RED)//uncle存在且为红
				{
					grandfather->_col = RED;
					parent->_col = uncle->_col = BLACK;
					cur = grandfather;
					parent = cur->_parent;
				}
				else //uncle不存在或者uncle为黑
				{
					if (parent->_left == cur)//直线
					{
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else//折线
					{
						RotateL(parent);
						RotateR(grandfather);
						grandfather->_col = RED;
						cur->_col = BLACK;
					}
					break;
				}
			}
			else//父亲是右孩子
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;
					cur = grandfather;
					parent = cur->_parent;//迭代
				}
				else//uncle不存在或uncle为黑
				{
					if (parent->_right == cur)
					{
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else//折线
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
		subL->_right = parent;//subL不可能为空 旋转时平衡因子肯定为2或-2 即该节点往下一层一定存在
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

		//孩子不一定存在，所以检查红色结点的父亲是否为红
		if (root->_col == RED)
		{
			if ((root->_parent)->_col == RED)
			{
				cout << "连续的红色结点" << endl;
				return false;
			}
			//这里不能直接return  true 因为还要检查这个结点的子树是否存在连续的红结点
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
				cout << "每条路径上黑色节点不一样" << endl;
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
			cout << "根节点为红色" << endl;
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
			else//找到了
			{
				break;
			}
		}
		if (cur == nullptr)
		{
			return false;
		}
		if (cur == _root && cur->_right == nullptr && cur->_left != nullptr)//直接删除根 且右子树为空
		{
			_root = cur->_left;
			_root->_col = BLACK;
			_root->_parent = nullptr;
			delete cur;
			cur = nullptr;
			return true;
		}
		if (cur == _root && cur->_right != nullptr && cur->_left == nullptr)//直接删除根 且右子树为空
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

		if (cur->_right == nullptr && cur->_left == nullptr)//叶子
		{
			if (cur->_col == RED)//红色叶子直接删
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
			else//黑色叶子
			{
				realDel = cur;//记录下来待处理
			}
		}
		else if (cur->_right == nullptr && cur->_left != nullptr)//只有左子树
		{
			if (cur->_col == BLACK)//上黑下红的黑色单支
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
			else//我认为这种情况不会有 如果有就打印
			{
				cout << "出现了只有左子树的单支，但上面的颜色不是黑色" << endl;
			}

		}
		else if (cur->_left == nullptr && cur->_right != nullptr)//只有右子树
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
			else//我认为这种情况不会有
			{
				cout << "出现了只有右子树的单支，但上面的颜色不是黑色" << endl;
			}
		}
		else//有左右子树
		{
			Node* minRight = cur->_right;
			Node* minRightParent = cur;
			while (minRight->_left)
			{
				minRightParent = minRight;
				minRight = minRight->_left;
			}
			memcpy(&cur->_data, &minRight->_data,sizeof(cur->_data));
			//cur->_data = minRight->_data;//当T为pair的时候报错了不知道原因  报错：尝试引用已删除的函数
			if (minRight->_col == RED && minRight->_right == nullptr)//没有右子树，替换的这个是红色叶子
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
			else if (minRight->_col == BLACK && minRight->_right != nullptr)//存在右子树且不是叶子
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
			else if (minRight->_col == BLACK && minRight->_right == nullptr)//黑色叶子
			{
				realDel = minRight;//记录下来待处理
			}
			else//我认为这种情况不会有
			{
				cout << "存在了不符合要求的情况" << endl;
			}
		}
		if (realDel == nullptr)
		{
			return true;
		}
		//删除黑色叶子
		/*
			1.黑色叶子为根时
			2.兄弟侄子全黑  --> 看父亲
			3.兄弟为黑 侄子不全黑-->看直线/折线
			4.兄弟为红 旋转成情况2
		*/
		if (realDel == _root)
		{
			cout << "已删除根" << endl;
			_root = nullptr;
			return true;
		}
		//不是根节点 黑色叶子肯定有父亲
		Node* del = realDel;//记录待删除的结点  
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
				//兄弟侄子全黑
				if (DelParent->_col == RED)
				{
					//父亲为红
					DelParent->_col = BLACK;
					sibling->_col = RED;
					break;
				}
				else
				{
					//父亲为黑
					sibling->_col = RED;
					realDel = realDel->_parent;
				}

			}
			else if (sibling->_col == BLACK && (!(nephew_right->_col == BLACK && nephew_left->_col == BLACK)))
			{
				//兄弟为黑 侄子不全黑

				if (DelParent->_right == sibling && nephew_right->_col == RED)
				{
					//直线
					sibling->_col = DelParent->_col;
					DelParent->_col = BLACK;
					RotateL(DelParent);
					nephew_right->_col = BLACK;

				}
				else if (DelParent->_left == sibling && nephew_left->_col == RED)
				{
					//直线
					sibling->_col = DelParent->_col;
					DelParent->_col = BLACK;
					RotateR(DelParent);
					nephew_left->_col = BLACK;
				}
				else if (DelParent->_left == sibling && nephew_right->_col == RED)
				{
					//折线
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
					//折线
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
				//兄弟为红
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

		if (delParent->_left == del)//到这里的都是叶子
		{
			delParent->_left = del->_right;

			/*if (del->_right)
			{
				cout << "del->_right非空" << endl;
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
					cout << "del->_right非空" << endl;
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
	const K& operator()(const K& k)//这里不能忘了引用
		//假如没有引用，k就是一个形参，return的k的那块空间就被销毁了，返回值那里就拿不到k
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








