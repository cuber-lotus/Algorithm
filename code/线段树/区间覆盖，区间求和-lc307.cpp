// 区间覆盖，区间求和
#define ls (root << 1)
#define rs (root << 1 | 1)
class SegTree {
private:
    struct Node {
        int val = 0;
        int lazy = 0;
    };
    vector<Node> tree;
    vector<int>  arr;
    int n = 0;

public:
    SegTree() = default;
    // [1, n]
    SegTree(vector<int>& arr, int n) {
        this->arr = arr;
        this->n = n;
        this->tree.resize(n * 4);
    }

public:
    void pushUp(int root) {
        tree[root].val = tree[ls].val + tree[rs].val;
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

    void pushDown(int root, int left, int right) {
        // 预处理区间长度
        int mid = (right - left) / 2 + left;
        int leftLen = (mid - left) + 1;
        int rightLen = right - mid;

        // val覆盖，lazy传递也覆盖
        if (tree[root].lazy != 0) {
            tree[ls].val = tree[root].lazy * leftLen;
            tree[ls].lazy = tree[root].lazy;

            tree[rs].val = tree[root].lazy * rightLen;
            tree[rs].lazy = tree[root].lazy;

            tree[root].lazy = 0;
        }
    }

    void update(int root, int left, int right, int from, int to, int val) {
        if (to < left || right < from) {
            return ;
        }
        if (from <= left && right <= to) {
            tree[root].val = val * (right - left + 1);
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
        if (to < left || right < from) {
            return 0;
        }
        if (from <= left && right <= to) {
            return tree[root].val;
        }

        pushDown(root, left, right);
        int mid = (right - left) / 2 + left;
        return query(ls, left, mid, from, to)
             + query(rs, mid + 1, right, from, to);
    }
};

class NumArray {
private:
    // [1, n]
    vector<int> arr;
    int n = 0;
    SegTree tree;

public:
    // nums: [0, n-1]
    // -> arr: [1, n]
    NumArray(vector<int>& nums) {
        n = nums.size();
        arr = std::move(nums);
        arr.insert(arr.begin(), 0);
        tree = SegTree(arr, n);
        tree.build(1, 1, n);
    }
    
    // [1, n]
    // 直接覆盖
    void update(int index, int val) {
        index += 1;
        arr[index] = val;
        tree.update(1, 1, n, index, index, val);
    }
    
    // [1, n]
    int sumRange(int left, int right) {
        left += 1, right += 1;
        return tree.query(1, 1, n, left, right);
    }
};