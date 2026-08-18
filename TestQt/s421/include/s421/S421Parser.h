#pragma once
// S421Parser.h - S-421 XML 文件解析器

#include "S421DataModel.h"
#include <string>
#include <memory>
#include <unordered_map>

// 前向声明 pugixml 类型，避免在头文件中包含 pugixml.hpp
namespace pugi {
    class xml_document;
    class xml_node;
}

namespace s421 {

// ============================================================
//  S421Parser - 解析 S-421 XML 文件
//
//  支持三种输入格式：
//  1. .s421   - 直接解析 XML
//  2. .s421p  - 先尝试 ZIP 解压，再解析 XML；失败则直接当 XML 解析
//  3. 交换集  - 先解析 CATALOG.001.XML，再定位 .s421 文件
// ============================================================
class S421Parser {
public:
    S421Parser();
    ~S421Parser();

    // 解析 .s421 或 .s421.gml 文件（直接 XML）
    // 参数: 文件路径
    // 返回: 解析成功返回 Dataset 对象（调用方获得所有权），失败返回 nullptr
    std::unique_ptr<Dataset> parseFile(const std::string& filePath);

    // 解析 .s421p 压缩容器文件
    std::unique_ptr<Dataset> parseCompressedFile(const std::string& filePath);

    // 解析 S-100 兼容交换集（目录结构）
    // 规格书 §3.1.1 第三种格式：读取 CATALOG.001.XML 定位 .s421 文件
    // 参数: exchangeSetDir - 交换集目录路径（包含 CATALOG.001.XML）
    std::unique_ptr<Dataset> parseExchangeSet(const std::string& exchangeSetDir);

    // 解析 XML 字符串（用于测试或网络接收）
    std::unique_ptr<Dataset> parseString(const std::string& xmlContent);

    // 获取最后一次错误信息
    const std::string& getLastError() const { return lastError_; }

private:
    std::string lastError_;

    // ---- 内部解析方法 ----

    // 从 pugixml 文档对象构建 Dataset
    std::unique_ptr<Dataset> buildFromDoc(pugi::xml_document& doc);

    // 解析 Route 元素
    std::unique_ptr<Route> parseRoute(pugi::xml_node routeNode);

    // 解析 RouteInfo 元素
    std::unique_ptr<RouteInfo> parseRouteInfo(pugi::xml_node infoNode);

    // 解析 RouteWaypoint 元素
    std::unique_ptr<RouteWaypoint> parseWaypoint(pugi::xml_node wpNode);

    // 解析 RouteWaypointLeg 元素
    std::unique_ptr<RouteWaypointLeg> parseLeg(pugi::xml_node legNode);

    // 解析 RouteActionPoint 元素
    std::unique_ptr<RouteActionPoint> parseActionPoint(pugi::xml_node apNode);

    // 解析 RouteSchedule 及其子元素
    std::unique_ptr<RouteSchedule> parseSchedule(pugi::xml_node schedNode);
    std::unique_ptr<RouteScheduleElement> parseScheduleElement(pugi::xml_node elemNode);

    // 解析 RouteExtensions
    RouteExtension parseExtension(pugi::xml_node extNode);

    // ---- 辅助方法 ----

    // 解析几何坐标（Point → Position）
    Position parsePointGeometry(pugi::xml_node geometryNode);

    // 解析 gml:pos 字符串 "lat lon" → Position
    Position parseGmlPos(const std::string& posStr);

    // 安全获取子节点文本（不存在则返回 optional 的空值）
    std::optional<std::string> getOptText(pugi::xml_node parent, const char* childName);
    std::optional<double> getOptDouble(pugi::xml_node parent, const char* childName);
    std::optional<int> getOptInt(pugi::xml_node parent, const char* childName);
    std::optional<bool> getOptBool(pugi::xml_node parent, const char* childName);

    // 通过 xlink:href="#GML.ID" 在文档中查找对应节点
    // 参数 docNode: 文档根节点的引用
    // 参数 href:   xlink:href 的值，如 "#RTE.INFO"
    // 返回: 找到的节点（找不到则返回空节点）
    pugi::xml_node resolveRef(pugi::xml_node docNode, const char* href);
};

} // namespace s421
