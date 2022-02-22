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
			:_str(new char[1])//Ϊʲô����Ҫ����1��  ����Ϊ�˷�'\0' 
			//��Ȼsize()�ͻ�����ÿ�ָ��
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
		string& operator=(string s)//���صĲ���void ����string&
		{
			swap(s);
			return *this;
		}
		string(const string& s)//������������ǳ�������� ����ָ��ʱ�������
			:_str(nullptr)
		{
			string tmp(s._str);
			swap(tmp);
		}
		string(char* str)
			:_size(strlen(str))
			, _capacity(_size)//��ʼ��Ϊ_size
		{
			_str = new char[_capacity+1];//_capacity+1���1�Ǹ�\0��
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
	//�����������Ҫ��������size_t�������������ռ��� ��Ȼ��������岻��ȷ
}





