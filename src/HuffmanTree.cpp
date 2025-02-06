#include "HuffmanTree.hpp"

struct CompareNodes
{
    bool operator()(const std::shared_ptr<HuffmanTree::Node> &a,
                    const std::shared_ptr<HuffmanTree::Node> &b)
    {
        return a->frequency > b->frequency;
    }
};

HuffmanTree::HuffmanTree(const std::map<char, int> &frequencies)
{
    // 创建优先队列来构建Huffman树
    std::priority_queue<std::shared_ptr<Node>,
                        std::vector<std::shared_ptr<Node>>,
                        CompareNodes>
        queue;

    // 将所有字符加入队列
    for (const auto &[ch, freq] : frequencies)
    {
        queue.push(std::make_shared<Node>(ch, freq));
    }

    // 构建Huffman树
    while (queue.size() > 1)
    {
        auto left = queue.top();
        queue.pop();
        auto right = queue.top();
        queue.pop();

        auto parent = std::make_shared<Node>('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        queue.push(parent);
    }

    if (!queue.empty())
    {
        root = queue.top();
    }
}

std::map<char, std::string> HuffmanTree::generateEncodingTable() const
{
    std::map<char, std::string> codes;
    if (root)
    {
        generateCodes(root, "", codes);
    }
    return codes;
}

void HuffmanTree::generateCodes(std::shared_ptr<Node> node, const std::string &currentCode,
                                std::map<char, std::string> &codes) const
{
    if (!node)
        return;

    // 如果是叶子节点，存储编码
    if (!node->left && !node->right)
    {
        codes[node->character] = currentCode;
        return;
    }

    // 递归处理左右子树
    generateCodes(node->left, currentCode + "0", codes);
    generateCodes(node->right, currentCode + "1", codes);
}