#include <cstdio>
#include <algorithm>
#include <cstdarg>
#include <cctype>
#include <vector>
#include <map>
#include <queue>
#include <stack>
using namespace std;
const int M = 1e5, N = M * 3;
vector<int> g[N], ng[N];
struct d { int x, y, t, id; } a[M];
int cnt, ccnt, con[N], in[N], f[N], s[N];
bool cmpx(const d &a, const d &b) { return a.x < b.x; }
bool cmpy(const d &a, const d &b) { return a.y < b.y; }
void tarjan(int u, int n)
{
	static int ts[N], low[N], time = 1;
	static stack<int> s;
	static bool ins[N];
	if (ts[u] != 0) return;
	ts[u] = low[u] = time++;
	s.push(u);
	ins[u] = true;
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i];
		if (ts[v] == 0)
		{
			tarjan(v, n);
			low[u] = min(low[u], low[v]);
		}
		else if (ins[v]) low[u] = min(low[u], ts[v]);
	}
	if (low[u] == ts[u])
	{
		int v;
		do
		{
			v = s.top();
			s.pop();
			ins[v] = false;
			con[v] = ccnt;
			if (v < n) ::s[ccnt]++;
		} while (u != v);
		ccnt++;
	}
}
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
int main()
{
	int n, r, c;
	read(3, &n, &r, &c);
	cnt = n;
	map<pair<int, int>, int> m;
	for (int i = 0; i < n; i++)
	{
		read(3, &a[i].x, &a[i].y, &a[i].t);
		m[make_pair(a[i].x, a[i].y)] = a[i].id = i;
	}
	sort(a, a + n, cmpx);
	for (int i = 0; i < n; i++)
	{
		g[cnt].push_back(a[i].id);
		if (a[i].t == 1) g[a[i].id].push_back(cnt);
		if (i == n - 1 || a[i].x != a[i + 1].x) cnt++;
	}
	sort(a, a + n, cmpy);
	for (int i = 0; i < n; i++)
	{
		g[cnt].push_back(a[i].id);
		if (a[i].t == 2) g[a[i].id].push_back(cnt);
		if (i == n - 1 || a[i].y != a[i + 1].y) cnt++;
	}
	for (int k = 0; k < n; k++)
		if (a[k].t == 3)
			for (int i = a[k].x - 1; i <= a[k].x + 1; i++)
				for (int j = a[k].y - 1; j <= a[k].y + 1; j++)
					if (!(i == a[k].x && j == a[k].y) && m.find(make_pair(i, j)) != m.end())
						g[a[k].id].push_back(m[make_pair(i, j)]);
	for (int i = 0; i < cnt; i++) tarjan(i, n);
	for (int u = 0; u < cnt; u++)
		for (int i = 0; i < g[u].size(); i++)
		{
			int v = g[u][i];
			if (con[u] != con[v])
			{
				ng[con[u]].push_back(con[v]);
				in[con[v]]++;
			}
		}
	queue<int> q;
	for (int i = 0; i < ccnt; i++)
		if (in[i] == 0)
		{
			q.push(i);
			f[i] = s[i];
		}
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		for (int i = 0; i < ng[u].size(); i++)
		{
			int v = ng[u][i];
			f[v] = max(f[v], f[u] + s[v]);
			if (--in[v] == 0) q.push(v);
		}
	}
	printf("%d", *max_element(f, f + ccnt));
}