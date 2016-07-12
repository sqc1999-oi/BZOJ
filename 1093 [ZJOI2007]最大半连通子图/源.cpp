#include <cstdio>
#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <vector>
#include <stack>
#include <queue>
using namespace std;
const int N = 1e5, M = 1e6;
vector<int> g[N], ng[N];
int ts[N], low[N], cnt, con[N], s[N], in[N];
bool flag[N];
void tarjan(int u)
{
	static int time = 0;
	static bool vis[N];
	static stack<int> s;
	vis[u] = true;
	ts[u] = low[u] = time++;
	s.push(u);
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i];
		if (vis[v])
		{
			tarjan(v);
			low[u] = min(low[u], low[v]);
		}
		else low[u] = min(low[u], ts[v]);
	}
	if (low[u] == ts[u])
	{
		int v;
		do
		{
			v = s.top();
			s.pop();
			con[v] = cnt;
			::s[cnt]++;

		} while (u != v);
		cnt++;
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
	int n, m, x;
	read(3, &n, &m, &x);
	for (int i = 0; i < n; i++)
	{
		int u, v;
		read(2, &u, &v);
		g[u - 1].push_back(v - 1);
	}
	tarjan(0);
	for (int u = 0; u < n; u++)
		for (int i = 0; i < g[u].size(); i++)
		{
			int v = g[u][i];
			ng[con[u]].push_back(con[v]);
		}
	for (int i = 0; i < cnt; i++) g[i].clear();
	for (int u = 0; u < cnt; u++)
	{
		for (int i = 0; i < ng[u].size(); i++) flag[g[u][i]] = false;
		for (int i = 0; i < ng[u].size(); i++)
		{
			int v = g[u][i];
			if (flag[v]) continue;
			flag[v] = true;
			in[v]++;
			g[u].push_back(v);
		}
	}
	queue<int> q;
	for (int i = 0; i < cnt; i++)
		if (in[i] == 0) q.push(i);
	vector<int> v;
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		v.push_back(u);
		for (int i = 0; i < g[u].size(); i++)
		{
			int v = g[u][i];
			if (--in[v] == 0) q.push(v);
		}
	}
}