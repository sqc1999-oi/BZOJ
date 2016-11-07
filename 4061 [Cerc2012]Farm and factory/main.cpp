#include <cstdio>
#include <cstdarg>
#include <cctype>
#include <cstring>
#include <queue>
#include <functional>
#include <vector>
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
const int N = 1e5, M = 3e5;
struct edge
{
	int to, pow, next;
	edge(int to, int pow, int next) :to(to), pow(pow), next(next) {}
	edge() = default;
} e[M * 2];
int g[N], cnt;
long long x[N], y[N];
void add_edge(int u, int v, int w)
{
	e[cnt] = edge(v, w, g[u]);
	g[u] = cnt++;
}
void sssp(int n, int s, long long *dis)
{
	memset(dis, 0x3f, sizeof(long long)*n);
	typedef pair<int, int> pii;
	priority_queue<pii, vector<pii>, greater<pii> > q;
	q.push(pii(0, s));
	dis[s] = 0;
	while (!q.empty())
	{
		pii x = q.top();
		q.pop();
		int u = x.second;
		if (x.first > dis[u]) continue;
		for (int i = g[u]; i != -1; i = e[i].next)
		{
			int v = e[i].to;
			long long tmp = dis[v];
			dis[v] = min(dis[v], dis[u] + e[i].pow);
			if (dis[v] != tmp) q.push(pii(dis[v], v));
		}
	}
}
int main()
{
	int T;
	read(1, &T);
	while (T--)
	{
		cnt = 0;
		memset(g, 0xff, sizeof g);
		int n, m;
		read(2, &n, &m);
		for (int i = 0; i < m; i++)
		{
			int u, v, c;
			read(3, &u, &v, &c);
			u--, v--;
			add_edge(u, v, c);
			add_edge(v, u, c);
		}
		sssp(n, 0, x);
		sssp(n, 1, y);
		for (int i = 0; i < n; i++)
		{
			x[i] += y[i];
			y[i] = x[i] - y[i] * 2;
		}
		sort(x, x + n);
		sort(y, y + n);
		long long ans = 0;
		for (int i = 0; i < n; i++)
			if (i * 2 + 1 != n)
				ans += (x[i] + y[i]) * (i < n / 2 ? -1 : 1);
		printf("%.10f\n", ans / 2.0 / n);
	}
}
