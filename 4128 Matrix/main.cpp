#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#include <map>
#include <cstring>
#include <cassert>
#include <cmath>
using namespace std;
struct matrix
{
private:
	int n, m, p;
	int **data;
public:
	matrix(int n, int m, int p) :n(n), m(m), p(p)
	{
		data = new int *[n];
		for (int i = 0; i < n; i++)
		{
			data[i] = new int[m];
			memset(data[i], 0x00, sizeof(int)*m);
		}
	}
	matrix(const matrix &a) :n(a.n), m(a.m), p(a.p)
	{
		data = new int *[n];
		for (int i = 0; i < n; i++)
		{
			data[i] = new int[m];
			memcpy(data[i], a[i], sizeof(int)*m);
		}
	}
	int *const operator [](int k) const { return data[k]; }
	matrix operator *(const matrix &a) const
	{
		assert(m == a.n && p == a.p);
		matrix ret(n, a.m, p);
		for (int k = 0; k < m; k++)
			for (int i = 0; i < n; i++)
				for (int j = 0; j < a.m; j++)
					ret[i][j] = (ret[i][j] + (long long)data[i][k] * a[k][j]) % p;
		return ret;
	}
	bool operator <(const matrix &a) const
	{
		assert(n == a.n && m == a.m && p == a.p);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
			{
				if (data[i][j] < a[i][j]) return true;
				else if (data[i][j] > a[i][j]) return false;
			}
		return false;
	}
	int get_n() const { return n; }
	int get_m() const { return m; }
	int get_p() const { return p; }
};
int bsgs(const matrix &a, const matrix &b, int p)
{
	assert(a.get_n() == a.get_m() && a.get_n() == b.get_n() && a.get_m() == b.get_m() && a.get_p() == b.get_p());
	map<matrix, int> m;
	int s = ceil(sqrt(p));
	matrix t(a.get_n(), a.get_m(), a.get_p());
	for (int i = 0; i < a.get_n(); i++) t[i][i] = 1;
	for (int i = 0; i < s; i++)
	{
		m[t*b] = i;
		t = t*a;
	}
	matrix t2 = t;
	for (int i = 1; i <= s; i++)
	{
		map<matrix, int>::iterator it = m.find(t2);
		if (it != m.end()) return i*s - it->second;
		t2 = t2*t;
	}
	return -1;
}
int main()
{
	int n, p;
	scanf("%d%d", &n, &p);
	matrix a(n, n, p), b(n, n, p);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			scanf("%d", &a[i][j]);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			scanf("%d", &b[i][j]);
	printf("%d", bsgs(a, b, p));
}
