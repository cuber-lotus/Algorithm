#include <bits/stdc++.h>
using namespace std;

struct MatrixSum {
    // 外部查询是[0, n-1]，则offset=1
    // 外部查询是[1, n]，则offset=0
    int offset;
    // 限定[1, n]
    vector<vector<int>> mat;

    MatrixSum(vector<vector<int>>& matrix, int offset = 0) : offset(1 - offset) {
        const int n = matrix.size(), m = matrix[0].size();
        mat = vector<vector<int>>(n + 1, vector<int>(m + 1));
        for (int i = 1; i <= n ; i += 1) {
            for (int j = 1; j <= m; j += 1) {
                // 加两侧，减重合
                mat[i][j] = matrix[i - 1][j - 1]
                            + mat[i][j - 1]
                            + mat[i - 1][j]
                            - mat[i - 1][j - 1];
            }
        }
    }
    
    int getSum(int row1, int col1, int row2, int col2) {
        row1 += offset, col1 += offset;
        row2 += offset, col2 += offset;
        // 大 - 两侧 + 小
        return mat[row2][col2]
                - mat[row2][col1 - 1]
                - mat[row1 - 1][col2]
                + mat[row1 - 1][col1 - 1];
    }
};

/**
 * 模板
 * [304. 二维区域和检索 - 矩阵不可变](https://leetcode.cn/problems/range-sum-query-2d-immutable/description/)
 * 
 * 前缀和+差分
 * [2132. 用邮票贴满网格图](https://leetcode.cn/problems/stamping-the-grid/)
 */