// MiniUnzip.h - 最小 ZIP 文件读取器（纯 C++17 + zlib 解压）
//
// MiniZip 的镜像：从 ZIP 取文件。S-421 的 .s421p 是 ZIP 压缩包
// （内含一个 .s421/.gml），解析器需要先解压拿到 XML 再解析。
//
// 支持范围：
//   - ZIP 结构解析（End of Central Directory / Central Directory / Local File Header）
//   - STORE  (method=0，无压缩)：直接拷贝字节
//   - DEFLATE (method=8，常见压缩)：用 zlib 的 uncompress() 解压
//
// 注意：ZIP 用小端序存数字。x86/ARM 普通机器也是小端，但代码用
// 显式"拼字节"读法确保跨平台一致。

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

namespace s421 {

// ============================================================
//  MiniUnzip - 最小 ZIP 读取器
// ============================================================
class MiniUnzip {
public:
    // 打开 ZIP 文件并解析其目录结构
    // 返回 true = 是合法 ZIP 且解析成功；false = 不是 ZIP 或损坏（看 getError）
    bool open(const std::string& zipPath);

    // 读取 ZIP 内第一个条目，返回 {文件名, 解压后内容}
    // 失败（无条目或解压失败）返回 nullopt
    std::optional<std::pair<std::string, std::string>> readFirstEntry();

    // 获取错误信息
    const std::string& getError() const { return error_; }

private:
    // ZIP 内一个条目的目录信息（来自 Central Directory）
    struct Entry {
        std::string name;        // 文件名
        uint16_t method;         // 压缩方法：0=STORE, 8=DEFLATE
        uint32_t compSize;       // 压缩后大小
        uint32_t uncompSize;     // 原始大小
        uint32_t localOffset;    // 本地文件头在 ZIP 中的偏移
    };

    std::string error_;
    std::vector<uint8_t> data_;     // 整个 ZIP 文件读进内存（小文件，够用）
    std::vector<Entry> entries_;    // 解析出的条目列表

    // 从 data_ 的 offset 处读小端整数
    uint16_t readU16(size_t offset) const;
    uint32_t readU32(size_t offset) const;

    // 解析 End of Central Directory，返回中央目录的起始偏移；失败返回 nullopt
    std::optional<size_t> findCentralDir();

    // 解析中央目录，填充 entries_
    bool parseCentralDir(size_t cdOffset);

    // 解压一个条目的数据（按 method 选择 STORE 直拷或 DEFLATE 用 zlib）
    std::optional<std::string> extractEntry(const Entry& e);
};

} // namespace s421
