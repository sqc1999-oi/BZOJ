#include <cstdio>
#include <cstdarg>
#include <cctype>
#include <algorithm>
using namespace std;
void read(int n, ...)
{
	va_list li;
	va_start(li, n);
	for (int i = 0; i < n; i++)
	{
		int &x = *va_arg(li, int *);
		x = 0;
		char ch = 0;
		bool nega = false;
		do
		{
			if (ch == '-') nega = true;
			else nega = false;
			ch = getchar();
		} while (!isdigit(ch));
		do
		{
			(x *= 10) += ch - '0';
			ch = getchar();
		} while (isdigit(ch));
		if (nega) x = -x;
	}
}
template<typename T> class rb_tree
{
	enum rb_color { RED, BLACK };
	struct rb_node
	{
		T value;
		rb_color color;
		int count, size;
		rb_node *left, *right, *father;
		rb_node()
		{
			value = T();
			color = BLACK;
			count = size = 0;
			left = right = father = NULL;
		}
		rb_node(T value, rb_node *father, rb_node *nil)
		{
			this->value = value;
			this->father = father;
			left = right = nil;
			size = count = 1;
			color = RED;
		}
	};
	rb_node *root, *nil;
	rb_node *get_node_by_value(T value)
	{
		rb_node *x = root;
		while (x != nil)
		{
			if (value < x->value) x = x->left;
			else if (x->value < value) x = x->right;
			else break;
		}
		return x;
	}
	rb_node *get_node_by_rank(int rank)
	{
		rb_node *x = root;
		while (x != nil)
		{
			if (rank <= x->left->size) x = x->left;
			else if (rank > x->size - x->right->size)
			{
				rank -= x->size - x->right->size;
				x = x->right;
			}
			else break;
		}
		return x;
	}
	rb_node *get_minimum_node(rb_node *x)
	{
		while (x->left != nil) x = x->left;
		return x;
	}
	rb_node *get_maximum_node(rb_node *x)
	{
		while (x->right != nil) x = x->right;
		return x;
	}
	rb_node *get_predecessor_node(rb_node *x)
	{
		if (x->left != nil) return get_maximum_node(x->left);
		rb_node *y = x->father;
		while (y != nil && y->right != x)
		{
			x = y;
			y = y->father;
		}
		return y;
	}
	rb_node *get_successor_node(rb_node *x)
	{
		if (x->right != nil) return get_minimum_node(x->right);
		rb_node *y = x->father;
		while (y != nil && y->left != x)
		{
			x = y;
			y = y->father;
		}
		return y;
	}
	void update_size(rb_node *x)
	{
		x->size = x->left->size + x->count + x->right->size;
	}
	void maintain_size(rb_node *x)
	{
		while (x != nil)
		{
			update_size(x);
			x = x->father;
		}
	}
	void left_rotate(rb_node *x)
	{
		rb_node *y = x->right;
		x->right = y->left;
		if (x->right != nil) x->right->father = x;
		y->father = x->father;
		if (y->father == nil) root = y;
		else if (y->father->left == x) y->father->left = y;
		else y->father->right = y;
		y->left = x;
		x->father = y;
		update_size(x);
		update_size(y);
	}
	void right_rotate(rb_node *x)
	{
		rb_node *y = x->left;
		x->left = y->right;
		if (x->left != nil) x->left->father = x;
		y->father = x->father;
		if (y->father == nil) root = y;
		else if (y->father->left == x) y->father->left = y;
		else y->father->right = y;
		y->right = x;
		x->father = y;
		update_size(x);
		update_size(y);
	}
	void maintain_for_insertion(rb_node *x)
	{
		while (x->father->color == RED)
		{
			bool flag = x->father == x->father->father->left;
			rb_node *y = flag ? x->father->father->right : x->father->father->left;
			if (y->color == RED)
			{
				x->father->color = BLACK;
				y->color = BLACK;
				x->father->father->color = RED;
				x = x->father->father;
			}
			else
			{
				if (x == (flag ? x->father->right : x->father->left))
				{
					x = x->father;
					flag ? left_rotate(x) : right_rotate(x);
				}
				x->father->color = BLACK;
				x->father->father->color = RED;
				flag ? right_rotate(x->father->father) : left_rotate(x->father->father);
			}
		}
		root->color = BLACK;
	}
	void maintain_for_removal(rb_node *x)
	{
		while (x != root && x->color == BLACK)
		{
			bool flag = x == x->father->left;
			rb_node *y = flag ? x->father->right : x->father->left;
			if (y->color == RED)
			{
				x->father->color = RED;
				y->color = BLACK;
				flag ? left_rotate(x->father) : right_rotate(x->father);
				y = flag ? x->father->right : x->father->left;
			}
			if (y->left->color == BLACK && y->right->color == BLACK)
			{
				y->color = RED;
				x = x->father;
			}
			else
			{
				if ((flag ? y->right : y->left)->color == BLACK)
				{
					y->color = RED;
					(flag ? y->left : y->right)->color = BLACK;
					flag ? right_rotate(y) : left_rotate(y);
					y = flag ? x->father->right : x->father->left;
				}
				y->color = x->father->color;
				x->father->color = BLACK;
				(flag ? y->right : y->left)->color = BLACK;
				flag ? left_rotate(x->father) : right_rotate(x->father);
				x = root;
			}
		}
		x->color = BLACK;
	}
	void transplant_node(rb_node *x, rb_node *y)
	{
		y->father = x->father;
		if (y->father == nil) root = y;
		else if (y->father->left == x) y->father->left = y;
		else y->father->right = y;
	}
	rb_node *insert_node(T value)
	{
		rb_node *x = root, *y = nil;
		while (x != nil)
		{
			y = x;
			if (value < x->value) x = x->left;
			else if (x->value < value) x = x->right;
			else
			{
				x->count++;
				maintain_size(x);
				return x;
			}
		}
		rb_node *z = new rb_node(value, y, nil);
		if (y == nil) root = z;
		else if (value < y->value) y->left = z;
		else y->right = z;
		maintain_size(y);
		maintain_for_insertion(z);
		return z;
	}
	void remove_node(rb_node *x)
	{
		if (x == nil) return;
		if (--x->count > 0)
		{
			maintain_size(x);
			return;
		}
		rb_node *y = x, *z, *a = x->father;
		rb_color original_color = y->color;
		if (x->left == nil)
		{
			z = x->right;
			transplant_node(x, x->right);
		}
		else if (x->right == nil)
		{
			z = x->left;
			transplant_node(x, x->left);
		}
		else
		{
			y = get_minimum_node(x->right);
			original_color = y->color;
			z = y->right;
			if (y->father == x) z->father = y;
			else
			{
				a = y->father;
				transplant_node(y, y->right);
				y->right = x->right;
				y->right->father = y;
			}
			transplant_node(x, y);
			y->left = x->left;
			y->left->father = y;
			y->color = x->color;
			maintain_size(y);
		}
		maintain_size(a);
		if (original_color == BLACK) maintain_for_removal(z);
	}
public:
	rb_tree()
	{
		nil = new rb_node();
		root = nil;
	}
	void insert(T value)
	{
		insert_node(value);
	}
	void remove(T value)
	{
		remove_node(get_node_by_value(value));
	}
	int get_rank_by_value(T value)
	{
		int rank = 1;
		rb_node *x = root;
		while (x != nil)
		{
			if (value < x->value) x = x->left;
			else if (x->value < value)
			{
				rank += x->size - x->right->size;
				x = x->right;
			}
			else break;
		}
		return rank + x->left->size;
	}
	T get_value_by_rank(int rank)
	{
		return get_node_by_rank(rank)->value;
	}
	T get_predecessor(T value)
	{
		rb_node *x = insert_node(value);
		T res = get_predecessor_node(x)->value;
		remove_node(x);
		return res;
	}
	T get_successor(T value)
	{
		rb_node *x = insert_node(value);
		T res = get_successor_node(x)->value;
		remove_node(x);
		return res;
	}
};
int main()
{
	int n;
	read(1, &n);
	rb_tree<int> tree;
	for (int i = 0; i < n; i++)
	{
		int opt, x;
		read(2, &opt, &x);
		if (opt == 1) tree.insert(x);
		else if (opt == 2) tree.remove(x);
		else if (opt == 3) printf("%d\n", tree.get_rank_by_value(x));
		else if (opt == 4) printf("%d\n", tree.get_value_by_rank(x));
		else if (opt == 5) printf("%d\n", tree.get_predecessor(x));
		else printf("%d\n", tree.get_successor(x));;
	}
}
