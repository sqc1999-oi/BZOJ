#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#include <cassert>
#include <map>
#include <cmath>
using namespace std;
int exgcd(int a, int b, int &x, int &y)
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	int ret = exgcd(b, a%b, y, x);
	y -= x*(a / b);
	return ret;
}
int inv(int a, int p)
{
	int x, y;
	int d = exgcd(a, p, x, y);
	return d == 1 ? (x%p + p) % p : -1;
}
int bsgs(int a, int b, int p)
{
	map<int, int> m;
	int s = ceil(sqrt(p));
	long long t = 1;
	for (int i = 0; i < s; i++)
	{
		m[t] = i;
		t = (t*a) % p;
	}
	long long t2 = 1;
	for (int i = 0; i < s; i++)
	{
		map<int, int>::iterator it = m.find((long long)b*inv(t2, p) % p);
		if (it != m.end()) return i*s + it->second;
		t2 = (t2*t) % p;
	}
	return -1;
}
int mod_equ(int a, int b, int p)
{
	int x, y;
	int d = exgcd(a, p, x, y);
	if (b%d != 0) return -1;
	p /= d;
	return ((long long)x*(b / d) % p + p) % p;
}
int main()
{
	int T;
	scanf("%d", &T);
	while (T--)
	{
		int p, a, b, x1, t;
		scanf("%d%d%d%d%d", &p, &a, &b, &x1, &t);
		if (x1 == t) puts("1");
		else if (a == 0) puts(b == t ? "2" : "-1");
		else if (a == 1)
		{
			int ans = mod_equ(b, t - x1, p);
			printf("%d\n", ans == -1 ? -1 : ans + 1);
		}
		else
		{
			int c = inv(a - 1, p);
			int x = mod_equ((x1 + (long long)b*c) % p, (t + (long long)b*c) % p, p);
			int ans = bsgs(a, x, p);
			printf("%d\n", ans == -1 ? -1 : ans + 1);
		}
	}
}
