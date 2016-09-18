#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cassert>
#include <cstdarg>
#include <cctype>
using namespace std;
const int N = 5000000;
struct seg_node
{
	seg_node *l, *r;
	long long val;
} pool[N];
struct node_list_iterator
{
	vector<seg_node *> v;
	long long get_val(bool flag)
	{
		long long ret = 0;
		for (int i = 0; i < v.size(); i++) ret += (flag ? v[i]->r : v[i]->l)->val;
		return ret;
	}
	void move(bool flag)
	{
		for (int i = 0; i < v.size(); i++)
			v[i] = flag ? v[i]->r : v[i]->l;
	}
};
seg_node *new_node()
{
	static int cnt = 0;
	assert(cnt < N);
	return &pool[cnt++];
}
int lowbit(int x) { return x&-x; }
void update(int k, int x, int *t, int n)
{
	while (k <= n)
	{
		t[k] += x;
		k += lowbit(k);
	}
}
long long query(int k, int *t)
{
	long long ret = 0;
	while (k > 0)
	{
		ret += t[k];
		k -= lowbit(k);
	}
	return ret;
}
seg_node *update(seg_node *x, int l, int r, int k, long long val)
{
	if (x->val == 0)
	{
		seg_node *tmp = new_node();
		memcpy(tmp, x, sizeof(seg_node));
		x = tmp;
	}
	x->val += val;
	if (l != r - 1)
		if (k < (l + r) / 2) x->l = update(x->l, l, (l + r) / 2, k, val);
		else x->r = update(x->r, (l + r) / 2, r, k, val);
	return x;
}
void update(int k, int p, int x, seg_node **t, int n)
{
	while (k <= n)
	{
		t[k] = update(t[k], 0, n, p, x);
		k += lowbit(k);
	}
}
void init_iterator(node_list_iterator &it, int k, seg_node **t)
{
	while (k > 0)
	{
		it.v.push_back(t[k]);
		k -= lowbit(k);
	}
}
long long query(int tl, int tr, int k, seg_node **t, int n, bool flag)
{
	node_list_iterator il, ir;
	init_iterator(il, tl - 1, t);
	init_iterator(ir, tr, t);
	int l = 0, r = n;
	long long ret = 0;
	while (l != r - 1)
	{
		if (k < (l + r) / 2 ^ flag)
		{
			il.move(flag);
			ir.move(flag);
			(flag ? l : r) = (l + r) / 2;
		}
		else
		{
			ret += ir.get_val(flag) - il.get_val(flag);
			il.move(flag ^ 1);
			ir.move(flag ^ 1);
			(flag ? r : l) = (l + r) / 2;
		}
	}
	return ret;
}
void delete_n(int n, ...)
{
	va_list li;
	va_start(li, n);
	while (n--) delete[] va_arg(li, void *);
	va_end(li);
}
void read(FILE *f, int n, ...)
{
	va_list li;
	va_start(li, n);
	while (n--)
	{
		int &x = *va_arg(li, int *), ch;
		x = 0;
		do ch = getc(f);
		while (!isdigit(ch));
		do
		{
			(x *= 10) += ch - '0';
			ch = getc(f);
		} while (isdigit(ch));
	}
}
int main()
{
	FILE *in = stdin, *out = stdout;
	int n, m;
	read(in, 2, &n, &m);
	int *a = new int[n], *t = new int[n + 1], *p = new int[n];
	long long ans = 0, *cnt = new long long[n];
	memset(t + 1, 0x00, sizeof(int)*n);
	for (int i = 0; i < n; i++)
	{
		read(in, 1, &a[i]);
		ans += cnt[i] = query(n, t) - query(a[i], t);
		update(a[i], 1, t, n);
		a[i]--;
		p[a[i]] = i;
	}
	memset(t + 1, 0x00, sizeof(int)*n);
	for (int i = n - 1; i >= 0; i--)
	{
		cnt[i] += query(a[i], t);
		update(a[i] + 1, 1, t, n);
	}
	delete[] t;
	seg_node **st = new seg_node *[n + 1], *nul = new_node();
	nul->l = nul->r = nul;
	nul->val = 0;
	for (int i = 0; i <= n; i++) st[i] = nul;
	for (int i = 0; i < m; i++)
	{
		fprintf(out, "%lld\n", ans);
		int x;
		read(in, 1, &x);
		x = p[x - 1];
		ans -= cnt[x] - query(1, x, a[x], st, n, true) - query(x + 2, n, a[x], st, n, false);
		update(x + 1, a[x], 1, st, n);
	}
	delete_n(4, a, p, cnt, st);
}
