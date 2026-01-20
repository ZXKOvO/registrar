// Module
// File: academic_secretary.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-17   Id:2024051604015   xxh
// Description: 教学秘书模块 - 领域实体
//
export module registrar:academic_secretary;
import std;
using std::string;

// 教学秘书类 - 领域实体
export class AcademicSecretary
{
public:
    // 构造函数
    AcademicSecretary(string id, string name, string password);
    
    // 身份验证 - 领域实体的行为
    bool authenticate(string password);
    
    // 信息查询 - 领域实体的行为
    string info();
    bool hasId(string id);

private:
    string _id;        // 教学秘书ID
    string _name;      // 教学秘书姓名
    string _password;  // 密码
};

// ==================== 构造函数实现 ====================

AcademicSecretary::AcademicSecretary(string id, string name, string password)
    : _id(id)
    , _name(name)
    , _password(password)
{}

// ==================== 身份验证实现 ====================

bool AcademicSecretary::authenticate(string password)
{
    return _password == password;
}

// ==================== 信息查询实现 ====================

string AcademicSecretary::info()
{
    return format("{}   {}", _id, _name);
}

bool AcademicSecretary::hasId(string id)
{
    return _id == id;
}
