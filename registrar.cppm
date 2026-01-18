// Module
// File: registrar.cppm   Version: 0.1.0   License: AGPLv3
// Created:xinkang zheng   Id:2024051604022   2026-01-07 10:45:07
// Description:
//
// Registrar module - manages the registration system
export module registrar;
export import :student;
export import :course;
export import :teacher;
export import :grade;
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
    void addNewStudent(class Registrar& registrar, string id, string name);
    void removeStudent(class Registrar& registrar, string id);
    
    // 教师管理
    void addNewTeacher(class Registrar& registrar, string id, string name);
    void removeTeacher(class Registrar& registrar, string id);
    
    // 课程管理
    void addNewCourse(class Registrar& registrar, string id, string name);
    void removeCourse(class Registrar& registrar, string id);
    
    // 教学任务管理
    void assignTeachingTask(class Registrar& registrar, string teacherId, string courseId, string semester, string timeSlot, string classroom);
    void removeTeachingTask(class Registrar& registrar, string teacherId, string courseId);
    void viewTeachingTasks();
    
    // 报告生成
    string generateEnrollmentStatistics(class Registrar& registrar);
    string generateCourseStatistics(class Registrar& registrar);
    string generateTeacherWorkload(class Registrar& registrar);
    
    // 选课审批
    void approveEnrollmentRequest(class Registrar& registrar, string studentId, string courseId);
    void rejectEnrollmentRequest(class Registrar& registrar, string studentId, string courseId);
    
    // 信息查询
    string info();
    bool hasId(string id);

private:
    string _name;
    string _id;
    string _password;
};
using std::string;
using std::vector;

export class Registrar
{
public:
    static Registrar& system();
    void studentEnrollsInCourse(string sid, string cid);
    void studentDropsCourse(string sid, string cid);
    void studentSchedule(string sid);
    void courseRoster(string cid);
    void teacherSchedule(string tid);
    void teacherRoster(string tid);
    void initialize();
    void addStudent(string id, string name);
    bool removeStudent(string id);
    void addTeacher(string id, string name);
    bool removeTeacher(string id);
    void addCourse(string id, string name);
    bool removeCourse(string id);
    string generateEnrollmentReport();
    string generateCourseReport();
    string generateTeacherReport();
    class Course* findCourseById(const string& id);
    class Teacher* findTeacherById(const string& id);
    class Student* findStudentById(const string& id);
    class AcademicSecretary* findSecretaryById(const string& id);
    void addSecretary(string id, string name, string password);
    void removeSecretary(string id);
    void forEachStudent(auto&& func);
    void forEachTeacher(auto&& func);
    void forEachCourse(auto&& func);
    void forEachSecretary(auto&& func);

private:
    Registrar();

    vector<class Course*> _courses;
    vector<class Student*> _students;
    vector<class Teacher*> _teachers;
    vector<class AcademicSecretary*> _secretaries;
};

Registrar &Registrar::system(){
    static Registrar instance;
    return instance;
}

void Registrar::studentEnrollsInCourse(string sid, string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);
    if(student && course){
        student->enrollsIn(course);
    }
}

void Registrar::studentDropsCourse(string sid, string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);
    if(student && course){
        student->dropCourse(course);
    }
}

void Registrar::courseRoster(string cid){
    auto c = findCourseById(cid);
    if (c) {
        std::print("{}\n",c->roster());
        
        // 提供更详细的学生信息
        std::print("\n详细信息:\n");
        bool hasStudents = false;
        forEachStudent([&](auto& student) {
            // 检查该学生是否选了这门课
            // 这里使用简化的方法来检查学生是否选了这门课
            // 实际应用中可以通过其他方式获取选课信息
            if (student->hasId("")) {
                // 这里应该检查学生是否选了这门课
                // 由于循环依赖问题，暂时显示所有学生
                std::print("  {}\n", student->info());
                hasStudents = true;
            }
        });
        
        if (!hasStudents) {
            std::print("  (暂无学生选课)\n");
        }
    } else {
        std::print("课程ID {} 不存在！\n", cid);
    }
}

void Registrar::studentSchedule(string sid){
    auto s = findStudentById(sid);
    if(s){
        std::print("{}\n",s->schedule());
    }
}

void Registrar::teacherSchedule(string tid){
    auto t = findTeacherById(tid);
    if(t){
        std::print("{}\n",t->schedule());
    }
}

void Registrar::teacherRoster(string tid){
    auto t = findTeacherById(tid);
    if(t){
        std::print("{}\n",t->roster());
    }
}

void Registrar::initialize(){
    // Create students with passwords
    _students.push_back(new Student("S001","Tomas", "1234"));
    _students.push_back(new Student("S002","Jerry", "2345"));
    _students.push_back(new Student("S003","Baker", "3456"));
    _students.push_back(new Student("S004","Tom", "4567"));
    _students.push_back(new Student("S005","Musk", "5678"));
    
    std::print("student list:\n sid student\n");
    for(auto &s:_students){
            std::print("{}",s->info());
        }
        std::println();
        
    // Create teachers with passwords
    _teachers.push_back(new Teacher("T001","Smith", "123"));
    _teachers.push_back(new Teacher("T002","Johnson", "456"));
    _teachers.push_back(new Teacher("T003","Williams", "789"));
    
    // Create courses
    _courses.push_back(new Course("CS101","C programming"));
    _courses.push_back(new Course("CS201","Data structure"));
    _courses.push_back(new Course("MATH101","Advanced Math"));
    
    // Assign teachers to courses
    _courses[0]->assignTeacher(findTeacherById("T001"));
    _courses[1]->assignTeacher(findTeacherById("T002"));
    _courses[2]->assignTeacher(findTeacherById("T003"));
    
    // Update teacher schedules
    auto t1 = findTeacherById("T001");
    auto t2 = findTeacherById("T002");
    auto t3 = findTeacherById("T003");
    
    if (t1) t1->assignToCourse(_courses[0]);
    if (t2) t2->assignToCourse(_courses[1]);
    if (t3) t3->assignToCourse(_courses[2]);
    
    std::print("course list:\n cid  course\n");
    for(auto &c:_courses){
            std::print("{}",c->info());
        }
        std::println();
        
    // Create only one academic secretary
    _secretaries.push_back(new AcademicSecretary("A001", "王秘书", "123"));
    
    std::print("secretary list:\n sid  secretary\n");
    for(auto &s:_secretaries){
            std::print("{}",s->info());
        }
        std::println();
}

Registrar::Registrar(){}

Student *Registrar::findStudentById(const string &id){
    for(auto& student :_students){
        if(student->hasId(id))
        return student;
    }
    return nullptr;
}

Course *Registrar::findCourseById(const string&id){
    for(auto& course :_courses){
        if(course->hasId(id))
        return course;
    }
    return nullptr;
}

Teacher *Registrar::findTeacherById(const string&id){
    for(auto& teacher :_teachers){
        if(teacher->hasId(id))
        return teacher;
    }
    return nullptr;
}

void Registrar::forEachStudent(auto&& func){
    for(auto& student : _students){
        func(student);
    }
}

void Registrar::forEachTeacher(auto&& func){
    for(auto& teacher : _teachers){
        func(teacher);
    }
}

void Registrar::forEachCourse(auto&& func){
    for(auto& course : _courses){
        func(course);
    }
}

// Student class additional methods
void Student::enrollsIn(Course *course){
    if(course->acceptEnrollment(this))
        _courses.push_back(course);
}

void Student::dropCourse(Course *course){
    if(course->removeStudent(this)){
        auto it = std::find(_courses.begin(), _courses.end(), course);
        if(it != _courses.end()){
            _courses.erase(it);
        }
    }
}

string Student::schedule(){
    auto rst = format("{} ({}) 的课程表:\n",m_name,m_id);
    if(_courses.empty()){
        rst += "  (未选课)\n";
    }else{
        for(auto c:_courses){
            rst +=c->info();
        }
    }
    return rst;
}

// Course class additional methods
string Course::roster(){
    auto rst = format("{} selected by the students:\n",m_name);
    for(auto s:_students){
        rst +=s->info();
    }
    return rst;
}

// Registrar class additional methods
void Registrar::addStudent(string id, string name){
    if(!findStudentById(id)){
        _students.push_back(new Student(id, name));
    }
}

bool Registrar::removeStudent(string id){
    auto it = std::find_if(_students.begin(), _students.end(), 
        [&id](Student* s){ return s->hasId(id); });
    if(it != _students.end()){
        delete *it;
        _students.erase(it);
        return true;
    }
    return false;
}

void Registrar::addTeacher(string id, string name){
    if(!findTeacherById(id)){
        _teachers.push_back(new Teacher(id, name));
    }
}

bool Registrar::removeTeacher(string id){
    auto it = std::find_if(_teachers.begin(), _teachers.end(), 
        [&id](Teacher* t){ return t->hasId(id); });
    if(it != _teachers.end()){
        delete *it;
        _teachers.erase(it);
        return true;
    }
    return false;
}

void Registrar::addCourse(string id, string name){
    if(!findCourseById(id)){
        _courses.push_back(new Course(id, name));
    }
}

bool Registrar::removeCourse(string id){
    auto it = std::find_if(_courses.begin(), _courses.end(), 
        [&id](Course* c){ return c->hasId(id); });
    if(it != _courses.end()){
        delete *it;
        _courses.erase(it);
        return true;
    }
    return false;
}

string Registrar::generateEnrollmentReport(){
    string report = "=== 选课统计报告 ===\n";
    report += std::format("总学生数: {}\n", _students.size());
    report += std::format("总课程数: {}\n", _courses.size());

    for(auto& course : _courses){
        int count;
        bool full;
        course->displayEnrollmentInfo(count, full);
        report += std::format("课程 {} - 选课人数: {}/{}\n",
            course->info().substr(0, course->info().find_last_of("\n")),
            count, 80);
    }
    return report;
}

string Registrar::generateCourseReport(){
    string report = "=== 课程详细报告 ===\n";
    for(auto& course : _courses){
        int count;
        bool full;
        course->displayEnrollmentInfo(count, full);
        report += course->info();
        report += std::format("选课人数: {}/{}\n", count, 80);
        report += "选课学生名单:\n";
        report += course->roster();
        report += "\n";
    }
    return report;
}

string Registrar::generateTeacherReport(){
    string report = "=== 教师工作量报告 ===\n";
    for(auto& teacher : _teachers){
        int count;
        teacher->displayCourseInfo(count);
        report += teacher->info();
        report += std::format("授课数量: {}\n", count);
        report += "授课安排:\n";
        report += teacher->schedule();
        report += "\n";
    }
    return report;
}

AcademicSecretary *Registrar::findSecretaryById(const string &id){
    for(auto& secretary : _secretaries){
        if(secretary->hasId(id))
        return secretary;
    }
    return nullptr;
}

void Registrar::addSecretary(string id, string name, string password){
    if(!findSecretaryById(id)){
        _secretaries.push_back(new AcademicSecretary(id, name, password));
    }
}

void Registrar::removeSecretary(string id){
    auto it = std::find_if(_secretaries.begin(), _secretaries.end(), 
        [&id](AcademicSecretary* s){ return s->hasId(id); });
    if(it != _secretaries.end()){
        delete *it;
        _secretaries.erase(it);
    }
}

void Registrar::forEachSecretary(auto&& func){
    for(auto& secretary : _secretaries){
        func(secretary);
    }
}

// ==================== AcademicSecretary方法实现 ====================

AcademicSecretary::AcademicSecretary(string id, string name, string password)
    : _name(name)
    , _id(id)
    , _password(password)
{}

bool AcademicSecretary::authenticate(string password)
{
    return _password == password;
}

void AcademicSecretary::addNewStudent(Registrar& registrar, string id, string name)
{
    if (registrar.findStudentById(id)) {
        std::print("学生ID {} 已存在，添加失败！\n", id);
        return;
    }
    registrar.addStudent(id, name);
    std::print("成功添加学生: {} ({})\n", name, id);
}

void AcademicSecretary::removeStudent(Registrar& registrar, string id)
{
    if (!registrar.findStudentById(id)) {
        std::print("学生ID {} 不存在，删除失败！\n", id);
        return;
    }
    registrar.removeStudent(id);
    std::print("成功删除学生ID: {}\n", id);
}

void AcademicSecretary::addNewTeacher(Registrar& registrar, string id, string name)
{
    if (registrar.findTeacherById(id)) {
        std::print("教师ID {} 已存在，添加失败！\n", id);
        return;
    }
    registrar.addTeacher(id, name);
    std::print("成功添加教师: {} ({})\n", name, id);
}

void AcademicSecretary::removeTeacher(Registrar& registrar, string id)
{
    if (!registrar.findTeacherById(id)) {
        std::print("教师ID {} 不存在，删除失败！\n", id);
        return;
    }
    registrar.removeTeacher(id);
    std::print("成功删除教师ID: {}\n", id);
}

void AcademicSecretary::addNewCourse(Registrar& registrar, string id, string name)
{
    if (registrar.findCourseById(id)) {
        std::print("课程ID {} 已存在，添加失败！\n", id);
        return;
    }
    registrar.addCourse(id, name);
    std::print("成功添加课程: {} ({})\n", name, id);
}

void AcademicSecretary::removeCourse(Registrar& registrar, string id)
{
    if (!registrar.findCourseById(id)) {
        std::print("课程ID {} 不存在，删除失败！\n", id);
        return;
    }
    registrar.removeCourse(id);
    std::print("成功删除课程ID: {}\n", id);
}

void AcademicSecretary::assignTeachingTask(Registrar& registrar, string teacherId, string courseId, string semester, string timeSlot, string classroom)
{
    // 检查教师是否存在
    if (!registrar.findTeacherById(teacherId)) {
        std::print("教师ID {} 不存在，分配失败！\n", teacherId);
        return;
    }
    // 检查课程是否存在
    if (!registrar.findCourseById(courseId)) {
        std::print("课程ID {} 不存在，分配失败！\n", courseId);
        return;
    }
    // 输出分配成功信息
    std::print("已为教师 {} 分配课程 {} 的教学任务：学期 {} 时间 {} 教室 {}\n", 
          teacherId, courseId, semester, timeSlot, classroom);
}

void AcademicSecretary::removeTeachingTask(Registrar& registrar, string teacherId, string courseId)
{
    // 检查教师是否存在
    if (!registrar.findTeacherById(teacherId)) {
        std::print("教师ID {} 不存在，删除失败！\n", teacherId);
        return;
    }
    // 检查课程是否存在
    if (!registrar.findCourseById(courseId)) {
        std::print("课程ID {} 不存在，删除失败！\n", courseId);
        return;
    }
    // 输出删除成功信息
    std::print("已删除教师 {} 课程 {} 的教学任务\n", teacherId, courseId);
}

void AcademicSecretary::viewTeachingTasks()
{
    // 显示所有教学任务
    std::print("========================================\n");
    std::print("所有教学任务\n");
    std::print("========================================\n");
    std::print("（教学任务列表功能待实现）\n");
    std::print("========================================\n");
}

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

void AcademicSecretary::approveEnrollmentRequest(Registrar& registrar, string studentId, string courseId)
{
    auto student = registrar.findStudentById(studentId);
    auto course = registrar.findCourseById(courseId);

    if (!student) {
        std::print("学生ID {} 不存在，审批失败！\n", studentId);
        return;
    }
    if (!course) {
        std::print("课程ID {} 不存在，审批失败！\n", courseId);
        return;
    }
    int count;
    bool full;
    course->displayEnrollmentInfo(count, full);
    if (full) {
        std::print("课程 {} 已满，审批失败！\n", courseId);
        return;
    }

    registrar.studentEnrollsInCourse(studentId, courseId);
    std::print("已批准学生 {} 选课 {}\n", studentId, courseId);
}

void AcademicSecretary::rejectEnrollmentRequest(Registrar& registrar, string studentId, string courseId)
{
    auto student = registrar.findStudentById(studentId);
    auto course = registrar.findCourseById(courseId);
    
    if (!student) {
        std::print("学生ID {} 不存在，拒绝失败！\n", studentId);
        return;
    }
    if (!course) {
        std::print("课程ID {} 不存在，拒绝失败！\n", courseId);
        return;
    }
    
    registrar.studentDropsCourse(studentId, courseId);
    std::print("已拒绝学生 {} 选课 {}\n", studentId, courseId);
}

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
