#include "Compressor.hpp"
#include <fstream>
#include <iostream>
#include <bitset>

bool Compressor::compress(const std::string &inputFile, const std::string &outputFile)
{
    try
    {
        // 读取输入文件
        auto data = readFile(inputFile);
        if (data.empty())
        {
            std::cerr << "错误： 输入文件为空或无法读取" << std::endl;
            return false;
        }

        // 计算频率
        auto frequencies = calculateFrequencies(data);

        // 构建Huffman树
        HuffmanTree tree(frequencies);

        // 生成编码表
        auto encodingTable = createEncodingTable(tree);

        // 压缩数据
        std::string bits;
        for (char c : data)
        {
            bits += encodingTable[c];
        }

        // 写入压缩文件头信息
        std::vector<char> compressedData;

        // 写入原始文件大小
        uint64_t fileSize = data.size();
        for (int i = 0; i < 8; ++i)
        {
            compressedData.push_back(static_cast<char>((fileSize >> (i * 8)) & 0xFF));
        }

        // 写入编码表大小
        uint16_t tableSize = static_cast<uint16_t>(frequencies.size());
        compressedData.push_back(static_cast<char>(tableSize & 0xFF));
        compressedData.push_back(static_cast<char>((tableSize >> 8) & 0xFF));

        // 写入编码表
        for (const auto &[ch, freq] : frequencies)
        {
            compressedData.push_back(ch);
            for (int i = 0; i < 4; ++i)
            {
                compressedData.push_back(static_cast<char>((freq >> (i * 8)) & 0xFF));
            }
        }

        // 写入压缩后的数据
        auto compressedBytes = bitsToBytes(bits);
        compressedData.insert(compressedData.end(), compressedBytes.begin(), compressedBytes.end());

        return writeFile(outputFile, compressedData);
    }
    catch (const std::exception &e)
    {
        std::cerr << "压缩错误： " << e.what() << std::endl;
        return false;
    }
}

bool Compressor::decompress(const std::string &inputFile, const std::string &outputFile)
{
    try
    {
        // 读取压缩文件
        auto compressedData = readFile(inputFile);
        if (compressedData.size() < 10)
        {
            std::cerr << "错误： 压缩文件格式无效" << std::endl;
            return false;
        }

        size_t pos = 0;

        // 读取原始文件大小
        uint64_t originalSize = 0;
        for (int i = 0; i < 8; ++i)
        {
            if (pos >= compressedData.size())
            {
                std::cerr << "错误： 压缩文件格式无效" << std::endl;
                return false;
            }
            originalSize |= static_cast<uint64_t>(static_cast<unsigned char>(compressedData[pos++])) << (i * 8);
        }

        // 读取编码表大小
        if (pos + 2 > compressedData.size())
        {
            std::cerr << "错误： 压缩文件格式无效" << std::endl;
            return false;
        }
        uint16_t tableSize = static_cast<unsigned char>(compressedData[pos++]);
        tableSize |= static_cast<uint16_t>(static_cast<unsigned char>(compressedData[pos++])) << 8;

        // 读取编码表
        std::map<char, int> frequencies;
        for (int i = 0; i < tableSize; ++i)
        {
            if (pos >= compressedData.size())
            {
                std::cerr << "错误： 压缩文件格式无效" << std::endl;
                return false;
            }
            char ch = compressedData[pos++];
            int freq = 0;
            for (int j = 0; j < 4; ++j)
            {
                if (pos >= compressedData.size())
                {
                    std::cerr << "错误： 压缩文件格式无效" << std::endl;
                    return false;
                }
                freq |= static_cast<int>(static_cast<unsigned char>(compressedData[pos++])) << (j * 8);
            }
            frequencies[ch] = freq;
        }

        // 重建Huffman树
        HuffmanTree tree(frequencies);

        // 解压数据
        std::vector<char> decompressedData;
        std::string bits = bytesToBits(std::vector<char>(compressedData.begin() + pos, compressedData.end()),
                                       originalSize * 8);

        auto node = tree.getRoot();
        for (char bit : bits)
        {
            if (!node)
                break;

            if (bit == '0')
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }

            if (node && !node->left && !node->right)
            {
                decompressedData.push_back(node->character);
                node = tree.getRoot();
            }
        }

        if (decompressedData.size() != originalSize)
        {
            std::cerr << "错误： 解压缩数据大小与原始数据大小不符" << std::endl;
            return false;
        }

        return writeFile(outputFile, decompressedData);
    }
    catch (const std::exception &e)
    {
        std::cerr << "解压缩错误： " << e.what() << std::endl;
        return false;
    }
}

std::vector<char> Compressor::readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("无法打开输入文件");
    }

    return std::vector<char>((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
}

bool Compressor::writeFile(const std::string &filename, const std::vector<char> &data)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        return false;
    }

    file.write(data.data(), data.size());
    return true;
}

std::map<char, int> Compressor::calculateFrequencies(const std::vector<char> &data)
{
    std::map<char, int> frequencies;
    for (char c : data)
    {
        frequencies[c]++;
    }
    return frequencies;
}

std::map<char, std::string> Compressor::createEncodingTable(const HuffmanTree &tree)
{
    return tree.generateEncodingTable();
}

std::vector<char> Compressor::bitsToBytes(const std::string &bits)
{
    std::vector<char> bytes;
    for (size_t i = 0; i < bits.length(); i += 8)
    {
        std::string byte = bits.substr(i, 8);
        while (byte.length() < 8)
        {
            byte += '0';
        }
        bytes.push_back(static_cast<char>(std::bitset<8>(byte).to_ulong()));
    }
    return bytes;
}

std::string Compressor::bytesToBits(const std::vector<char> &bytes, size_t validBits)
{
    std::string bits;
    for (char byte : bytes)
    {
        std::bitset<8> b(static_cast<unsigned char>(byte));
        bits += b.to_string();
    }
    return bits.substr(0, validBits);
}