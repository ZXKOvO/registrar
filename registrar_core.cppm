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

private:
    Registrar();

    std::vector<Course*> _courses;
    std::vector<Student*> _students;
    std::vector<Teacher*> _teachers;
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
    }
}

void Registrar::studentDropsCourse(std::string sid, std::string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);
    if(student && course){
        student->dropCourse(course);
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
    // Create students
    _students.push_back(new Student("S001","Tomas"));
    _students.push_back(new Student("S002","Jerry"));
    _students.push_back(new Student("S003","Baker"));
    _students.push_back(new Student("S004","Tom"));
    _students.push_back(new Student("S005","Musk"));
    
    print("student list:\n sid student\n");
    for(auto &s:_students){
            print("{}",s->info());
        }
        println();
        
    // Create teachers
    _teachers.push_back(new Teacher("T001","Smith"));
    _teachers.push_back(new Teacher("T002","Johnson"));
    _teachers.push_back(new Teacher("T003","Williams"));
    
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
        _students.push_back(new Student(id, name));
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
        _teachers.push_back(new Teacher(id, name));
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
        _courses.push_back(new Course(id, name));
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
