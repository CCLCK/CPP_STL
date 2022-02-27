#include<iostream>
#include<algorithm>
#include<vector>
#include<assert.h>
#include<cstring>
using namespace std;


namespace ck
{
	template<class T>
	//模板函数的声明和定义最好写在一起 不然可能出现链接错误
	class vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;
		iterator begin()
		{
			return _start;
		}
		const_iterator begin() const
			//这个const必须要加  const修饰成常对象后this指针改变形成重载
		{
			return _start;
		}
		iterator end()
		{
			return _finish;
		}
		vector()
			:_start(nullptr)
			, _finish(nullptr)
			, _endofstorage(nullptr)
		{

		}
		void test(int a);
	
		vector(size_t n, const T val)
		{
			reserve(n);
			//干脆写一个push_back来实现
			

		}

		size_t size()
		{
			return _finish - _start;
		}
		size_t capacity()
		{
			return _endofstorage - _endofstorage;
		}
		void reserve(size_t n)
		{
			size_t sz = size();
			T* tmp = new T[n];
			memcpy(tmp, _start,sz*sizeof(T));
			delete[] _start;
			_start = tmp;
			_finish = _start + sz;
			_endofstorage = _start + n;
		}
	private:
		T* _start;
		T* _finish;
		T* _endofstorage;
	};

}

void test()
{
	/*ck::vector<int>v;
	v.reserve(4);*/
}