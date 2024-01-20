/**
 * https://www.luogu.com.cn/problem/P3919
 * P3919 【模板】可持久化线段树 1（可持久化数组）
 * ==============================================================
 * https://blog.csdn.net/weixin_62712365/article/details/126217543
 */
#include <iostream>
using namespace std;

constexpr int M = 1e6 + 10;

/**
 * 注意，这里不要用非0初始化
 * 会消耗编译时间
 */
struct Node {
    int val;
    int ls ;
    int rs ;
} tree[M * 25];

int arr[M], root[M], tot = 0;

void build(int &root, int left, int right) {
    root = ++tot;

    if (left == right) {
        tree[root].val = arr[left];
        return;
    }

    int mid = (right - left) / 2 + left;
    build(tree[root].ls, left, mid);
    build(tree[root].rs, mid + 1, right);
}

void update(int pre, int &cur, int left, int right, int pos, int val) {
    cur       = ++tot;
    tree[cur] = tree[pre];

    if (left == right) {
        tree[cur].val = val;
        return;
    }

    int mid = (right - left) / 2 + left;
    if (pos <= mid) {
        update(tree[pre].ls, tree[cur].ls, left, mid, pos, val);
    } else {
        update(tree[pre].rs, tree[cur].rs, mid + 1, right, pos, val);
    }
}

int query(int root, int left, int right, int pos) {
    if (left == right) {
        return tree[root].val;
    }

    int mid = (right - left) / 2 + left;
    if (pos <= mid) {
        return query(tree[root].ls, left, mid, pos);
    } else {
        return query(tree[root].rs, mid + 1, right, pos);
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    // cin >> n >> m;

    for (int i = 1; i <= n; i += 1) {
        scanf("%d", &arr[i]);
        // cin >> arr[i];
    }

    // 初始化为0号版本
    build(root[0], 1, n);

    // 每次操作生成一个版本
    int ver, op, loc, val;
    for (int i = 1; i <= m; i += 1) {
        // cin >> ver >> op >> loc;
        scanf("%d %d %d", &ver, &op, &loc);
        if (op == 1) {
            scanf("%d", &val);
            // cin >> val;
            // 更新后生成一个新版本
            update(root[ver], root[i], 1, n, loc, val);
        } else {
            val = query(root[ver], 1, n, loc);
            // 查询也要生成一个版本
            root[i] = root[ver];
            printf("%d\n", val);
        }
    }

    return 0;
}