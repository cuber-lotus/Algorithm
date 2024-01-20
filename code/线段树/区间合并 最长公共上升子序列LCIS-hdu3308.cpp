/**
 * 杭电 LCIS 严格递增
 * https://acm.hdu.edu.cn/showproblem.php?pid=3308
 * 单点修改，区间查询
 * 区间合并 经典例题
 */
#include <bits/stdc++.h>
using namespace std;
#define ls (root << 1)
#define rs (root << 1 | 1)

const int M = 10 + 100000;
// 结构体中维护的是下标root的LCIS的最大值
struct SegTreeNode {
    int val;         // 总区间最大值
    int lmax, rmax;  // 左右，前缀后缀最大值
};

SegTreeNode segTree[M << 2];
int         arr[M];

// ls和rs能给root的贡献
void pushUp(int root, int left, int right) {
    // 左侧的前缀直接抄左边，右侧的后缀直接抄右边
    segTree[root].lmax = segTree[ls].lmax;
    segTree[root].rmax = segTree[rs].rmax;
    segTree[root].val  = max(segTree[ls].val, segTree[rs].val);

    int mid      = (right - left) / 2 + left;
    int leftLen  = mid - left + 1;
    int rightLen = right - mid;
    if (arr[mid] < arr[mid + 1]) {
        // 左侧全覆盖，则可接上右侧的前缀
        if (segTree[root].lmax == leftLen) {
            segTree[root].lmax += segTree[rs].lmax;
        }
        // 右侧全覆盖，则可接上左侧的后缀
        if (segTree[root].rmax == rightLen) {
            segTree[root].rmax += segTree[ls].rmax;
        }
        // 和 左侧的后缀+右侧的前缀 比较
        segTree[root].val =
            max(segTree[root].val, segTree[ls].rmax + segTree[rs].lmax);
    }
}

void build(int root, int left, int right) {
    if (left == right) {
        // 初始均为1，单个点，长度为1的区间的LCIS为1
        segTree[root].val = segTree[root].lmax = segTree[root].rmax = 1;
        return;
    }

    int mid = (right - left) / 2 + left;
    build(ls, left, mid);
    build(rs, mid + 1, right);
    pushUp(root, left, right);
}

void update(int root, int left, int right, int from, int to, int val) {
    if (from > right || to < left) {
        return;
    }
    if (from <= left && right <= to) {
        // val已经在调用前使用，这里规范的写一下而已
        return;
    }

    int mid = (right - left) / 2 + left;
    update(ls, left, mid, from, to, val);
    update(rs, mid + 1, right, from, to, val);
    pushUp(root, left, right);
}

int query(int root, int left, int right, int from, int to) {
    if (from > right || to < left) {
        return 0;  // 这里返回1也可以，但逻辑不对
    }
    if (from <= left && right <= to) {
        return segTree[root].val;
    }

    int mid  = (right - left) / 2 + left;
    int lmax = query(ls, left, mid, from, to);
    int rmax = query(rs, mid + 1, right, from, to);
    // 右侧压根没分到，只考虑左侧
    if (to < mid) {
        return lmax;
    }
    // 左侧压根没分到，只考虑右侧
    if (from > mid) {
        return rmax;
    }

    // 上面提前return了，现在左右都有贡献值
    int ans = max(lmax, rmax);
    if (arr[mid] < arr[mid + 1]) {
        // 左侧的后缀 和 右侧的前缀 合并
        // 保证区间在[from, to]内，做两次min
        ans = max(ans, min(mid - from + 1, segTree[ls].rmax) +
                           min(to - mid, segTree[rs].lmax));
    }

    return ans;
}

void solve() {
    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    build(1, 1, n);

    for (int i = 1, a, b; i <= m; i++) {
        char ch;
        cin >> ch >> a >> b;

        if (ch == 'U') {
            a += 1;      // 本题输入是[0, n-1]
            arr[a] = b;  // 直接在原序列上改
            // 还是要update() 目的是为了pushUp()
            update(1, 1, n, a, a, b);
        } else {
            a += 1, b += 1;
            cout << query(1, 1, n, a, b) << endl;
        }
    }
}

int main(void) {
    int T = 1;
    cin >> T;
    for (int i = 1; i <= T; i++) {
        solve();
    }
    return 0;
}
