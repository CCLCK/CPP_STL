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
		//string()
		//	:_str(new char[1])//为什么这里要设置1个  就是为了放'\0' 
		//	//不然size()就会解引用空指针
		//	, _size(0)
		//	, _capacity(0)
		//{
		//	*_str = '\0';
		//}
		typedef char* iterator;
		typedef const char* const_iterator;
		iterator begin()
		{
			return _str;
		}
		iterator end()
		{
			return _str + _size;
		}	
		const_iterator begin() const//注意这个返回值  常对象的返回值常常设置为可读
		{
			return _str;
		}
		const_iterator end() const
		{
			return _str + _size;
		}
		void swap(string& s)
		{
			::swap(_str, s._str);
			::swap(_size, s._size);
			::swap(_capacity, s._capacity);
		}
		string& operator=(string s)//返回的不是void 而是string&
			//这里的string为什么不能用const修饰保护 因为实现是借助了交换
			//如果交换const常量不就是修改const 所以会报错
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
		string(char* str="")
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

		char& operator[](size_t pos)
		{
			assert(pos < _size);
			return _str[pos];
		}
		const char& operator[](size_t pos)const
		{
			assert(pos < _size);
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
		void push_back(char ch)
		{
			/*if (_size >= _capacity)
			{
				size_t newcapacity = (_capacity == 0 ? 4 : 2 * _capacity);
				reserve(newcapacity);
				_capacity = newcapacity;
			}
			_str[_size] = ch; _size++;
			_str[_size] = '\0'; */
			insert(_size, ch);
		}
		void append(char* str)
		{	
			/*int len = strlen(str);
			if (len + _size > _capacity)
			{
				reserve(len + _size);
			}
			strcpy((_size + _str), str);
			_size = _size + len;*/
			insert(_size, str);
		}
		size_t find(char c, size_t pos = 0) const
		{
			for (int i = pos; i < _size; i++)
			{
				if (_str[i] == c)
				{
					return i;
				}
			}
			return npos;
		}
		size_t find(const char* s, size_t pos = 0)
		{
			char* ret=strstr(_str, s);
			if (ret == nullptr)
			{
				return npos;
			}
			return ret - _str;
		}
		size_t size()
		{
			return _size;
		}
		size_t size() const
		{
			return _size;
		}
		size_t capacity()
		{
			return _capacity;
		}	
		void clear()
		{
			_str[0] = '\0';
			_size = 0;
		}
		string& insert(size_t pos, char ch)
		{
			assert(pos >= 0 && pos <= _size);
			if (_size + 1 > _capacity)
			{
				reserve(_size + 1);
			}
			
			for (size_t i = _size+1; i > pos; i--)//这是一种新的方法处理
				//或者把i>pos 写成(int)i>(int)pos 也行 因为问题的本质是size_t没有负数 而-1就是INT_MAX*2+1
			{
				_str[i] = _str[i-1];
			}
			_str[pos] = ch;
			_size++;
			return *this;
		}
		
		string& insert(size_t pos, const char* str)
		{
			assert(pos >= 0 && pos <= _size);
			int len = strlen(str);
			if (len == 0)
			{
				return *this;//防止产生bug
			}
			if (_size + len > _capacity)
			{
				reserve(_size + len);
			}
			for (size_t i = _size + len; i >= pos+len; i--)
			{
				//能不能写成for (size_t i = _size + len; i-len >= pos; i--)
				//绝对不行啊  i-len会出现bug 比如i-len==0>=pos(0) 再执行一次i就变成-1（size_t全1最大）
				//就导致了死循环  而这里i>=pos+len就不会有这种情况 因为i不会为0  前面对len==0的情况进行了处理
				_str[i] = _str[i - len];
			}
			int j = 0;
			for (int i = pos; j < len; i++)
			{
				_str[i] = str[j];
				j++;
			}
			_size += len;
			return *this;
		}
		string& erase(size_t pos, size_t len=npos)//删到pos+len-1
		{
			assert(pos >= 0 && pos < _size);
			if (len == 0)
			{
				return *this;
			}
			if (len==npos||pos + len >= _size)//删干净
			{
				_str[pos] = '\0';
				_size = pos+1;
			}
			else
			{
				strcpy(_str + pos, _str + pos + len);
				_size -= len;
			}
			return *this;
		}
		string& operator+=(const char ch)
		{
			push_back(ch);
			return *this;
		}
		string& operator+=(char* str)
		{
			append(str);
			return *this;
		}
		string& operator+=(string& s)
		{
			append(s._str);
			return *this;
		}
	private:
		char* _str;
		size_t _size;
		size_t _capacity;
		static const size_t npos;
	};
	const size_t string::npos = -1;
	//这个东西必须要加上类型size_t，必须在命名空间内 不然会造成意义不明确
	string operator+(const string&s1, char ch)
	{
	//存在深拷贝 尽量少用
	string ret = s1;
	ret += ch;
	return ret;
	}
	string operator+(const string&s1, char* str)
	{
	string ret = s1;//开一块临时空间
	ret += str;//在这块临时空间处理
	return ret;
	}
	ostream& operator<<(ostream& out, const string& s)//这个更加要注意返回值和参数类型
	{
		for (size_t i = 0; i < s.size(); ++i)
		{
			out << s[i];
		}
		return out;//连续使用的时候必须有这个返回值
	}
	istream& operator>>(istream& in,string& s)
	{
		char ch =in.get();//这里不用getchar，用流对象
		//in>>ch这个不行 因为in本身碰到空格或者换行就停止了
		while (ch != ' '&& ch != '\n')
		{
			s += ch;
			ch=in.get();
		}
		return in;
	}
	istream& getline(istream& in, string& s)
	{
		char ch = in.get();//这里不用getchar，用流对象
		//in>>ch这个不行 因为in本身碰到空格或者换行就停止了
		while (ch != '\n')
		{
			s += ch;
			ch = in.get();
		}
		return in;
	}
	//> =
	bool operator>(const string& s1, const string& s2)
	{
		int len1 = s1.size();
		int len2 = s2.size();
		int i = 0;
		while (i < len1&&i < len2)
		{
			if (s1[i]<s2[i])
			{
				return false;
			}
			else if (s1[i]>s2[i])
			{
				return true;
			}
			else
			{
				;
			}
			i++;
		}
		if (i != len1&&i == len2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator==(const string&s1, const string& s2)
	{
		int len1 = s1.size();
		int len2 = s2.size();
		int i = 0;
		while (i < len1&&i < len2)
		{
			if (s1[i] != s2[i])
			{
				return false;
			}
			i++;
		}
		if (i == len1&&i == len2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	inline bool operator>=(const string&s1, const string& s2)
	{
		return ((s1 > s2) || (s1 == s2));
	}
	inline bool operator<=(const string&s1, const string& s2)
	{
		return !(s1 > s2);
	}
	inline bool operator!=(const string&s1, const string& s2)
	{
		return !(s1 == s2);
	}
	inline bool operator<(const string&s1, const string& s2)
	{
		return !(s1>=s2);
	}
}