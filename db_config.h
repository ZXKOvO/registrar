#pragma once

// 数据库配置头文件
namespace DBConfig {
    // 数据库连接配置
    inline const char* getConnectionString() {
        return "dbname=registrar user=postgres password=123 hostaddr=127.0.0.1 port=5432";
    }
    
    // 数据库名称
    inline const char* getDatabaseName() {
        return "registrar";
    }
    
    // 数据库用户名
    inline const char* getUsername() {
        return "postgres";
    }
    
    // 数据库密码
    inline const char* getPassword() {
        return "123";
    }
    
    // 数据库主机
    inline const char* getHost() {
        return "127.0.0.1";
    }
    
    // 数据库端口
    inline int getPort() {
        return 5432;
    }
}