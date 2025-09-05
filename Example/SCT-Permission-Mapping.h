// SCT-Permission-Mapping.h
// SCT-Permission.h
// SCT-Permission-Mapping.h原始版本的映射表，这个映射表是不可以用的因为仅展示用
// iOS·SCT团队权限与iOS权限的映射表
// 目前兼容iOS15～16.6.1或者iOS15～17.0
// SCT-Permission.h版本号: 1.1-27
// com.sct.developers-开头均为iOS·SCT权限
// com.apple开头均为iOS(Apple)的权限
com.apple.security.application-groups = com.sct.developers-application-groups // 应用组权限，允许二进制文件访问共享的容器目录或进行进程间通信
com.apple.security.background-task = com.sct.developers-background-task // 允许二进制文件在后台运行任务
com.apple.private.security.container-required = com.sct.developers-container-required // 允许二进制文件访问设备上的所有文件（受隐私限制的除外）
com.apple.security.network.client = com.sct.developers-network.client // 允许二进制文件进行网络客户端操作
com.apple.security.network.server = com.sct.developers-network.server // 允许二进制文件作为网络服务器监听端口
com.apple.security.get-task-allow = com.sct.developers-get-task-allow // 允许二进制文件调试其他进程（通常需要开发者证书）
com.apple.private.skip-library-validation = com.sct.developers-skip-library-validation // 允许二进制文件安装其他应用或包
com.apple.private.security.no-sandbox = com.sct.developers-security.no-sandbox // 允许二进制文件加载未签名的代码或插件
com.apple.private.allow-explicit-app-level-api = com.sct.developers-allow-explicit-app-level-api // 允许二进制文件绕过沙盒限制（常见于越狱环境）
com.apple.private.tcc.allow = com.sct.developers-tcc.allow // 允许二进制文件使用动态代码生成（如JIT编译）
com.apple.private.tcc.allow.overridable = com.sct.developers-tcc.allow.overridable // 允许二进制文件访问敏感数据（如通讯录、日历），需用户授权
com.apple.private.security.keychain = com.sct.developers-security.keychain // 允许二进制文件使用设备硬件密钥链
com.apple.private.security.storage.AppData = com.sct.developers-security.storage.AppData // 允许二进制文件执行特权操作（如root权限操作，常见于越狱）
com.apple.private.filesystems.userfs = com.sct.developers-filesystems.userfs // 允许二进制文件管理文件和目录（包括系统区域）
com.apple.private.power-management = com.sct.developers-power-management // 允许二进制文件控制设备电源管理或睡眠
com.apple.private.kernel.extensions = com.sct.developers-kernel.extensions // 允许二进制文件访问内核扩展或驱动（需额外权限）
com.apple.private.network.socket-delegate = com.sct.developers-network.socket-delegate // 允许二进制文件进行系统级配置（如网络设置）
com.apple.private.cs.debugger = com.sct.developers-cs.debugger // 允许二进制文件绕过代码签名强制要求
com.apple.private.media.entitlement = com.sct.developers-media.entitlement // 允许二进制文件使用设备摄像头或麦克风
com.apple.private.location.allow = com.sct.developers-location.allow // 允许二进制文件访问位置服务（需用户授权）
com.apple.private.device.identifiers = com.sct.developers-device.identifiers // 允许二进制文件读取设备标识符（如UDID）
com.apple.private.mach-services = com.sct.developers-mach-services // 允许二进制文件执行进程间通信（IPC）的底层操作
com.apple.private.distributed-notifications = com.sct.developers-distributed-notifications // 允许二进制文件使用分布式通知机制
com.apple.private.logging.entitlement = com.sct.developers-logging.entitlement // 允许二进制文件访问系统日志或诊断数据
com.apple.private.bluetooth.allow = com.sct.developers-bluetooth.allow // 允许二进制文件控制蓝牙硬件
com.apple.private.health.allow = com.sct.developers-health.allow // 允许二进制文件访问健康相关数据（需用户授权）
com.apple.private.payment.entitlement = com.sct.developers-payment.entitlement // 允许二进制文件使用Apple Pay或支付服务
com.apple.private.homekit.allow = com.sct.developers-homekit.allow // 允许二进制文件访问家庭Kit设备数据
com.apple.private.nfc.allow = com.sct.developers-nfc.allow // 允许二进制文件使用设备NFC功能
com.apple.private.motion.allow = com.sct.developers-motion.allow // 允许二进制文件访问运动传感器协处理器数据
com.apple.private.siri.allow = com.sct.developers-private.siri.allow // 允许二进制文件使用Siri集成功能