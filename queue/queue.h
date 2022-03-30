#include <algorithm>
#include <iostream>
#include <deque>
using namespace std;

namespace ck
{
	template<class T, class Container = std::deque<T>>
	class queue
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
		const T& front()
		{
			return _con.front();
		}
		const T& back()
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


