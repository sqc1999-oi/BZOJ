#include <vector>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <algorithm>
#define M 15
using namespace std;

//typedef __float128 ld; //AC
typedef long double ld; //WA
typedef vector<long long> Poly;

int n, m, a[M];

Poly pow_1_x[M * M], f[1 << 10];

Poly operator +(const Poly &x, const Poly &y)
{
	int i;
	Poly re(max(x.size(), y.size()), 0);
	for (i = 0; i < x.size(); i++)
		re[i] += x[i];
	for (i = 0; i < y.size(); i++)
		re[i] += y[i];
	return re;
}

Poly operator -(const Poly &x, const Poly &y)
{
	int i;
	Poly re(max(x.size(), y.size()), 0);
	for (i = 0; i < x.size(); i++)
		re[i] += x[i];
	for (i = 0; i < y.size(); i++)
		re[i] -= y[i];
	return re;
}

Poly operator *(const Poly &x, const Poly &y)
{
	int i, j;
	Poly re(x.size() + y.size() - 1, 0);
	for (i = 0; i < x.size(); i++)
		for (j = 0; j < y.size(); j++)
			re[i + j] += x[i] * y[j];
	return re;
}

ld Integrate(const Poly &x)
{
	int i;
	ld re = 0;
	for (i = 0; i < x.size(); i++)
		re += (ld)x[i] / (i + 1);
	return re;
}

int Count(int x)
{
	int re = 0;
	for (; x; x -= x & -x)
		re++;
	return re;
}

int main()
{
	int i, j, k, x, y;
	cin >> n >> m;
	for (i = 1; i <= m; i++)
	{
		scanf("%d%d", &x, &y);
		a[x] |= 1 << y - 1;
		a[y] |= 1 << x - 1;
	}
	new(&pow_1_x[0])Poly(1, 1);
	new(&pow_1_x[1])Poly(2, 0);
	pow_1_x[1][0] = 1;
	pow_1_x[1][1] = -1;
	for (i = 2; i <= m; i++)
		pow_1_x[i] = pow_1_x[i - 1] * pow_1_x[1];
	for (i = 1; i < 1 << n; i++)
	{
		new(&f[i])Poly(1, 1);
		for (x = 1; x <= n; x++)
			if (i >> x - 1 & 1)
				break;
		for (j = i ^ (1 << x - 1); j; (--j) &= i ^ (1 << x - 1))
		{
			int temp = i ^ j, cnt = 0;
			for (k = 1; k <= n; k++)
				if (temp >> k - 1 & 1)
					cnt += Count(a[k] & j);
			f[i] = f[i] - f[temp] * pow_1_x[cnt];
		}
	}
	Poly disconnect = Poly(1, 1) - f[(1 << n) - 1];
	ld ans = Integrate(disconnect);
	cout << fixed << setprecision(6) << (double)ans << endl;
	return 0;
}
