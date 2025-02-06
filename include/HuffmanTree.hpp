#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include <memory>
#include <map>
#include <queue>
#include <string>

class HuffmanTree
{
public:
    struct Node
    {
        char character;
        int frequency;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(char c, int freq) : character(c), frequency(freq), left(nullptr), right(nullptr) {}
    };

    // 构建Huffman树
    explicit HuffmanTree(const std::map<char, int> &frequencies);

    // 获取根节点
    std::shared_ptr<Node> getRoot() const { return root; }

    // 生成编码表
    std::map<char, std::string> generateEncodingTable() const;

private:
    std::shared_ptr<Node> root;

    // 递归生成编码
    void generateCodes(std::shared_ptr<Node> node, const std::string &currentCode,
                       std::map<char, std::string> &codes) const;
};

#endif // HUFFMAN_TREE_HPP