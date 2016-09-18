#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cctype>
#include <vector>
#include <algorithm>
#include <queue>
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
const int N = 2e5, K = 18;
int g[N], c[N], cnt, dis[N], p[N], f[N][K + 1], dep[N], max[N][K + 1];
bool inq[N];
struct data
{
	int u, v, w;
	data(int u, int v, int w) :u(u), v(v), w(w) {}
};
struct edge
{
	int to, pow, next;
	edge(int to, int pow, int next) :to(to), pow(pow), next(next) {}
	edge() = default;
} e[N * 2];
void add_edge(int u, int v, int w)
{
	e[cnt] = edge(v, w, g[u]);
	g[u] = cnt++;
}
bool cmp(const data &a, const data &b) { return a.w < b.w; }
int getf(int x) { return x == p[x] ? x : p[x] = getf(p[x]); }
void dfs(int u)
{
	for (int i = 1; i <= K; i++)
	{
		f[u][i] = f[f[u][i - 1]][i - 1];
		max[u][i] = std::max(max[u][i - 1], max[f[u][i - 1]][i - 1]);
	}
	for (int i = g[u]; i != -1; i = e[i].next)
	{
		int v = e[i].to;
		if (v != f[u][0])
		{
			f[v][0] = u;
			max[v][0] = e[i].pow;
			dep[v] = dep[u] + 1;
			dfs(v);
		}
	}
}
int main()
{
	int n, s, m;
	read(3, &n, &s, &m);
	for (int i = 0; i < s; i++) read(1, &c[i]);
	memset(g, 0xff, sizeof g);
	for (int i = 0; i < m; i++)
	{
		int u, v, d;
		read(3, &u, &v, &d);
		u--, v--;
		add_edge(u, v, d);
		add_edge(v, u, d);
	}
	std::queue<int> q;
	memset(dis, 0x3f, sizeof dis);
	for (int i = 0; i < s; i++)
	{
		c[i]--;
		q.push(c[i]);
		dis[c[i]] = 0;
		inq[c[i]] = true;
	}
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		inq[u] = false;
		for (int i = g[u]; i != -1; i = e[i].next)
		{
			int v = e[i].to, tmp = dis[v];
			dis[v] = std::min(dis[v], dis[u] + e[i].pow);
			if (tmp != dis[v] && !inq[v])
			{
				q.push(v);
				inq[v] = true;
			}
		}
	}
	std::vector<data> v;
	for (int i = 0; i < n; i++)
		for (int j = g[i]; j != -1; j = e[j].next)
			v.push_back(data(i, e[j].to, dis[i] + e[j].pow + dis[e[j].to]));
	sort(v.begin(), v.end(), cmp);
	cnt = 0;
	memset(g, 0xff, sizeof g);
	for (int i = 0; i < n; i++) p[i] = i;
	for (int i = 0; i < v.size(); i++)
	{
		int a = v[i].u, b = v[i].v, x = getf(a), y = getf(b);
		if (x != y)
		{
			p[x] = y;
			add_edge(a, b, v[i].w);
			add_edge(b, a, v[i].w);
		}
	}
	for (int i = 0; i < n;i++)
		if (dep[i] == 0)
			dfs(i);
	int t;
	read(1, &t);
	for (int i = 0; i < t; i++)
	{
		int x, y, b;
		read(3, &x, &y, &b);
		x--, y--;
		if (getf(x) != getf(y))
		{
			printf("NIE\n");
			continue;
		}
		if (dep[x] < dep[y]) std::swap(x, y);
		int max = 0;
		for (int j = K; j >= 0; j--)
			if (dep[f[x][j]] >= dep[y])
			{
				max = std::max(max, ::max[x][j]);
				x = f[x][j];
			}
		for (int j = K; j >= 0; j--)
			if (f[x][j] != f[y][j])
			{
				max = std::max(max, std::max(::max[x][j], ::max[y][j]));
				x = f[x][j];
				y = f[y][j];
			}
		if (x != y) max = std::max(max, std::max(::max[x][0], ::max[y][0]));
		printf("%s\n", max <= b ? "TAK" : "NIE");
	}
}
