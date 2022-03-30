#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
using namespace std;



namespace ck
{
	template<class T>
	class Less
	{
	public:
		bool operator()(const T&x, const T & y)
		{
			return x < y;
		}
	};

	template<class T>
	class Greater
	{
	public:
		bool operator()(const T&x, const T& y)
		{
			return x>y;
		}
	};

	template<class T,class Container=vector<T>,class Compare=Greater<int>>
	class priority_queue//默认是建大堆
		//大堆输出为啥是有序的序列（明明没有对其进行排序） 原因：每次弹出队头元素 其实就是在对其进行排序
	{
	public:
		priority_queue(){}
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				_con.push_back(*first);
				first++;
			}
			for (int i = (_con.size() - 1 - 1) / 2; i >= 0; i--)
			{
				AdjustDown(i);
			}

		}
		void AdjustUp(size_t child)
		{
			Compare cmp;
			size_t parent = (child - 1) / 2;
			while (child>0)
			{
				if (cmp(_con[child], _con[parent]))
				{
					std::swap(_con[child], _con[parent]);
					child = parent;
					parent =(child-1)/2;
				}
				else
				{
					break;
				}			
			}
		}
		void AdjustDown(size_t parent)
		{
			size_t child = 2 * parent + 1;
			Compare cmp;
			while (child<_con.size())
			{
				if (child + 1 < _con.size() && !cmp(_con[child], _con[child+1]))
				{
					child++;
				}
				if (cmp(_con[child],_con[parent]))//孩子比父亲大
				{
					std::swap(_con[child], _con[parent]);
					parent = child;
					child = 2 * parent + 1;
				}
				else
				{
					break;
				}
			}

		}
		void push(const T& x)
		{
			_con.push_back(x);
			AdjustUp(_con.size() - 1);
		}
		void pop()
		{
			swap(_con[0], _con[_con.size() - 1]);
			_con.pop_back();
			AdjustDown(0);
		}
		const T& top() const
		{
			return _con[0];
		}
		size_t size()
		{
			return _con.size();
		}
		bool empty()
		{
			return _con.empty();
		}
	private:
		Container _con;
	};
	void test2()
	{
		
		std::vector<int>v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		priority_queue<int,vector<int>,Less<int>>pq(v.begin(),v.end());
		while (!pq.empty())
		{
			cout << pq.top() << " ";
			pq.pop();
		}
		cout << endl;
	}
}






