#include<iostream>
#include<string>
#include<assert.h>
#include<algorithm>
using namespace std;

namespace ck
{
	class string
	{
	public:
		string()
			:_str(new char[1])//为什么这里要设置1个  就是为了放'\0' 
			//不然size()就会解引用空指针
			, _size(0)
			, _capacity(0)
		{
			*_str = '\0';
		}
		void swap(string& s)
		{
			::swap(_str, s._str);
			::swap(_size, s._size);
			::swap(_capacity, s._capacity);
		}
		string& operator=(string s)//返回的不是void 而是string&
		{
			swap(s);
			return *this;
		}
		string(const string& s)//拷贝函数分深浅拷贝两种 存在指针时采用深拷贝
			:_str(nullptr)
		{
			string tmp(s._str);
			swap(tmp);
		}
		string(char* str)
			:_size(strlen(str))
			, _capacity(_size)//初始化为_size
		{
			_str = new char[_capacity+1];//_capacity+1里的1是给\0的
			strcpy(_str, str);		
		}
		~string()
		{
			delete[] _str;
			_size = 0;
			_capacity = 0;
			_str = nullptr;
		}
		size_t size()
		{
			return strlen(_str);
		}
		char& operator[](size_t pos)
		{
			return _str[pos];
		}
		void resize(size_t n,char ch='\0')
		{
			assert(n >= 0);
			if (n < _size)
			{
				_str[n] = '\0';
				_size = n;
			}
			else 
			{
				if (n>_capacity)
				{
					reserve(n);
				}
				for (size_t i = _size; i < n; i++)
				{
					_str[i] = ch;
				}
				_str[n] = '\0';
				_size = n;
			}
		
		}
		void reserve(size_t n = 0)
		{
			if (n > _capacity)
			{
				_capacity = n;
				char* tmp=new char[_capacity+1];
				strcpy(tmp, _str);
				delete[] _str;
				_str = tmp;
			}
		}
		const char* c_str()
		{
			return _str;
		}
	private:
		char* _str;
		size_t _size;
		size_t _capacity;
		static const size_t npos;
	};
	const size_t string::npos = -1;
	//这个东西必须要加上类型size_t，必须在命名空间内 不然会造成意义不明确
}





