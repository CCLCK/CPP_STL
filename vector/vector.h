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
			//�ɴ�дһ��push_back��ʵ��
			

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