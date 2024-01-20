// 区间覆盖，区间查最值
// 动态开点
class SegTree {
private:
    // 动态开点
    // 指针开点法
    struct Node {
        int val  = 0;
        int lazy = 0;
        Node* ls = nullptr;
        Node* rs = nullptr;
    };

    int          n   = 1e9; // 数据范围
    int          tot = 0;   // 使用计数
    vector<Node> tree;      // 预估数组

public:
    // 可以用外部传参的方式
    // 也可以直接根据题目的总数据范围，算出最大值直接写死数组大小
    //  并且可以写成全局或者静态的方式来优化
    //  但这样每次使用需要手动初始化一下
    SegTree(int n, int cnt) : n(n), tree(cnt) {}
    
private:
    // 指针法和数组法的结合
    // 用指针去指向数组的具体位置
    void check (Node* root) {
        if (root->ls == nullptr) {
            root->ls = &tree[++tot];
        }
        if (root->rs == nullptr) {
            root->rs = &tree[++tot];
        }
    }

    // 维护最值
    void pushUp(Node* root) {
        root->val = max(root->ls->val, root->rs->val);
    }

    // 区间覆盖
    void pushDown(Node* root) {
        // 动态开点的检查放到 pushDown 的最开始
        // 因为 update 和 query 的操作都会用到 pushDown
        check(root);

        // 正常处理lazy标记
        if (root->lazy != 0) {
            root->ls->val  = root->val;
            root->ls->lazy = root->lazy;
            root->rs->val  = root->val;
            root->rs->lazy = root->lazy;

            root->lazy = 0;
        }
    }

    // 区间覆盖
    void update(Node *root, int left, int right, int from, int to, int val) {
        if (from > right || to < left) {
            return;
        }
        if (from <= left && right <= to) {
            root->val  = val;
            root->lazy = val;
            return;
        }

        pushDown(root);
        int mid = (right - left) / 2 + left;
        update(root->ls, left, mid, from, to, val);
        update(root->rs, mid + 1, right, from, to, val);
        pushUp(root);
    }

    int query(Node *root, int left, int right, int from, int to) {
        if (from > right || to < left) {
            return 0;
        }
        if (from <= left && right <= to) {
            return root->val;
        }

        pushDown(root);
        int mid = (right - left) / 2 + left;
        return max(
            query(root->ls, left, mid, from, to),
            query(root->rs, mid + 1, right, from, to)
        );
    }

public:  // 统一动态开点时外部调用接口
    void update(int left, int right, int val) {
        update(&tree[0], 1, n, left, right, val);
    }

    int query(int left, int right) {
        return query(&tree[0], 1, n, left, right);
    }
};

class Solution {
private:    
    static const int N = 1e8 + 1e6;

public:
    vector<int> fallingSquares(vector<vector<int>>& positions) {
        const int n = positions.size();

        const int N       = 1e8 + 1e6 + 1;  // 区间长度
        const int logN    = ceil(log2(N));  // log2(区间长度)的上取整
        const int M       = n * 2;          // 调用次数
        const int nodeCnt = 4 * M * logN;   // 可以调整参数和加偏移做个保守的计算
        SegTree tree(N, nodeCnt);

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