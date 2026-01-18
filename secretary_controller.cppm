// Module
// File: secretary_controller.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-18
// Author: xxh 2024051604015
// Description: 教学秘书控制器 - 应用逻辑层
//
export module registrar:secretary_controller;
import std;
import :registrar_core;
using std::string;
using std::format;
using std::print;

// 教学秘书控制器 - 应用逻辑层
export class SecretaryController
{
public:
    // 构造函数
    SecretaryController(Registrar& registrar, string secretaryId);
    
    // ========== 学生管理 ==========
    bool addStudent(string id, string name);
    bool removeStudent(string id);
    void displayStudentList();
    
    // ========== 教师管理 ==========
    bool addTeacher(string id, string name);
    bool removeTeacher(string id);
    void displayTeacherList();
    
    // ========== 课程管理 ==========
    bool addCourse(string id, string name);
    bool removeCourse(string id);
    void displayCourseList();
    
    // ========== 教学任务管理 ==========
    bool assignTeachingTask(string teacherId, string courseId, 
                            string semester, string timeSlot, string classroom);
    bool removeTeachingTask(string teacherId, string courseId);
    void displayTeachingTasks();
    
    // ========== 报表生成 ==========
    void generateEnrollmentReport();
    void generateCourseReport();
    void generateTeacherWorkloadReport();
    
    // ========== 选课审批 ==========
    bool approveEnrollment(string studentId, string courseId);
    bool rejectEnrollment(string studentId, string courseId);
    
private:
    Registrar& _registrar;      // 持有Registrar的引用
    string _secretaryId;        // 当前操作的教学秘书ID
};

// 构造函数
SecretaryController::SecretaryController(Registrar& registrar, string secretaryId)
    : _registrar(registrar)
    , _secretaryId(secretaryId)
{}

// ========== 学生管理实现 ==========

bool SecretaryController::addStudent(string id, string name)
{
    // 业务逻辑验证
    if (_registrar.findStudentById(id)) {
        print("错误：学生ID {} 已存在！\n", id);
        return false;
    }
    
    // 调用领域层
    _registrar.addStudent(id, name);
    print("成功添加学生: {} ({})\n", name, id);
    return true;
}

bool SecretaryController::removeStudent(string id)
{
    // 业务逻辑验证
    if (!_registrar.findStudentById(id)) {
        print("错误：学生ID {} 不存在！\n", id);
        return false;
    }
    
    // 调用领域层
    _registrar.removeStudent(id);
    print("成功删除学生ID: {}\n", id);
    return true;
}

void SecretaryController::displayStudentList()
{
    print("=== 学生列表 ===\n");
    print("学号\t姓名\t已选课程数\n");
    print("----------------------------\n");
    
    _registrar.forEachStudent([&](auto& student) {
        print("{}\n", student->info());
    });
}

// ========== 教师管理实现 ==========

bool SecretaryController::addTeacher(string id, string name)
{
    if (_registrar.findTeacherById(id)) {
        print("错误：教师ID {} 已存在！\n", id);
        return false;
    }
    
    _registrar.addTeacher(id, name);
    print("成功添加教师: {} ({})\n", name, id);
    return true;
}

bool SecretaryController::removeTeacher(string id)
{
    if (!_registrar.findTeacherById(id)) {
        print("错误：教师ID {} 不存在！\n", id);
        return false;
    }
    
    _registrar.removeTeacher(id);
    print("成功删除教师ID: {}\n", id);
    return true;
}

void SecretaryController::displayTeacherList()
{
    print("=== 教师列表 ===\n");
    print("教师ID\t姓名\t授课数\n");
    print("------------------------\n");
    
    _registrar.forEachTeacher([&](auto& teacher) {
        auto info = teacher->info();
        int count;
        teacher->displayCourseInfo(count);
        print("{}\t{}\n",
            info.substr(0, info.find('\n')),
            count);
    });
}

// ========== 课程管理实现 ==========

bool SecretaryController::addCourse(string id, string name)
{
    if (_registrar.findCourseById(id)) {
        print("错误：课程ID {} 已存在！\n", id);
        return false;
    }
    
    _registrar.addCourse(id, name);
    print("成功添加课程: {} ({})\n", name, id);
    return true;
}

bool SecretaryController::removeCourse(string id)
{
    if (!_registrar.findCourseById(id)) {
        print("错误：课程ID {} 不存在！\n", id);
        return false;
    }
    
    _registrar.removeCourse(id);
    print("成功删除课程ID: {}\n", id);
    return true;
}

void SecretaryController::displayCourseList()
{
    print("=== 课程列表 ===\n");
    print("课程ID\t课程名称\t选课人数\t状态\n");
    print("------------------------------------------------\n");
    
    _registrar.forEachCourse([&](auto& course) {
        auto info = course->info();
        int count;
        bool full;
        course->displayEnrollmentInfo(count, full);
        print("{}\t{}/80\t{}\n",
            info.substr(0, info.find('\n')),
            count,
            full ? "已满" : "可选");
    });
}

// ========== 教学任务管理实现 ==========

bool SecretaryController::assignTeachingTask(string teacherId, string courseId, 
                                              string semester, string timeSlot, string classroom)
{
    // 验证教师是否存在
    if (!_registrar.findTeacherById(teacherId)) {
        print("错误：教师ID {} 不存在！\n", teacherId);
        return false;
    }
    
    // 验证课程是否存在
    if (!_registrar.findCourseById(courseId)) {
        print("错误：课程ID {} 不存在！\n", courseId);
        return false;
    }
    
    // TODO: 实际保存教学任务到数据库
    // 这里可以创建TeachingTask对象并保存
    print("已为教师 {} 分配课程 {} 的教学任务：学期 {} 时间 {} 教室 {}\n", 
              teacherId, courseId, semester, timeSlot, classroom);
    
    return true;
}

bool SecretaryController::removeTeachingTask(string teacherId, string courseId)
{
    // 验证教师是否存在
    if (!_registrar.findTeacherById(teacherId)) {
        print("错误：教师ID {} 不存在！\n", teacherId);
        return false;
    }
    
    // 验证课程是否存在
    if (!_registrar.findCourseById(courseId)) {
        print("错误：课程ID {} 不存在！\n", courseId);
        return false;
    }
    
    // TODO: 实际删除教学任务
    print("已删除教师 {} 课程 {} 的教学任务\n", teacherId, courseId);
    
    return true;
}

void SecretaryController::displayTeachingTasks()
{
    print("========================================\n");
    print("所有教学任务\n");
    print("========================================\n");
    print("（教学任务列表功能待实现）\n");
    print("========================================\n");
}

// ========== 报表生成实现 ==========

void SecretaryController::generateEnrollmentReport()
{
    print("{}\n", _registrar.generateEnrollmentReport());
}

void SecretaryController::generateCourseReport()
{
    print("{}\n", _registrar.generateCourseReport());
}

void SecretaryController::generateTeacherWorkloadReport()
{
    print("{}\n", _registrar.generateTeacherReport());
}

// ========== 选课审批实现 ==========

bool SecretaryController::approveEnrollment(string studentId, string courseId)
{
    auto student = _registrar.findStudentById(studentId);
    auto course = _registrar.findCourseById(courseId);
    
    if (!student) {
        print("错误：学生ID {} 不存在！\n", studentId);
        return false;
    }
    
    if (!course) {
        print("错误：课程ID {} 不存在！\n", courseId);
        return false;
    }
    
    // 检查课程是否已满
    int count;
    bool full;
    course->displayEnrollmentInfo(count, full);
    if (full) {
        print("错误：课程 {} 已满！\n", courseId);
        return false;
    }
    
    _registrar.studentEnrollsInCourse(studentId, courseId);
    print("已批准学生 {} 选课 {}\n", studentId, courseId);
    return true;
}

bool SecretaryController::rejectEnrollment(string studentId, string courseId)
{
    auto student = _registrar.findStudentById(studentId);
    auto course = _registrar.findCourseById(courseId);
    
    if (!student) {
        print("错误：学生ID {} 不存在！\n", studentId);
        return false;
    }
    
    if (!course) {
        print("错误：课程ID {} 不存在！\n", courseId);
        return false;
    }
    
    _registrar.studentDropsCourse(studentId, courseId);
    print("已拒绝学生 {} 选课 {}\n", studentId, courseId);
    return true;
}