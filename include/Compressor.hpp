#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "HuffmanTree.hpp"

class Compressor
{
public:
    // 压缩文件
    bool compress(const std::string &inputFile, const std::string &outputFile);

    // 解压缩文件
    bool decompress(const std::string &inputFile, const std::string &outputFile);

private:
    // 读取文件内容
    std::vector<char> readFile(const std::string &filename);

    // 写入文件内容
    bool writeFile(const std::string &filename, const std::vector<char> &data);

    // 统计字符频率
    std::map<char, int> calculateFrequencies(const std::vector<char> &data);

    // 创建Huffman编码表
    std::map<char, std::string> createEncodingTable(const HuffmanTree &tree);

    // 将比特流写入字节
    std::vector<char> bitsToBytes(const std::string &bits);

    // 将字节转换为比特流
    std::string bytesToBits(const std::vector<char> &bytes, size_t validBits);
};

#endif // COMPRESSOR_HPP