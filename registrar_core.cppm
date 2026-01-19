// Module
// File: registrar_core.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-17
// Description: Registrar核心模块 - 定义Registrar类
//
module;
#include <string>
#include <vector>
#include <format>
#include <algorithm>
#include <print>

export module registrar:registrar_core;
export import :student;
export import :course;
export import :teacher;
export import :grade;
export import :teaching_task;
export import :data_manager;

using std::string;
using std::vector;
using std::format;
using std::print;
using std::println;

export class Registrar
{
public:
    static Registrar& system();
    void studentEnrollsInCourse(std::string sid, std::string cid);
    void studentDropsCourse(std::string sid, std::string cid);
    void studentSchedule(std::string sid);
    void courseRoster(std::string cid);
    void teacherSchedule(std::string tid);
    void teacherRoster(std::string tid);
    void initialize();
    void initializeDefaultData();
    void saveAllData();
    void addStudent(std::string id, std::string name);
    void removeStudent(std::string id);
    void addTeacher(std::string id, std::string name);
    void removeTeacher(std::string id);
    void addCourse(std::string id, std::string name);
    void removeCourse(std::string id);
    std::string generateEnrollmentReport();
    std::string generateCourseReport();
    std::string generateTeacherReport();
    Course* findCourseById(const std::string& id);
    Teacher* findTeacherById(const std::string& id);
    Student* findStudentById(const std::string& id);
    void forEachStudent(auto&& func);
    void forEachTeacher(auto&& func);
    void forEachCourse(auto&& func);
    
    // 为教师设置课程信息（用于从数据库加载）
    void setTeacherCourses();
    
    // 为学生设置课程信息（用于从数据库加载）
    void setStudentCourses();
    
    // 教学任务管理
    void addTeachingTask(TeachingTask* task);
    bool removeTeachingTask(const std::string& teacherId, const std::string& courseId);
    std::vector<TeachingTask*> getTeachingTasks();
    
    // 数据管理
    void setDataManager(DataManager* dataManager);
    void loadAllData();

private:
    Registrar();
    DataManager* _dataManager;

    std::vector<Course*> _courses;
    std::vector<Student*> _students;
    std::vector<Teacher*> _teachers;
    std::vector<TeachingTask*> _teachingTasks;
};

Registrar &Registrar::system(){
    static Registrar instance;
    return instance;
}

void Registrar::studentEnrollsInCourse(std::string sid, std::string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);
    if(student && course){
        student->enrollsIn(course);
        
        // 保存选课数据到数据库
        if (_dataManager) {
            _dataManager->saveEnrollment(sid, cid);
            print("选课数据已保存到数据库\n");
        }
    }
}

void Registrar::studentDropsCourse(std::string sid, std::string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);
    if(student && course){
        student->dropCourse(course);
        
        // 从数据库删除选课数据
        if (_dataManager) {
            _dataManager->removeEnrollment(sid, cid);
            print("退课数据已从数据库删除\n");
        }
    }
}

void Registrar::courseRoster(std::string cid){
    auto c = findCourseById(cid);
    print("{}\n",c->roster());
}

void Registrar::studentSchedule(std::string sid){
    auto s = findStudentById(sid);
    if(s){
        print("{}\n",s->schedule());
    }
}

void Registrar::teacherSchedule(std::string tid){
    auto t = findTeacherById(tid);
    if(t){
        print("{}\n",t->schedule());
    }
}

void Registrar::teacherRoster(std::string tid){
    auto t = findTeacherById(tid);
    if(t){
        print("{}\n",t->roster());
    }
}

void Registrar::initialize(){
    // 尝试从数据库加载数据
    if (_dataManager) {
        // 使用loadAllData方法加载数据
        loadAllData();
        
        // 如果数据库中没有数据，则初始化默认数据
        if (_students.empty() && _teachers.empty() && _courses.empty()) {
            print("数据库为空，初始化默认数据...\n");
            initializeDefaultData();
            // 重新加载数据
            loadAllData();
        }
    } else {
        // 如果没有数据管理器，则初始化默认数据
        initializeDefaultData();
    }
}

void Registrar::initializeDefaultData(){
    // Create students
    _students.push_back(new Student("S001","Tomas", "1234"));
    _students.push_back(new Student("S002","Jerry", "2345"));
    _students.push_back(new Student("S003","Baker", "3456"));
    _students.push_back(new Student("S004","Tom", "4567"));
    _students.push_back(new Student("S005","Musk", "5678"));
        
    // Create teachers
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
    
    // 创建教学任务并保存到数据库
    if (_dataManager) {
        _dataManager->saveTeachingTask(new TeachingTask("T001", "CS101", "2024-Spring", "Mon 8:00-10:00", "A101"));
        _dataManager->saveTeachingTask(new TeachingTask("T002", "CS201", "2024-Spring", "Tue 10:00-12:00", "B202"));
        _dataManager->saveTeachingTask(new TeachingTask("T003", "MATH101", "2024-Spring", "Wed 14:00-16:00", "C303"));
    }
    
    print("course list:\n cid  course\n");
    for(auto &c:_courses){
            print("{}",c->info());
        }
        println();
}

Registrar::Registrar(){}

Student *Registrar::findStudentById(const std::string &id){
    for(auto& student :_students){
        if(student->hasId(id))
        return student;
    }
    return nullptr;
}

Course *Registrar::findCourseById(const std::string&id){
    for(auto& course :_courses){
        if(course->hasId(id))
        return course;
    }
    return nullptr;
}

Teacher *Registrar::findTeacherById(const std::string&id){
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

void Registrar::addStudent(std::string id, std::string name){
    if(!findStudentById(id)){
        auto student = new Student(id, name, "123");
        _students.push_back(student);
        
        // 保存到数据库
        if (_dataManager) {
            _dataManager->saveStudent(student);
            print("学生 {} 已保存到数据库\n", id);
        }
    }
}

void Registrar::removeStudent(std::string id){
    auto it = std::find_if(_students.begin(), _students.end(), 
        [&id](Student* s){ return s->hasId(id); });
    if(it != _students.end()){
        delete *it;
        _students.erase(it);
    }
}

void Registrar::addTeacher(std::string id, std::string name){
    if(!findTeacherById(id)){
        auto teacher = new Teacher(id, name, "123");
        _teachers.push_back(teacher);
        
        // 保存到数据库
        if (_dataManager) {
            _dataManager->saveTeacher(teacher);
            print("教师 {} 已保存到数据库\n", id);
        }
    }
}

void Registrar::removeTeacher(std::string id){
    auto it = std::find_if(_teachers.begin(), _teachers.end(), 
        [&id](Teacher* t){ return t->hasId(id); });
    if(it != _teachers.end()){
        delete *it;
        _teachers.erase(it);
    }
}

void Registrar::addCourse(std::string id, std::string name){
    if(!findCourseById(id)){
        auto course = new Course(id, name);
        _courses.push_back(course);
        
        // 保存到数据库
        if (_dataManager) {
            _dataManager->saveCourse(course);
            print("课程 {} 已保存到数据库\n", id);
        }
    }
}

void Registrar::removeCourse(std::string id){
    auto it = std::find_if(_courses.begin(), _courses.end(), 
        [&id](Course* c){ return c->hasId(id); });
    if(it != _courses.end()){
        delete *it;
        _courses.erase(it);
    }
}

std::string Registrar::generateEnrollmentReport(){
    std::string report = "=== 选课统计报告 ===\n";
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

std::string Registrar::generateCourseReport(){
    std::string report = "=== 课程详细报告 ===\n";
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

std::string Registrar::generateTeacherReport(){
    std::string report = "=== 教师工作量报告 ===\n";
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

void Registrar::setDataManager(DataManager* dataManager) {
    _dataManager = dataManager;
}

void Registrar::saveAllData() {
    if (!_dataManager) return;
    
    // 保存所有学生
    for (auto student : _students) {
        _dataManager->saveStudent(student);
    }
    
    // 保存所有教师
    for (auto teacher : _teachers) {
        _dataManager->saveTeacher(teacher);
    }
    
    // 保存所有课程
    for (auto course : _courses) {
        _dataManager->saveCourse(course);
    }
    
    // 保存所有教学任务
    for (auto task : _teachingTasks) {
        _dataManager->saveTeachingTask(task);
    }
}

void Registrar::loadAllData() {
    if (!_dataManager) return;
    
    print("正在从数据库加载数据...\n");
    
    // 加载所有学生
    auto students = _dataManager->loadStudents();
    _students.insert(_students.end(), students.begin(), students.end());
    
    // 加载所有教师
    auto teachers = _dataManager->loadTeachers();
    _teachers.insert(_teachers.end(), teachers.begin(), teachers.end());
    
    // 加载所有课程
    auto courses = _dataManager->loadCourses();
    _courses.insert(_courses.end(), courses.begin(), courses.end());
    
    // 加载教学任务
    auto tasks = _dataManager->loadTeachingTasks();
    _teachingTasks.insert(_teachingTasks.end(), tasks.begin(), tasks.end());
    
    // 如果教学任务为空，则创建默认教学任务
    if (_teachingTasks.empty() && !_students.empty() && !_teachers.empty() && !_courses.empty()) {
        print("数据库中没有教学任务，初始化默认教学任务...\n");
        _dataManager->saveTeachingTask(new TeachingTask("T001", "CS101", "2024-Spring", "Mon 8:00-10:00", "A101"));
        _dataManager->saveTeachingTask(new TeachingTask("T002", "CS201", "2024-Spring", "Tue 10:00-12:00", "B202"));
        _dataManager->saveTeachingTask(new TeachingTask("T003", "MATH101", "2024-Spring", "Wed 14:00-16:00", "C303"));
        
        // 重新加载教学任务
        tasks = _dataManager->loadTeachingTasks();
        _teachingTasks.insert(_teachingTasks.end(), tasks.begin(), tasks.end());
    }
    
    // 设置教师的课程信息
    setTeacherCourses();
    
    // 设置学生的课程信息
    setStudentCourses();
    
    // 加载学生选课数据并更新学生的课程列表
    auto enrollments = _dataManager->loadEnrollments();
    for (auto enrollment : enrollments) {
        auto student = findStudentById(enrollment->enrolledStudentId());
        auto course = findCourseById(enrollment->enrolledCourseId());
        if (student && course) {
            student->enrollsIn(course);
        }
    }
    
    print("数据加载完成。共加载 {} 个学生, {} 个教师, {} 个课程, {} 个教学任务\n", 
          _students.size(), _teachers.size(), _courses.size(), _teachingTasks.size());
}

// ========== 教学任务管理实现 ==========

void Registrar::addTeachingTask(TeachingTask* task) {
    _teachingTasks.push_back(task);
    if (_dataManager) {
        _dataManager->saveTeachingTask(task);
    }
}

bool Registrar::removeTeachingTask(const std::string& teacherId, const std::string& courseId) {
    auto it = std::find_if(_teachingTasks.begin(), _teachingTasks.end(),
        [&teacherId, &courseId](TeachingTask* task) {
            return task->matches(teacherId, courseId);
        });
    
    if (it != _teachingTasks.end()) {
        if (_dataManager) {
            _dataManager->removeTeachingTask(teacherId, courseId);
        }
        delete *it;
        _teachingTasks.erase(it);
        return true;
    }
    return false;
}

std::vector<TeachingTask*> Registrar::getTeachingTasks() {
    return _teachingTasks;
}

void Registrar::setTeacherCourses() {
    if (!_dataManager) return;
    
    // 为每个教师设置其教授的课程
    for (auto teacher : _teachers) {
        vector<Course*> teacherCourses;
        
        // 遍历所有教学任务，找到该教师的课程
        for (auto task : _teachingTasks) {
            if (task->matchesTeacher(teacher->getId())) {
                auto course = findCourseById(task->assignedCourseId());
                if (course) {
                    teacherCourses.push_back(course);
                }
            }
        }
        
        // 设置教师的课程列表
        teacher->setCourses(teacherCourses);
    }
}

// 设置学生的课程信息
void Registrar::setStudentCourses() {
    if (!_dataManager) return;
    
    // 为每个学生设置其选修的课程
    for (auto student : _students) {
        vector<Course*> studentCourses;
        
        // 遍历所有选课记录，找到该学生的课程
        auto enrollments = _dataManager->loadEnrollments();
        for (auto enrollment : enrollments) {
            if (enrollment->involvesStudent(student->getId())) {
                auto course = findCourseById(enrollment->enrolledCourseId());
                if (course) {
                    studentCourses.push_back(course);
                }
            }
        }
        
        // 设置学生的课程列表
        student->setCourses(studentCourses);
    }
}
