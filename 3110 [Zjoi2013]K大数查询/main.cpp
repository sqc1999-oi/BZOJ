#include <cstdio>
#include <cstdarg>
#include <cctype>
#include <cstring>
#include <algorithm>
using namespace std;
void read(int n, ...)
{
	va_list li;
	va_start(li, n);
	while (n--)
	{
		int &x = *va_arg(li, int *), ch, f = 1;
		x = 0;
		do
		{
			ch = getchar();
			if (ch == '-') f = -1;
		}
		while (!isdigit(ch));
		do
		{
			x = x * 10 + ch - '0';
			ch = getchar();
		} while (isdigit(ch));
		x *= f;
	}
	va_end(li);
}
const int N = 5e4;
struct seg_node
{
	int l, r;
	unsigned v, f;
	seg_node *lc, *rc;
	seg_node(int l, int r, seg_node *lc, seg_node *rc) :l(l), r(r), v(), f(), lc(lc), rc(rc) {}
	static seg_node *build(int l, int r)
	{
		if (l == r - 1) return new seg_node(l, r, NULL, NULL);
		int mid = (l + r) / 2;
		return new seg_node(l, r, build(l, mid), build(mid, r));
	}
	void push_down()
	{
		if (f == 0) return;
		lc->v += (lc->r - lc->l)*f;
		lc->f += f;
		rc->v += (rc->r - rc->l)*f;
		rc->f += f;
		f = 0;
	}
	void update(int l, int r, int v)
	{
		if (this->l == l&&this->r == r)
		{
			this->f += v;
			this->v += (r - l)*v;
		}
		else
		{
			this->push_down();
			if (r <= this->lc->r) this->lc->update(l, r, v);
			else if (l >= this->rc->l) this->rc->update(l, r, v);
			else
			{
				this->lc->update(l, this->lc->r, v);
				this->rc->update(this->rc->l, r, v);
			}
			this->v = this->lc->v + this->rc->v;
		}
	}
	unsigned query(int l, int r)
	{
		if (this->l == l&&this->r == r) return this->v;
		this->push_down();
		if (r <= this->lc->r) return this->lc->query(l, r);
		if (l >= this->rc->l) return this->rc->query(l, r);
		return this->lc->query(l, this->lc->r) + this->rc->query(this->rc->l, r);
	}
} *tree = seg_node::build(0, N);
struct query
{
	int l, r, v, id, t;
	bool operator <(const query &b) const { return id < b.id; }
} a[N], t1[N], t2[N];
int ans[N];
void solve(int l, int r, int ql, int qr)
{
	if (ql == qr) return;
	if (l == r - 1)
	{
		for (int i = ql; i < qr; i++)
			if (a[i].t == 2) ans[a[i].id] = l;
		return;
	}
	int mid = (l + r) / 2, cnt = 0;
	for (int i = ql; i < qr; i++)
	{
		if (a[i].t == 1)
		{
			if (a[i].v < mid) t1[cnt++] = a[i];
			else
			{
				t2[i - ql - cnt] = a[i];
				tree->update(a[i].l - 1, a[i].r, 1);
			}
		}
		else if (a[i].t == 2)
		{
			unsigned x = tree->query(a[i].l - 1, a[i].r);
			if (x < a[i].v)
			{
				a[i].v -= x;
				t1[cnt++] = a[i];
			}
			else t2[i - ql - cnt] = a[i];
		}
	}
	for (int i = 0; i < qr - ql - cnt; i++)
		if (t2[i].t == 1) tree->update(t2[i].l - 1, t2[i].r, -1);
	for (int i = 0; i < cnt; i++) a[ql + i] = t1[i];
	for (int i = 0; i < qr - ql - cnt; i++) a[ql + cnt + i] = t2[i];
	solve(l, mid, ql, ql + cnt);
	solve(mid, r, ql + cnt, qr);
}
int main()
{
	int n, m;
	read(2, &n, &m);
	for (int i = 0; i < m; i++)
	{
		read(4, &a[i].t, &a[i].l, &a[i].r, &a[i].v);
		a[i].id = i;
	}
	solve(-n, n + 1, 0, m);
	sort(a, a + m);
	for (int i = 0; i < m; i++)
		if (a[i].t == 2) printf("%d\n", ans[i]);
}
