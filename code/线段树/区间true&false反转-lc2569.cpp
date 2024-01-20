class SegTree {
#define ls (root << 1)
#define rs (root << 1 | 1)

private:
    struct SegTreeNode {
        // root range的总和
        int val = 0;
        // 只有两个状态 0/1
        int lazy = 0;
    };
    vector<int> &arr;
    vector<SegTreeNode> tree;

public:
    SegTree(vector<int>& _arr, int n) : arr(_arr) {
        tree = vector<SegTreeNode>(n << 2);
        build(1, 1, n);
    }

public:
    void pushUp(int root) {
        tree[root].val = tree[ls].val + tree[rs].val;
    }

    void pushDown(int root, int left, int right) {
        int mid = (right - left) / 2 + left;
        int leftLen = mid - left + 1;
        int rightLen = right - mid;
        if (tree[root].lazy) {
            tree[ls].lazy ^= 1;
            tree[ls].val = leftLen - tree[ls].val;

            tree[rs].lazy ^= 1;
            tree[rs].val = rightLen - tree[rs].val;

            tree[root].lazy = 0;
        }
    }

    void build(int root, int left, int right) {
        tree[root].val = tree[root].lazy = 0;
        if (left == right) {
            tree[root].val = arr[left];
            return ;
        }

        int mid = (right - left) / 2 + left;
        build(ls, left, mid);
        build(rs, mid + 1, right);
        pushUp(root);
    }

    // 本题的val没有用到
    void update(int root, int left, int right, int from, int to, int val) {
        if (from > right || to < left) {
            return ;
        }
        if (from <= left && right <= to) {
            int len = right - left + 1;
            tree[root].val = len - tree[root].val;
            tree[root].lazy ^= 1;
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
        return 
            query(ls, left, mid, from, to) + 
            query(rs, mid + 1, right, from, to);
    }
};

class Solution {
public:
    vector<long long> handleQuery(vector<int>& nums1, vector<int>& nums2, vector<vector<int>>& queries) {
        const int n = nums1.size();
        // [0, n - 1] => [1, n]
        nums1.insert(nums1.begin(), 0);
        SegTree tree(nums1, n);

        vector<long long> ans;
        // 本质上与num2数组没关系
        long long sum = reduce(nums2.begin(), nums2.end(), 0LL);
        for (auto&& query : queries) {
            const int type = query[0];
            if (type == 1) {
                int left = query[1] + 1;
                int right = query[2] + 1;
                // 最后一个val参数没用到，占位一下
                tree.update(1, 1, n, left, right, 1);
            } else if (type == 2) {
                // 根据题意维护sum
                sum += 1LL * tree.query(1, 1, n, 1, n) * query[1];
            } else {
                ans.push_back(sum);
            }
        }

        return ans;
    }
};