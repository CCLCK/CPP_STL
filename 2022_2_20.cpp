#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

//
//class Solution {
//public:
//	string reverseOnlyLetters(string s) {
//		string::iterator left = s.begin();
//		string::iterator right = s.end()-1;
//		while (left<s.end() && right >= s.begin()&&left <= right)
//		{
//			while (left<s.end() && left <= right&&!isalpha(*left)) left++;
//			while (right >= s.begin() && left <= right&&!isalpha(*right)) right--;
//			if (left<s.end() && right >= s.begin()&&isalpha(*left) && isalpha(*right))
//			{
//				swap(*left, *right);
//				left++;
//				right--;
//			}	
//		}
//		
//		return s;
//		
//	}
//};
//int main()
//{
//	Solution sl;
//	sl.reverseOnlyLetters("ab-cd");
//	return 0;
//}

//
//int main()
//{
//	string s = "hello world";
//	
//	s.reserve(25);
//	cout << s.capacity() << endl;
//	cout << s << endl;
//
//	return 0;
//}


//class Solution {
//public:
//	int firstUniqChar(string s) {
//		int a[30] = { 0 };
//		int reco[30] = { 0 };
//		int len = s.size();
//		for (int i = 0; i < len; i++)
//		{
//			a[s[i] - 'a']++; 
//		}
//		int ans = 2147483647;
//		for (int i = 0; i < 26; i++)
//		{
//			if (a[i]==1)
//			{
//				int tmp = s.find(i+'a');
//				if (tmp < ans)
//				{
//					ans = tmp;
//				}
//			}
//		}
//		
//	
//		return ans;
//	}
//};

class Solution {
public:
	int lengthOfLastWord(string s) {
		int len = s.size() ;
		int x = len-1;
		while (x >= 0 && s[x] == ' ')
		{
			x--;
		}
		int y = x;
		int cnt = 0;
		while (y >= 0 && s[y] != ' ')
		{
			cnt++;
			y--;
		}
		return cnt;
	}
};
int main()
{
	Solution sl;
	cout << sl.lengthOfLastWord("hello nowcoder") << endl;
	return 0;
}


