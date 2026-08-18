#pragma once
// S421Validator.h - S-421 数据校验引擎
//
// 对应船端技术规格书 §3.3.2 四层校验要求：
// 1. 格式校验：XML 结构符合 S-421 Schema
// 2. 属性校验：强制属性不缺失，值域合规
// 3. 逻辑校验：时间顺序、航点数量、ID 唯一性
// 4. 错误反馈：标准化提示（错误类型 + 影响对象 + 修复建议）

#include "S421DataModel.h"
#include <string>
#include <vector>
#include <ostream>

namespace s421 {

// ============================================================
//  校验结果 — 一条错误或警告
// ============================================================
struct ValidationError {
    // 错误级别
    enum class Level {
        Error,      // 错误：必须修复
        Warning     // 警告：建议修复
    };

    Level level;                // 错误级别
    std::string category;       // 校验类别：format/attribute/logic
    std::string objectId;       // 影响对象（如 "RouteInfo"、"Waypoint#3"）
    std::string field;          // 字段名（如 "routeInfoVesselMMSI"）
    std::string message;        // 错误描述 + 修复建议

    // 返回可读的错误字符串
    std::string toString() const {
        std::string prefix = (level == Level::Error) ? "❌ [错误]" : "⚠️ [警告]";
        return prefix + " [" + category + "] " + objectId + "." + field + ": " + message;
    }
};

// ============================================================
//  校验结果集合
// ============================================================
struct ValidationResult {
    std::vector<ValidationError> errors;    // 所有错误
    std::vector<ValidationError> warnings;  // 所有警告

    // 是否通过校验（无错误）
    bool ok() const { return errors.empty(); }

    // 总错误数
    size_t errorCount() const { return errors.size(); }

    // 总警告数
    size_t warningCount() const { return warnings.size(); }

    // Format all results to stream
    void format(std::ostream& os) const;
};

// ============================================================
//  S421Validator - S-421 数据校验器
//
// 用法：
//   S421Validator validator;
//   ValidationResult result = validator.validate(dataset);
//   if (!result.ok()) { ... 处理错误 ... }
// ============================================================
class S421Validator {
public:
    S421Validator() = default;

    // 校验 Dataset（主入口）
    // 返回 ValidationResult，包含所有错误和警告
    ValidationResult validate(const Dataset& dataset);

private:
    // ---- 第 1 层：格式校验 ----
    // 检查 XML 文档结构是否基本完整
    void checkStructure(const Dataset& dataset, ValidationResult& result);

    // ---- 第 2 层：属性校验 ----
    // 检查强制属性是否存在、值域是否合规
    void checkRouteAttributes(const Route& route, ValidationResult& result);
    void checkRouteInfoAttributes(const RouteInfo& info, ValidationResult& result);
    void checkWaypointAttributes(const RouteWaypoint& wp, ValidationResult& result);
    void checkLegAttributes(const RouteWaypointLeg& leg, ValidationResult& result);
    void checkActionPointAttributes(const RouteActionPoint& ap, ValidationResult& result);
    void checkScheduleAttributes(const RouteSchedule& sched, ValidationResult& result);
    void checkScheduleElementAttributes(const RouteScheduleElement& elem, ValidationResult& result);

    // ---- 第 3 层：逻辑校验 ----
    // 检查数据之间的逻辑关系
    void checkLogic(const Route& route, ValidationResult& result);

    // ---- 辅助方法 ----

    // 添加一条错误
    void addError(ValidationResult& result, const std::string& category,
                  const std::string& objectId, const std::string& field,
                  const std::string& message);

    // 添加一条警告
    void addWarning(ValidationResult& result, const std::string& category,
                    const std::string& objectId, const std::string& field,
                    const std::string& message);

    // 检查字符串是否为空
    static bool isEmpty(const std::string& s);

    // 检查 MMSI 是否合法（9位数字，XSD MMSINumberText）
    static bool isValidMMSI(const std::string& mmsi);

    // 检查 IMO 是否合法（7位数字，XSD IMONumberText）
    static bool isValidIMO(const std::string& imo);

    // 检查港口代码是否合法（5位字母，XSD PortIDType）
    static bool isValidPortID(const std::string& portId);

    // 检查 ISO 8601 时间格式（简单检查）
    static bool isValidTimeFormat(const std::string& time);
};

} // namespace s421
