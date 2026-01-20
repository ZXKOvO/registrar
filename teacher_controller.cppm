// Module
// File: teacher_controller.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-18
// Author: xxh 2024051604015
// Description: 教师控制器 - 应用逻辑层
//
export module registrar:teacher_controller;
import std;
import :registrar_core;
using std::string;
using std::format;
using std::print;

// 教师控制器 - 应用逻辑层
export class TeacherController
{
public:
    // 构造函数
    TeacherController(Registrar& registrar);
    
    // ========== 课程信息查询 ==========
    // 显示教师课程表
    void displaySchedule(string teacherId);
    
    // 显示课程学生名单
    void displayCourseRoster(string teacherId, string courseId);
    
    // ========== 成绩管理 ==========
    // 录入成绩
    // 返回：true表示录入成功，false表示失败
    bool addGrade(string teacherId, string studentId, string courseId, 
                  double score, string comment);
    
    // 修改成绩
    // 返回：true表示修改成功，false表示失败
    bool updateGrade(string teacherId, string studentId, string courseId,
                     double newScore, string newComment);
    
    // 显示所有成绩
    void displayAllGrades(string teacherId);
    
    // 显示所有课程的学生名单（含详细信息）
    void displayAllCourseRosters(string teacherId);
    
    // ========== 业务验证 ==========
    // 检查教师是否教授某课程
    bool hasCourse(string teacherId, string courseId);
    
private:
    Registrar& _registrar;  // 持有Registrar的引用
};

// 构造函数实现
TeacherController::TeacherController(Registrar& registrar)
    : _registrar(registrar)
{}

// ========== 课程信息查询实现 ==========
void TeacherController::displaySchedule(string teacherId)
{
    auto teacher = _registrar.findTeacherById(teacherId);
    if (!teacher) {
        print("错误：教师ID不存在！\n");
        return;
    }
    
    print("教师信息:\n");
    print("{}\n", teacher->info());
    print("\n授课安排:\n");
    print("{}\n", teacher->schedule());
    
    if (teacher->schedule().empty() || teacher->schedule().find("(未安排课程)") != string::npos) {
        print("提示：该教师暂无授课安排\n");
    }
}

void TeacherController::displayCourseRoster(string teacherId, string courseId)
{
    // 1. 验证教师是否存在
    auto teacher = _registrar.findTeacherById(teacherId);
    if (!teacher) {
        print("错误：教师ID不存在！\n");
        return;
    }
    
    // 2. 验证该教师是否教授此课程
    if (!teacher->hasCourse(courseId)) {
        print("错误：该教师没有教授此课程！\n");
        return;
    }
    
    // 3. 获取课程学生名单
    auto course = _registrar.findCourseById(courseId);
    if (!course) {
        print("错误：课程ID不存在！\n");
        return;
    }
    
    // 手动构建详细的学生名单
    print("  学生ID\t姓名\n");
    print("  ----------------\n");
    
    // 遍历所有学生，找到选修该课程的学生
    _registrar.forEachStudent([&](auto& student) {
        if (student->hasCourse(courseId)) {
            print("  {}\t{}\n", student->getId(), student->getName());
        }
    });
}

// 录入成绩
bool TeacherController::addGrade(string teacherId, string studentId, string courseId, 
                                  double score, string comment)
{
    // 1. 验证教师是否存在
    auto teacher = _registrar.findTeacherById(teacherId);
    if (!teacher) {
        print("错误：教师ID {} 不存在！\n", teacherId);
        return false;
    }
    
    // 2. 验证该教师是否教授此课程
    if (!teacher->hasCourse(courseId)) {
        print("错误：您没有教授课程 {}！\n", courseId);
        return false;
    }
    
    // 3. 验证学生是否存在
    auto student = _registrar.findStudentById(studentId);
    if (!student) {
        print("错误：学生ID {} 不存在！\n", studentId);
        return false;
    }
    
    // 4. 验证课程是否存在
    auto course = _registrar.findCourseById(courseId);
    if (!course) {
        print("错误：课程ID {} 不存在！\n", courseId);
        return false;
    }
    
    // 5. 检查学生是否选了该课程
    if (!student->hasCourse(courseId)) {
        print("错误：该学生未选修此课程，无法录入成绩！\n");
        return false;
    }
    
    // 6. 检查是否已经录入过成绩
    auto existingGrade = teacher->findGrade(studentId, courseId);
    if (existingGrade) {
        print("错误：该学生此课程的成绩已存在，请使用修改成绩功能！\n");
        return false;
    }
    
    // 7. 验证成绩范围
    if (score < 0 || score > 100) {
        print("错误：成绩必须在0-100之间！\n");
        return false;
    }
    
    // 8. 执行录入成绩操作
    auto grade = teacher->gradeCourse(studentId, courseId, score, comment);
    
    // 保存成绩到数据库
    if (_registrar.getDataManager()) {
        _registrar.getDataManager()->saveGrade(grade);
        print("成绩已保存到数据库\n");
    }
    return true;
}

// 修改成绩
bool TeacherController::updateGrade(string teacherId, string studentId, string courseId,
                                    double newScore, string newComment)
{
    // 1. 验证教师是否存在
    auto teacher = _registrar.findTeacherById(teacherId);
    if (!teacher) {
        print("错误：教师ID {} 不存在！\n", teacherId);
        return false;
    }
    
    // 2. 验证该教师是否教授此课程
    if (!teacher->hasCourse(courseId)) {
        print("错误：您没有教授课程 {}！\n", courseId);
        return false;
    }
    
    // 3. 验证成绩范围
    if (newScore < 0 || newScore > 100) {
        print("错误：成绩必须在0-100之间！\n");
        return false;
    }
    
    // 4. 执行更新成绩操作
    auto grade = teacher->updateGrade(studentId, courseId, newScore, newComment);
    if (!grade) {
        print("错误：未找到该学生的成绩记录，请先录入成绩！\n");
        return false;
    }
    
    // 更新数据库中的成绩
    if (_registrar.getDataManager()) {
        _registrar.getDataManager()->saveGrade(grade);
        print("成绩已更新到数据库\n");
    }
    
    return true;
}

// 显示所有成绩
void TeacherController::displayAllGrades(string teacherId)
{
    auto teacher = _registrar.findTeacherById(teacherId);
    if (!teacher) {
        print("错误：教师ID不存在！\n");
        return;
    }
    
    teacher->displayGrades();
}

// 显示所有课程的学生名单（含详细信息）
void TeacherController::displayAllCourseRosters(string teacherId)
{
    auto teacher = _registrar.findTeacherById(teacherId);
    if (!teacher) {
        print("错误：教师ID不存在！\n");
        return;
    }
    
    auto result = std::format("{}'s student rosters (详细):\n", teacher->getId());
    
    // 获取教师的所有课程
    _registrar.forEachCourse([&](auto& course) {
        // 检查教师是否教授该课程
        if (teacher->hasCourse(course->identifier())) {
            result += std::format("{}:\n", course->info());
            
            // 手动构建学生名单，包含ID和姓名
            result += "  学生ID\t姓名\n";
            result += "  ----------------\n";
            
            // 遍历所有学生，找到选修该课程的学生
            _registrar.forEachStudent([&](auto& student) {
                if (student->hasCourse(course->identifier())) {
                    result += std::format("  {}\t{}\n", student->getId(), student->getName());
                }
            });
            
            result += "\n";
        }
    });
    
    print("{}\n", result);
}

// 检查教师是否教授某课程
bool TeacherController::hasCourse(string teacherId, string courseId)
{
    auto teacher = _registrar.findTeacherById(teacherId);
    if (!teacher) {
        return false;
    }
    
    return teacher->hasCourse(courseId);
}
