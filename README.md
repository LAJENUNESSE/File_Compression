# C++ 文件压缩器

这是一个基于Huffman编码实现的文件压缩器，支持任意文件的压缩和解压缩操作。

## 功能特点

- 支持任意类型文件的压缩和解压缩
- 使用Huffman编码实现无损压缩
- 完整的错误处理
- 跨平台支持
- 使用现代C++17特性
- 高效的内存管理

## 编译要求

- C++17兼容的编译器
- CMake 3.10或更高版本

## 编译步骤

- 替换文件路径
```bash
test\test.ps1 
$rootDir = "PATH_TO\File_Compression"
```

- 创建构建目录：
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

测试这个文件压缩器。首先，需要创建一个测试文件，然后尝试压缩和解压缩它。以下是测试步骤：

1. 首先，创建一个测试文件：

```bash
# 在File_Compression_R1目录下创建test目录
mkdir test
cd test

# 创建一个测试文本文件
echo "This is a test file for compression. 
It contains multiple lines of text.
We will use this to test our compression program.
Some repeated text might help compression ratio.
Some repeated text might help compression ratio.
Testing 123, testing 123!" > test.txt
```

2. 测试压缩功能：
```bash
# 切换到包含编译好的程序的目录
cd ..\build\bin\Debug

# 压缩文件
compressor.exe -c ..\..\..\test\test.txt ..\..\..\test\test.compressed
```

3. 测试解压缩功能：
```bash
# 解压缩文件
compressor.exe -d ..\..\..\test\test.compressed ..\..\..\test\test_decompressed.txt
```

4. 验证结果：
```bash
# 比较原文件和解压缩后的文件
fc ..\..\..\test\test.txt ..\..\..\test\test_decompressed.txt
```

测试不同类型的文件：

1. 二进制文件测试：
```bash
# 创建一个包含二进制数据的文件
copy ..\..\..\build\bin\Debug\compressor.exe ..\..\..\test\binary_test.exe
compressor.exe -c ..\..\..\test\binary_test.exe ..\..\..\test\binary_test.compressed
compressor.exe -d ..\..\..\test\binary_test.compressed ..\..\..\test\binary_test_decompressed.exe
```

2. 大文件测试：
```bash
# 创建一个较大的文件
fsutil file createnew ..\..\..\test\large_file.dat 1048576
compressor.exe -c ..\..\..\test\large_file.dat ..\..\..\test\large_file.compressed
compressor.exe -d ..\..\..\test\large_file.compressed ..\..\..\test\large_file_decompressed.dat
```

3. 错误处理测试：
```bash
# 测试不存在的文件
compressor.exe -c nonexistent.txt output.compressed

# 测试损坏的压缩文件
echo "This is not a valid compressed file" > ..\..\..\test\invalid.compressed
compressor.exe -d ..\..\..\test\invalid.compressed ..\..\..\test\output.txt
```

4. 性能测试：
```bash
# 使用PowerShell测量执行时间
Measure-Command { .\compressor.exe -c ..\..\..\test\large_file.dat ..\..\..\test\large_file.compressed }
```

检查压缩效果：
```powershell
# 检查原文件大小
(Get-Item ..\..\..\test\test.txt).length

# 检查压缩文件大小
(Get-Item ..\..\..\test\test.compressed).length
```

以下是一个完整的测试脚本（保存为test.ps1）：  
使用这种方式须在`vscode`添加拓展`@id:ms-vscode.powershell`

```powershell name=test.ps1
# 创建测试目录
$testDir = "..\..\..\test"
New-Item -ItemType Directory -Force -Path $testDir

# 创建测试文件
$testContent = @"
This is a test file for compression.
It contains multiple lines of text.
We will use this to test our compression program.
Some repeated text might help compression ratio.
Some repeated text might help compression ratio.
Testing 123, testing 123!
"@
Set-Content -Path "$testDir\test.txt" -Value $testContent

Write-Host "Testing text file compression..."
# 压缩测试
.\compressor.exe -c "$testDir\test.txt" "$testDir\test.compressed"
if ($LASTEXITCODE -eq 0) {
    Write-Host "Compression successful"
    $originalSize = (Get-Item "$testDir\test.txt").Length
    $compressedSize = (Get-Item "$testDir\test.compressed").Length
    Write-Host "Original size: $originalSize bytes"
    Write-Host "Compressed size: $compressedSize bytes"
    Write-Host "Compression ratio: $([math]::Round(100 * $compressedSize / $originalSize, 2))%"
} else {
    Write-Host "Compression failed"
    exit 1
}

Write-Host "`nTesting text file decompression..."
# 解压缩测试
.\compressor.exe -d "$testDir\test.compressed" "$testDir\test_decompressed.txt"
if ($LASTEXITCODE -eq 0) {
    Write-Host "Decompression successful"
    if ((Get-FileHash "$testDir\test.txt").Hash -eq (Get-FileHash "$testDir\test_decompressed.txt").Hash) {
        Write-Host "Files match - decompression was perfect!"
    } else {
        Write-Host "Error: Decompressed file doesn't match original"
        exit 1
    }
} else {
    Write-Host "Decompression failed"
    exit 1
}

Write-Host "`nTesting error handling..."
# 错误处理测试
Write-Host "Testing non-existent file..."
.\compressor.exe -c "nonexistent.txt" "$testDir\output.compressed"

Write-Host "`nTesting invalid compressed file..."
Set-Content -Path "$testDir\invalid.compressed" -Value "This is not a valid compressed file"
.\compressor.exe -d "$testDir\invalid.compressed" "$testDir\output.txt"

Write-Host "`nAll tests completed!"
```

要运行测试，只需在PowerShell中执行：
```powershell
.\test.ps1
```