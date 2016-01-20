#include <cstdio>
#include <algorithm>
#include <cstdarg>
#include <cctype>
using namespace std;
const int N = 1e5, P = 51061;
struct splay_node
{
	static splay_node *nul;
	struct constructor
	{
		constructor()
		{
			nul = new splay_node;
			nul->val = nul->sum = nul->mul_f = nul->size = 0;
			nul->l = nul->r = nul->p = nul;
		}
	};
	static constructor ctor;
	splay_node *l, *r, *p;
	unsigned size, val, sum, add_f, mul_f;
	bool rev_f;
	splay_node() :l(nul), r(nul), p(nul), size(1), val(1), sum(1), add_f(0), mul_f(1), rev_f(false) {}
	bool is_root() const { return p == nul || p->l != this&&p->r != this; }
	void reverse()
	{
		if (this == nul) return;
		swap(l, r);
		rev_f ^= 1;
	}
	void add(unsigned x)
	{
		if (this == nul) return;
		(add_f += x) %= P;
		(val += x) %= P;
		(sum += x*size%P) %= P;
	}
	void multiply(unsigned x)
	{
		if (this == nul) return;
		(mul_f *= x) %= P;
		(add_f *= x) %= P;
		(val *= x) %= P;
		(sum *= x) %= P;
	}
	void push_down()
	{
		if (rev_f)
		{
			l->reverse();
			r->reverse();
			rev_f = false;
		}
		if (mul_f != 1)
		{
			l->multiply(mul_f);
			r->multiply(mul_f);
			mul_f = 1;
		}
		if (add_f != 0)
		{
			l->add(add_f);
			r->add(add_f);
			add_f = 0;
		}
	}
	void maintain()
	{
		size = (l->size + r->size + 1) % P;
		sum = (l->sum + r->sum + val) % P;
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
void change_root(splay_node *x)
{
	access(x);
	x->reverse();
}
void extract(splay_node *x, splay_node *y)
{
	change_root(x);
	access(y);
}
void link(splay_node *x, splay_node *y)
{
	change_root(y);
	y->p = x;
	access(y);
}
void cut(splay_node *x, splay_node *y)
{
	extract(x, y);
	y->l = x->p = splay_node::nul;
	y->maintain();
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
int next_ch()
{
	int ch;
	do ch = getchar();
	while (ch == ' ' || ch == '\r' || ch == '\n');
	return ch;
}
splay_node a[N];
int main()
{
	int n, q;
	read(2, &n, &q);
	for (int i = 0; i < n - 1; i++)
	{
		int x, y;
		read(2, &x, &y);
		link(a + x - 1, a + y - 1);
	}
	for (int i = 0; i < q; i++)
	{
		int ch = next_ch(), x, y;
		unsigned v;
		switch (ch)
		{
		case '+':
			read(3, &x, &y, &v);
			extract(a + x - 1, a + y - 1);
			(a + y - 1)->add(v);
			break;
		case '-':
			read(2, &x, &y);
			cut(a + x - 1, a + y - 1);
			read(2, &x, &y);
			link(a + x - 1, a + y - 1);
			break;
		case '*':
			read(3, &x, &y, &v);
			extract(a + x - 1, a + y - 1);
			(a + y - 1)->multiply(v);
			break;
		case '/':
			read(2, &x, &y);
			extract(a + x - 1, a + y - 1);
			printf("%u\n", (a + y - 1)->sum);
		}
	}
}