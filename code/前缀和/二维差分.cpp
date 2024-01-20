#include <bits/stdc++.h>
using namespace std;

struct MatrixDiff {
    // 外部查询是[0, n-1]，则offset=1
    // 外部查询是[1, n]，则offset=0
    int offset;
    int n, m;
    // 限定[1, n]=>[0, n+1]
    vector<vector<int>> mat;

    MatrixDiff(int n, int m, int offset = 0) {
        this->n = n;
        this->m = m;
        this->offset = 1 - offset;
        mat = vector<vector<int>>(n + 2, vector<int>(m + 2));
    }

    void setValue(int x1, int y1, int x2, int y2, int val) {
        x1 += offset, y1 += offset;
        x2 += offset, y2 += offset;
        mat[x1][y1] += val;
        mat[x2 + 1][y2 + 1] += val;
        mat[x1][y2 + 1] += -val;
        mat[x2 + 1][y1] += -val;
    }

    void calc() {
        for (int i = 1; i <= n; i += 1) {
            for (int j = 1; j <= m; j += 1) {
                mat[i][j] += mat[i - 1][j]
                            + mat[i][j - 1]
                            - mat[i - 1][j - 1];
            }
        }
    }
};

/**
 * 前缀和+差分
 * [2132. 用邮票贴满网格图](https://leetcode.cn/problems/stamping-the-grid/)
 */