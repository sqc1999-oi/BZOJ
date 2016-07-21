#define _SCL_SECURE_NO_WARNINGS
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
const int N = 1e5, K = 2e5;
struct d
{
	int s, c, m, cnt, pow;
} a[N];
bool cmpc(const d &x, const d &y) { return x.c < y.c; }
bool cmps(const d &x, const d &y) { return x.s < y.s || x.s == y.s && (x.c < y.c || x.c == y.c&&x.m < y.m); }
bool cmpeq(const d &x, const d &y) { return x.s == y.s&&x.c == y.c&&x.m == y.m; }
int t[K + 1], cnt[N];
int lowbit(int x) { return x&-x; }
void update(int k, int x, int n) { for (; k <= n; k += lowbit(k)) t[k] += x; }
int query(int k)
{
	int ans = 0;
	for (; k > 0; k -= lowbit(k)) ans += t[k];
	return ans;
}
void cdq(int l, int r, int k)
{
	if (l == r - 1) return;
	int mid = (l + r) / 2;
	cdq(l, mid, k);
	cdq(mid, r, k);
	int j = l;
	for (int i = mid; i < r; i++)
	{
		for (; j < mid&&a[j].c <= a[i].c; j++) update(a[j].m, a[j].pow, k);
		a[i].cnt += query(a[i].m);
	}
	for (int i = l; i < j; i++) update(a[i].m, -a[i].pow, k);
	inplace_merge(a + l, a + mid, a + r, cmpc);
}
int main()
{
	int n, k;
	read(2, &n, &k);
	for (int i = 0; i < n; i++) read(3, &a[i].s, &a[i].c, &a[i].m);
	sort(a, a + n, cmps);
	for (int i = 0, j = 0; i < n; i = j)
	{
		for (; j < n&&cmpeq(a[i], a[j]); j++) a[i].pow++;
		a[i].cnt = a[i].pow - 1;
	}
	int m = unique(a, a + n, cmpeq) - a;
	cdq(0, m, k);
	for (int i = 0; i < m; i++) cnt[a[i].cnt] += a[i].pow;
	for (int i = 0; i < n; i++) printf("%d\n", cnt[i]);
}