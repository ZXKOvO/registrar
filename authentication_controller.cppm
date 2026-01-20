// Module
// File: authentication_controller.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-18
// Author: xxh 2024051604015
// Description: 认证控制器 - 应用逻辑层
//
export module registrar:authentication_controller;
import std;
import :registrar_core;
import :academic_secretary;
using std::string;

// 用户类型枚举
export enum class UserTypes
{
    STUDENT,
    TEACHER,
    SECRETARY
};

// 认证控制器 - 应用逻辑层
export class AuthenticationController
{
public:
    // 构造函数
    AuthenticationController(Registrar& registrar);
    
    // 用户登录
    // 返回：true表示登录成功，false表示失败
    bool login(string id, string password, UserTypes type);
    
    // 用户登出
    void logout();
    
    // 获取当前登录用户ID
    string getCurrentUserId() const;
    
    // 获取当前登录用户类型
    UserTypes getCurrentUserType() const;
    
    // 检查是否已登录
    bool isLoggedIn() const;
    
private:
    Registrar& _registrar;      // 持有Registrar的引用
    string _currentUserId;      // 当前登录用户ID
    UserTypes _currentUserType; // 当前登录用户类型
    bool _isLoggedIn;           // 登录状态
};

// 构造函数
AuthenticationController::AuthenticationController(Registrar& registrar)
    : _registrar(registrar)
    , _currentUserId("")
    , _currentUserType(UserTypes::STUDENT)
    , _isLoggedIn(false)
{}

// 用户登录实现
bool AuthenticationController::login(string id, string password, UserTypes type)
{
    switch (type)
    {
        case UserTypes::STUDENT:
        {
            auto student = _registrar.findStudentById(id);
            if (student && student->authenticate(password)) {
                _currentUserId = id;
                _currentUserType = type;
                _isLoggedIn = true;
                return true;
            }
            return false;
        }
        case UserTypes::TEACHER:
        {
            auto teacher = _registrar.findTeacherById(id);
            if (teacher && teacher->authenticate(password)) {
                _currentUserId = id;
                _currentUserType = type;
                _isLoggedIn = true;
                return true;
            }
            return false;
        }
        case UserTypes::SECRETARY:
        {
            // 硬编码教学秘书A001王秘书
            if (id == "A001") {
                AcademicSecretary tempSecretary("A001", "王秘书", "123");
                if (tempSecretary.authenticate(password)) {
                    _currentUserId = id;
                    _currentUserType = type;
                    _isLoggedIn = true;
                    return true;
                }
            }
            return false;
        }
        default:
            return false;
    }
}

// 用户登出实现
void AuthenticationController::logout()
{
    _currentUserId = "";
    _currentUserType = UserTypes::STUDENT;
    _isLoggedIn = false;
}

// 获取当前登录用户ID
string AuthenticationController::getCurrentUserId() const
{
    return _currentUserId;
}

// 获取当前登录用户类型
UserTypes AuthenticationController::getCurrentUserType() const
{
    return _currentUserType;
}

// 检查是否已登录
bool AuthenticationController::isLoggedIn() const
{
    return _isLoggedIn;
}
