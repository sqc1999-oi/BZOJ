#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdarg>
#include <cctype>
#include <algorithm>
#include <vector>
#include <stack>
#include <cstring>
using namespace std;
const int M = 500, N = M + 1;
int t[M * 2], cnt[N], pcnt[N], ccnt, ts[N], low[N], time;
bool flag[N], ins[N];
pair<int, int> p[M];
vector<int> g[N];
stack<pair<int, int> > s;
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
void dfs(int u, bool root = false)
{
	ts[u] = low[u] = time++;
	ins[u] = true;
	int ch_cnt = 0;
	vector<int> ch;
	for (int i = 0; i < g[u].size(); i++)
	{
		int v = g[u][i];
		if (ts[v] == -1)
		{
			ch_cnt++;
			s.push(make_pair(u, v));
			dfs(v);
			low[u] = min(low[u], low[v]);
			if (low[v] >= ts[u])
			{
				flag[u] = true;
				ch.push_back(ccnt);
				cnt[ccnt] = 1;
				pcnt[ccnt] = 0;
				pair<int, int> p;
				do
				{
					p = s.top();
					int x = p.second;
					s.pop();
					ins[x] = false;
					cnt[ccnt]++;
					if (flag[x]) pcnt[ccnt]++;
				} while (p.first != u);
				ccnt++;
			}
		}
		else if (ins[v]) low[u] = min(low[u], ts[v]);
	}
	if (!root&&flag[u] || root&&ch_cnt > 1)
		for (int i = 0; i < ch.size(); i++)
			pcnt[ch[i]]++;
}
int main()
{
	int m, test = 0;
	while (true)
	{
		read(1, &m);
		if (m == 0) break;
		for (int i = 0; i < m; i++)
		{
			read(2, &p[i].first, &p[i].second);
			t[i * 2] = p[i].first;
			t[i * 2 + 1] = p[i].second;
		}
		sort(t, t + m * 2);
		int n = unique(t, t + m * 2) - t;
		for (int i = 0; i < n; i++) g[i].clear();
		for (int i = 0; i < m; i++)
		{
			int u = lower_bound(t, t + n, p[i].first) - t, v = lower_bound(t, t + n, p[i].second) - t;
			g[u].push_back(v);
			g[v].push_back(u);
		}
		ccnt = 0;
		memset(ts, 0xff, sizeof ts);
		memset(flag, 0x00, sizeof flag);
		time = 0;
		for (int i = 0; i < n; i++)
			if (ts[i] == -1) dfs(i, true);
		int ans = 0;
		long long cnt = 1;
		for (int i = 0; i < ccnt; i++)
			if (::cnt[i] > 1)
			{
				if (pcnt[i] == 0)
				{
					cnt *= (long long)::cnt[i] * (::cnt[i] - 1) / 2;
					ans += 2;
				}
				else if (pcnt[i] == 1)
				{
					cnt *= ::cnt[i] - 1;
					ans++;
				}
			}
		printf("Case %d: %d %lld\n", ++test, ans, cnt);
	}
}
