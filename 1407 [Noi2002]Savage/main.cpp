#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
using namespace std;
const int N = 15;
int c[N], p[N], l[N];

int exgcd(int a, int b, int &x, int &y)
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	int d = exgcd(b, a % b, x, y);
	int t = x;
	x = y;
	y = t - y * (a / b);
	return d;
}

bool check(int n, int m)
{
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
		{
			int x, y, _p = p[i] - p[j], _c = c[j] - c[i];
			if (_p < 0)
			{
				_p *= -1;
				_c *= -1;
			}
			int d = exgcd(_p, m, x, y);
			if (_c % d != 0) continue;
			_c /= d;
			int _m = m / d;
			if ((x * _c % _m + _m) % _m <= min(l[i], l[j])) return false;
		}
	return true;
}

int main()
{
	int n, mcnt = 0;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		scanf("%d%d%d", &c[i], &p[i], &l[i]);
		mcnt = max(mcnt, c[i]);
		c[i]--;
	}
	for (int i = mcnt;; i++)
		if (check(n, i))
		{
			printf("%d\n", i);
			return 0;
		}
}
