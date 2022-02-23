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
		//	:_str(new char[1])//Ϊʲô����Ҫ����1��  ����Ϊ�˷�'\0' 
		//	//��Ȼsize()�ͻ�����ÿ�ָ��
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
		const_iterator begin() const//ע���������ֵ  ������ķ���ֵ��������Ϊ�ɶ�
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
		string& operator=(string s)//���صĲ���void ����string&
			//�����stringΪʲô������const���α��� ��Ϊʵ���ǽ����˽���
			//�������const�����������޸�const ���Իᱨ��
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
		string(char* str="")
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
			
			for (size_t i = _size+1; i > pos; i--)//����һ���µķ�������
				//���߰�i>pos д��(int)i>(int)pos Ҳ�� ��Ϊ����ı�����size_tû�и��� ��-1����INT_MAX*2+1
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
				return *this;//��ֹ����bug
			}
			if (_size + len > _capacity)
			{
				reserve(_size + len);
			}
			for (size_t i = _size + len; i >= pos+len; i--)
			{
				//�ܲ���д��for (size_t i = _size + len; i-len >= pos; i--)
				//���Բ��а�  i-len�����bug ����i-len==0>=pos(0) ��ִ��һ��i�ͱ��-1��size_tȫ1���
				//�͵�������ѭ��  ������i>=pos+len�Ͳ������������ ��Ϊi����Ϊ0  ǰ���len==0����������˴���
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
		string& erase(size_t pos, size_t len=npos)
		{

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





