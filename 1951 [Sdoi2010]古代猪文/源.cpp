#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
using namespace std;
const int P = 999911659, p[] = { 2,3,4679,35617 };
int fact[35617];
void calc_fact(int p, int *fact)
{
	fact[0] = 1;
	for (int i = 1; i < p; i++)
		fact[i] = (long long)fact[i - 1] * i % p;
}
int mod_power(int a, int b, int p)
{
	if (a == 0) return 0;
	long long ret = 1, t = a;
	while (b > 0)
	{
		if (b & 1) ret = (ret * t) % p;
		t = (t * t) % p;
		b >>= 1;
	}
	return ret;
}
int mod_inv(int a, int p)
{
	return mod_power(a, p - 2, p);
}
int mod_choose(int n, int m, int p, const int *fact)
{
	long long ret = 1;
	while (n > 0 && m > 0)
	{
		int tn = n % p, tm = m % p;
		if (tn < tm) return 0;
		ret = ret * fact[tn] % p * mod_inv((long long)fact[tm] * fact[tn - tm] % p, p) % p;
		n /= p;
		m /= p;
	}
	return ret;
}
int crt(int n, const int *a, const int *p)
{
	int m = 1;
	for (int i = 0; i < n; i++) m *= p[i];
	long long ret = 0;
	for (int i = 0; i < n; i++)
	{
		int _m = m / p[i];
		ret = (ret + (long long)a[i] * _m % m * mod_inv(_m, p[i]) % m) % m;
	}
	return ret;
}
int calc(int n, int p, const int *fact)
{
	int sum = 0;
	for (int i = 1; i * i <= n; i++)
		if (n % i == 0)
		{
			sum = (sum + mod_choose(n, i, p, fact)) % p;
			if (i * i != n) sum = (sum + mod_choose(n, n / i, p, fact)) % p;
		}
	return sum;
}
int main()
{
	int n, g;
	scanf("%d%d", &n, &g);
	int sum = 0;
	int a[4];
	for (int i = 0; i < 4; i++)
	{
		calc_fact(p[i], fact);
		a[i] = calc(n, p[i], fact);
	}
	printf("%d", mod_power(g%P, crt(4, a, p), P));
}
