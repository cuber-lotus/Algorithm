#define ls (root << 1)
#define rs (root << 1 | 1)
class SegTree {
private:
    struct Node {
        int val  = 0;
        int lazy = -1;  // true | false 的默认标记是非0/1
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
        if (tree[root].lazy != -1) {
            tree[ls].val  = tree[root].lazy * lLen;
            tree[ls].lazy = tree[root].lazy;
            tree[rs].val  = tree[root].lazy * rLen;
            tree[rs].lazy = tree[root].lazy;
            
            tree[root].lazy = -1;
        }
    }

    void update(int root, int left, int right, int from, int to, int val) {
        if (from > right || to < left) {
            return ;
        }
        if (from <= left && right <= to) {
            tree[root].val  = val * (right - left + 1);
            tree[root].lazy = val;
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
    bool isCovered(vector<vector<int>>& ranges, int left, int right) {
        const int n = 50 + 1;
        SegTree tree(n);
        for (auto&& arr : ranges) {
            tree.update(1, 1, n, arr[0], arr[1], 1);
        }
        // 覆盖的数量是否等于[r, l]的长度
        return tree.query(1, 1, n, left, right) == (right - left + 1);
    }
};