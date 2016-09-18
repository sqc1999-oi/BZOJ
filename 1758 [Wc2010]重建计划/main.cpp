#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#include <climits>
#include <cstring>
#include <cstdarg>
#include <cctype>
#include <cmath>
#include <vector>
#include <queue>
using namespace std;
const int N = 1e5, V = 1e6;
const double eps = 1e-5, M = -1e12;
typedef pair<double, int> pdi;
bool flag[N];
int sz[N], dep[N], depm[N];
double depw[N];
struct edge
{
	int to, w;
	edge(int to, int w) :to(to), w(w) {}
	bool operator <(const edge & e) const
	{
		return depm[to] < depm[e.to];
	}
};
vector<edge> g[N];
void get_nodes(int u, int fa, int *vi)
{
	vi[++vi[0]] = u;
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i].to;
		if (v != fa && !flag[v]) get_nodes(v, u, vi);
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
void calc_dep_max(int u, int fa)
{
	depm[u] = 0;
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i].to;
		if (v != fa && !flag[v])
		{
			calc_dep_max(v, u);
			depm[u] = max(depm[u], depm[v]);
		}
	}
	depm[u]++;
}
void calc_depw(int u, int fa, double x)
{
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i].to;
		if (v != fa && !flag[v])
		{
			depw[v] = depw[u] + g[u][i].w - x;
			calc_depw(v, u, x);
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
bool check(int u, int L, int U, double x)
{
	static double f[N], g[N];
	static int vi[N + 1];
	fill(f, f + depm[u], M);
	f[0] = 0;
	for (int i = 0; i < ::g[u].size(); i++)
	{
		int v = ::g[u][i].to;
		if (flag[v]) continue;
		fill(g, g + depm[v] + 1, M);
		vi[0] = 0;
		get_nodes(v, u, vi);
		depw[v] = ::g[u][i].w - x;
		calc_depw(v, u, x);
		for (int j = 1; j <= vi[0]; j++) g[dep[vi[j]]] = max(g[dep[vi[j]]], depw[vi[j]]);
		int a = depm[v], b = 0, c = 0;
		deque<pdi> q;
		double max = M;
		while (a >= 0)
		{
			while (b <= depm[v] && a + b < L) b++;
			c = ::max(c, b);
			while (c <= depm[v] && a + c <= U)
			{
				while (!q.empty() && (q.back().second < b || q.back().first <= f[c])) q.pop_back();
				q.push_back(make_pair(f[c], c));
				c++;
			}
			while (!q.empty() && q.front().second < b) q.pop_front();
			if (!q.empty()) max = ::max(max, q.front().first + g[a]);
			if (max > eps) return true;
			a--;
		}
		for (int j = 0; j <= depm[v]; j++) f[j] = ::max(f[j], g[j]);
	}
	return false;
}
double calc_ans(int u, int L, int U)
{
	sort(g[u].begin(), g[u].end());
	double l = 0, r = V;
	while (fabs(l - r) > eps)
	{
		double mid = (l + r) / 2;
		if (check(u, L, U, mid)) l = mid;
		else r = mid;
	}
	return l;
}
double solve(int u, int L, int U)
{
	calc_size(u, -1);
	if (sz[u] == 1 || sz[u] < L) return M;
	int x = INT_MAX;
	get_focus(u, -1, sz[u], u, x);
	calc_dep_max(u, -1);
	dep[u] = 0;
	calc_dep(u, -1);
	flag[u] = true;
	double ans = calc_ans(u, L, U);
	for (int i = 0; i < g[u].size(); i++)
		if (!flag[g[u][i].to]) ans = max(ans, solve(g[u][i].to, L, U));
	return ans;
}
void read(int n, ...)
{
	va_list li;
	va_start(li, n);
	for (int i = 0; i < n; i++)
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
int main()
{
	int n, L, U;
	read(3, &n, &L, &U);
	for (int i = 0; i < n - 1; i++)
	{
		int a, b, c;
		read(3, &a, &b, &c);
		a--, b--;
		g[a].push_back(edge(b, c));
		g[b].push_back(edge(a, c));
	}
	printf("%.3f", solve(0, L, U));
}
