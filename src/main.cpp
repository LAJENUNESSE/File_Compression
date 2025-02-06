#include <iostream>
#include <string>
#include "Compressor.hpp"

void printUsage()
{
    std::cout << "使用方法" << std::endl
              << "压缩：program -c <input_file> <output_file>" << std::endl
              << "解压缩：program -d <input_file> <output_file>" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printUsage();
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    Compressor compressor;

    try
    {
        if (mode == "-c")
        {
            std::cout << "压缩 " << inputFile << " 到 " << outputFile << "..." << std::endl;
            if (compressor.compress(inputFile, outputFile))
            {
                std::cout << "压缩成功" << std::endl;
                return 0;
            }
        }
        else if (mode == "-d")
        {
            std::cout << "解压缩 " << inputFile << " 到 " << outputFile << "..." << std::endl;
            if (compressor.decompress(inputFile, outputFile))
            {
                std::cout << "减压成功" << std::endl;
                return 0;
            }
        }
        else
        {
            printUsage();
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "错误： " << e.what() << std::endl;
        return 1;
    }

    std::cerr << "操作失败" << std::endl;
    return 1;
}