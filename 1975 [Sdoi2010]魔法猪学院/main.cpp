#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <cfloat>
#include <algorithm>
#include <cstdarg>
#include <cctype>
#include <queue>
#include <vector>
#include <functional>
using namespace std;
const int N = 5e3, M = 2e5, K = 18;
const double INF = DBL_MAX / 3;
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
template<typename T> struct heap
{
private:
	struct node
	{
		node *c, *r;
		T v;
		int t;
		node() :c(), r(), v(), t() {}
		explicit node(T v) :c(), r(), v(v), t() {}
		node *clone() const
		{
			node *t = new node;
			memcpy(t, this, sizeof(node));
			return t;
		}
		static node *merge(node *x, node *y)
		{
			if (x->v > y->v) swap(x, y);
			x = x->clone();
			y = y->clone();
			y->r = x->c;
			x->c = y;
			++x->t;
			return x;
		}
	};
	static vector<node *> merge(const vector<node *> &x, const vector<node *> &y)
	{
		static node *t[K];
		memset(t, 0x00, sizeof t);
		int i = 0, j = 0;
		while (i < x.size() || j < y.size())
		{
			node *z;
			if (i == x.size()) z = y[j], j++;
			else if (j == y.size()) z = x[i], i++;
			else if (x[i]->t < y[j]->t) z = x[i], i++;
			else if (y[j]->t < x[i]->t) z = y[j], j++;
			else z = node::merge(x[i], y[j]), i++, j++;
			if (t[z->t] != NULL)
			{
				z = node::merge(z, t[z->t]);
				t[z->t - 1] = NULL;
			}
			t[z->t] = z;
		}
		vector<node *> res;
		for (int k = 0; k < K; k++)
			if (t[k] != NULL) res.push_back(t[k]);
		return res;
	}
	node *get_min() const
	{
		node *x = rt[0];
		for (int i = 1; i < rt.size(); i++)
			if (rt[i]->v < x->v) x = rt[i];
		return x;
	}
	vector<node *> rt;
public:
	heap() :rt() {}
	explicit heap(vector<node *> v) :rt(v) {}
	T min() const { return get_min()->v; }
	heap *insert(T v) const
	{
		vector<node *> x;
		x.push_back(new node(v));
		return new heap(merge(rt, x));
	}
	heap *extract_min() const
	{
		vector<node *> v;
		node *m = get_min();
		for (node *x = m->c; x != NULL; x = x->r)
		{
			node *t = x->clone();
			t->r = NULL;
			v.push_back(t);
		}
		reverse(v.begin(), v.end());
		vector<node *> t = rt;
		t.erase(find(t.begin(), t.end(), m));
		return new heap(merge(t, v));
	}
	bool empty() { return rt.empty(); }
};
struct edge
{
	int to;
	double pow;
	edge(int to, double pow) :to(to), pow(pow) {}
};
vector<edge> g[N], rg[N];
vector<int> ch[N];
double dis[N];
int p[N];
typedef pair<double, int> pdi;
template<typename T> struct t { typedef priority_queue<T, vector<T>, greater<T> > pq; };
heap<pdi> *h[N];
void dijkstra(const vector<edge> *g, int s, int n)
{
	static bool flag[N];
	t<pdi>::pq q;
	fill(dis, dis + n, INF);
	dis[s] = 0;
	q.push(make_pair(0.0, s));
	while (!q.empty())
	{
		int u = q.top().second;
		q.pop();
		if (flag[u]) continue;
		flag[u] = true;
		for (int i = 0; i < g[u].size(); i++)
		{
			int v = g[u][i].to;
			double x = dis[u] + g[u][i].pow;
			if (x < dis[v])
			{
				dis[v] = x;
				q.push(make_pair(x, v));
			}
		}
	}
}
int main()
{
	int n, m;
	read(2, &n, &m);
	double e;
	scanf("%lf", &e);
	for (int i = 0; i < m; i++)
	{
		int u, v;
		read(2, &u, &v);
		u--, v--;
		double w;
		scanf("%lf", &w);
		rg[v].push_back(edge(u, w));
	}
	dijkstra(rg, n - 1, n);
	memset(p, 0xff, sizeof p);
	for (int u = 0; u < n; u++)
		if (dis[u] < INF)
			for (int i = 0; i < rg[u].size(); i++)
			{
				int v = rg[u][i].to;
				double w = rg[u][i].pow;
				if (dis[u] + w == dis[v] && p[v] == -1)
				{
					p[v] = u;
					ch[u].push_back(v);
				}
				else g[v].push_back(edge(u, w - dis[v] + dis[u]));
			}
	queue<int> q;
	q.push(n - 1);
	h[n - 1] = new heap<pdi>;
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		for (int i = 0; i < ch[u].size(); i++)
		{
			int v = ch[u][i];
			h[v] = h[u];
			for (int j = 0; j < g[v].size(); j++)
				h[v] = h[v]->insert(make_pair(g[v][j].pow, g[v][j].to));
			q.push(v);
		}
	}
	t<pair<double, pair<heap<pdi> *, heap<pdi> *> > >::pq pq;
	pq.push(make_pair(0.0, make_pair((heap<pdi> *)NULL, h[0])));
	int cnt = 0;
	while (!pq.empty())
	{
		double w = pq.top().first;
		e -= w + dis[0];
		if (e < 0) break;
		cnt++;
		heap<pdi> *x = pq.top().second.first, *y = pq.top().second.second;
		pq.pop();
		if (!y->empty()) pq.push(make_pair(w + y->min().first, make_pair(y, h[y->min().second])));
		if (x != NULL)
		{
			w -= x->min().first;
			x = x->extract_min();
			if (!x->empty()) pq.push(make_pair(w + x->min().first, make_pair(x, h[x->min().second])));
		}
	}
	printf("%d\n", cnt);
}
