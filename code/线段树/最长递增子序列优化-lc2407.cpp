#define ls (root << 1)
#define rs (root << 1 | 1)
class SegTree {
private:
    int n;
    // 维护一个最值，且单点更新
    vector<int> tree;

public:
    SegTree(int n) : n(n), tree(n * 4) {}

    void pushUp(int root) {
        tree[root] = max(tree[ls], tree[rs]);
    }

    // 区间覆盖
    // 单点更新
    void update(int root, int left, int right, int pos, int val) {
        if (pos > right || pos < left) {
            return ;
        }
        if (pos <= left && right <= pos) {
            tree[root] = val;
            return ;
        }

        int mid = (right - left) / 2 + left;
        update(ls, left, mid, pos, val);
        update(rs, mid + 1, right, pos, val);
        pushUp(root);
    }

    // 查询最值
    int query(int root, int left, int right, int from, int to) {
        if (from > right || to < left) {
            return INT_MIN / 2;
        }
        if (from <= left && right <= to) {
            return tree[root];
        }

        int mid = (right - left) / 2 + left;
        return max(
            query(ls, left, mid, from, to),
            query(rs, mid + 1, right, from, to)
        );
    }
};

class Solution {
public:
    int lengthOfLIS(vector<int>& nums, int k) {
        // 获取最大值，确定值域
        const int N = *max_element(nums.begin(), nums.end());
        // 注意：下面搜寻的范围是[0, N]
        // 如果max(1...)可能会出现1给1累计的情况
        SegTree tree(N);

        // 回到值域 LIS
        // 用线段树维护递增数值的递增长度
        for (int pos : nums) {
            int left  = max(0, pos - k);    // 往前走k个单位
            int right = max(0, pos - 1);    // 前一个位置的状态
            int val   = tree.query(1, 0, N, left, right);   // 查询比当前pos小的区间的最大递增长度
            // 迭代更新
            // 将当前数值作用域缓存，贡献+1
            // 类似于dp[i-1] + 1
            tree.update(1, 0, N, pos, val + 1);
        }

        // 整范围内的最值
        return tree.query(1, 0, N, 1, N);
    }
};