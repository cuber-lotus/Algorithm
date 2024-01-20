// 手写递增分配器
template<typename Type, size_t N>
class TreeAllocator {
    Type node[N];
    size_t i = 0;

public:
    Type* get() {
        auto p = node + i;
        i += 1;
        new (p) Type{};
        return p;
    }
};

// 区间 true | false
// 区间统计
class SegmentTree {
private:
    struct Node {
        int val  = 0;
        int lazy = -1;

        Node* left  = nullptr;
        Node* right = nullptr;
    };

    TreeAllocator<Node, 500010> alloc;
    Node*     root = nullptr;
    const int n    = 1e9 + 7;

public:
    SegmentTree() {
        root = alloc.get();
    }

private:
    void checkSon(Node* root) {
        if (nullptr == root->left) {
            root->left = alloc.get();
        }
        if (nullptr == root->right) {
            root->right = alloc.get();
        }
    }

    void pushUp(Node* root) {
        root->val = root->left->val + root->right->val;
    }

    // 每次调用 pushDown 首先第一步检查左右子树
    void pushDown(Node* root, int left, int right) {
        checkSon(root);

        if (root->lazy == -1) {
            return;
        }
        // lazy 只有1|0 表示true|false
        const int mid      = left + (right - left) / 2;
        const int leftLen  = mid - left + 1;
        const int rightLen = right - mid;
        
        root->left->val   = leftLen * root->lazy;
        root->right->val  = rightLen * root->lazy;
        root->left->lazy  = root->lazy;
        root->right->lazy = root->lazy;
        root->lazy        = -1;
    }

private:
    // bool val
    // true | false 的全覆盖
    void update(Node* root, int left, int right, int from, int to, bool val) {
        if (from > right || to < left) {
            return;
        }
        if (from <= left && right <= to) {
            root->val  = val * (right - left + 1);
            root->lazy = val;
            return;
        }

        pushDown(root, left, right);
        const int mid = left + (right - left) / 2;
        update(root->left, left, mid, from, to, val);
        update(root->right, mid + 1, right, from, to, val);
        pushUp(root);
    }

    int query(Node* root, int left, int right, int from, int to) {
        if (from > right || to < left) {
            return 0;
        }
        if (from <= left && right <= to) {
            return root->val;
        }

        pushDown(root, left, right);
        const int mid = left + (right - left) / 2;
        return query(root->left, left, mid, from, to) +
               query(root->right, mid + 1, right, from, to);
    }

public:
    void add(int left, int right, int val) {
        update(root, 1, n, left, right, val);
    }

    int ask(int left, int right) {
        return query(root, 1, n, left, right);
    }
};

class RangeModule {
    SegmentTree tree;

public:
    RangeModule() {
    }

    // RangeModule 的所有查询更新是 `left <= x < right`
    // SegmentTree 的所有查询是 `[left, right]`

    void addRange(int left, int right) {
        tree.add(left, right - 1, true);
    }

    bool queryRange(int left, int right) {
        // 查询区间为true的点数是否为区间长度
        return tree.ask(left, right - 1) == (right - left);
    }

    void removeRange(int left, int right) {
        tree.add(left, right - 1, false);
    }
};