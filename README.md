您的README.md已经非常完善了！我只做了一些小的优化和补充，使其更加清晰和专业：

```markdown
# SCT-Signature

一个为iOS二进制文件提供便捷签名服务的工具，通过自定义的权限映射系统，简化了复杂的权限申请流程。

> **开发团队**: iOS·StarCreate Team (iOS·星创团队)  
> **项目负责人**: AD  
> **许可证**: GNU General Public License v3.0 (GPL-3.0)

## 项目概述

SCT-Signature (又称 `sct-signer`) 是一个交互式命令行工具。其核心功能是解析用户定义的 `signature-p12.plist` 配置文件，将其中的 `com.sct.developers-*` 格式权限，通过内置映射表转换为标准的 iOS `com.apple.*` 权限，并最终使用 `ldid` 工具为二进制文件签名。

## 核心文件说明

| 文件名称 | 作用 | 是否必需 |
| :--- | :--- | :--- |
| `SCT-Signature.cpp` | 主程序源代码 | ✅ 编译必需 |
| `SCT-Permission.h` | **核心权限映射表** (SCT权限 <=> iOS标准权限) | ✅ 编译必需 |
| `entitlements.xml` | **给`sct-signer`自身签名**的权限文件 | ✅ 运行必需 |
| `signature-p12.plist` | **用户配置文件**，用于声明需要哪些SCT权限 | ✅ 签名必需 |
| `SCT-Permission-Mapping.h` | 详细的权限说明文档（**仅作参考**） | ❌ 编译时无需使用 |
| `developer-mode-signature-p12.plist` | `signature-p12.plist`的配置模板（**仅作参考**） | ❌ 编译时无需使用 |

## 快速开始

### 1. 准备工作：安装 ldid
此工具依赖 `ldid` 进行最终签名。请确保已在您的iOS设备（越狱环境）或macOS/Linux编译环境中安装它。
- **在iOS上**：通过Sileo、Cydia等包管理器搜索并安装 `ldid`。
- **在macOS上**：可通过Homebrew安装：`brew install ldid`
- **在Linux上**：可从源码编译或使用相应的包管理器安装

### 2. 编译并签名 sct-signer 自身
```bash
# 1. 编译源代码 (请根据您的环境使用合适的编译器，例如g++或clang++)
g++ -std=c++11 SCT-Signature.cpp -o sct-signer

# 2. 为编译出的 sct-signer 二进制文件签名，使其具备运行所需的权限
ldid -Sentitlements.xml sct-signer
```

### 3. 配置您的签名请求
1.  将 `developer-mode-signature-p12.plist` 复制为 `signature-p12.plist`。
2.  编辑 `signature-p12.plist`，在 `<array>` 标签内添加您需要的SCT权限。权限列表及含义请参考 `SCT-Permission-Mapping.h` 文件中的详细注释。
    ```xml
    <!-- 示例：申请后台任务和网络权限 -->
    <key>Permissions</key>
    <array>
        <string>com.sct.developers-background-task</string>
        <string>com.sct.developers-network.client</string>
        <!-- 也可以直接使用标准的Apple权限 -->
        <string>com.apple.private.security.container-required</string>
    </array>
    ```
    **注意**：`Metadata` 和 `EncryptedData` 字段目前可为空，但结构需保留。(为什么有EncryptedData和Method因为这是加密方法如: AES-256-GCM加密具体可以查看developer-mode-signature-p12.plist文件中的注释如果您比较懒可以直接在Permissions中写权限这是允许的)

### 4. 使用 sct-signer 进行签名
1.  将待签名的二进制文件（如 `my_tool`）和配置好的 `signature-p12.plist` 放入 `sct-signer` 所在目录。
2.  运行程序：
    ```bash
    ./sct-signer
    ```
3.  在交互菜单中选择操作：
    - **选项 `1`**: **签名二进制文件**。程序将自动完成权限转换并调用 `ldid` 为您的二进制文件签名。
    - **选项 `2`**: **转换plist文件**。仅将 `signature-p12.plist` 转换为标准的 `entitlements.plist` 文件供您查看，不执行签名。
    - **选项 `0`**: 退出程序。

## 权限配置说明

在 `signature-p12.plist` 中，您可以混合使用两种格式的权限：
- **SCT格式权限**：`com.sct.developers-*`（推荐，更简洁）
- **Apple标准权限**：`com.apple.*`（直接使用官方权限名）

系统会优先查找SCT权限映射，如果找不到对应的映射，会直接使用原权限名。

## 常见问题 (FAQ)

**Q: 我该如何选择需要哪些权限？**  
**A**: 请查阅 `SCT-Permission-Mapping.h` 文件，其中对每个 `com.sct.developers-*` 权限对应的iOS官方权限及其用途进行了详细说明。

**Q: 为什么一定要先给 `sct-signer` 自身签名？**  
**A**: 因为 `sct-signer` 需要执行"为其他文件签名"这一特权操作，它自身必须拥有一些基础权限（如文件系统访问、跳过库验证等）。`entitlements.xml` 就是为此准备的。

**Q: 签名后的 `entitlements.plist` 文件可以删除吗？**  
**A**: 可以。该文件是签名过程中自动生成的中间文件。签名信息已直接嵌入到目标二进制文件中，因此 `entitlements.plist` 在签名完成后即可删除。

**Q: 我可以重命名 `sct-signer` 吗？**  
**A**: 可以，程序的名称不会影响其功能。只需在签名和运行时使用相应的名称即可。

**Q: 支持哪些平台和环境？**  
**A**: 主要支持越狱iOS环境，也可以在macOS和Linux上编译运行，但需要配合适当的签名环境。

## 开源许可

本项目采用 **GNU General Public License v3.0 (GPL-3.0)** 许可证开源。
- 您可以自由地使用、修改和分发本软件。
- **任何基于本项目代码的衍生作品都必须以相同的许可证开源**。
- **禁止闭源商业使用**。

## 免责声明

本工具仅供学习和技术研究之用。请遵守当地法律法规，切勿用于非法用途。使用者需自行承担由此产生的一切后果。