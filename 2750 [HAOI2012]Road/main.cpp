#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;
const int N = 1500, M = 5000, P = 1e9 + 7, INF = 0x7f7f7f7f;

struct edge
{
	int u, v, w;
} e[M];

vector<int> g[N];
int dis[N], ans[M], in[N], in_cnt[M], out_cnt[M];
bool flag[M];

void spfa(int n, int s, int *dis)
{
	static bool inq[N];
	memset(dis, 0x3f, sizeof(int) * n);
	dis[s] = 0;
	queue<int> q;
	q.push(s);
	inq[s] = true;
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		inq[u] = false;
		for (int i = 0; i < g[u].size(); i++)
		{
			int v = e[g[u][i]].v;
			int tmp = dis[v];
			dis[v] = min(dis[v], dis[u] + e[g[u][i]].w);
			if (tmp != dis[v] && !inq[v])
			{
				q.push(v);
				inq[v] = true;
			}
		}
	}
}

void get_topo(int s, bool *flag, int *in, vector<int> &t)
{
	queue<int> q;
	q.push(s);
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		t.push_back(u);
		for (int i = 0; i < g[u].size(); i++)
			if (flag[g[u][i]])
			{
				int v = e[g[u][i]].v;
				if (--in[v] == 0) q.push(v);
			}
	}
}

void calc_flag(int m, int *dis, bool *flag)
{
	for (int i = 0; i < m; i++)
		flag[i] = dis[e[i].u] + e[i].w == dis[e[i].v];
}

void calc_in(int n, int m, bool *flag, int *in)
{
	memset(in, 0x00, sizeof(int) * n);
	for (int i = 0; i < m; i++)
		if (flag[i]) in[e[i].v]++;
}

void calc_in_cnt(int m, int s, vector<int> &t, bool *flag, int *cnt)
{
	memset(cnt, 0x00, sizeof(int) * m);
	cnt[s] = 1;
	for (int i = 0; i < t.size(); i++)
	{
		int u = t[i];
		for (int j = 0; j < g[u].size(); j++)
			if (flag[g[u][j]])
				(cnt[e[g[u][j]].v] += cnt[u]) %= P;
	}
}

void calc_out_cnt(int m, vector<int> &t, bool *flag, int *cnt)
{
	memset(cnt, 0x00, sizeof(int) * m);
	for (int i = t.size() - 1; i >= 0; i--)
	{
		int u = t[i];
		cnt[u] = 1;
		for (int j = 0; j < g[u].size(); j++)
			if (flag[g[u][j]])
				(cnt[u] += cnt[e[g[u][j]].v]) %= P;
	}
}

int main()
{
	ios::sync_with_stdio(false);
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < m; i++)
	{
		cin >> e[i].u >> e[i].v >> e[i].w;
		e[i].u-- , e[i].v--;
		g[e[i].u].push_back(i);
	}
	for (int i = 0; i < n; i++)
	{
		spfa(n, i, dis);
		calc_flag(m, dis, flag);
		calc_in(n, m, flag, in);
		vector<int> t;
		get_topo(i, flag, in, t);
		calc_in_cnt(m, i, t, flag, in_cnt);
		calc_out_cnt(m, t, flag, out_cnt);
		for (int j = 0; j < m; j++)
			if (flag[j]) (ans[j] += (long long)in_cnt[e[j].u] * out_cnt[e[j].v] % P) %= P;
	}
	for (int i = 0; i < m; i++) cout << ans[i] << endl;
}
