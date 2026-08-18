// MiniZip.h - 最小 ZIP 文件生成器（纯 C++17，无外部依赖）
//
// 仅实现"创建 ZIP 包"这一个功能，不需要解压。
// ZIP 结构：Local File Header + 文件数据 + Central Directory + End Record。

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <ctime>

namespace s421 {

// ============================================================
//  MiniZip - 最小 ZIP 文件写入器
// ============================================================
class MiniZip {
public:
    // 添加一个文件到 ZIP 中
    // fileName: ZIP 内的文件名（路径）
    // data:     文件内容
    void addFile(const std::string& fileName, const std::string& data);

    // 添加一个文件（从磁盘路径读取）
    bool addFileFromDisk(const std::string& fileName, const std::string& diskPath);

    // 写入 ZIP 文件
    bool writeTo(const std::string& zipPath);

    // 获取错误信息
    const std::string& getError() const { return error_; }

private:
    std::string error_;

    // ZIP 内部：一个文件的条目
    struct Entry {
        std::string fileName;       // 文件名
        std::vector<uint8_t> data;  // 文件数据（未压缩，STORE 模式）
        uint32_t crc32;             // CRC32 校验和
    };

    std::vector<Entry> entries_;

    // 计算 CRC32
    static uint32_t computeCRC32(const uint8_t* data, size_t length);

    // 写入小端序整数
    static void writeU16(std::ostream& os, uint16_t v);
    static void writeU32(std::ostream& os, uint32_t v);

    // 获取当前 DOS 日期时间
    static uint16_t dosTime();
    static uint16_t dosDate();
};

} // namespace s421
