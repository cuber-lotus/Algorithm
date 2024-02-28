// 2867. 统计树中的合法路径数目
// https://leetcode.cn/problems/count-valid-paths-in-a-tree
#include <bits/stdc++.h>
using namespace std;

class PrimeSieve {
private:
    int n;
    // false 质数
    // true  合数
    vector<bool> isPrime;
    vector<int>  primeList;

public:
    PrimeSieve(int n = 1e4 + 10) : isPrime(n + 1), n(n) {
        isPrime[0] = isPrime[1] = true;
        sieve();
    }

    bool operator[](size_t idx) const {
        return !isPrime[idx];
    }

private:
    // 欧拉筛
    void sieve() {
        for (int i = 2; i <= n; i++) {
            if (!isPrime[i]) {
                primeList.push_back(i);
            }
            for (int j = 0; j < primeList.size(); j++) {
                if (i * primeList[j] > n) {
                    break;
                }
                isPrime[i * primeList[j]] = true;
                if (i % primeList[j] == 0) {
                    break;
                }
            }
        }  // for
    }
};

const PrimeSieve prime(1e5 + 10);

struct UnionFind {
    vector<int> fa;
    vector<int> size;
    UnionFind(int n) : fa(n + 1), size(n + 1) {
        iota(fa.begin(), fa.end(), 0);
        fill(size.begin(), size.end(), 1);
    }

    int find(int x) {
        return x == fa[x] ? x : fa[x] = find(fa[x]);
    }

    // y -> x
    void merge(int x, int y) {
        if (find(x) == find(y)) {
            return;
        }
        size[find(x)] += size[find(y)];
        fa[find(y)] = find(x);
    }

    int getSccSize(int x) {
        return size[find(x)];
    }
};

class Solution {
private:
public:
    long long countPaths(int n, vector<vector<int>>& edges) {
        vector<vector<int>> grap;
        grap.resize(n + 1);

        UnionFind uf(n);
        for (auto&& e : edges) {
            int u = e[0], v = e[1];
            grap[u].push_back(v);
            grap[v].push_back(u);
            // 往小数字合并
            if (!prime[u] && !prime[v]) {
                uf.merge(u, v);
            }
        }

        long long ans = 0;
        // 枚举每一个质数点，作为根统计
        for (int i = 1; i <= n; i += 1) {
            if (prime[i] == false) {
                continue;
            }

            // 统计与当前质数所有联通点的个数
            int sumCount = 0;
            // 只考虑与当前质数联通的合数
            for (int nex : grap[i]) {
                if (prime[nex]) {
                    continue;
                }
                // 乘法原理的累计
                ans += uf.getSccSize(nex) * sumCount;
                sumCount += uf.getSccSize(nex);
            }
            ans += sumCount;
        }

        return ans;
    }
};