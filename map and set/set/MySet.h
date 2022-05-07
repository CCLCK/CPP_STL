#pragma once



#include "RBTree.h"


namespace ck
{
	template<class K>
	class set
	{

	public:
		
		struct setKeyOfT
		{
			const K& operator()(const K& k)//���ﲻ����������
				//����û�����ã�k����һ���βΣ�return��k���ǿ�ռ�ͱ������ˣ�����ֵ������ò���k
			{
				return k;
			}
		};
		typedef typename RBTree<K, K, setKeyOfT>::iterator iterator;
		iterator begin()
		{
			return _rbt.begin();
		}
		iterator end()
		{
			return _rbt.end();
		}
		pair<iterator, bool> insert(const K& key)
		{
			return _rbt.Insert(key);
		}
		size_t erase(const K& key)
		{
			return _rbt.Erase(key);
		}
	
	private:
		RBTree<K, K, setKeyOfT> _rbt;//����ʱΪʲô����֪��setKeyOfT�����ͣ����紫�����int����Ϊ����һ��ģ�����K������setKeyOfT����౻Ψһȷ����
	};
	void test_set1()
	{
		set<int>s;
		srand(time(0));
		s.insert(2);
		s.insert(1);
		s.insert(3);
		s.insert(4);
		s.insert(6);
		s.insert(5);
		s.insert(7);
		s.insert(8);
		for (auto e : s)
		{
			cout << e << endl;
		}
		s.erase(1);
		s.erase(2);
		s.erase(3);
		s.erase(4);
		s.erase(5);
		s.erase(6);
		s.erase(7);
		s.erase(8);
		for (auto e : s)
		{
			cout << e << endl;
		}
	}
	void test_set2()
	{
		set<int>s;
		srand(time(0));
		const int n = 100;
		for (int i = 0; i < n; i++)
		{
			s.insert(rand());
		}
		/*for (auto e : s)
		{
			������ô���Ի���ֵ�����ʧЧ�����
			s.erase(e);
		}*/
		for (auto e : s)
		{
			cout << e << endl;
		}
	}

}



