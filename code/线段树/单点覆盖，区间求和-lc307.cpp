// 单点覆盖，区间求和
#define ls (root << 1)
#define rs (root << 1 | 1)
class SegTree {
private:
    // 单点操作不用lazy
    struct Node {
        int val = 0;
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

    // 对pos点覆盖为val
    void update(int root, int left, int right, int pos, int val) {
        if (pos < left || pos > right) {
            return ;
        }
        if (pos <= left && right <= pos) {
            tree[root].val = val;
            return ;
        }

        // 单点操作不用pushDown
        int mid = (right - left) / 2 + left;
        update(ls, left, mid, pos, val);
        update(rs, mid + 1, right, pos, val);
        pushUp(root);
    }

    int query(int root, int left, int right, int from, int to) {
        if (to < left || right < from) {
            return 0;
        }
        if (from <= left && right <= to) {
            return tree[root].val;
        }

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
        tree.update(1, 1, n, index, val);
    }
    
    // [1, n]
    int sumRange(int left, int right) {
        left += 1, right += 1;
        return tree.query(1, 1, n, left, right);
    }
};