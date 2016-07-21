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
const int N = 3e5, X = 1e6;
namespace bit
{
	int t[X + 1];
	int lowbit(int x) { return x&-x; }
	void update(int k, int x, int n) { for (; k <= n; k += lowbit(k)) t[k] = max(t[k], x); }
	void clear(int k, int n) { for (; k <= n; k += lowbit(k)) t[k] = 0; }
	int query(int k)
	{
		int ans = 0;
		for (; k > 0; k -= lowbit(k)) ans = max(ans, t[k]);
		return ans;
	}
}
struct d
{
	static bool fx, fy;
	static int max;
	static void update_flag(bool x, bool y)
	{
		fx = x;
		fy = y;
	}
	int _x, _y, id;
	int x() const { return fx ? max - _x : _x; }
	int y() const { return fy ? max - _y : _y; }
} a[N * 2];
bool d::fx, d::fy;
int d::max;
int ans[N];
bool cmpx(const d &a, const d &b) { return a.x() < b.x(); }
void cdq(int l, int r, int max)
{
	if (l == r - 1) return;
	int mid = (l + r) / 2;
	cdq(l, mid, max);
	cdq(mid, r, max);
	int j = l;
	for (int i = mid; i < r; i++)
	{
		for (; j < mid&&a[j].x() >= a[i].x(); j++)
			if (a[j].id == -1) bit::update(a[j].y(), a[j].x() + a[j].y(), max);
		if (a[i].id != -1) ans[a[i].id] = ::max(ans[a[i].id], bit::query(max) - bit::query(a[i].y() - 1) - (d::fx + d::fy)*max - a[i].x() - a[i].y());
	}
	for (int i = l; i < j; i++)
		if (a[i].id == -1) bit::clear(a[i].y(), max);
	inplace_merge(a + l, a + mid, a + r, cmpx);
}
int main()
{
	int n, m, max = 0;
	read(2, &n, &m);
	for (int i = 0; i < n; i++)
	{
		read(2, &a[i]._x, &a[i]._y);
		a[i].id = -1;
		max = ::max(max, ::max(a[i]._x, a[i]._y));
	}
	int qcnt = 0;
	for (int i = 0; i < m; i++)
	{
		int t;
		read(3, &t, &a[i + n]._x, &a[i + n]._y);
		if (t == 2) a[i + n].id = qcnt++;
		else a[i + n].id = -1;
		max = ::max(max, ::max(a[i + n]._x, a[i + n]._y));
	}
	d::max = max + 1;
	d::update_flag(false, false);
	cdq(0, n + m, max);
	d::update_flag(true, false);
	cdq(0, n + m, max);
	d::update_flag(false, true);
	cdq(0, n + m, max);
	d::update_flag(true, true);
	cdq(0, n + m, max);
	for (int i = 0; i < qcnt; i++) printf("%d\n", ans[i]);
}