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

    Node rootNode;
    int  n = 1e9;

public:
    SegTree(int n) : n(n) {}

private:
    // 将动态开点的职责，分离到一个函数中
    //! 注意:
    //! 对于C/C++语言，本模板不处理最后的内存释放问题！
    //! 有CG的语言不用也考虑这种问题
    void check (Node* root) {
        if (root->ls == nullptr) {
            root->ls = new Node{};
        }
        if (root->rs == nullptr) {
            root->rs = new Node{};
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
        update(&rootNode, 1, n, left, right, val);
    }

    int query(int left, int right) {
        return query(&rootNode, 1, n, left, right);
    }
};

class Solution {
private:    
    static const int N = 1e8 + 1e6;

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