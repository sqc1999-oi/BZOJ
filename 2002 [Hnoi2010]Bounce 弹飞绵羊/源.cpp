#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <cctype>
using namespace std;
const int N = 2e5;
struct splay_node
{
	static splay_node *nul;
	struct constructor
	{
		constructor()
		{
			nul = new splay_node;
			nul->s = 0;
			nul->l = nul->r = nul->p = nul;
		}
	};
	static constructor ctor;
	splay_node *l, *r, *p;
	int s;
	splay_node() :l(nul), r(nul), p(nul), s(1) {}
	void update() {	s = l->s + r->s + 1; }
	bool is_root() { return p == nul || p->l != this&&p->r != this; }
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
		if (r != nul) r->p = this;
		transplant(x);
		x->l = this;
		p = x;
		update();
		x->update();
	}
	void right_rotate()
	{
		splay_node *x = l;
		l = x->r;
		if (l != nul) l->p = this;
		transplant(x);
		x->r = this;
		p = x;
		update();
		x->update();
	}
	splay_node *splay()
	{
		while (!is_root())
		{
			splay_node *x = p->p;
			if (p->is_root())
				if (p->l == this) p->right_rotate();
				else p->left_rotate();
			else
				if (p->l == this)
					if (x->l == p) x->right_rotate(), p->right_rotate();
					else p->right_rotate(), x->left_rotate();
				else
					if (x->r == p) x->left_rotate(), p->left_rotate();
					else p->left_rotate(), x->right_rotate();
		}
		return this;
	}
};
splay_node::constructor splay_node::ctor;
splay_node *splay_node::nul;
splay_node *access(splay_node *n)
{
	splay_node *x = n, *y = splay_node::nul;
	while (x != splay_node::nul)
	{
		x->splay();
		x->r = y;
		x->update();
		y = x;
		x = x->p;
	}
	return n->splay();
}
splay_node a[N];
void cut(splay_node *x)
{
	access(x);
	x->l = x->l->p = splay_node::nul;
	x->update();
}
void link(splay_node *x, splay_node *y)
{
	access(x);
	access(y);
	y->l = x;
	x->p = y;
	y->update();
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
int main()
{
	int n;
	read(1, &n);
	for (int i = 0; i < n; i++)
	{
		int k;
		read(1, &k);
		if (i + k < n) link(a + i + k, a + i);
	}
	int m;
	read(1, &m);
	for (int i = 0; i < m; i++)
	{
		int op, x;
		read(2, &op, &x);
		if (op == 1) printf("%d\n", access(a + x)->s);
		else
		{
			int k;
			read(1, &k);
			cut(a + x);
			if (x + k < n) link(a + x + k, a + x);
		}
	}
}