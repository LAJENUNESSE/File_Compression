# 设置控制台编码为 UTF-8
$OutputEncoding = [Console]::OutputEncoding = [Text.UTF8Encoding]::UTF8

# 设置路径
$rootDir = "C:\Users\liu69\Desktop\File_Compression"
$testDir = "$rootDir\test"
$compressorPath = "$rootDir\build\bin\Debug\compressor.exe"

# 确保compressor.exe存在
if (-not (Test-Path $compressorPath)) {
    Write-Host "错误：在路径 $compressorPath 找不到 compressor.exe"
    Write-Host "请确保您已成功构建项目"
    exit 1
}

# 创建测试目录
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

Write-Host "测试文本文件压缩..."
# 压缩测试
& $compressorPath -c "$testDir\test.txt" "$testDir\test.compressed"
if ($LASTEXITCODE -eq 0) {
    Write-Host "压缩成功"
    $originalSize = (Get-Item "$testDir\test.txt").Length
    $compressedSize = (Get-Item "$testDir\test.compressed").Length
    Write-Host "原文件大小：$originalSize 字节"
    Write-Host "压缩文件大小：$compressedSize 字节"
    Write-Host "压缩比率：$([math]::Round(100 * $compressedSize / $originalSize, 2))%"
}
else {
    Write-Host "压缩失败"
    exit 1
}

Write-Host "`n测试文本文件解压缩..."
# 解压缩测试
& $compressorPath -d "$testDir\test.compressed" "$testDir\test_decompressed.txt"
if ($LASTEXITCODE -eq 0) {
    Write-Host "解压缩成功"
    if ((Get-FileHash "$testDir\test.txt").Hash -eq (Get-FileHash "$testDir\test_decompressed.txt").Hash) {
        Write-Host "文件匹配 - 解压缩完全正确!"
    }
    else {
        Write-Host "错误：解压缩后的文件与原始文件不匹配"
        exit 1
    }
}
else {
    Write-Host "解压缩失败"
    exit 1
}

Write-Host "`n测试错误处理..."
# 错误处理测试
Write-Host "测试不存在的文件..."
& $compressorPath -c "nonexistent.txt" "$testDir\output.compressed"

Write-Host "`n测试无效的压缩文件..."
Set-Content -Path "$testDir\invalid.compressed" -Value "This is not a valid compressed file"
& $compressorPath -d "$testDir\invalid.compressed" "$testDir\output.txt"

Write-Host "`n所有测试完成!"