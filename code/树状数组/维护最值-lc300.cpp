// 维护最大值
class BIT {
public:
    inline static int lowbit(int x) {
        return x & -x;
    }
private:
    int n;
    vector<int> tree;

public:
    BIT(int n) : n(n), tree(n + 1){}

    void update(int i, int val) {
        for (; i <= n; i += lowbit(i)) {
            tree[i] = max(tree[i], val);
        }
    }

    int query(int i) {
        int ans = 0;
        for (; i > 0; i -= lowbit(i)) {
            ans = max(ans, tree[i]);
        }
        return ans;
    }
};

class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        // 由于nums的量是确定的，可以离散化
        // 利用值域确定话

        // [离散化]
        vector<int> disc = nums;
        sort(disc.begin(), disc.end());
        disc.erase(unique(disc.begin(), disc.end()), disc.end());
        // [离散化]

        int n = disc.size();
        BIT tree(n);
        for (int x : nums) {
            int idx = 1 + (lower_bound(disc.begin(), disc.end(), x) - disc.begin());
            // 查询比当前数值小的总量
            int val = tree.query(idx - 1);
            tree.update(idx, val + 1);
        }

        return tree.query(n);
    }
};
