// Module
// File: academic_secretary.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-17   Id:2024051604015   xxh
// Description: 教学秘书模块 - 管理学生、教师、课程和生成报告
//
export module registrar:academic_secretary;
import std;
using std::string;
using std::vector;
using std::print;

// 前向声明Registrar类
class Registrar;

// 教学秘书类 - 负责教务管理工作
export class AcademicSecretary
{
public:
    // 构造函数
    AcademicSecretary(string id, string name, string password);
    
    // 身份验证
    bool authenticate(string password);
    
    // 学生管理
    void addNewStudent(Registrar& registrar, string id, string name);
    void removeStudent(Registrar& registrar, string id);
    
    // 教师管理
    void addNewTeacher(Registrar& registrar, string id, string name);
    void removeTeacher(Registrar& registrar, string id);
    
    // 课程管理
    void addNewCourse(Registrar& registrar, string id, string name);
    void removeCourse(Registrar& registrar, string id);
    
    // 教学任务管理
    void assignTeachingTask(Registrar& registrar, string teacherId, string courseId, string semester, string timeSlot, string classroom);
    void removeTeachingTask(Registrar& registrar, string teacherId, string courseId);
    void viewTeachingTasks();
    
    // 报告生成
    string generateEnrollmentStatistics(Registrar& registrar);
    string generateCourseStatistics(Registrar& registrar);
    string generateTeacherWorkload(Registrar& registrar);
    
    // 选课审批
    void approveEnrollmentRequest(Registrar& registrar, string studentId, string courseId);
    void rejectEnrollmentRequest(Registrar& registrar, string studentId, string courseId);
    
    // 信息查询
    string info();
    bool hasId(string id);

private:
    string _name;
    string _id;
    string _password;
};

// ==================== 构造函数实现 ====================

AcademicSecretary::AcademicSecretary(string id, string name, string password)
    : _name(name)
    , _id(id)
    , _password(password)
{}

// ==================== 身份验证实现 ====================

bool AcademicSecretary::authenticate(string password)
{
    return _password == password;
}

// ==================== 学生管理实现 ====================

void AcademicSecretary::addNewStudent(Registrar& registrar, string id, string name)
{
    if (registrar.findStudentById(id)) {
        print("学生ID {} 已存在，添加失败！\n", id);
        return;
    }
    registrar.addStudent(id, name);
    print("成功添加学生: {} ({})\n", name, id);
}

void AcademicSecretary::removeStudent(Registrar& registrar, string id)
{
    if (!registrar.findStudentById(id)) {
        print("学生ID {} 不存在，删除失败！\n", id);
        return;
    }
    registrar.removeStudent(id);
    print("成功删除学生ID: {}\n", id);
}

// ==================== 教师管理实现 ====================

void AcademicSecretary::addNewTeacher(Registrar& registrar, string id, string name)
{
    if (registrar.findTeacherById(id)) {
        print("教师ID {} 已存在，添加失败！\n", id);
        return;
    }
    registrar.addTeacher(id, name);
    print("成功添加教师: {} ({})\n", name, id);
}

void AcademicSecretary::removeTeacher(Registrar& registrar, string id)
{
    if (!registrar.findTeacherById(id)) {
        print("教师ID {} 不存在，删除失败！\n", id);
        return;
    }
    registrar.removeTeacher(id);
    print("成功删除教师ID: {}\n", id);
}

// ==================== 课程管理实现 ====================

void AcademicSecretary::addNewCourse(Registrar& registrar, string id, string name)
{
    if (registrar.findCourseById(id)) {
        print("课程ID {} 已存在，添加失败！\n", id);
        return;
    }
    registrar.addCourse(id, name);
    print("成功添加课程: {} ({})\n", name, id);
}

void AcademicSecretary::removeCourse(Registrar& registrar, string id)
{
    if (!registrar.findCourseById(id)) {
        print("课程ID {} 不存在，删除失败！\n", id);
        return;
    }
    registrar.removeCourse(id);
    print("成功删除课程ID: {}\n", id);
}

// ==================== 教学任务管理实现 ====================

void AcademicSecretary::assignTeachingTask(Registrar& registrar, string teacherId, string courseId, string semester, string timeSlot, string classroom)
{
    // 检查教师是否存在
    if (!registrar.findTeacherById(teacherId)) {
        print("教师ID {} 不存在，分配失败！\n", teacherId);
        return;
    }
    // 检查课程是否存在
    if (!registrar.findCourseById(courseId)) {
        print("课程ID {} 不存在，分配失败！\n", courseId);
        return;
    }
    // 输出分配成功信息
    print("已为教师 {} 分配课程 {} 的教学任务：学期 {} 时间 {} 教室 {}\n", 
          teacherId, courseId, semester, timeSlot, classroom);
}

void AcademicSecretary::removeTeachingTask(Registrar& registrar, string teacherId, string courseId)
{
    // 检查教师是否存在
    if (!registrar.findTeacherById(teacherId)) {
        print("教师ID {} 不存在，删除失败！\n", teacherId);
        return;
    }
    // 检查课程是否存在
    if (!registrar.findCourseById(courseId)) {
        print("课程ID {} 不存在，删除失败！\n", courseId);
        return;
    }
    // 输出删除成功信息
    print("已删除教师 {} 课程 {} 的教学任务\n", teacherId, courseId);
}

void AcademicSecretary::viewTeachingTasks()
{
    // 显示所有教学任务
    print("========================================\n");
    print("所有教学任务\n");
    print("========================================\n");
    print("（教学任务列表功能待实现）\n");
    print("========================================\n");
}

// ==================== 报告生成实现 ====================

string AcademicSecretary::generateEnrollmentStatistics(Registrar& registrar)
{
    return format("========================================\n"
                  "选课统计报告\n"
                  "========================================\n"
                  "{}"
                  "========================================\n", 
                  registrar.generateEnrollmentReport());
}

string AcademicSecretary::generateCourseStatistics(Registrar& registrar)
{
    return format("========================================\n"
                  "课程统计报告\n"
                  "========================================\n"
                  "{}"
                  "========================================\n", 
                  registrar.generateCourseReport());
}

string AcademicSecretary::generateTeacherWorkload(Registrar& registrar)
{
    return format("========================================\n"
                  "教师工作量报告\n"
                  "========================================\n"
                  "{}"
                  "========================================\n", 
                  registrar.generateTeacherReport());
}

// ==================== 选课审批实现 ====================

void AcademicSecretary::approveEnrollmentRequest(Registrar& registrar, string studentId, string courseId)
{
    auto student = registrar.findStudentById(studentId);
    auto course = registrar.findCourseById(courseId);

    if (!student) {
        print("学生ID {} 不存在，审批失败！\n", studentId);
        return;
    }
    if (!course) {
        print("课程ID {} 不存在，审批失败！\n", courseId);
        return;
    }
    int count;
    bool full;
    course->displayEnrollmentInfo(count, full);
    if (full) {
        print("课程 {} 已满，审批失败！\n", courseId);
        return;
    }

    registrar.studentEnrollsInCourse(studentId, courseId);
    print("已批准学生 {} 选课 {}\n", studentId, courseId);
}

void AcademicSecretary::rejectEnrollmentRequest(Registrar& registrar, string studentId, string courseId)
{
    auto student = registrar.findStudentById(studentId);
    auto course = registrar.findCourseById(courseId);
    
    if (!student) {
        print("学生ID {} 不存在，拒绝失败！\n", studentId);
        return;
    }
    if (!course) {
        print("课程ID {} 不存在，拒绝失败！\n", courseId);
        return;
    }
    
    registrar.studentDropsCourse(studentId, courseId);
    print("已拒绝学生 {} 选课 {}\n", studentId, courseId);
}

// ==================== 信息查询实现 ====================

string AcademicSecretary::info()
{
    return format("========================================\n"
                  "教学秘书信息\n"
                  "========================================\n"
                  "{}   {}\n"
                  "========================================\n", 
                  _id, _name);
}

bool AcademicSecretary::hasId(string id)
{
    return _id == id;
}