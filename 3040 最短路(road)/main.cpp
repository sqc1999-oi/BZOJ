#include <cstdio>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cctype>
#include <cstdarg>
#include <cstring>
#include <climits>
using namespace std;
template<typename T> class fib_heap
{
	struct heap_node
	{
		static heap_node *nul;
		struct constructor
		{
			constructor()
			{
				if (nul != NULL) return;
				nul = new heap_node(T());
				nul->p = nul->c = nul;
			}
		} static ctor;
		T key;
		int degree;
		bool mark;
		heap_node *p, *c, *l, *r;
		explicit heap_node(const T &key) :key(key), degree(), mark(), p(nul), c(nul), l(this), r(this) { }
	} *min;
	int n;
	void insert_node(heap_node *&x, heap_node *y) const
	{
		if (y == heap_node::nul) return;
		if (x == heap_node::nul) x = y;
		heap_node *a = x->l, *b = x;
		a->r = b->l = y;
		y->l = a;
		y->r = b;
	}
	void remove_node(heap_node *x) const
	{
		x->l->r = x->r;
		x->r->l = x->l;
	}
	void link_node(heap_node *x, heap_node *y) const
	{
		insert_node(x->c, y);
		y->p = x;
		y->mark = false;
		++x->degree;
	}
	void consolidate()
	{
		int d = int(ceil(log2(n)));
		heap_node **a = new heap_node *[d + 1];
		for (int i = 0; i <= d; i++) a[i] = heap_node::nul;
		heap_node *w = min;
		do
		{
			heap_node *x = w;
			w = w->r;
			x->l = x->r = x;
			while (a[x->degree] != heap_node::nul)
			{
				heap_node *y = a[x->degree];
				if (y->key < x->key) swap(x, y);
				link_node(x, y);
				a[x->degree - 1] = heap_node::nul;
			}
			a[x->degree] = x;
		} while (w != min);
		min = heap_node::nul;
		for (int i = 0; i <= d; i++)
			if (a[i] != heap_node::nul)
			{
				insert_node(min, a[i]);
				if (a[i]->key < min->key) min = a[i];
			}
		delete[] a;
	}
	void cut(heap_node *x)
	{
		if (x->p == heap_node::nul) return;
		remove_node(x);
		if (--x->p->degree == 0) x->p->c = heap_node::nul;
		else x->p->c = x->r;
		insert_node(min, x);
		heap_node *y = x->p;
		x->p = heap_node::nul;
		x->mark = false;
		if (y != heap_node::nul)
			if (!y->mark) y->mark = true;
			else cut(y);
	}
public:
	class iterator
	{
		friend class fib_heap;
		fib_heap *h;
		heap_node *x;
		iterator(fib_heap *heap, heap_node *node) :h(heap), x(node) { }
	public:
		iterator() :h()
		{
			heap_node::ctor = typename heap_node::constructor();
			x = heap_node::nul;
		}
		bool is_nul() const { return x == heap_node::nul; }
		T get_key() const
		{
			assert(!is_nul());
			return x->key;
		}
		void decrease_key(const T &key)
		{
			assert(!is_nul() && !(x->key < key));
			x->key = key;
			if (x->p != heap_node::nul && x->key < x->p->key) h->cut(x);
			if (x->key < h->min->key) h->min = x;
		}
	};
	fib_heap() :n(0), min(heap_node::nul) {}
	bool empty() const { return min == heap_node::nul; }
	int size() const { return n; }
	iterator insert(const T &key)
	{
		heap_node *x = new heap_node(key);
		insert_node(min, x);
		n++;
		if (x->key < min->key) min = x;
		return iterator(this, x);
	}
	iterator get_min() const
	{
		assert(!empty());
		return iterator(this, min);
	}
	T extract_min()
	{
		assert(!empty());
		heap_node *t = min;
		heap_node *w = t->c;
		do
		{
			heap_node *x = w;
			w = w->r;
			x->p = heap_node::nul;
			x->mark = false;
			insert_node(t, x);
		} while (w != t->c);
		remove_node(t);
		if (--n == 0) min = heap_node::nul;
		else
		{
			min = t->r;
			consolidate();
		}
		T ret = t->key;
		delete t;
		return ret;
	}
};
template<typename T> typename fib_heap<T>::heap_node *fib_heap<T>::heap_node::nul;
template<typename T> typename fib_heap<T>::heap_node::constructor fib_heap<T>::heap_node::ctor;
void read(int n, ...)
{
	char *li;
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
const int N = 1e6, M = 1e7;
struct node
{
	int u;
	long long dis;
	node(int u, long long dis) :u(u), dis(dis) {}
	node() :u(-1), dis(LLONG_MAX) {}
	bool operator <(const node &x) const { return dis < x.dis; }
};
struct edge
{
	int to, pow, next;
} e[M];
int g[N];
long long dis[N];
fib_heap<node>::iterator p[N];
void add_edge(int u, int v, int w)
{
	static int cnt = 0;
	e[cnt].to = v;
	e[cnt].pow = w;
	e[cnt].next = g[u];
	g[u] = cnt++;
}
int main()
{
	memset(g, 0xff, sizeof g);
	int n, m, t, rxa, rxc, rya, ryc, rp, x = 0, y = 0;
	read(8, &n, &m, &t, &rxa, &rxc, &rya, &ryc, &rp);
	for (int i = 0; i < m; i++)
	{
		int u, v, w;
		if (i < t)
		{
			x = (x*rxa + rxc) % rp;
			y = (y*rya + ryc) % rp;
			u = min(x%n + 1, y%n + 1);
			v = max(y%n + 1, y%n + 1);
			w = 1e8 - 100 * u;
		}
		else read(3, &u, &v, &w);
		add_edge(u - 1, v - 1, w);
	}
	fib_heap<node> h;
	p[0] = h.insert(node(0, 0));
	memset(dis, 0x3f, sizeof dis);
	dis[0] = 0;
	while (!h.empty())
	{
		int u = h.extract_min().u;
		for (int i = g[u]; i != -1; i = e[i].next)
		{
			int v = e[i].to;
			long long tmp = dis[v];
			dis[v] = min(dis[v], dis[u] + e[i].pow);
			if (dis[v] != tmp)
				if (p[v].is_nul()) p[v] = h.insert(node(v, dis[v]));
				else p[v].decrease_key(node(v, dis[v]));
		}
	}
	printf("%lld\n", dis[n - 1]);
}
