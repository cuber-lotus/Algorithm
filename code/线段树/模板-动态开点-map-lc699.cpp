#define ls (root << 1)
#define rs (root << 1 | 1)
// 区间覆盖，区间查最值
// 动态开点
class SegTree {
private:
    struct Node {
        int val  = 0;
        int lazy = 0;
    };

    // 借助c++中的map自动开点
    unordered_map<int, Node> tree;
    int n = 1e9;

public:
    SegTree(int n) : n(n) {}

    // 维护最值
    void pushUp(int root) {
        tree[root].val = max(tree[ls].val, tree[rs].val);
    }

    // 区间覆盖
    void pushDown(int root) {
        if (tree[root].lazy != 0) {
            tree[ls].val  = tree[root].val;
            tree[ls].lazy = tree[root].lazy;
            tree[rs].val  = tree[root].val;
            tree[rs].lazy = tree[root].lazy;

            tree[root].lazy = 0;
        }
    }

    // 区间覆盖
    void update(int root, int left, int right, int from, int to, int val) {
        if (from > right || to < left) {
            return;
        }
        if (from <= left && right <= to) {
            tree[root].val  = val;
            tree[root].lazy = val;
            return;
        }

        pushDown(root);
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

        pushDown(root);
        int mid = (right - left) / 2 + left;
        return max(
            query(ls, left, mid, from, to),
            query(rs, mid + 1, right, from, to)
        );
    }

public:  // 统一动态开点时外部调用接口
    void update(int left, int right, int val) {
        update(1, 1, n, left, right, val);
    }

    int query(int left, int right) {
        return query(1, 1, n, left, right);
    }
};

class Solution {
private:    
    static const int N = 1e8 + 1e6 + 1;
    
public:
    vector<int> fallingSquares(vector<vector<int>>& positions) {
        const int n = positions.size();
        SegTree   tree(N);

        vector<int> ans(n);
        for (int i = 0; i < n; i += 1) {
            int len   = positions[i][1];
            int left  = positions[i][0];
            int right = left + len - 1;
            // 查询旧值，再累计长度
            int val = tree.query(left, right);
            tree.update(left, right, val + len);
            // 整个区间的最大值
            ans[i] = tree.query(1, N);
        }

        return ans;
    }
};