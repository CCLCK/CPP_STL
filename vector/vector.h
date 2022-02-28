#include<iostream>
#include<algorithm>
#include<vector>
#include<assert.h>
#include<cstring>
using namespace std;


namespace ck
{
	template<class T>
	//ģ�庯���������Ͷ������д��һ�� ��Ȼ���ܳ������Ӵ���
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
			//���const����Ҫ��  const���γɳ������thisָ��ı��γ�����
		{
			return _start;
		}
		iterator end()
		{
			return _finish;
		}
		const_iterator end() const//const����ʡ
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
			//ǰ����Ҫ��ʼ��һ�� ��������ǰ��֪��sizeΪ0 push_back�����źÿ��ٿռ�
			while (first != last)
			{
				push_back(*first);
				first++;
			}
		}
		//vector(size_t n, const T val)
		
		//{//������캯��д�˿��ܻ��vector(InputIterator first, InputIterator last)��ͻ
		//	reserve(n);
		//	//�ɴ�дһ��push_back��ʵ��
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
			//��ͳд����Ҫ���ռ���Ҫ����֮��� ʮ���鷳
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
			
			//����Ҳ�����������Ҫ����
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
			return pos;//����ɾ��Ԫ�ص���һ��λ�� 
			//ע��۲�pos��λ�þ���ɾ֮ǰ�Ǹ�Ԫ�ص���һ��λ��
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
			//ע��ɾ������ɾ��������
			/*assert(!empty());
			_finish--;*/
			erase(end()-1);
		}
		T& operator[](size_t n)
		{
			assert(n < size());
			return *(_start + n);//����Ҳ����д��_start[n]������������ݹ�
			//��Ϊ_start��ԭ��ָ�룬������������[]
		}
		size_t size() const //�ĵ���ô�����,ȷʵconst���θ���һ��
		{
			return _finish - _start;
		}
		size_t capacity()const //�ĵ���ô�����,ȷʵconst���θ���һ��
		{
			return _endofstorage - _start;
		}
		void resize(size_t n,const T& val=T())
		//���TΪintʱ��val=int()��ʵ����0
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
			//size()�Ĵ�С ����ָ�����
			//����һ�ݴ�С,��Ϊ���ٿռ�� ����_start���� ��_finish��û���ü���
			//��ʱ��ȥ����size()�����������bug
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
//�����������ã���Ϊ�������õ���ǳ��������ʱ�һ�ûд�������죩
//�������һ��ռ�delete����
{
	for (auto e : v)
	{
		cout << e << " ";
	}
	cout << endl;

}