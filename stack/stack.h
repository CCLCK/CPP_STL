#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
using namespace std;

namespace ck
{
	template<class T,class Container=std::deque<T>>
	class stack
	{
	public:
		bool empty()
		{
			return _con.empty();
		}
		void push(const T& x)
		{
			return _con.push_back(x);
		}
		size_t size()
		{
			return _con.size();
		}
		const T& top()
		{
			return _con.back();
		}
		void pop()
		{
			return _con.pop_back();
		}

	private:
		Container _con;
	};
}
void test1()
{
	ck::stack<int>st;
	cout << st.empty() << endl;
	st.push(1);
	st.push(2);
	st.push(3);
	cout << st.top() << endl;
	st.pop();
	cout << st.top() << endl;
	cout << st.size() << endl;
	
}





