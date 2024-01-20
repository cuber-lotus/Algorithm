#define INF 0x3f3f3f3f
#define ls (root << 1)
#define rs (root << 1 | 1)

// 连续子数组的最大和
// 本模板 无update 无lazy
class SegTree {
private:
    // 根据题意，求最大和
    // 默认取-inf，视为非法值
    struct Node {
        int val = -INF;    // 区间最大和
        int lmax = -INF;   // 左侧连续最大和
        int rmax = -INF;   // 右侧连续最大和
        int sum = -INF;    // 整合区间最大和
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
        // 总和是两端的总和
        tree[root].sum = tree[ls].sum + tree[rs].sum;
        // 左侧连续区间
        // * (只选一边)左区间的左连续最大和
        // * (合并)右区间的左连续最大和 + 左区间的全部总和
        tree[root].lmax = max(tree[ls].lmax, tree[ls].sum + tree[rs].lmax);
        // 右侧连续区间
        // * (只选一边)右区间的右连续最大和
        // * (合并)左区间的右连续最大和 + 右区间的全部总和
        tree[root].rmax = max(tree[rs].rmax, tree[rs].sum + tree[ls].rmax);
        // 最重要的val(区间最大和)
        // * (合并)左区间的右连续最大和 + 右区间的左连续最大和
        // * (只选一边)左区间的val
        // * (只选一边)右区间的val
        tree[root].val = max({
            tree[ls].rmax + tree[rs].lmax,
            tree[ls].val,
            tree[rs].val
        });
    }

    void build(int root, int left, int right) {
        // 区间长度为1的单个区间的所有值都是一样的
        if (left == right) {
            tree[root].val = 
            tree[root].lmax = 
            tree[root].rmax = 
            tree[root].sum = 
                arr[left];
            return ;
        }

        int mid = (right - left) / 2 + left;
        build(ls, left, mid);
        build(rs, mid + 1, right);
        pushUp(root);
    }

    Node query(int root, int left, int right, int from, int to) {
        // 无效范围返回一个无效值
        if (from > right || to < left) {
            return Node{};
        }
        if (from <= left && right <= to) {
            return tree[root];
        }

        int mid = (right - left) / 2 + left;
        Node lNode = query(ls, left, mid, from, to);
        Node rNode = query(rs, mid + 1, right, from, to);

        Node ans;
        // 同pushUp的维护规则
        ans.val = max({
            lNode.rmax + rNode.lmax,
            lNode.val,
            rNode.val
        });
        ans.lmax = max(lNode.lmax, lNode.sum + rNode.lmax);
        ans.rmax = max(rNode.rmax, rNode.sum + lNode.rmax);
        // 区间总和在此处非核心维护信息
        // ans.sum = 0;
        return ans;
    }
};

class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        const int n = nums.size();
        const int N = n * 2;
        
        // 破环成链
        vector<int> arr(N + 1);
        for (int i = 1; i <= n; i += 1) {
            arr[i] = arr[i + n] = nums[i - 1];
        }

        SegTree tree(arr, N);
        tree.build(1, 1, N);

        // 遍历新链，搜寻区间长度是原数组长度
        int ans = -INF;
        for (int i = 1; i <= n; i += 1) {
            ans = max(ans, tree.query(1, 1, N, i, i + n - 1).val);
        }

        return ans;
    }
};
/* 
[918. 环形子数组的最大和](https://leetcode.cn/problems/maximum-sum-circular-subarray/)

[53. 最大子数组和](https://leetcode.cn/problems/maximum-subarray/)
*/