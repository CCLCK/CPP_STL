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
		const_iterator end() const//const不能省
		{
			return _finish;
		}
		vector()
			:_start(nullptr)
			, _finish(nullptr)
			, _endofstorage(nullptr)
		{

		}
		template<class InputIterator>
		vector(InputIterator first, InputIterator last)
			:_start(nullptr)
			, _finish(nullptr)
			, _endofstorage(nullptr)
		{
			//前面需要初始化一下 这样插入前才知道size为0 push_back函数才好开辟空间
			while (first != last)
			{
				push_back(*first);
				first++;
			}
		}
		//vector(size_t n, const T val)
		
		//{//这个构造函数写了可能会和vector(InputIterator first, InputIterator last)冲突
		//	reserve(n);
		//	//干脆写一个push_back来实现
		//	int tmp = n;
		//	while (tmp--)
		//	{
		//		push_back(val);
		//	}
		//}

		vector(const vector<T>& v)
			:_start(nullptr)
			, _finish(nullptr)
			, _endofstorage(nullptr)
		{
			vector<T>tmp(v.begin(),v.end());
			swap(tmp);
		}
		vector<T>& operator=(vector<T> x)
		{
			//传统写法又要开空间又要拷贝之类的 十分麻烦
			swap(x);
			return *this;
		}
		void swap(vector& v)
		{
			::swap(_start, v._start);
			::swap(_finish, v._finish);
			::swap(_endofstorage, v._endofstorage);
		}
		~vector()
		{
			delete[] _start;
			_start = nullptr;
			_finish = nullptr;
			_endofstorage = nullptr;
		}
		iterator insert(iterator pos, const T&val)
		{
			
			//插入也不能随便插入得要断言
			assert(pos >= begin() && pos <= end());
			if (_finish == _endofstorage)
			{	
				int ret = pos - _start;
				int cap = capacity();
				size_t newcapacity = (cap == 0 ? 4 : 2 * cap);
				reserve(newcapacity);
				pos = _start + ret;
			}
			
			iterator x = _finish;
			while (x >= pos+1)
			{
				*x = *(x - 1);
				x--;
			}
			*pos = val;
			_finish++;
			return pos;
		}
		iterator erase(iterator pos)
		{
			assert(pos >= begin() && pos < end());
			iterator x = pos + 1;
			while (x < _finish)
			{
				*(x - 1) = *x;
				x++;
			}
			_finish--;
			return pos;//返回删除元素的下一个位置 
			//注意观察pos的位置就是删之前那个元素的下一个位置
		}
		void push_back(const T&val)
		{
			/*if (_finish == _endofstorage)
			{
				int cap = capacity();
				size_t newcapacity = (cap == 0 ? 4 : 2 * cap);
				reserve(newcapacity);
			}
			*(_finish) = val;
			_finish++;*/
			insert(end(), val);
		}
		bool empty()
		{
			return (size() == 0);
		}
		void pop_back()
		{
			//注意删除不能删除空容器
			/*assert(!empty());
			_finish--;*/
			erase(end()-1);
		}
		T& operator[](size_t n)
		{
			assert(n < size());
			return *(_start + n);//这里也可以写成_start[n]，不会造成死递归
			//因为_start是原生指针，不会调用这里的[]
		}
		size_t size() const //文档这么定义的,确实const修饰更好一点
		{
			return _finish - _start;
		}
		size_t capacity()const //文档这么定义的,确实const修饰更好一点
		{
			return _endofstorage - _start;
		}
		void resize(size_t n,const T& val=T())
		//如果T为int时，val=int()其实就是0
		{
			//n<=sz  n>sz&&n<=cap  n>cap
			if (n <= size())
			{
				_finish = _start + n;
			}
			else 
			{
				if (n > capacity())
				{
					reserve(n);
				}
				while (_finish < _start + n)
				{
					*_finish = val;
					_finish++;
				}
			}
		}
		void reserve(size_t n)
		{
			size_t sz = size();
			//size()的大小 两个指针相减
			//拷贝一份大小,因为开辟空间后 可能_start变了 而_finish还没来得及变
			//此时若去调用size()函数则造成了bug
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

void print(ck::vector<int>& v)
//这里必须加引用，因为不加引用等于浅拷贝（此时我还没写拷贝构造）
//会造成用一块空间delete两次
{
	for (auto e : v)
	{
		cout << e << " ";
	}
	cout << endl;

}