#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <queue>
#include <map>
#include <vector>
#include <cstring>
#include <climits>
#include <stack>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <functional>
using namespace std;
typedef pair<int, int> pii;
typedef pair<long long, int> pli;
const int N = 2e5;
const long long INF = 0x3f3f3f3f3f3f3f3f;
struct edge
{
	int to, pow;
	edge(int to, int pow) : to(to), pow(pow) {}
};
struct node
{
	node *lc, *rc;
	int l, r;
	long long min;
	node(node *lc, node *rc, int l, int r)
		: lc(lc), rc(rc), l(l), r(r), min(LLONG_MAX) {}
	static node *build(int l, int r)
	{
		if (l >= r - 1) return new node(NULL, NULL, l, r);
		int mid = (l + r) / 2;
		return new node(build(l, mid), build(mid, r), l, r);
	}
	void update(int l, int r, long long v)
	{
		if (min <= v) return;
		if (l == this->l && r == this->r) min = ::min(min, v);
		else if (r <= lc->r) lc->update(l, r, v);
		else if (l >= rc->l) rc->update(l, r, v);
		else
		{
			lc->update(l, lc->r, v);
			rc->update(rc->l, r, v);
		}
	}
	long long query(int p)
	{
		if (l == r - 1) return min;
		if (p < lc->r) return ::min(min, lc->query(p));
		return ::min(min, rc->query(p));
	}
};
vector<edge> g[N];
long long diss[N], dist[N];
bool flag[N];
int f[N], fs[N], ft[N];
void dijkstra(int s, int flag, int *f, long long *dis, int n)
{
	static bool vis[N];
	priority_queue<pli, vector<pli>, greater<pli> > q;
	q.push(pli(0, s));
	memset(dis, 0x3f, n * sizeof(long long));
	dis[s] = 0;
	memset(vis, 0x00, n * sizeof(bool));
	while (!q.empty())
	{
		int u = q.top().second;
		q.pop();
		if (vis[u]) continue;
		vis[u] = true;
		for (int i = 0; i < g[u].size(); i++)
		{
			int v = g[u][i].to;
			long long x = dis[u] + g[u][i].pow;
			if (x == dis[v] && !::flag[v])
			{
				if (flag == 1) f[v] = min(f[v], f[u]);
				else if (flag == 2) f[v] = max(f[v], f[u]);
			}
			if (x < dis[v])
			{
				if (flag == 0) f[v] = u;
				else if (!::flag[v]) f[v] = f[u];
				dis[v] = x;
				q.push(pli(dis[v], v));
			}
		}
	}
}
pii make_pii(int x, int y)
{
	return x < y ? pii(x, y) : pii(y, x);
}
void read(int n, ...)
{
	va_list li;
	va_start(li, n);
	while (n--)
	{
		int &x = *va_arg(li, int *), ch;
		x = 0;
		do ch = getchar(); while (!isdigit(ch));
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
	int n, m;
	read(2, &n, &m);
	for (int i = 0; i < m; i++)
	{
		int u, v, w;
		read(3, &u, &v, &w);
		u--, v--;
		g[u].push_back(edge(v, w));
		g[v].push_back(edge(u, w));
	}
	int s, t;
	read(2, &s, &t);
	s--, t--;
	dijkstra(s, 0, f, diss, n);
	if (diss[t] >= INF)
	{
		int q;
		read(1, &q);
		while (q--) puts("Infinity");
		return 0;
	}
	flag[t] = true;
	map<pii, int> e;
	stack<pii> st;
	fill(fs, fs + n, INT_MAX);
	fill(ft, ft + n, INT_MIN);
	for (int x = t, i = 0; x != s; i++)
	{
		st.push(pii(x, f[x]));
		x = f[x];
		flag[x] = true;
	}
	for (int i = 0; !st.empty(); i++, st.pop())
	{
		pii p = st.top();
		e[make_pii(p.first, p.second)] = ft[p.first] = fs[p.second] = i;
	}
	fs[t] = e.size();
	ft[s] = -1;
	dijkstra(s, 1, fs, diss, n);
	dijkstra(t, 2, ft, dist, n);
	node *tree = node::build(0, e.size());
	for (int i = 0; i < n; i++)
		for (int j = 0; j < g[i].size(); j++)
		{
			int u = i, v = g[i][j].to, x = fs[u], y = ft[v];
			if (e.find(make_pii(u, v)) != e.end() || x > y) continue;
			tree->update(x, y + 1, diss[u] + g[i][j].pow + dist[v]);
		}
	int q;
	read(1, &q);
	for (int i = 0; i < q; i++)
	{
		int x, y;
		read(2, &x, &y);
		x--, y--;
		map<pii, int>::iterator it = e.find(make_pii(x, y));
		long long ans;
		if (it == e.end()) ans = diss[t];
		else ans = tree->query(it->second);
		if (ans >= INF) puts("Infinity");
		else printf("%lld\n", ans);
	}
}
