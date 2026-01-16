//Creator: Wiktor Lewicki, www.viksolve.pl
#include <bits/stdc++.h>
using namespace std;
const long long K = 31;
const long long R = (1ll << K); //this implementation works for the range [1, 2^K]

struct dynamic_seg_tree{
	struct sgtree_node{
		__int128_t sum;
		__int128_t to_add;
		sgtree_node* left;
		sgtree_node* right;
		sgtree_node(){
			sum = 0;
			to_add = 0;
			left = nullptr;
			right = nullptr;
		}
	};
	
	long long min_idx;
	long long max_idx;
	sgtree_node* root;
	sgtree_node* get_new_node(){
		sgtree_node* nd;
		try{
			return nd = new sgtree_node;
		}
		catch(...){
			cout << "memory allocating error\n";
			exit(0);
		}
	}
	dynamic_seg_tree() : min_idx(1), max_idx(R){
		root = get_new_node();
	}
	long long a;
	long long b;
	__int128_t val;
	
	void UPDATE(long long a, long long b, __int128_t val){
		this -> a = a;
		this -> b = b;
		this -> val = val;
		update(root, min_idx, max_idx);
	}
	void update(sgtree_node* node, long long l, long long r){
		if(a <= l && r <= b){
			node -> sum += __int128_t(val) * __int128_t(r - l + 1);
			node -> to_add += __int128_t(val);
			return;
		}
		long long mid = (l + r) >> 1;
		if(node -> to_add){
			if(node -> left == nullptr){
				node -> left = get_new_node();
			}
			if(node -> right == nullptr){
				node -> right = get_new_node();
			}
			node -> left -> sum += node -> to_add * __int128_t(mid - l + 1);
			node -> right -> sum += node -> to_add * __int128_t(r - mid);
			node -> left -> to_add += node -> to_add;
			node -> right -> to_add += node -> to_add;
			node -> to_add = 0;
		}
		if(a <= mid){
			if(node -> left == nullptr){
				node -> left = get_new_node();
			}
			update(node -> left, l, mid);
		}
		if(mid < b){
			if(node -> right == nullptr){
				node -> right = get_new_node();
			}
			update(node -> right, mid + 1, r);
		}
		node -> sum = 0;
		node -> sum += (node -> left != nullptr) ? node -> left -> sum : 0ll;
		node -> sum += (node -> right != nullptr) ? node -> right -> sum : 0ll;
	}
	__int128_t QUERY(long long a, long long b){
		this -> a = a;
		this -> b = b;
		return query(root, min_idx, max_idx);
	}
	__int128_t query(sgtree_node* node, long long l, long long r){
		if(a <= l && r <= b){
			return node -> sum;
		}
		long long mid = (l + r) >> 1;
		if(node -> to_add){
			if(node -> left == nullptr){
				node -> left = get_new_node();
			}
			if(node -> right == nullptr){
				node -> right = get_new_node();
			}
			node -> left -> sum += node -> to_add * __int128_t(mid - l + 1);
			node -> right -> sum += node -> to_add * __int128_t(r - mid);
			node -> left -> to_add += node -> to_add;
			node -> right -> to_add += node -> to_add;
			node -> to_add = 0;
		}
		__int128_t res = 0;
		if(a <= mid && node -> left != nullptr){
			res += query(node -> left, l, mid);
		}
		if(mid < b && node -> right != nullptr){
			res += query(node -> right, mid + 1, r);
		}
		node -> sum = 0;
		node -> sum += (node -> left != nullptr) ? node -> left -> sum : 0ll;
		node -> sum += (node -> right != nullptr) ? node -> right -> sum : 0ll;
		return res;
	}	
};
struct viksolve_tree{
	struct viksolve_node{
		dynamic_seg_tree* segtree;
		dynamic_seg_tree* sum;
		viksolve_node* left;
		viksolve_node* right;
		viksolve_node(){
			segtree = new dynamic_seg_tree;
			sum = new dynamic_seg_tree;
			left = nullptr;
			right = nullptr;
		}
	};
	viksolve_node* root;
	long long min_idx, max_idx;
	long long x1, y1, x2, y2, val;
	viksolve_node* get_viksolve_node(){
		viksolve_node* nd;
		try{
			return nd = new viksolve_node;
		}
		catch(...){
			cout << "memory allocating error\n";
			exit(0);
		}
	}
	viksolve_tree() : min_idx(1), max_idx(R){
		root = get_viksolve_node();
	}
	
	void UPDATE(long long x1, long long y1, long long x2, long long y2, long long val){
		this -> x1 = x1;
		this -> y1 = y1;
		this -> x2 = x2;
		this -> y2 = y2;
		this -> val = val;
		update(root, min_idx, max_idx);
	}
	void update(viksolve_node* node, long long l, long long r){
		
		__int128_t overleap = min(r, x2) - max(l, x1) + 1;
		node -> sum -> UPDATE(y1, y2, __int128_t(val) * __int128_t(overleap));
		if(x1 <= l && r <= x2){
			node -> segtree -> UPDATE(y1, y2, val);
			return;
		}
		long long mid = (l + r) >> 1;
		if(x1 <= mid){
			if(node -> left == nullptr){
				node -> left = get_viksolve_node();
			}
			update(node -> left, l, mid);
		}
		if(mid < x2){
			if(node -> right == nullptr){
				node -> right = get_viksolve_node();
			}
			update(node -> right, mid + 1, r);
		}
	}
	__int128_t QUERY(long long x1, long long y1, long long x2, long long y2){
		this -> x1 = x1;
		this -> y1 = y1;
		this -> x2 = x2;
		this -> y2 = y2;
		this -> val = val;
		return query(root, min_idx, max_idx);
	}
	__int128_t query(viksolve_node* node, long long l, long long r){
		if(x1 <= l && r <= x2){
			return node -> sum -> QUERY(y1, y2);
		}
		__int128_t overleap = min(r, x2) - max(l, x1) + 1;
		__int128_t res = __int128_t(overleap) * node -> segtree -> QUERY(y1, y2);
		long long mid = (l + r) >> 1;
		if(x1 <= mid && node -> left != nullptr){
			res += query(node -> left, l, mid);
		}
		if(mid < x2 && node -> right != nullptr){
			res += query(node -> right, mid + 1, r);
		}
		return res;
	}
};
ostream& operator<<(ostream& dest, __int128_t val){
	if(val == 0) return dest << '0';
	if(val < 0) {
		dest << '-';
		val = -val;
	}
	char buf[50];
	int n = 0;
	while(val > 0){
		buf[n++] = char('0' + (int)(val % 10));
		val /= 10;
	}
	while(n--){
		dest << buf[n];
	}
	return dest;
}
int main(){
	viksolve_tree seg;
	int q;
	cin >> q;
	while(q--){
		int c;
		cin >> c;
		if(c == 1){
			int x1, y1, x2, y2, val;
			cin >> x1 >> y1 >> x2 >> y2 >> val;
			seg.UPDATE(x1, y1, x2, y2, val);
		}
		else{
			int x1, y1, x2, y2;
			cin >> x1 >> y1 >> x2 >> y2;
			cout << seg.QUERY(x1, y1, x2, y2) << "\n";
		}
	}
}
