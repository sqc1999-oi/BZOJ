#include <cstdio>
#include <algorithm>
#include <vector>
#include <climits>
#include <queue>
#include <cstring>
#include <cstdarg>
#include <cctype>
using namespace std;
const int N = 30000;
struct edge
{
	int to, w;
	edge(int to, int w) :to(to), w(w) { }
	bool operator <(const edge &b) const { return to < b.to; }
};
vector<edge> g[N], g2[N];
bool flag[N], inq[N], vis[N];
int sz[N], dep[N], depw[N], dis[N];
long long ans, cnt;
void get_nodes(int u, int fa, vector<int> &vi)
{
	vi.push_back(u);
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i].to;
		if (v != fa && !flag[v])
			get_nodes(v, u, vi);
	}
}
void calc_size(int u, int fa)
{
	sz[u] = 1;
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i].to;
		if (v != fa && !flag[v])
		{
			calc_size(v, u);
			sz[u] += sz[v];
		}
	}
}
void calc_dep(int u, int fa)
{
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i].to;
		if (v != fa && !flag[v])
		{
			dep[v] = dep[u] + 1;
			calc_dep(v, u);
		}
	}
}
void calc_depw(int u, int fa)
{
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i].to;
		if (v != fa && !flag[v])
		{
			depw[v] = depw[u] + g[u][i].w;
			calc_depw(v, u);
		}
	}
}
void get_focus(int u, int fa, int n, int &ans, int &min)
{
	int max = n - sz[u];
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i].to;
		if (v != fa && !flag[v])
		{
			max = ::max(max, sz[v]);
			get_focus(v, u, n, ans, min);
		}
	}
	if (max < min)
	{
		min = max;
		ans = u;
	}
}
void calc_ans(int u, int k)
{
	static int f[N + 1][2], g[N + 1][2];
	f[0][1] = 1;
	for (int i = 1; i <= k; i++) f[i][0] = f[i][1] = 0;
	for (int l = 0; l < ::g[u].size(); l++)
	{
		int x = ::g[u][l].to;
		if (flag[x]) continue;
		vector<int> v;
		get_nodes(x, u, v);
		for (int i = 1; i <= k; i++) g[i][0] = g[i][1] = 0;
		for (int i = 0; i < v.size(); i++)
		{
			if (depw[v[i]] > g[dep[v[i]]][0])
			{
				g[dep[v[i]]][0] = depw[v[i]];
				g[dep[v[i]]][1] = 0;
			}
			if (depw[v[i]] == g[dep[v[i]]][0]) g[dep[v[i]]][1]++;
		}
		for (int j = 1; j <= k; j++)
		{
			if (g[j][1] == 0 || f[k - j - 1][1] == 0) continue;
			if (g[j][0] + f[k - j - 1][0] > ans)
			{
				ans = g[j][0] + f[k - j - 1][0];
				cnt = 0;
			}
			if (g[j][0] + f[k - j - 1][0] == ans) cnt += (long long)g[j][1] * f[k - j - 1][1];
		}
		for (int j = 1; j <= k; j++)
		{
			if (g[j][1] == 0) continue;
			if (g[j][0] > f[j][0])
			{
				f[j][0] = g[j][0];
				f[j][1] = 0;
			}
			if (g[j][0] == f[j][0]) f[j][1] += g[j][1];
		}
	}
}
void solve(int u, int k)
{
	calc_size(u, -1);
	int x = INT_MAX;
	get_focus(u, -1, sz[u], u, x);
	flag[u] = true;
	calc_size(u, -1);
	dep[u] = depw[u] = 0;
	calc_dep(u, -1);
	calc_depw(u, -1);
	calc_ans(u, k);
	for (int i = 0; i < g[u].size(); i++)
		if (!flag[g[u][i].to]) solve(g[u][i].to, k);
}
void get_graph(int u)
{
	vis[u] = true;
	for (int i = 0; i < g2[u].size(); i++)
	{
		int v = g2[u][i].to, w = g2[u][i].w;
		if (!vis[v] && dis[u] + w == dis[v])
		{
			g[u].push_back(edge(v, w));
			g[v].push_back(edge(u, w));
			get_graph(v);
		}
	}
}
void read(int n, ...)
{
	va_list li;
	va_start(li, n);
	for (int i = 0; i < n; i++)
	{
		int &x = *va_arg(li, int *);
		x = 0;
		char ch;
		do ch = getchar();
		while (!isdigit(ch));
		do
		{
			(x *= 10) += ch - '0';
			ch = getchar();
		} while (isdigit(ch));
	}
	va_end(li);
}
int main()
{
	int n, m, k;
	read(3, &n, &m, &k);
	for (int i = 0; i < m; i++)
	{
		int a, b, c;
		read(3, &a, &b, &c);
		g2[a - 1].push_back(edge(b - 1, c));
		g2[b - 1].push_back(edge(a - 1, c));
	}
	for (int i = 0; i < n; i++)
		sort(g2[i].begin(), g2[i].end());
	memset(dis, 0x7f, sizeof dis);
	queue<int> q;
	q.push(0);
	inq[0] = true;
	dis[0] = 0;
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		inq[u] = false;
		for (int i = 0; i < g2[u].size(); i++)
		{
			int v = g2[u][i].to;
			int tmp = dis[v];
			dis[v] = min(dis[v], dis[u] + g2[u][i].w);
			if (tmp != dis[v] && !inq[v])
			{
				q.push(v);
				inq[v] = true;
			}
		}
	}
	get_graph(0);
	solve(0, k);
	printf("%lld %lld", ans, cnt);
}
