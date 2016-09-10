#include <cstdio>
#include <algorithm>
#include <cstdarg>
#include <cctype>
using namespace std;
const int N = 1e4;
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
	splay_node *l, *r, *p;
	bool rev;
	splay_node() :l(nul), r(nul), p(nul), rev(false) {}
	bool is_root() { return p == nul || p->l != this&&p->r != this; }
	void reverse()
	{
		if (this == nul) return;
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
	}
	void right_rotate()
	{
		splay_node *x = l;
		l = x->r;
		l->p = this;
		transplant(x);
		x->r = this;
		p = x;
	}
	void splay()
	{
		static splay_node *st[N];
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
splay_node *splay_node::nul;
splay_node::constructor splay_node::ctor;
void access(splay_node *n)
{
	splay_node *x = n, *y = splay_node::nul;
	while (x != splay_node::nul)
	{
		x->splay();
		x->r = y;
		y = x;
		x = x->p;
	}
	n->splay();
}
void change_root(splay_node *x)
{
	access(x);
	x->reverse();
}
void link(splay_node *x, splay_node *y)
{
	change_root(y);
	y->p = x;
	access(y);
}
void cut(splay_node *x, splay_node *y)
{
	change_root(x);
	access(y);
	y->l = x->p = splay_node::nul;
}
splay_node *find_root(splay_node *x)
{
	while (x->p != splay_node::nul) x = x->p;
	return x;
}
splay_node a[N];
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
int next_ch()
{
	int ch;
	do ch = getchar();
	while (ch == ' ' || ch == '\r' || ch == '\n');
	return ch;
}
int main()
{
	int n, m;
	read(2, &n, &m);
	for (int i = 0; i < m; i++)
	{
		int u, v, ch = next_ch();
		read(2, &u, &v);
		if (ch == 'C') link(a + u - 1, a + v - 1);
		else if (ch == 'D') cut(a + u - 1, a + v - 1);
		else if (ch == 'Q') printf("%s\n", find_root(a + u - 1) == find_root(a + v - 1) ? "Yes" : "No");
	}
}