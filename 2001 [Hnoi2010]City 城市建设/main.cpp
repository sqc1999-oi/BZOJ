#define _SCL_SECURE_NO_WARNINGS
#include <cstdio>
#include <climits>
#include <cstdarg>
#include <cctype>
#include <algorithm>
using namespace std;
void read(int n, ...)
{
	va_list li;
	va_start(li, n);
	while (n--)
	{
		int &x = *va_arg(li, int *), ch;
		x = 0;
		do ch = getchar();
		while (!isdigit(ch));
		do
		{
			x = x * 10 + ch - '0';
			ch = getchar();
		} while (isdigit(ch));
	}
	va_end(li);
}
const int N = 2e4, M = 5e4, Q = 5e4, K = 16;
struct edge { int u, v, id; } e[16][M], t[M];
struct query { int k, w; } q[Q];
long long ans[Q];
int w[M], f[N], nw[M];
bool cmp(const edge &a, const edge &b) { return (nw[a.id] == 0 ? w[a.id] : nw[a.id]) < (nw[b.id] == 0 ? w[b.id] : nw[b.id]); }
void init_set(int n, edge *e)
{
	for (int i = 0; i < n; i++)
	{
		f[e[i].u] = e[i].u;
		f[e[i].v] = e[i].v;
	}
}
int find(int x) { return x == f[x] ? x : f[x] = find(f[x]); }
void contraction(int &m, edge *e, long long &v)
{
	int cnt = 0;
	sort(e, e + m, cmp);
	init_set(m, e);
	for (int i = 0; i < m; i++)
	{
		int x = find(e[i].u), y = find(e[i].v);
		if (x != y)
		{
			f[x] = y;
			if (nw[e[i].id] != INT_MIN) t[cnt++] = e[i];
		}
	}
	init_set(m, e);
	for (int i = 0; i < cnt; i++)
	{
		f[find(t[i].u)] = find(t[i].v);
		v += w[t[i].id];
	}
	cnt = 0;
	for (int i = 0; i < m; i++)
	{
		int x = find(e[i].u), y = find(e[i].v);
		if (x != y)
		{
			t[cnt].u = x;
			t[cnt].v = y;
			t[cnt].id = e[i].id;
			cnt++;
		}
	}
	m = cnt;
	copy(t, t + m, e);
}
void reduction(int &m, edge *e)
{
	int cnt = 0;
	sort(e, e + m, cmp);
	init_set(m, e);
	for (int i = 0; i < m; i++)
	{
		int x = find(e[i].u), y = find(e[i].v);
		if (x != y)
		{
			f[x] = y;
			t[cnt++] = e[i];
		}
		else if (nw[e[i].id] == INT_MAX) t[cnt++] = e[i];
	}
	m = cnt;
	copy(t, t + m, e);
}
void solve(int l, int r, long long v, int m, int dep)
{
	if (l == r - 1)
	{
		w[q[l].k] = q[l].w;
		ans[l] = v;
		sort(e[dep], e[dep] + m, cmp);
		init_set(m, e[dep]);
		for (int i = 0; i < m; i++)
		{
			int x = find(e[dep][i].u), y = find(e[dep][i].v);
			if (x != y)
			{
				f[x] = y;
				ans[l] += w[e[dep][i].id];
			}
		}
		return;
	}
	for (int i = l; i < r; i++) nw[q[i].k] = INT_MIN;
	contraction(m, e[dep], v);
	for (int i = l; i < r; i++) nw[q[i].k] = INT_MAX;
	reduction(m, e[dep]);
	for (int i = l; i < r; i++) nw[q[i].k] = 0;
	int mid = (l + r) / 2;
	copy(e[dep], e[dep] + m, e[dep + 1]);
	solve(l, mid, v, m, dep + 1);
	copy(e[dep], e[dep] + m, e[dep + 1]);
	solve(mid, r, v, m, dep + 1);
}
int main()
{
	int n, m, q;
	read(3, &n, &m, &q);
	for (int i = 0; i < m; i++)
	{
		read(3, &e[0][i].u, &e[0][i].v, &w[i]);
		e[0][i].u--, e[0][i].v--;
		e[0][i].id = i;
	}
	for (int i = 0; i < q; i++)
	{
		read(2, &::q[i].k, &::q[i].w);
		::q[i].k--;
	}
	solve(0, q, 0, m, 0);
	for (int i = 0; i < q; i++) printf("%lld\n", ans[i]);
}
