#pragma once
// S421Logger.h - S-421 SDK 轻量级日志系统
//
// 设计目标：
//   - 零开销：编译时通过宏 S421_LOG_LEVEL 完全裁剪掉日志代码
//   - 四个级别：ERROR > WARN > INFO > DEBUG（数字越大越详细）
//   - 生产默认：仅输出 ERROR（安静模式，不打扰用户）
//   - 单例模式：全局唯一实例，任何地方都可以直接使用
//   - 零堆分配：不使用 new/malloc，不依赖 <iostream>
//
// 使用方法：
//   S421_LOG_ERROR("解析失败: {}", msg);     // 错误日志（始终输出）
//   S421_LOG_WARN("缺少可选字段: {}", name); // 警告日志
//   S421_LOG_INFO("已解析 {} 个航点", count); // 信息日志
//   S421_LOG_DEBUG("解析引用: {}", href);     // 调试日志（最详细）

#include <cstdio>
#include <cstdarg>
#include <cstring>

namespace s421 {

// ============================================================
//  LogLevel - 日志严重程度枚举（用于运行时过滤）
//
//  数字越大越详细，生产环境默认 Error（只输出错误）。
//  开发调试时可设为 Debug（输出全部信息）。
// ============================================================
enum class LogLevel : int {
    None  = 0,   // 关闭所有日志
    Error = 1,   // 仅错误（生产默认）
    Warn  = 2,   // 错误 + 警告
    Info  = 3,   // 错误 + 警告 + 信息
    Debug = 4    // 全部（最详细，包含调试信息）
};

// ============================================================
//  Logger - 日志记录器（线程局部单例）
//
//  默认输出位置：
//    ERROR/WARN → stderr（标准错误流）
//    INFO/DEBUG → stdout（标准输出流）
//  可通过 setOutput() 重定向到文件。
//
//  注意：通常不直接调用 log()，而是通过 S421_LOG_xxx 宏调用。
// ============================================================
class Logger {
public:
    // instance - 获取全局唯一实例
    static Logger& instance() {
        static Logger inst;
        return inst;
    }

    // setLevel - 设置最低输出级别（低于此级别的日志被丢弃）
    void setLevel(LogLevel level) { level_ = level; }

    // level - 获取当前日志级别
    LogLevel level() const { return level_; }

    // setOutput - 设置输出文件指针
    // 传入 nullptr 可重置为默认（stderr/stdout）
    void setOutput(FILE* fp) { output_ = fp; }

    // log - 核心日志函数（可变参数，printf 风格）
    //
    // 参数:
    //   level - 日志级别
    //   file  - 源文件名（通过 __FILE__ 宏自动传入）
    //   line  - 行号（通过 __LINE__ 宏自动传入）
    //   fmt   - printf 风格格式字符串（如 "已解析 %d 个航点"）
    //   ...   - 可变参数（对应 fmt 中的 %d、%s 等）
    //
    // 输出格式: [ERROR] main.cpp:42: 已解析 5 个航点
    void log(LogLevel level, const char* file, int line,
             const char* fmt, ...) {
        // 运行时过滤：消息级别 > 当前设置级别则丢弃
        if (static_cast<int>(level) > static_cast<int>(level_)) return;

        const char* prefix = "";
        switch (level) {
            case LogLevel::Error: prefix = "[ERROR]"; break;
            case LogLevel::Warn:  prefix = "[WARN] "; break;
            case LogLevel::Info:  prefix = "[INFO] "; break;
            case LogLevel::Debug: prefix = "[DEBUG]"; break;
            default: return;
        }

        // ERROR/WARN → stderr，INFO/DEBUG → stdout
        FILE* out = output_ ? output_
                   : (level <= LogLevel::Warn ? stderr : stdout);

        // 只取文件名（兼容 / 和 \ 两种路径分隔符）
        const char* basename = strrchr(file, '/');
        const char* basename2 = strrchr(file, '\\');
        if (basename2 && basename2 > basename) basename = basename2;
        if (!basename) basename = file; else ++basename;

        fprintf(out, "%s %s:%d: ", prefix, basename, line);

        // va_list 处理可变参数（C 标准库机制）
        va_list args;
        va_start(args, fmt);
        vfprintf(out, fmt, args);
        va_end(args);

        fprintf(out, "\n");
        fflush(out);  // 立即刷新，确保日志及时输出
    }

private:
    Logger() = default;                         // 私有构造（单例模式）
    LogLevel level_ = LogLevel::Error;          // 默认只输出错误（生产安静模式）
    FILE* output_ = nullptr;                    // 自定义输出文件（nullptr=使用默认）
};

// ============================================================
//  编译时级别控制宏
//
//  通过编译选项设置全局日志级别：
//    g++ -DS421_LOG_LEVEL=0  →  编译掉所有日志（零开销）
//    g++ -DS421_LOG_LEVEL=4  →  启用所有日志
//    不设置（默认）           →  启用所有日志，但运行时过滤
//
//  if constexpr 是 C++17 的编译时条件判断：
//    if constexpr (false) { ... } 中的代码根本不会被编译
//    实现真正的"零开销"——没有函数调用、没有字符串比较
// ============================================================
#ifndef S421_LOG_LEVEL
#define S421_LOG_LEVEL 4
#endif

// S421_LOG_ERROR - 错误日志（级别 1，最高优先级）
#define S421_LOG_ERROR(fmt, ...) do { \
    if constexpr (S421_LOG_LEVEL >= 1) \
        s421::Logger::instance().log(s421::LogLevel::Error, \
            __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)

// S421_LOG_WARN - 警告日志（级别 2）
#define S421_LOG_WARN(fmt, ...) do { \
    if constexpr (S421_LOG_LEVEL >= 2) \
        s421::Logger::instance().log(s421::LogLevel::Warn, \
            __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)

// S421_LOG_INFO - 信息日志（级别 3）
#define S421_LOG_INFO(fmt, ...) do { \
    if constexpr (S421_LOG_LEVEL >= 3) \
        s421::Logger::instance().log(s421::LogLevel::Info, \
            __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)

// S421_LOG_DEBUG - 调试日志（级别 4，最详细）
// ##__VA_ARGS__（GNU/MSVC 扩展）允许宏传 0 个额外参数：S421_LOG_DEBUG("hi")
#define S421_LOG_DEBUG(fmt, ...) do { \
    if constexpr (S421_LOG_LEVEL >= 4) \
        s421::Logger::instance().log(s421::LogLevel::Debug, \
            __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)

} // namespace s421
