// 字典树
class Trie {
private:
    vector<Trie*> children;
    bool isEnd = false;

public:
    Trie() : children(26) {}

    void insert(const string &s) {
        auto node = this;
        for (auto ch : s) {
            int idx = ch - 'a';
            if (node->children[idx] == nullptr) {
                node->children[idx] = new Trie{};
            }
            node = node->children[idx];
        }
        node->isEnd = true;
    }

    // 查找所有匹配的长度
    vector<int> query_length(const string &s) {
        auto node = this;
        vector<int> ans;
        for (int len = 0; auto ch : s) {
            int idx = ch - 'a';
            if (node->children[idx] == nullptr) {
                break;
            }
            node = node->children[idx];
            len += 1;
            // 有确定的结束标志，则记录
            if (node->isEnd) {
                ans.push_back(len);
            }
        }
        return ans;
    }
};

class Solution {
public:
    int minExtraChar(string s, vector<string>& dictionary) {
        Trie tree;
        for (auto&& s : dictionary) {
            tree.insert(s);
        }
        
        const int n = s.size();
        // dp[i][j]
        // 考虑到第i位上时，已经覆盖率j个单位的最优值
        vector<vector<int>> memo(n, vector<int>(n, 0x3f3f3f3f));
        // 以idx为起点的串
        // 已经有sum个点被覆盖了
        function<int(int, int)> dfs = [&](int idx, int sum) -> int {
            // 总点数 - 被覆盖的点
            if (idx >= n) {
                return n - sum;
            }

            // 记忆化
            int& ans = memo[idx][sum];
            if (ans != 0x3f3f3f3f) {
                return ans;
            }
            
            // 从字典树对sub开始查找所有的可能
            auto lenList = tree.query_length(s.substr(idx));
            //! 无脑跳过一个字符
            ans = min(ans, dfs(idx + 1, sum));
            // 尝试跳过所有可能的前缀串
            // 下标+len
            // 覆盖数+len
            for (int len : lenList) {
                ans = min(ans, dfs(idx + len, sum + len));
            }
            return ans;
        };

        return dfs(0, 0);
    }
};

// [2707. 字符串中的额外字符](https://leetcode.cn/problems/extra-characters-in-a-string/)