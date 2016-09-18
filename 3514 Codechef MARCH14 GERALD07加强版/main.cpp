#include <cstdio>
#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <cassert>
#include <cstring>
#include <climits>
using namespace std;
const int N = 2e5, M = 5e6;
struct splay_node
{
	static splay_node *nul;
	struct constructor
	{
		constructor()
		{
			nul = new splay_node;
			nul->l = nul->r = nul->p = nul;
		}
	};
	static constructor ctor;
	int id;
	splay_node *l, *r, *p;
	bool rev;
	int min;
	splay_node() :id(INT_MAX), l(nul), r(nul), p(nul), rev(), min(INT_MAX) {}
	bool is_root() { return p == nul || p->l != this&&p->r != this; }
	void reverse()
	{
		swap(l, r);
		rev ^= 1;
	}
	void push_down()
	{
		if (!rev) return;
		l->reverse();
		r->reverse();
		rev = false;
	}
	void maintain() { min = std::min(id, std::min(l->min, r->min)); }
	void transplant(splay_node *x)
	{
		x->p = p;
		if (p->l == this) p->l = x;
		else if (p->r == this) p->r = x;
	}
	void left_rotate()
	{
		splay_node *x = r;
		r = x->l;
		r->p = this;
		transplant(x);
		x->l = this;
		p = x;
		maintain();
		x->maintain();
	}
	void right_rotate()
	{
		splay_node *x = l;
		l = x->r;
		l->p = this;
		transplant(x);
		x->r = this;
		p = x;
		maintain();
		x->maintain();
	}
	void splay()
	{
		static splay_node *st[N * 2];
		int k = 0;
		splay_node *t = this;
		while (!t->is_root())
		{
			st[k++] = t;
			t = t->p;
		}
		st[k++] = t;
		while (k > 0) st[--k]->push_down();
		while (!is_root())
		{
			if (p->is_root())
				if (p->l == this) p->right_rotate();
				else p->left_rotate();
			else
			{
				splay_node *x = p->p;
				if (p->l == this)
					if (x->l == p) x->right_rotate(), p->right_rotate();
					else p->right_rotate(), x->left_rotate();
				else
					if (x->r == p) x->left_rotate(), p->left_rotate();
					else p->left_rotate(), x->right_rotate();
			}
		}
	}
};
splay_node::constructor splay_node::ctor;
splay_node *splay_node::nul;
void access(splay_node *n)
{
	splay_node *x = n, *y = splay_node::nul;
	while (x != splay_node::nul)
	{
		x->splay();
		x->r = y;
		x->maintain();
		y = x;
		x = x->p;
	}
	n->splay();
}
splay_node *find_root(splay_node *x)
{
	while (x->p != splay_node::nul) x = x->p;
	return x;
}
void change_root(splay_node *x)
{
	access(x);
	x->reverse();
}
void extract(splay_node *x, splay_node *y)
{
	assert(find_root(x) == find_root(y));
	change_root(x);
	access(y);
}
void link(splay_node *x, splay_node *y)
{
	assert(find_root(x) != find_root(y));
	change_root(y);
	y->p = x;
	access(y);
}
void cut(splay_node *x, splay_node *y)
{
	assert(find_root(x) == find_root(y));
	extract(x, y);
	assert(y->l == x&&x->p == y);
	y->l = x->p = splay_node::nul;
	y->maintain();
}

struct seg_node
{
	seg_node *l, *r;
	int sum;
} pool[M];
seg_node *new_node()
{
	static int cnt = 0;
	assert(cnt < M);
	return &pool[cnt++];
}
seg_node *update(seg_node *n, int l, int r, int k, int v)
{
	seg_node *x = new_node();
	memcpy(x, n, sizeof(seg_node));
	x->sum += v;
	if (l != r - 1)
		if (k < (l + r) / 2) x->l = update(x->l, l, (l + r) / 2, k, v);
		else x->r = update(x->r, (l + r) / 2, r, k, v);
	return x;
}
int query(seg_node *x, seg_node *y, int l, int r, int k)
{
	if (l == r - 1) return 0;
	if (k < (l + r) / 2) return query(x->l, y->l, l, (l + r) / 2, k);
	return query(x->r, y->r, (l + r) / 2, r, k) + y->l->sum - x->l->sum;
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
			(x *= 10) += ch - '0';
			ch = getchar();
		} while (isdigit(ch));
	}
	va_end(li);
}
struct edge { int u, v; };
int main()
{
	int n, m, k, type;
	read(4, &n, &m, &k, &type);
	splay_node *a = new splay_node[n + m];
	for (int i = 0; i < m; i++) a[i + n].id = a[i + n].min = i;
	seg_node **st = new seg_node *[m + 1];
	st[0] = new_node();
	st[0]->l = st[0]->r = st[0];
	st[0]->sum = 0;
	edge *e = new edge[m];
	for (int i = 0; i < m; i++)
	{
		int x, y, ntr = -1;
		read(2, &x, &y);
		x--, y--;
		e[i].u = x;
		e[i].v = y;
		if (x == y) ntr = i;
		else
		{
			if (find_root(a + x) == find_root(a + y))
			{
				extract(a + x, a + y);
				ntr = (a + y)->min;
				cut(a + e[ntr].u, a + ntr + n);
				cut(a + e[ntr].v, a + ntr + n);
			}
			link(a + x, a + i + n);
			link(a + y, a + i + n);
		}
		st[i + 1] = update(st[i], 0, m + 1, ntr + 1, 1);
	}
	delete[] a;
	delete[] e;
	int last = 0;
	for (int i = 0; i < k; i++)
	{
		int l, r;
		read(2, &l, &r);
		if (type == 1)
		{
			l ^= last;
			r ^= last;
		}
		last = n - query(st[l - 1], st[r], 0, m + 1, l);
		printf("%d\n", last);
	}
	delete[] st;
}
