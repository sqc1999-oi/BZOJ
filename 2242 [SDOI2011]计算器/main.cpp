#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#include <cassert>
#include <map>
#include <cmath>
using namespace std;
int power(int a, int b, int p)
{
	long long ret = 1, t = a;
	while (b > 0)
	{
		if (b & 1) ret = (ret*t) % p;
		b >>= 1;
		t = (t*t) % p;
	}
	return ret;
}
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
	int t, k;
	scanf("%d%d", &t, &k);
	int(*solve)(int, int, int);
	if (k == 1) solve = power;
	else if (k == 2) solve = mod_equ;
	else solve = bsgs;
	while (t--)
	{
		int a, b, p;
		scanf("%d%d%d", &a, &b, &p);
		int ans = solve(a, b, p);
		if (ans == -1) puts("Orz, I cannot find x!");
		else printf("%d\n", ans);
	}
}
