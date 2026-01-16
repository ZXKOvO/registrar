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
export import :academic_secretary;
import std;
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
    void removeStudent(string id);
    void addTeacher(string id, string name);
    void removeTeacher(string id);
    void addCourse(string id, string name);
    void removeCourse(string id);
    string generateEnrollmentReport();
    string generateCourseReport();
    string generateTeacherReport();
    class Course* findCourseById(const string& id);
    class Teacher* findTeacherById(const string& id);
    
private:
    Registrar();
    class Student* findStudentById(const string& id);
    
    vector<class Course*> _courses;
    vector<class Student*> _students;
    vector<class Teacher*> _teachers;
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
    std::print("{}\n",c->roster());
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
    // Create students
    _students.push_back(new Student("S001","Tomas"));
    _students.push_back(new Student("S002","Jerry"));
    _students.push_back(new Student("S003","Baker"));
    _students.push_back(new Student("S004","Tom"));
    _students.push_back(new Student("S005","Musk"));
    
    std::print("student list:\n sid student\n");
    for(auto &s:_students){
            std::print("{}",s->info());
        }
        std::println();
        
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
    
    std::print("course list:\n cid  course\n");
    for(auto &c:_courses){
            std::print("{}",c->info());
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

void Registrar::removeStudent(string id){
    auto it = std::find_if(_students.begin(), _students.end(), 
        [&id](Student* s){ return s->hasId(id); });
    if(it != _students.end()){
        delete *it;
        _students.erase(it);
    }
}

void Registrar::addTeacher(string id, string name){
    if(!findTeacherById(id)){
        _teachers.push_back(new Teacher(id, name));
    }
}

void Registrar::removeTeacher(string id){
    auto it = std::find_if(_teachers.begin(), _teachers.end(), 
        [&id](Teacher* t){ return t->hasId(id); });
    if(it != _teachers.end()){
        delete *it;
        _teachers.erase(it);
    }
}

void Registrar::addCourse(string id, string name){
    if(!findCourseById(id)){
        _courses.push_back(new Course(id, name));
    }
}

void Registrar::removeCourse(string id){
    auto it = std::find_if(_courses.begin(), _courses.end(), 
        [&id](Course* c){ return c->hasId(id); });
    if(it != _courses.end()){
        delete *it;
        _courses.erase(it);
    }
}

string Registrar::generateEnrollmentReport(){
    string report = "=== 选课统计报告 ===\n";
    report += std::format("总学生数: {}\n", _students.size());
    report += std::format("总课程数: {}\n", _courses.size());
    
    for(auto& course : _courses){
        report += std::format("课程 {} - 选课人数: {}/{}\n", 
            course->info().substr(0, course->info().find_last_of("\n")), 
            course->getEnrollmentCount(), 80);
    }
    return report;
}

string Registrar::generateCourseReport(){
    string report = "=== 课程详细报告 ===\n";
    for(auto& course : _courses){
        report += course->info();
        report += std::format("选课人数: {}/{}\n", course->getEnrollmentCount(), 80);
        report += "选课学生名单:\n";
        report += course->roster();
        report += "\n";
    }
    return report;
}

string Registrar::generateTeacherReport(){
    string report = "=== 教师工作量报告 ===\n";
    for(auto& teacher : _teachers){
        report += teacher->info();
        report += std::format("授课数量: {}\n", teacher->getCourseCount());
        report += "授课安排:\n";
        report += teacher->schedule();
        report += "\n";
    }
    return report;
}
