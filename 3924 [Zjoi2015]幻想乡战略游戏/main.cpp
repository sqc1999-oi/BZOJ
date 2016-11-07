#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#include <vector>
#include <climits>
using namespace std;
const int N = 1e5;
typedef pair<int, int> pii;
struct edge
{
	int to, w;
	edge(int to, int w) :to(to), w(w) { }
};
vector<edge> e;
vector<int> g[N];
vector<pii> p[N];
bool flag[N];
int sz[N], dep[N], ch[N], gr[N * 2];
long long sum[N][2], sumc[N * 2][2];
bool check(int u, int fa) { return u != fa && !flag[u]; }
void calc_size(int u, int fa)
{
	sz[u] = 1;
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = e[g[u][i]].to;
		if (check(v, fa))
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
		int v = e[g[u][i]].to;
		if (check(v, fa))
		{
			dep[v] = dep[u] + e[g[u][i]].w;
			calc_dep(v, u);
		}
	}
}
void calc_focus(int u, int fa, int n, int &min, int &focus)
{
	int max = n - sz[u];
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = e[g[u][i]].to;
		if (check(v, fa))
		{
			max = ::max(max, sz[v]);
			calc_focus(v, u, n, min, focus);
		}
	}
	if (max < min)
	{
		min = max;
		focus = u;
	}
}
void get_nodes(int u, int fa, vector<int> &vi)
{
	vi.push_back(u);
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = e[g[u][i]].to;
		if (check(v, fa)) get_nodes(v, u, vi);
	}
}
int solve(int u)
{
	calc_size(u, -1);
	int x = INT_MAX;
	calc_focus(u, -1, sz[u], x, u);
	flag[u] = true;
	dep[u] = 0;
	calc_dep(u, -1);
	vector<int> vi;
	get_nodes(u, -1, vi);
	for (int i = 0; i < vi.size(); i++) p[vi[i]].push_back(pii(u, dep[vi[i]]));
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = e[g[u][i]].to;
		if (check(v, -1))
		{
			int t = solve(v);
			ch[t] = g[u][i];
			gr[g[u][i]] = t;
		}
	}
	return u;
}
void update(int u, int val)
{
	for (int i = p[u].size() - 1; i >= 0; i--)
	{
		int v = p[u][i].first, dis = p[u][i].second;
		sum[v][0] += val;
		sum[v][1] += (long long)val*dis;
		if (i < p[u].size() - 1)
		{
			int anc = ch[p[u][i + 1].first];
			sumc[anc][0] += val;
			sumc[anc][1] += (long long)val*dis;
		}
	}
}
long long query(int u)
{
	long long ret = 0;
	for (int i = p[u].size() - 1; i >= 0; i--)
	{
		int v = p[u][i].first, dis = p[u][i].second;
		ret += sum[v][0] * dis + sum[v][1];
		if (i < p[u].size() - 1)
		{
			int anc = ch[p[u][i + 1].first];
			ret -= sumc[anc][0] * dis + sumc[anc][1];
		}
	}
	return ret;
}
long long calc_ans(int u)
{
	long long t = query(u);
	for (int i = 0; i < g[u].size(); i++)
		if (query(e[g[u][i]].to) < t) return calc_ans(gr[g[u][i]]);
	return t;
}
int main()
{
	int n, q;
	scanf("%d%d", &n, &q);
	for (int i = 0; i < n - 1; i++)
	{
		int a, b, c;
		scanf("%d%d%d", &a, &b, &c);
		a--, b--;
		g[a].push_back(e.size());
		e.push_back(edge(b, c));
		g[b].push_back(e.size());
		e.push_back(edge(a, c));
	}
	int r = solve(0);
	for (int i = 0; i < q; i++)
	{
		int u, e;
		scanf("%d%d", &u, &e);
		update(u - 1, e);
		printf("%lld\n", calc_ans(r));
	}
}
