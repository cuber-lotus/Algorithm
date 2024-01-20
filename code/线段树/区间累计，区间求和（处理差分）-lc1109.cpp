#define ls (root << 1)
#define rs (root << 1 | 1)
class SegTree {
private:
    struct Node {
        long long val  = 0;
        long long lazy = 0;
    };

    int n;
    vector<Node> tree;

public:
    // 可以理解为build全是0
    SegTree(int n) : n(n), tree(n * 4) {}

    void pushUp(int root) {
        tree[root].val = tree[ls].val + tree[rs].val;
    }

    void pushDown(int root, int left, int right) {
        int mid  = (right - left) / 2 + left;
        int lLen = mid - left + 1;
        int rLen = right - mid;
        if (tree[root].lazy != 0) {
            tree[ls].val  += tree[root].lazy * lLen;
            tree[ls].lazy += tree[root].lazy;
            tree[rs].val  += tree[root].lazy * rLen;
            tree[rs].lazy += tree[root].lazy;
            
            tree[root].lazy = 0;
        }
    }

    void update(int root, int left, int right, int from, int to, long long val) {
        if (from > right || to < left) {
            return ;
        }
        if (from <= left && right <= to) {
            tree[root].val  += val * (right - left + 1);
            tree[root].lazy += val;
            return ;
        }

        pushDown(root, left, right);
        int mid = (right - left) / 2 + left;
        update(ls, left, mid, from, to, val);
        update(rs, mid + 1, right, from, to, val);
        pushUp(root);
    }

    int query(int root, int left, int right, int from, int to) {
        if (from > right || to < left) {
            return 0;
        }
        if (from <= left && right <= to) {
            return tree[root].val;
        }

        pushDown(root, left, right);
        int mid = (right - left) / 2 + left;
        return query(ls, left, mid, from, to) +
            query(rs, mid + 1, right, from, to);
    }
};

class Solution {
public:
    vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
        SegTree tree(n);
        // 区间累计
        for (auto&& arr : bookings) {
            int left  = arr[0];
            int right = arr[1];
            int val   = arr[2];
            tree.update(1, 1, n + 1, left, right, val);
        }

        vector<int> ans(n);
        // 区间求和
        for (int i = 1; i <= n; i += 1) {
            ans[i - 1] = tree.query(1, 1, n + 1, i, i);
        }
        return ans;
    }
};