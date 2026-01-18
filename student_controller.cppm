// Module
// File: student_controller.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-18
// Author: xxh 2024051604015
// Description: 学生控制器 - 应用逻辑层
//
export module registrar:student_controller;
import std;
import :registrar_core;
using std::string;
using std::format;
using std::print;

// 学生控制器 - 应用逻辑层
export class StudentController
{
public:
    // 构造函数
    StudentController(Registrar& registrar);
    
    // ========== 选课管理 ==========
    // 学生选课业务流程
    // 返回：true表示选课成功，false表示失败
    bool enrollCourse(string studentId, string courseId);
    
    // 学生退课业务流程
    // 返回：true表示退课成功，false表示失败
    bool dropCourse(string studentId, string courseId);
    
    // ========== 信息查询 ==========
    // 显示学生课程表
    void displaySchedule(string studentId);
    
    // 显示可选课程列表
    void displayAvailableCourses();
    
    // 显示学生成绩
    void displayGrades(string studentId);
    
    // ========== 业务验证 ==========
    // 检查学生是否可以选修某课程
    bool canEnrollCourse(string studentId, string courseId, string& errorMessage);
    
private:
    Registrar& _registrar;  // 持有Registrar的引用
};

// 构造函数实现
StudentController::StudentController(Registrar& registrar)
    : _registrar(registrar)
{}

// 选课业务流程实现
bool StudentController::enrollCourse(string studentId, string courseId)
{
    // 1. 验证学生是否存在
    auto student = _registrar.findStudentById(studentId);
    if (!student) {
        print("错误：学生ID {} 不存在！\n", studentId);
        return false;
    }
    
    // 2. 验证课程是否存在
    auto course = _registrar.findCourseById(courseId);
    if (!course) {
        print("错误：课程ID {} 不存在！\n", courseId);
        return false;
    }
    
    // 3. 检查课程是否已满
    int count;
    bool full;
    course->displayEnrollmentInfo(count, full);
    if (full) {
        print("错误：课程 {} 已满，无法选课！\n", courseId);
        return false;
    }
    
    // 4. 检查学生是否已选过该课程
    if (student->hasCourse(courseId)) {
        print("错误：您已选修过课程 {}！\n", courseId);
        return false;
    }
    
    // 5. 执行选课操作
    _registrar.studentEnrollsInCourse(studentId, courseId);
    return true;
}

// 退课业务流程实现
bool StudentController::dropCourse(string studentId, string courseId)
{
    // 1. 验证学生是否存在
    auto student = _registrar.findStudentById(studentId);
    if (!student) {
        print("错误：学生ID {} 不存在！\n", studentId);
        return false;
    }
    
    // 2. 验证课程是否存在
    auto course = _registrar.findCourseById(courseId);
    if (!course) {
        print("错误：课程ID {} 不存在！\n", courseId);
        return false;
    }
    
    // 3. 检查学生是否选了该课程
    if (!student->hasCourse(courseId)) {
        print("错误：您未选修课程 {}，无法退课！\n", courseId);
        return false;
    }
    
    // 4. 执行退课操作
    _registrar.studentDropsCourse(studentId, courseId);
    return true;
}

// ========== 信息查询实现 ==========
void StudentController::displaySchedule(string studentId)
{
    auto student = _registrar.findStudentById(studentId);
    if (!student) {
        print("错误：学生ID {} 不存在！\n", studentId);
        return;
    }
    
    print("学生: {}\n", student->info());
    print("课程表:\n");
    print("{}\n", student->schedule());
}

void StudentController::displayAvailableCourses()
{
    print("=== 可选课程列表 ===\n");
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

void StudentController::displayGrades(string studentId)
{
    auto student = _registrar.findStudentById(studentId);
    if (!student) {
        print("错误：学生ID {} 不存在！\n", studentId);
        return;
    }

    print("学生: {}\n", student->info());
    print("\n=== 成绩列表 ===\n");
    print("课程ID\t课程名称\t成绩\t评语\n");
    print("----------------------------------------\n");

    // 遍历所有教师，查找该学生的成绩
    bool found = false;
    _registrar.forEachTeacher([&](auto& teacher) {
        auto grade = teacher->findGradesByStudent(studentId);
        if (grade) {
            print("{}", grade->info());
            found = true;
        }
    });

    if (!found)
    {
        print("暂无成绩记录\n");
    }
}

// 检查学生是否可以选修某课程
bool StudentController::canEnrollCourse(string studentId, string courseId, string& errorMessage)
{
    // 1. 验证学生是否存在
    auto student = _registrar.findStudentById(studentId);
    if (!student) {
        errorMessage = "学生ID不存在";
        return false;
    }
    
    // 2. 验证课程是否存在
    auto course = _registrar.findCourseById(courseId);
    if (!course) {
        errorMessage = "课程ID不存在";
        return false;
    }
    
    // 3. 检查课程是否已满
    int count;
    bool full;
    course->displayEnrollmentInfo(count, full);
    if (full) {
        errorMessage = "课程已满";
        return false;
    }
    
    // 4. 检查学生是否已选过该课程
    if (student->hasCourse(courseId)) {
        errorMessage = "已选修过该课程";
        return false;
    }
    
    errorMessage = "";
    return true;
}