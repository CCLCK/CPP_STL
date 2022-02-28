#define _CRT_SECURE_NO_WARNINGS 1

#include"vector.h"



int main()
{
	ck::vector<int>v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	ck::vector<int>v2(v.begin(), v.end());
	print(v2);
	
	v2.resize(6);
	print(v2);
	cout << v2.size()<<endl;

	ck::vector<int>v3(v2);
	print(v3);

	ck::vector<int>v4;
	v4 = v3;
	v4.pop_back();
	v4.pop_back();
	v4.pop_back();
	cout << v4.size() << endl;
	print(v4);


	ck::vector<int>v5;
	v5.insert(v5.begin(), 1);
	v5.insert(v5.begin(), 2);
	v5.insert(v5.begin(), 3);
	v5.insert(v5.begin(), 4);
	v5.erase(v5.begin());
	v5.erase(v5.begin());
	v5.erase(v5.begin());
	v5.erase(v5.begin());
	cout << v5.size() << endl;
	print(v5);



	ck::vector<int>v6;
	v6.push_back(1);
	v6.push_back(2);
	v6.push_back(2);
	v6.push_back(3);
	v6.push_back(4);
	print(v6);

	auto it = v6.begin();
	while (it < v6.end())
	{
		if (*it % 2 == 0)
		{
			v6.erase(it);
		}
		else
		{
			it++;
		}
	}
	print(v6);
	return 0;
}





