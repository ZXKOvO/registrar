// Module
// File: academic_secretary.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-16
// Description: 教学秘书模块 - 管理学生、教师、课程和生成报告
//
export module registrar:academic_secretary;
import registrar;
import std;
using std::string;
using std::vector;

// 教学秘书类 - 负责教务管理工作
export class AcademicSecretary
{
public:
    // 构造函数
    AcademicSecretary(string id, string name, string password);
    
    // 身份验证
    bool authenticate(string password);
    
    // 学生管理
    void addNewStudent(string id, string name);
    void removeStudent(string id);

private:
    string _name;
    string _id;
    string _password;
    Registrar& _registrar;
};

// ==================== 构造函数实现 ====================

AcademicSecretary::AcademicSecretary(string id, string name, string password)
    : _name(name)
    , _id(id)
    , _password(password)
    , _registrar(Registrar::system())
{}

// ==================== 身份验证实现 ====================

bool AcademicSecretary::authenticate(string password)
{
    return _password == password;
}

// ==================== 学生管理实现 ====================

void AcademicSecretary::addNewStudent(string id, string name)
{
    if (_registrar.findStudentById(id)) {
        std::println("学生ID {} 已存在，添加失败！", id);
        return;
    }
    _registrar.addStudent(id, name);
    std::println("成功添加学生: {} ({})", name, id);
}

void AcademicSecretary::removeStudent(string id)
{
    if (!_registrar.findStudentById(id)) {
        std::println("学生ID {} 不存在，删除失败！", id);
        return;
    }
    _registrar.removeStudent(id);
    std::println("成功删除学生ID: {}", id);
}