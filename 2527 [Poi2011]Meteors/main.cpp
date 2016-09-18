#include <cstdio>
#include <cstdarg>
#include <cctype>
#include <algorithm>
#include <vector>
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
const int N = 3e5;
struct seg_node
{
	int l, r;
	long long v;
	seg_node *lc, *rc;
	seg_node(int l, int r, seg_node *lc, seg_node *rc) :l(l), r(r), v(), lc(lc), rc(rc) {}
	static seg_node *build(int l, int r)
	{
		if (l == r - 1) return new seg_node(l, r, NULL, NULL);
		int mid = (l + r) / 2;
		return new seg_node(l, r, build(l, mid), build(mid, r));
	}
	void update(int l, int r, int v)
	{
		if (this->l == l&&this->r == r) this->v += v;
		else if (r <= this->lc->r) this->lc->update(l, r, v);
		else if (l >= this->rc->l) this->rc->update(l, r, v);
		else
		{
			this->lc->update(l, this->lc->r, v);
			this->rc->update(this->rc->l, r, v);
		}
	}
	long long query(int k) const
	{
		if (this->l == k&&this->r - 1 == k) return this->v;
		if (k < this->lc->r) return this->lc->query(k) + this->v;
		return this->rc->query(k) + this->v;
	}
} *tree = seg_node::build(0, N);
struct query
{
	int v, id;
	vector<int> li;
	bool operator <(const query &b) const { return id < b.id; }
} q[N], t1[N], t2[N];
struct update
{
	int l, r, v;
} u[N + 1];
int ans[N];
void solve(int l, int r, int ql, int qr, int m)
{
	if (ql == qr) return;
	if (l == r - 1)
	{
		for (int i = ql; i < qr; i++) ans[q[i].id] = l;
		return;
	}
	int mid = (l + r) / 2;
	for (int i = l; i < mid; i++)
	{
		if (u[i].l < u[i].r) tree->update(u[i].l, u[i].r, u[i].v);
		else
		{
			tree->update(u[i].l, m, u[i].v);
			tree->update(0, u[i].r, u[i].v);
		}
	}
	int cnt = 0;
	for (int i = ql; i < qr; i++)
	{
		long long x = 0;
		for (int j = 0; j < q[i].li.size(); j++)
		{
			x += tree->query(q[i].li[j]);
			if (x >= q[i].v) break;
		}
		if (x >= q[i].v) t1[cnt++] = q[i];
		else
		{
			q[i].v -= x;
			t2[i - ql - cnt] = q[i];
		}
	}
	for (int i = l; i < mid; i++)
	{
		if (u[i].l < u[i].r) tree->update(u[i].l, u[i].r, -u[i].v);
		else
		{
			tree->update(u[i].l, m, -u[i].v);
			tree->update(0, u[i].r, -u[i].v);
		}
	}
	for (int i = 0; i < cnt; i++) q[ql + i] = t1[i];
	for (int i = 0; i < qr - ql - cnt; i++) q[ql + cnt + i] = t2[i];
	solve(l, mid, ql, ql + cnt, m);
	solve(mid, r, ql + cnt, qr, m);
}
int main()
{
	int n, m;
	read(2, &n, &m);
	for (int i = 0; i < m; i++)
	{
		int x;
		read(1, &x);
		q[x - 1].li.push_back(i);
	}
	for (int i = 0; i < n; i++)
	{
		read(1, &q[i].v);
		q[i].id = i;
	}
	int k;
	read(1, &k);
	for (int i = 0; i < k; i++)
	{
		read(3, &u[i].l, &u[i].r, &u[i].v);
		u[i].l--;
	}
	solve(0, k + 1, 0, n, m);
	sort(q, q + n);
	for (int i = 0; i < n; i++)
		if (ans[i] < k) printf("%d\n", ans[i] + 1);
		else printf("NIE\n");
}
