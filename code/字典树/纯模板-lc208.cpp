class Trie {
private:
    // 可以改为 vector 或者其他
    std::array<Trie*, 26> children;
    int isEnd = false;

public:
    // 构造
    // std::array 清零
    Trie() :children{} {}

    // 向当前对象插入字符串s
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

    // 查询字符串s是否存在
    bool search(const string &s) {
        auto node = searchPrefix(s);
        return node && node->isEnd;
    }
    
    // 查询是否有前缀
    bool startsWith(const string &prefix) {
        return searchPrefix(prefix) != nullptr;
    }

private:
    // 不考虑终止符
    // 查询是否有匹配的节点
    Trie* searchPrefix(const string &s) {
        auto node = this;
        for (auto ch : s) {
            int idx = ch - 'a';
            if (node->children[idx] == nullptr) {
                return nullptr;
            }
            node = node->children[idx];
        }
        return node;
    }
};

// [208. 实现 Trie (前缀树)](https://leetcode.cn/problems/implement-trie-prefix-tree/)