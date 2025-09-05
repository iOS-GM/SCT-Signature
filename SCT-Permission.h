// SCT-Permission.h
#ifndef SCT_PERMISSION_H
#define SCT_PERMISSION_H

#include <map>
#include <string>

// iOS·SCT团队权限与iOS权限的映射表
// com.sct.developers-开头均为iOS·SCT权限
// com.apple开头均为iOS(Apple)的权限
static std::map<std::string, std::string> permissionMap = {
    {"com.sct.developers-application-groups", "com.apple.security.application-groups"}, // 应用组权限
    {"com.sct.developers-background-task", "com.apple.security.background-task"}, // 后台任务权限
    {"com.sct.developers-container-required", "com.apple.private.security.container-required"}, // 容器访问权限
    {"com.sct.developers-network.client", "com.apple.security.network.client"}, // 网络客户端权限
    {"com.sct.developers-network.server", "com.apple.security.network.server"}, // 网络服务器权限
    {"com.sct.developers-get-task-allow", "com.apple.security.get-task-allow"}, // 调试权限
    {"com.sct.developers-skip-library-validation", "com.apple.private.skip-library-validation"}, // 跳过库验证
    {"com.sct.developers-security.no-sandbox", "com.apple.private.security.no-sandbox"}, // 无沙盒权限
    {"com.sct.developers-allow-explicit-app-level-api", "com.apple.private.allow-explicit-app-level-api"}, // 显式API权限
    {"com.sct.developers-tcc.allow", "com.apple.private.tcc.allow"}, // TCC访问权限
    {"com.sct.developers-tcc.allow.overridable", "com.apple.private.tcc.allow.overridable"}, // 可覆盖TCC权限
    {"com.sct.developers-security.keychain", "com.apple.private.security.keychain"}, // 钥匙串权限
    {"com.sct.developers-security.storage.AppData", "com.apple.private.security.storage.AppData"}, // 应用数据存储
    {"com.sct.developers-filesystems.userfs", "com.apple.private.filesystems.userfs"}, // 文件系统权限
    {"com.sct.developers-power-management", "com.apple.private.power-management"}, // 电源管理权限
    {"com.sct.developers-kernel.extensions", "com.apple.private.kernel.extensions"}, // 内核扩展权限
    {"com.sct.developers-network.socket-delegate", "com.apple.private.network.socket-delegate"}, // 网络套接字权限
    {"com.sct.developers-cs.debugger", "com.apple.private.cs.debugger"}, // 代码签名调试权限
    {"com.sct.developers-media.entitlement", "com.apple.private.media.entitlement"}, // 媒体权限
    {"com.sct.developers-location.allow", "com.apple.private.location.allow"}, // 位置权限
    {"com.sct.developers-device.identifiers", "com.apple.private.device.identifiers"}, // 设备标识符权限
    {"com.sct.developers-mach-services", "com.apple.private.mach-services"}, // Mach服务权限
    {"com.sct.developers-distributed-notifications", "com.apple.private.distributed-notifications"}, // 分布式通知权限
    {"com.sct.developers-logging.entitlement", "com.apple.private.logging.entitlement"}, // 日志权限
    {"com.sct.developers-bluetooth.allow", "com.apple.private.bluetooth.allow"}, // 蓝牙权限
    {"com.sct.developers-health.allow", "com.apple.private.health.allow"}, // 健康权限
    {"com.sct.developers-payment.entitlement", "com.apple.private.payment.entitlement"}, // 支付权限
    {"com.sct.developers-homekit.allow", "com.apple.private.homekit.allow"}, // HomeKit权限
    {"com.sct.developers-nfc.allow", "com.apple.private.nfc.allow"}, // NFC权限
    {"com.sct.developers-motion.allow", "com.apple.private.motion.allow"}, // 运动权限
    {"com.sct.developers-private.siri.allow", "com.apple.private.siri.allow"} // Siri权限
};

#endif // SCT_PERMISSION_MAPPING_TABLE_H