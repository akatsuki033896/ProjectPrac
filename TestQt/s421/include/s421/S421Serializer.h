#pragma once
// S421Serializer.h - S-421 数据序列化器（Dataset → XML）
//
// 与 S421Parser 互为镜像：
//   Parser:     .s421 XML 文件 → Dataset 对象
//   Serializer: Dataset 对象 → .s421 XML 文件

#include "S421DataModel.h"
#include <string>
#include <map>

// 前向声明 pugixml 类型
namespace pugi {
    class xml_document;
    class xml_node;
}

namespace s421 {

// ============================================================
//  S421Serializer - 将 Dataset 序列化为 S-421 XML
//
//  核心流程：
//  1. 构建 IdMap（为所有对象分配 gml:id）
//  2. 按标准顺序输出 member/imember
//  3. optional 字段只在有值时输出
//
//  设计原则：
//  - 不修改 Dataset 对象（const 引用传入）
//  - gml:id 动态生成，从对象的 id 字段推导
//  - 与 Parser 的命名保持一致
// ============================================================
class S421Serializer {
public:
    S421Serializer();
    ~S421Serializer();

    // ---- 公共 API ----

    // 序列化 Dataset 到 .s421 XML 文件
    // 参数: dataset - 要序列化的数据集（只读）
    //       filePath - 输出文件路径
    // 返回: true=成功, false=失败（调用 getLastError() 获取原因）
    bool serializeToFile(const Dataset& dataset, const std::string& filePath);

    // 序列化 Dataset 到 XML 字符串
    // 返回: XML 字符串（失败时返回空字符串，检查 getLastError）
    std::string serializeToString(const Dataset& dataset);

    // 序列化为 .s421p 压缩容器（ZIP 包含 .s421）
    // 规格书 §3.1.2：强制输出格式
    // 参数: dataset - 数据集
    //       filePath - .s421p 输出路径
    //       compress - true=ZIP压缩, false=纯XML兼容（默认 true）
    bool serializeToCompressed(const Dataset& dataset,
                               const std::string& filePath,
                               bool compress = true);

    // 序列化为 S-100 兼容交换集（目录结构）
    // 规格书 §3.1.1 第三种格式
    // 生成文件：
    //   outputDir/CATALOG.001.XML  — 交换目录
    //   outputDir/MD_xxx.s421.xml  — ISO 19115 元数据
    //   outputDir/xxx.s421         — 航线数据
    //   outputDir/support/         — 辅助目录
    bool serializeToExchangeSet(const Dataset& dataset,
                                const std::string& outputDir,
                                const std::string& producingAgency = "Ship",
                                const std::string& agencyContact = "");

    // 获取最后一次错误信息
    const std::string& getLastError() const { return lastError_; }

    // ---- 配置选项 ----

    // 设置坐标输出精度（小数位数，默认 6）
    void setCoordinatePrecision(int digits);

private:
    std::string lastError_;
    int coordPrecision_ = 6;

    // ============================================================
    //  IdMap - 为所有对象分配 gml:id
    //
    //  S-421 XML 中每个对象都需要 gml:id 属性，
    //  容器通过 xlink:href="#ID" 引用它们。
    //  IdMap 在序列化时动态生成，不改数据模型。
    // ============================================================
    struct IdMap {
        // 固定 ID（容器/单例对象）
        std::string routeId;             // "RTE"
        std::string infoId;              // "RTE.INFO"
        std::string wptsContainerId;     // "RTE.WPTS"
        std::string aptsContainerId;     // "RTE.APTS"
        std::string schedsContainerId;   // "RTE.SCHEDS"

        // 按对象 id 索引的 ID 映射（key = 对象 id，value = gml:id 字符串）
        std::map<int, std::string> waypointIds;    // key=wp.id, "RTE.WPT.{id}"
        std::map<int, std::string> legIds;         // key=顺序号, "RTE.WPT.LEG.{n}"
        std::map<int, std::string> actionPointIds; // key=ap.id, "RTE.APT.{id}"
        std::map<int, std::string> scheduleIds;    // key=sched.id, "RTE.SCHED.{id}"

        // Schedule 子类别 ID（从 schedule id 推导）
        // key=sched.id, value="RTE.SCHED.{id}.MAN" 等
        std::map<int, std::string> schedManualIds;
        std::map<int, std::string> schedCalcIds;
        std::map<int, std::string> schedRecIds;

        // Schedule Element ID
        // key="MANUAL.1.1" (类别.调度id.顺序号), value="RTE.SCHED.1.MAN.ELEMENT.1"
        std::map<std::string, std::string> elementIds;
    };

    // 从 Dataset 构建 ID 映射
    IdMap buildIdMap(const Dataset& dataset) const;

    // ---- XML 文档构建 ----

    // 构建完整的 XML 文档
    bool buildDocument(pugi::xml_document& doc, const Dataset& dataset);

    // ---- 元素写入方法 ----

    // 写入根元素 <S421:Dataset>（含命名空间声明）
    void writeDatasetRoot(pugi::xml_node& root, const Dataset& dataset, const IdMap& ids);

    // 写入 <gml:boundedBy> 空间范围
    void writeBoundedBy(pugi::xml_node& parent, const Route& route);

    // 写入 <member><S421:Route ...></member>
    void writeRoute(pugi::xml_node& parent, const Route& route, const IdMap& ids);

    // 写入 <imember><S421:RouteInfo ...></imember>
    void writeRouteInfo(pugi::xml_node& parent, const RouteInfo& info, const IdMap& ids);

    // 写入 <member><S421:RouteWaypoints ...></member>（容器）
    void writeWaypointsContainer(pugi::xml_node& parent, const Route& route, const IdMap& ids);

    // 写入 <member><S421:RouteWaypoint ...></member>
    void writeWaypoint(pugi::xml_node& parent, const RouteWaypoint& wp,
                       const RouteWaypoint* nextWp, int wpIndex, const IdMap& ids);

    // 写入 <member><S421:RouteWaypointLeg ...></member>
    void writeLeg(pugi::xml_node& parent, const RouteWaypointLeg& leg,
                  const Position& startPos, const Position& endPos,
                  int legIndex, const IdMap& ids);

    // 写入 <member><S421:RouteActionPoints ...></member>（容器）
    void writeActionPointsContainer(pugi::xml_node& parent, const Route& route, const IdMap& ids);

    // 写入 <member><S421:RouteActionPoint ...></member>
    void writeActionPoint(pugi::xml_node& parent, const RouteActionPoint& ap, const IdMap& ids);

    // 写入调度相关（全部在 <imember> 中）
    void writeSchedulesContainer(pugi::xml_node& parent, const Route& route, const IdMap& ids);
    void writeSchedule(pugi::xml_node& parent, const RouteSchedule& sched, const IdMap& ids);
    void writeScheduleElement(pugi::xml_node& parent, const RouteScheduleElement& elem,
                              const std::string& elemId, const std::string& collectionId,
                              const char* collectionNodeName, const IdMap& ids);

    // ---- 几何写入 ----
    void writePointGeometry(pugi::xml_node& parent, const Position& pos,
                            const std::string& pointGmlId);
    void writeCurveGeometry(pugi::xml_node& parent, const Position& start,
                            const Position& end, const std::string& curveGmlId);

    // ---- 厂商扩展写入 ----
    void writeExtensions(pugi::xml_node& parent, const char* wrapperName,
                         const std::vector<RouteExtension>& extensions,
                         const std::string& baseId);

    // ---- 辅助方法 ----

    // 格式化坐标为 "lat lon" 字符串
    std::string formatPos(const Position& pos) const;

    // 写入 xlink:href 引用元素
    // <childName xlink:href="#targetId" xlink:arcrole="roleUri" />
    void writeRef(pugi::xml_node& parent, const char* childName,
                  const std::string& targetId, const char* roleUri);

    // 写入可选子元素（只在有值时写入）
    void writeOptText(pugi::xml_node& parent, const char* name,
                      const std::optional<std::string>& value);
    void writeOptDouble(pugi::xml_node& parent, const char* name,
                        const std::optional<double>& value);
    void writeOptInt(pugi::xml_node& parent, const char* name,
                     const std::optional<int>& value);
    void writeOptBool(pugi::xml_node& parent, const char* name,
                      const std::optional<bool>& value);
};

} // namespace s421
