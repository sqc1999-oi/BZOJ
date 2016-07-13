#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <vector>
#include <stack>
#include <queue>
#include <cstring>
using namespace std;
const int N = 1e5, M = 1e6;
vector<int> g[N + 1], ng[N + 1];
int cnt, con[N], s[N + 1], in[N + 1], f[N + 1][2];
int flag[N + 1];
void tarjan(int u)
{
	static int time = 1, ts[N], low[N];
	static bool ins[N];
	static stack<int> s;
	if (ts[u] != 0) return;
	ts[u] = low[u] = time++;
	s.push(u);
	ins[u] = true;
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i];
		if (ts[v] == 0)
		{
			tarjan(v);
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
	for (int i = 0; i < m; i++)
	{
		int u, v;
		read(2, &u, &v);
		g[u - 1].push_back(v - 1);
	}
	for (int i = 0; i < n; i++) tarjan(i);
	for (int u = 0; u < n; u++)
		for (int i = 0; i < g[u].size(); i++)
		{
			int v = g[u][i];
			if (con[u] != con[v]) ng[con[u]].push_back(con[v]);
		}
	for (int i = 0; i < cnt; i++) ng[i].push_back(cnt);
	for (int i = 0; i <= cnt; i++) g[i].clear();
	memset(flag, 0xff, sizeof flag);
	for (int u = 0; u < cnt; u++)
	{
		for (int i = 0; i < ng[u].size(); i++)
		{
			int v = ng[u][i];
			if (flag[v] == u) continue;
			flag[v] = u;
			in[v]++;
			g[u].push_back(v);
		}
	}
	queue<int> q;
	for (int i = 0; i < cnt; i++)
		if (in[i] == 0)
		{
			q.push(i);
			f[i][0] = s[i];
			f[i][1] = 1;
		}
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		for (int i = 0; i < g[u].size(); i++)
		{
			int v = g[u][i];
			if (f[u][0] + s[v] > f[v][0])
			{
				f[v][0] = f[u][0] + s[v];
				f[v][1] = f[u][1];
			}
			else if (f[u][0] + s[v] == f[v][0]) f[v][1] = (f[v][1] + f[u][1]) % x;
			if (--in[v] == 0) q.push(v);
		}
	}
	printf("%d\n%d\n", f[cnt][0], f[cnt][1]);
}