#define INF 0x3f3f3f3f
#define ls (root << 1)
#define rs (root << 1 | 1)
// 区间查最值
class SegTree {
private:
    // 本题没有更新update操作
    // 不用lazy
    struct Node {
        int val;
        // int lazy;
    };
    vector<Node> tree;
    vector<int> arr;
    int n;

public:
    SegTree(vector<int>& arr, int n) {
        this->n = n;
        this->arr = arr;
        this->tree.resize(n * 4);
    }

    void pushUp(int root) {
        tree[root].val = max(tree[ls].val, tree[rs].val);
    }

    void build(int root, int left, int right) {
        if (left == right) {
            tree[root].val = arr[left];
            return ;
        }

        int mid = (right - left) / 2 + left;
        build(ls, left, mid);
        build(rs, mid + 1, right);
        pushUp(root);
    }

    // 查询最大值，默认无效返回无穷小
    int query(int root, int left, int right, int from, int to) {
        if (from > right || to < left) {
            return -INF;
        }
        if (from <= left && right <= to) {
            return tree[root].val;
        }

        // 本题没有更新update操作
        // 不用pushDown
        int mid = (right - left) / 2 + left;
        return max(
            query(ls, left, mid, from, to),
            query(rs, mid + 1, right, from, to)
        );
    }
};

class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        const int n = nums.size();
        // [0, n - 1] -> [1, n]
        nums.insert(nums.begin(), 0);

        SegTree tree(nums, n);
        tree.build(1, 1, n);

        vector<int> ans;
        for (int i = 1; i + k - 1 <= n; i += 1) {
            ans.push_back(tree.query(1, 1, n, i, i + k - 1));
        }

        return ans;
    }
};