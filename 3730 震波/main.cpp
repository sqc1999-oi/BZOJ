#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#include <vector>
#include <climits>
using namespace std;
typedef pair<int, int> pii;
const int N = 100000;
vector<int> e, g[N];
vector<pii> p[N];
int ch[N], sz[N], dep[N], _val[N], val[N];
vector<int> t[N], tc[N * 2];
bool flag[N];

void calc_size(int u, int fa)
{
	sz[u] = 1;
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = e[g[u][i]];
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
		int v = e[g[u][i]];
		if (v != fa && !flag[v])
		{
			dep[v] = dep[u] + 1;
			calc_dep(v, u);
		}
	}
}

void get_focus(int u, int fa, int n, int &ans, int &min)
{
	int max = n - sz[u];
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = e[g[u][i]];
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

void get_nodes(int u, int fa, int x)
{
	p[u].push_back(pii(x, dep[u]));
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = e[g[u][i]];
		if (v != fa && !flag[v])
			get_nodes(v, u, x);
	}
}

void solve(int u, int fa)
{
	calc_size(u, -1);
	int x = INT_MAX;
	get_focus(u, -1, sz[u], u, x);
	flag[u] = true;
	calc_size(u, -1);
	ch[u] = fa;
	dep[u] = 1;
	calc_dep(u, -1);
	get_nodes(u, -1, u);
	t[u].resize(sz[u] + 1);
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = e[g[u][i]];
		if (!flag[v])
		{
			tc[g[u][i]].resize(sz[v] + 2);
			solve(v, g[u][i]);
		}
	}
}

int lowbit(int x)
{
	return x & -x;
}

void t_update(vector<int> &x, int pos, int val)
{
	while (pos < x.size())
	{
		x[pos] += val;
		pos += lowbit(pos);
	}
}

int t_query(vector<int> &x, int pos)
{
	pos = min(pos, (int)x.size() - 1);
	int ret = 0;
	while (pos > 0)
	{
		ret += x[pos];
		pos -= lowbit(pos);
	}
	return ret;
}

void update(int u, int k)
{
	int w = k - val[u];
	for (int i = p[u].size() - 1; i >= 0; i--)
	{
		int v = p[u][i].first, dis = p[u][i].second;
		t_update(t[v], dis, w);
		if (i < p[u].size() - 1)
			t_update(tc[ch[p[u][i + 1].first]], dis, w);
	}
	val[u] = k;
}

int query(int u, int k)
{
	int ret = 0;
	for (int i = p[u].size() - 1; i >= 0; i--)
	{
		int v = p[u][i].first, dis = k - p[u][i].second + 1;
		if (dis <= 0) continue;
		ret += t_query(t[v], dis);
		if (i < p[u].size() - 1)
			ret -= t_query(tc[ch[p[u][i + 1].first]], dis);
	}
	return ret;
}

int main()
{
	int n, m;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; i++) scanf("%d", &_val[i]);
	for (int i = 0; i < n - 1; i++)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		a--, b--;
		g[a].push_back(e.size());
		e.push_back(b);
		g[b].push_back(e.size());
		e.push_back(a);
	}
	solve(0, -1);
	for (int i = 0; i < n; i++) update(i, _val[i]);
	int last = 0;
	for (int i = 0; i < m; i++)
	{
		int op;
		int x, y;
		scanf("%d%d%d", &op, &x, &y);
		x ^= last, y ^= last;
		if (op == 0) printf("%d\n", last = query(x - 1, y + 1));
		else update(x - 1, y);
	}
}
