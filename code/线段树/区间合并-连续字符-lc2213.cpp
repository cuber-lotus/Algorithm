#define INF (0x3f3f3f3f)
#define ls  (root << 1)
#define rs  (root << 1 | 1)

// 单点更新
// 区间合并
// 本线段树与该题强耦合，维护连续字符的最长长度
class SegTree {
private:
    // 有效长度的节点均为1
    // 注意:char的本质是一个整形数值
    struct Node {
        int lch  = -INF;  // 左侧字符
        int rch  = -INF;  // 右侧字符
        int lmax = -INF;  // 左侧连续最大长度
        int rmax = -INF;  // 右侧连续最大长度
        int sum  = -INF;  // 整个区间的总状态
    };

    int          n;
    string       s;
    vector<Node> tree;

public:
    // s: [1, n]
    SegTree(int n, const string& s) : n(n), s(" " + s), tree(n * 4) {
    }

    //! 区间合并的核心
    // 左右两个子节点贡献给上一层的大范围节点（尝试合并）
    void merge(Node& root, const Node& lnode, const Node& rnode, int left, int right) {
        const int mid  = (right - left) / 2 + left;
        const int lLen = mid - left + 1;
        const int rLen = right - mid;

        // 维护左右字符
        root.lch = lnode.lch;
        root.rch = rnode.rch;

        // 维护左连续
        //! 沿用左区间的左连续
        //! 考虑合并
        //! - 若左区间是个整体
        //! - 能与右区间的左端点相连
        root.lmax = lnode.lmax;
        if (lnode.sum == lLen && lnode.rch == rnode.lch) {
            root.lmax = max(root.lmax, lnode.sum + rnode.lmax);
        }

        // 维护右连续
        //! 沿用右区间的右连续
        //! 考虑合并
        //! - 若右区间是个整体
        //! - 且能与左区间的右端点相连
        root.rmax = rnode.rmax;
        if (rnode.sum == rLen && rnode.lch == lnode.rch) {
            root.rmax = max(root.rmax, rnode.sum + lnode.rmax);
        }

        // 维护整个区间的总状态
        //! 沿用左右两个区间的最值
        //! 考虑合并
        //! - 左区间的右端点 == 右区间的左端点
        root.sum = max(lnode.sum, rnode.sum);
        if (lnode.rch == rnode.lch) {
            root.sum = max(root.sum, lnode.rmax + rnode.lmax);
        }
    }

    void pushUp(int root, int left, int right) {
        merge(tree[root], tree[ls], tree[rs], left, right);
    }

    void build(int root, int left, int right) {
        // 找到了长度为1的单位的节点
        // 左右字符都是同一个
        // 左连续，右连续，总状态都是1单位
        if (left == right) {
            tree[root].lch = tree[root].rch = s[left];
            tree[root].lmax = tree[root].rmax = tree[root].sum = 1;
            return;
        }

        int mid = (right - left) / 2 + left;
        build(ls, left, mid);
        build(rs, mid + 1, right);
        pushUp(root, left, right);
    }

    // 注意，本题是单点操作
    // 不需要lazy和pushDown
    // 这个的递归判断是个人书写风格问题
    void update(int root, int left, int right, int pos, int val) {
        if (pos > right || pos < left) {
            return;
        }
        // 注意，这里的长度为1
        if (pos <= left && right <= pos) {
            tree[root].lch = tree[root].rch = val;
            tree[root].lmax = tree[root].rmax = tree[root].sum = 1;
            return;
        }

        int mid = (right - left) / 2 + left;
        update(ls, left, mid, pos, val);
        update(rs, mid + 1, right, pos, val);
        pushUp(root, left, right);
    }

    Node query(int root, int left, int right, int from, int to) {
        // 返回默认无效值
        if (from > right || to < left) {
            return Node{};
        }
        if (from <= left && right <= to) {
            return tree[root];
        }

        int  mid   = (right - left) / 2 + left;
        Node lnode = query(ls, left, mid, from, to);
        Node rnode = query(rs, mid + 1, right, from, to);
        Node ans;
        merge(ans, lnode, rnode, left, right);
        return ans;
    }
};

class Solution {
public:
    vector<int> longestRepeating(string s, string queryCharacters, vector<int>& queryIndices) {
        const int n = s.size();
        SegTree   tree(n, s);
        tree.build(1, 1, n);

        vector<int> ans(queryIndices.size());
        for (int i = 0; i < queryIndices.size(); i += 1) {
            int pos = queryIndices[i] + 1;
            int val = queryCharacters[i];

            tree.update(1, 1, n, pos, val);
            ans[i] = tree.query(1, 1, n, 1, n).sum;
        }

        return ans;
    }
};