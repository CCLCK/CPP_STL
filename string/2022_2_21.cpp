#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<string>
#include<algorithm>
using namespace std;


//int main()
//{
//	printf("%d\n", isalpha('A'));//1
//	printf("%d\n", isalpha('a'));//2
//	printf("%d\n", isalpha('1'));//0  大写返回1 小写返回2 非字母返回0
//	return 0;
//}


//class Solution {
//public:
//	bool isPalindrome(string s) {
//		if (s.size() == 0)
//		{
//			return true;
//		}
//		string tmp;
//		int len = s.size();
//		for (int i = 0; i < len; i++)
//		{
//			if (isalpha(s[i])==1)
//			{
//				tmp += (s[i] + 32);
//			}
//			else if (isalpha(s[i]) == 2||isdigit(s[i]))
//			{
//				tmp += s[i];
//			}
//		}
//		int l = 0;
//		int r = tmp.size()-1;
//		bool f = true;
//		while (l <= r)
//		{
//			if (tmp[l] != tmp[r])
//			{
//				f = false;
//				break;
//			}
//			l++;
//			r--;
//		}
//		return f;
//	}
//};
class Solution {
public:
	string addStrings(string num1, string num2) {
		int len1 = num1.size();
		int len2 = num2.size();
		string longnum = num1;
		string shortnum = num2;
		if (len1 < len2)
		{
			longnum = num2;
			shortnum = num1;
		}
		int x = 0;
		string ans;
		int cnt = longnum.size();
		int l = longnum.size();
		int  t= shortnum.size();
		ans.reserve(longnum.size() + 1);		
		while (t)
		{
			 x += longnum[l - 1] + shortnum[t - 1]-'0'-'0';
			 ans+= (x % 10 + '0');
			 x /= 10;
			 l--; t--;
		}
		int y = 0;
		while (l)
		{
			y += (x)+longnum[l - 1]-'0';
			x = 0;
			ans += (y % 10 + '0');
			y /= 10;
			l--;
		}
		if (y)
		{
			ans += '1';
		}
		int left = 0, right = ans.size()-1;
		while (left < right)
		{
			swap(ans[left], ans[right]);
			left++;
			right--;
		}
		return ans;
	}
};
int main()
{
	cout << Solution().addStrings("9","9999991") << endl;
	return 0;
}














