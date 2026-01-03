// Module registrar: defines Registrar class
// File: registrar.cppm   Version: 1.0      License: AGPLv3
// Created: Wei Gong open-src@qq.com      2025-12-12
//
// Description:
//     The interface and implementation of Register class are logically separated.

// Change Log:
//     [v1.1] Wei Gong open-src@qq.com   2025-12-12
//         * added the singleton interface Registrar::singleton().
//     [v1.2] Wei Gong open-src@qq.com   2025-12-12
//         * added the implementation of Registrar class
//         * added the implementation of Student-Course object interactions
//     [v1.3] Wei Gong open-src@qq.com   2025-12-12 18:04:07
//         * added Registar::studentSchedule()
//         * added the implementation of Student::schedule

export module registrar;
export import :student;
export import :course;

import std;
using std::string; using std::vector;

export class Registrar
{
public:
    static Registrar& singleton();  //static function member
    void studentEnrollsInCourse(string sid, string cid);
    void studentSchedule(const string& sid);
    void courseRoster(string cid);
    void initialize();

private:
    Registrar();  // Prohibit creating objects directly
    class Student* findStudentById(const string& id);
    class Course* findCourseById(const string& id);

    vector<class Course*> _courses;
    vector<class Student*> _students;
};

// ----- The implementaion of class Registrar -----
Registrar &Registrar::singleton(){
    static Registrar instance;
    return instance;
}

void Registrar::studentEnrollsInCourse(string sid, string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);

    if (student && course) {
        student->enrollsIn(course);
    }
}

void Registrar::studentSchedule(const string &sid)
{
    auto s = findStudentById(sid);
    print("{}\n",s->schedule());
}

void Registrar::courseRoster(string cid){
    auto c = findCourseById(cid);
    print("{}\n", c->roster());
}

void Registrar::initialize(){
    _students.push_back(new Student("S001", "Thomas"));
    _students.push_back(new Student("S002", "Jerry"));
    _students.push_back(new Student("S003", "Baker"));
    _students.push_back(new Student("S004", "Tom"));
    _students.push_back(new Student("S005", "Musk"));

    _courses.push_back(new Course("CS101", "C Programming"));
    _courses.push_back(new Course("CS201", "Data structure"));
    _courses.push_back(new Course("MATH101", "Advanced Math"));
}

Registrar::Registrar(){}

Student *Registrar::findStudentById(const string &id){
    for (auto& student :_students) {
        if (student->hasId(id))
            return student;
    }
    return nullptr;
}

Course *Registrar::findCourseById(const string &id){
    for (auto& course : _courses) {
        if (course->hasId(id) )
            return course;
    }
    return nullptr;
}

// ----- Implementation of Student-Course object interactions -----

void Student::enrollsIn(Course *course){
    if(course->acceptEnrollment(this))
        _courses.push_back(course);
}

string Course::roster(){
    auto rst = format("{} selected by the students:\n", m_name);
    for (auto s : _students) {
        rst += s->info(); // 课程对象委托学生对象自己输出相关信息
    }
    return rst;
}

string Student::schedule()
{
    auto s = format("{}'s schedule:\n", m_name);
    for(auto &c: _courses){
        s += c->info();
    }
    return s;
}
