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
    
private:
    Registrar();
    class Student* findStudentById(const string& id);
    class Course* findCourseById(const string& id);
    class Teacher* findTeacherById(const string& id);
    
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
