#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <cstring>
#include <cctype>
#include <cfloat>
#include <iomanip>
const int N = 1e4, M = N * 2, K = 10, L = 20;
using namespace std;
template<int n> class string_dic
{
	static const int S = 26 + 26 + 10 + 1;
	int node_cnt, ep_cnt;
	struct node
	{
		int id;
		node *link[S];
		node() :id() { memset(link, 0x00, sizeof link); }
	} pool[n + 1], *r;
	static int to_int(char ch)
	{
		if (ch == '_') return S - 1;
		if (isupper(ch)) return ch - 'A';
		if (islower(ch)) return ch - 'a' + 26;
		return ch - '0' + 52;
	}
	node *new_node() { return &pool[node_cnt++]; }
public:
	string_dic() :node_cnt(), ep_cnt(), r(new_node()) {}
	void insert(const string &s, int &flag)
	{
		if (s == "start") flag = 1;
		else if (s == "end") flag = 2;
		else if (s.find("gas") != string::npos) flag = 3;
		else flag = 0;
		node *c = r;
		for (int i = 0; i < s.length(); i++)
		{
			node *&x = c->link[to_int(s[i])];
			if (x == NULL) x = new_node();
			c = x;
		}
		c->id = ep_cnt++;
	}
	int find(const string &s) const
	{
		node *c = r;
		for (int i = 0; i < s.length(); i++) c = c->link[to_int(s[i])];
		return c->id;
	}
};
template<int n, typename pow_t> struct layer_graph
{
	struct vertex
	{
		pow_t pow;
		vector<int> link;
		vertex() :pow() {}
	} g[n];
	struct edge
	{
		int from, to, cnt;
		pow_t pow;
		edge(int from, int to, int cnt, pow_t pow) : from(from), to(to), cnt(cnt), pow(pow) {}
	};
	vector<edge> e;
	void add_edge(int u, int v, int cnt, pow_t w)
	{
		g[u].link.push_back(e.size());
		e.push_back(edge(u, v, cnt, w));
	}
};
template<int n, int layer_cnt, typename pow_t> class layer_spfa_solver
{
	typedef layer_graph<n, pow_t> graph_t;
	typedef typename graph_t::edge edge_t;
	struct node
	{
		int id, layer;
		node(int id, int layer) :id(id), layer(layer) {}
	};
	bool inq[layer_cnt + 1][n];
	pow_t &get(pow_t **a, const node &x) const { return a[x.layer][x.id]; }
public:
	layer_spfa_solver() { memset(inq, 0x00, sizeof inq); }
	void solve(int s, pow_t pow_limit, int layer_limit, const graph_t &gp, pow_t **dis)
	{
		for (int i = 0; i <= layer_cnt; i++)
			fill(dis[i], dis[i] + n, DBL_MAX / 3);
		dis[0][s] = 0;
		queue<node> q;
		q.push(node(s, 0));
		inq[0][s] = true;
		while (!q.empty())
		{
			node u = q.front();
			q.pop();
			inq[u.layer][u.id] = false;
			for (int i = 0; i < gp.g[u.id].link.size(); i++)
			{
				edge_t e = gp.e[gp.g[u.id].link[i]];
				if (u.layer + e.cnt > layer_limit) continue;
				node v(e.to, u.layer + e.cnt);
				pow_t tmp = get(dis, v);
				if (get(dis, u) + e.pow + gp.g[v.id].pow > pow_limit) continue;
				get(dis, v) = min(get(dis, v), get(dis, u) + e.pow + gp.g[v.id].pow);
				if (tmp != get(dis, v) && !inq[v.layer][v.id])
				{
					q.push(v);
					inq[v.layer][v.id] = true;
				}
			}
		}
	}
};
int main()
{
	ios::sync_with_stdio(false);
	int n, m, k, limit, cost, s = 0, t = 0;
	cin >> n >> m >> k >> limit >> cost;
	string_dic<N * L> *dic = new string_dic<N * L>;
	layer_graph<N, double> *gp = new layer_graph<N, double>;
	bool *has_light = new bool[N];
	memset(has_light, 0x00, sizeof(bool)*N);
	int *flag = new int[N];
	for (int i = 0; i < n; i++)
	{
		string name;
		int a, b;
		cin >> name >> a >> b;
		dic->insert(name, flag[i]);
		if (a > 0)
		{
			gp->g[i].pow = a*a / 2.0 / (a + b);
			has_light[i] = true;
		}
		if (flag[i] == 1) s = i;
		else if (flag[i] == 2) t = i;
	}
	for (int i = 0; i < m; i++)
	{
		string u, v, name;
		int w;
		cin >> u >> v >> name >> w;
		int u_id = dic->find(u), v_id = dic->find(v);
		gp->add_edge(u_id, v_id, has_light[v_id], w);
		gp->add_edge(v_id, u_id, has_light[u_id], w);
	}
	layer_spfa_solver<N, K, double> *solver = new layer_spfa_solver<N, K, double>;
	layer_graph<N, double> *new_gp = new layer_graph<N, double>;
	double **dis = new double *[K + 1];
	for (int i = 0; i <= K; i++) dis[i] = new double[N];
	for (int i = 0; i < n; i++)
		if (flag[i] > 0)
		{
			new_gp->g[i].pow = cost;
			solver->solve(i, limit, k, *gp, dis);
			for (int j = 0; j < n; j++)
				if (i != j && flag[j] > 0)
					for (int l = 0; l <= k; l++)
						if (dis[l][j] < DBL_MAX / 3)
							new_gp->add_edge(i, j, l, dis[l][j]);
		}
	solver->solve(s, DBL_MAX, k, *new_gp, dis);
	double ans = DBL_MAX;
	for (int i = 0; i <= k; i++) ans = min(ans, dis[i][t]);
	cout << fixed << setprecision(3) << ans - cost;
}
