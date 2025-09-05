// SCT-Signature.cpp
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <vector>
#include <map>
#include <algorithm>

// 包含权限映射表
#include "SCT-Permission.h"

// 开发阶段版本信息
#define SCT_SIGNER_VERSION "v1.2-6"

// clang优化提示
#ifdef __clang__
#define CLANG_OPTIMIZE __attribute__((optnone))
#else
#define CLANG_OPTIMIZE
#endif

// 简单的XML解析器
class SimplePlistParser {
private:
    std::string content;
    size_t pos;
    
    void skipWhitespace() {
        while (pos < content.size() && isspace(content[pos])) pos++;
    }
    
public:
    SimplePlistParser(const std::string& data) : content(data), pos(0) {}
    
    bool parsePermissions(std::vector<std::string>& permissions) {
        // 寻找Permissions键
        size_t perms_start = content.find("<key>Permissions</key>");
        if (perms_start == std::string::npos) {
            std::cerr << "警告: 未找到Permissions键" << std::endl;
            return false;
        }
        
        // 找到后面的array
        size_t array_start = content.find("<array>", perms_start);
        if (array_start == std::string::npos) {
            std::cerr << "错误: 未找到array标签" << std::endl;
            return false;
        }
        
        size_t array_end = content.find("</array>", array_start);
        if (array_end == std::string::npos) {
            std::cerr << "错误: 未找到array结束标签" << std::endl;
            return false;
        }
        
        // 提取array中的所有string
        std::string array_content = content.substr(array_start + 7, array_end - array_start - 7);
        size_t str_pos = 0;
        
        while (true) {
            size_t str_start = array_content.find("<string>", str_pos);
            if (str_start == std::string::npos) break;
            
            size_t str_end = array_content.find("</string>", str_start);
            if (str_end == std::string::npos) break;
            
            std::string permission = array_content.substr(str_start + 8, str_end - str_start - 8);
            // 移除可能的空白字符
            permission.erase(0, permission.find_first_not_of(" \t\n\r\f\v"));
            permission.erase(permission.find_last_not_of(" \t\n\r\f\v") + 1);
            
            if (!permission.empty()) {
                permissions.push_back(permission);
            }
            
            str_pos = str_end + 9;
        }
        
        return true;
    }
};

// 转换SCT权限到iOS标准权限
std::string convertSctToIosPermission(const std::string& sctPermission) {
    auto it = permissionMap.find(sctPermission);
    if (it != permissionMap.end()) {
        return it->second;
    }
    // 如果找不到映射，检查是否是标准iOS权限
    if (sctPermission.find("com.apple.") == 0) {
        return sctPermission;
    }
    // 未知权限，发出警告但继续
    std::cerr << "警告: 未知权限 '" << sctPermission << "'，将原样使用" << std::endl;
    return sctPermission;
}

// 生成entitlements.plist文件
bool generateEntitlements(const std::vector<std::string>& permissions, const std::string& outputPath) {
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        std::cerr << "错误: 无法创建entitlements.plist文件: " << outputPath << std::endl;
        return false;
    }
    
    // 写入entitlements.plist头部
    outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    outFile << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
    outFile << "<plist version=\"1.0\">\n";
    outFile << "<dict>\n";
    
    // 写入权限（转换SCT权限到iOS标准权限）
    for (const auto& perm : permissions) {
        std::string iosPerm = convertSctToIosPermission(perm);
        outFile << "    <key>" << iosPerm << "</key>\n";
        outFile << "    <true/>\n";
    }
    
    // 写入文件尾部
    outFile << "</dict>\n";
    outFile << "</plist>\n";
    
    outFile.close();
    return true;
}

// 验证entitlements文件内容
bool verifyEntitlements(const std::string& entitlementsPath) {
    std::ifstream inFile(entitlementsPath);
    if (!inFile.is_open()) {
        std::cerr << "错误: 无法打开entitlements文件进行验证: " << entitlementsPath << std::endl;
        return false;
    }
    
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string content = buffer.str();
    inFile.close();
    
    // 简单的验证：检查是否包含基本的plist结构
    if (content.find("<?xml") == std::string::npos || 
        content.find("<plist") == std::string::npos ||
        content.find("<dict>") == std::string::npos) {
        std::cerr << "警告: entitlements文件格式可能不正确" << std::endl;
        return false;
    }
    
    std::cout << "entitlements文件验证通过，内容格式正确" << std::endl;
    return true;
}

// 显示entitlements文件内容
void showEntitlementsContent(const std::string& entitlementsPath) {
    std::ifstream inFile(entitlementsPath);
    if (!inFile.is_open()) {
        std::cerr << "无法打开entitlements文件查看内容: " << entitlementsPath << std::endl;
        return;
    }
    
    std::cout << "\n=== entitlements.plist 文件内容 ===" << std::endl;
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
    std::cout << "=== 内容结束 ===\n" << std::endl;
    
    inFile.close();
}

// 安全的命令执行函数（替代system）
int executeCommand(const std::string& command) CLANG_OPTIMIZE {
    std::cout << "执行命令: " << command << std::endl;
    
    // 使用fork + execvp来执行命令
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "错误: 创建子进程失败" << std::endl;
        return -1;
    }
    
    if (pid == 0) {
        // 子进程
        // 解析命令为参数数组
        std::vector<std::string> args;
        std::istringstream iss(command);
        std::string token;
        
        while (iss >> token) {
            // 处理带引号的参数
            if (token.front() == '"' && token.back() == '"') {
                token = token.substr(1, token.size() - 2);
            }
            args.push_back(token);
        }
        
        // 转换为char*数组
        std::vector<char*> argv;
        for (auto& arg : args) {
            argv.push_back(&arg[0]);
        }
        argv.push_back(nullptr);
        
        // 执行命令
        execvp(argv[0], argv.data());
        
        // 如果execvp失败
        std::cerr << "错误: 执行命令失败: " << command << std::endl;
        exit(EXIT_FAILURE);
    } else {
        // 父进程
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
}

// 检查文件是否存在
bool fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

// 获取文件名（不含路径）
std::string getBaseName(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) {
        return path;
    }
    return path.substr(pos + 1);
}

// 检查ldid是否可用（使用execvp替代system）
bool checkLdidAvailable() {
    pid_t pid = fork();
    if (pid == -1) {
        return false;
    }
    
    if (pid == 0) {
        // 子进程
        char* argv[] = {"which", "ldid", nullptr};
        execvp("which", argv);
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

// 验证二进制文件是否已签名
bool verifyBinarySignature(const std::string& binaryPath) {
    pid_t pid = fork();
    if (pid == -1) {
        return false;
    }
    
    if (pid == 0) {
        // 子进程
        std::string arg1 = "-e";
        char* argv[] = {"ldid", &arg1[0], const_cast<char*>(binaryPath.c_str()), nullptr};
        execvp("ldid", argv);
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

// 显示菜单
void showMenu() {
    std::cout << "\n=== iOS-SCT 签名器 " << SCT_SIGNER_VERSION << " ===" << std::endl;
    std::cout << "请选择操作:" << std::endl;
    std::cout << "1. 签名二进制文件 (默认)" << std::endl;
    std::cout << "2. 转换 signature-p12.plist 为 entitlements.plist (开发阶段)" << std::endl;
    std::cout << "0. 退出" << std::endl;
    std::cout << "请输入选项 (1/2/0): ";
}

// 选项1: 签名二进制文件
void optionSignBinary() {
    std::cout << "\n=== 签名二进制文件 ===" << std::endl;
    
    std::string plistPath = "signature-p12.plist";
    std::string binaryPath;
    
    // 检查signature-p12.plist是否存在
    if (!fileExists(plistPath)) {
        std::cerr << "错误: 当前目录下未找到 signature-p12.plist 文件" << std::endl;
        return;
    }
    
    // 获取二进制文件名
    std::cout << "请输入要签名的二进制文件名: ";
    std::getline(std::cin, binaryPath);
    
    if (binaryPath.empty()) {
        std::cerr << "错误: 未输入二进制文件名" << std::endl;
        return;
    }
    
    // 检查二进制文件是否存在
    if (!fileExists(binaryPath)) {
        std::cerr << "错误: 二进制文件不存在: " << binaryPath << std::endl;
        return;
    }
    
    // 检查ldid是否可用
    if (!checkLdidAvailable()) {
        std::cerr << "错误: ldid 工具未安装，请先安装 ldid" << std::endl;
        std::cerr << "可以通过以下方式安装:" << std::endl;
        std::cerr << "  macOS: brew install ldid" << std::endl;
        std::cerr << "  Linux: 从Cydia或越狱商店获取" << std::endl;
        std::cerr << "  或者从: https://github.com/ProcursusTeam/ldid" << std::endl;
        return;
    }
    
    // 读取plist文件内容
    std::ifstream plistFile(plistPath);
    if (!plistFile.is_open()) {
        std::cerr << "错误: 无法打开文件: " << plistPath << std::endl;
        return;
    }
    
    std::stringstream buffer;
    buffer << plistFile.rdbuf();
    std::string plistContent = buffer.str();
    plistFile.close();
    
    // 解析plist文件
    SimplePlistParser parser(plistContent);
    std::vector<std::string> permissions;
    
    if (!parser.parsePermissions(permissions)) {
        std::cerr << "警告: 解析Permissions失败或未找到权限" << std::endl;
    }
    
    if (permissions.empty()) {
        std::cout << "信息: 没有找到任何权限" << std::endl;
    } else {
        std::cout << "找到 " << permissions.size() << " 个权限:" << std::endl;
        for (const auto& perm : permissions) {
            std::string iosPerm = convertSctToIosPermission(perm);
            std::cout << "  " << perm << " -> " << iosPerm << std::endl;
        }
    }
    
    // 生成entitlements文件
    std::string entitlementsPath = "entitlements.plist";
    
    if (!generateEntitlements(permissions, entitlementsPath)) {
        return;
    }
    
    std::cout << "已生成 entitlements.plist 文件" << std::endl;
    
    // 验证entitlements文件
    if (!verifyEntitlements(entitlementsPath)) {
        std::cerr << "警告: entitlements文件验证失败" << std::endl;
    }
    
    // 使用ldid进行签名
    std::string command = "ldid -S" + entitlementsPath + " " + binaryPath;
    
    int result = executeCommand(command);
    
    if (result != 0) {
        std::cerr << "错误: ldid签名失败，代码: " << result << std::endl;
        return;
    }
    
    // 验证签名是否成功
    if (verifyBinarySignature(binaryPath)) {
        std::cout << "✓ 二进制文件签名验证成功" << std::endl;
    } else {
        std::cerr << "⚠️ 二进制文件签名验证失败" << std::endl;
    }
    
    std::cout << "\n✅ 签名成功完成!" << std::endl;
    std::cout << "📦 二进制文件: " << binaryPath << std::endl;
    std::cout << "📋 权限文件: entitlements.plist (已保留)" << std::endl;
}

// 选项2: 转换plist文件
void optionConvertPlist() {
    std::cout << "\n=== 转换 signature-p12.plist 为 entitlements.plist ===" << std::endl;
    
    std::string plistPath = "signature-p12.plist";
    
    // 检查文件是否存在
    if (!fileExists(plistPath)) {
        std::cerr << "错误: 当前目录下未找到 signature-p12.plist 文件" << std::endl;
        return;
    }
    
    // 读取plist文件内容
    std::ifstream plistFile(plistPath);
    if (!plistFile.is_open()) {
        std::cerr << "错误: 无法打开文件: " << plistPath << std::endl;
        return;
    }
    
    std::stringstream buffer;
    buffer << plistFile.rdbuf();
    std::string plistContent = buffer.str();
    plistFile.close();
    
    // 解析plist文件
    SimplePlistParser parser(plistContent);
    std::vector<std::string> permissions;
    
    if (!parser.parsePermissions(permissions)) {
        std::cerr << "警告: 解析Permissions失败或未找到权限" << std::endl;
    }
    
    if (permissions.empty()) {
        std::cout << "信息: 没有找到任何权限" << std::endl;
    } else {
        std::cout << "找到 " << permissions.size() << " 个权限:" << std::endl;
        for (const auto& perm : permissions) {
            std::string iosPerm = convertSctToIosPermission(perm);
            std::cout << "  " << perm << " -> " << iosPerm << std::endl;
        }
    }
    
    // 生成entitlements文件
    std::string entitlementsPath = "entitlements.plist";
    
    if (!generateEntitlements(permissions, entitlementsPath)) {
        return;
    }
    
    std::cout << "已生成 entitlements.plist 文件" << std::endl;
    
    // 验证entitlements文件
    if (!verifyEntitlements(entitlementsPath)) {
        std::cerr << "警告: entitlements文件验证失败" << std::endl;
    }
    
    // 显示文件内容
    showEntitlementsContent(entitlementsPath);
    
    std::cout << "✅ 转换完成! entitlements.plist 文件已保存到当前目录" << std::endl;
}

int main() {
    while (true) {
        showMenu();
        
        std::string input;
        std::getline(std::cin, input);
        
        // 处理空输入（默认选择选项1）
        if (input.empty()) {
            input = "1";
        }
        
        if (input == "1") {
            optionSignBinary();
        } else if (input == "2") {
            optionConvertPlist();
        } else if (input == "0") {
            std::cout << "感谢使用 iOS-SCT 签名器，再见!" << std::endl;
            break;
        } else {
            std::cout << "无效选项，请重新选择" << std::endl;
        }
        
        // 暂停一下，让用户看到结果
        std::cout << "\n按回车键继续...";
        std::getline(std::cin, input);
    }
    
    return 0;
}