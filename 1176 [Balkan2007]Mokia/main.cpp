#include <cstdio>
#include <cctype>
#include <cstdarg>
#include <algorithm>
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
const int W = 2e6, Q = 1e5;
struct d
{
	int x, y, t, pow, id;
	d(int x, int y, int t, int pow, int id) :x(x), y(y), t(t), pow(pow), id(id) {}
	d() {}
} a[Q * 4];
int t[W + 1], ans[Q];
bool cmpx(const d &a, const d &b) { return a.x < b.x; }
int lowbit(int x) { return x&-x; }
void update(int k, int x, int n) { for (; k <= n; k += lowbit(k)) t[k] += x; }
int query(int k)
{
	int ans = 0;
	for (; k > 0; k -= lowbit(k)) ans += t[k];
	return ans;
}
void cdq(int l, int r, int w)
{
	if (l == r - 1) return;
	int mid = (l + r) / 2;
	cdq(l, mid, w);
	cdq(mid, r, w);
	int j = l;
	for (int i = mid; i < r; i++)
	{
		for (; j < mid&&a[j].x <= a[i].x; j++)
			if (a[j].t == 0) update(a[j].y, a[j].pow, w);
		if (a[i].t == 1) ans[a[i].id] += query(a[i].y);
		else if (a[i].t == 2) ans[a[i].id] -= query(a[i].y);
	}
	for (int i = l; i < j; i++)
		if (a[i].t == 0) update(a[i].y, -a[i].pow, w);
	inplace_merge(a + l, a + mid, a + r, cmpx);
}
int main()
{
	int w, cnt = 0, qcnt = 0;
	read(2, &w, &w);
	while (true)
	{
		int t;
		read(1, &t);
		if (t == 1)
		{
			int x, y, p;
			read(3, &x, &y, &p);
			a[cnt++] = d(x, y, 0, p, -1);
		}
		else if (t == 2)
		{
			int x1, y1, x2, y2;
			read(4, &x1, &y1, &x2, &y2);
			a[cnt++] = d(x2, y2, 1, -1, qcnt);
			a[cnt++] = d(x1 - 1, y1 - 1, 1, -1, qcnt);
			a[cnt++] = d(x1 - 1, y2, 2, -1, qcnt);
			a[cnt++] = d(x2, y1 - 1, 2, -1, qcnt);
			qcnt++;
		}
		else if (t == 3) break;
	}
	cdq(0, cnt, w);
	for (int i = 0; i < qcnt; i++) printf("%d\n", ans[i]);
}