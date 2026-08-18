# S-421 预编静态库（直接链接，无需编译）

本包为 **Windows MSVC x64** 版的 S-421 静态库 + 头文件。
库已**自包含**（zlib、pugixml 均已编入），链接时无需任何外部依赖。

```
dist/
├── include/s421/*.h   ← 头文件（聚合头 S421.h 一次拿到全部 API）
└── lib/s421.lib        ← MSVC x64 静态库
```

## 集成步骤

### 1. 包含头文件
```cpp
#include <s421/S421.h>
```

### 2. 链接库
**CMake 工程（推荐）：**
把本包解压到某个目录（下例假设 `C:/s421`，请换成你实际的解压路径）：
```cmake
target_include_directories(你的程序 PRIVATE C:/s421/include)
target_link_directories(你的程序 PRIVATE C:/s421/lib)
target_link_libraries(你的程序 PRIVATE s421)   # 链接 s421.lib
```

**命令行（MSVC）：**
```bat
cl /std:c++17 /EHsc /IC:\s421\include main.cpp /link C:\s421\lib\s421.lib
```

## 注意事项

1. **C++17**：工程须开启（`/std:c++17`）。
2. **CRT 一致性**：`s421.lib` 以 Release 的 `/MD` CRT 编译；你的工程 Release 用 `/MD` 即可直接链。
3. **库已自包含**：zlib（解压 `.s421p`）、pugixml（XML 解析）都已编入库内，无需额外安装或链接。

## 核心 API 速查

```cpp
s421::S421Parser parser;
auto ds = parser.parseFile("route.s421");          // 解析 .s421
auto ds = parser.parseCompressedFile("x.s421p");   // 解析压缩包

s421::S421Validator validator;
auto result = validator.validate(*ds);             // 校验

s421::S421Serializer serializer;
serializer.serializeToCompressed(*ds, "out.s421p"); // 序列化
```
