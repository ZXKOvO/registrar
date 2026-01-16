// Course module - defines Course class
export module registrar:course;
import std;
// Module
// File: course.cppm   Version: 0.1.0   License: AGPLv3
// Created: XinKang Zheng  Id:2024051604022  2026-01-07 10:46:48
// Description:
//
using std::string;
using std::vector;

export class Course
{
public:
    Course(string id, string name);
    bool acceptEnrollment(class Student* student);
    bool removeStudent(class Student* student);
    string roster();
    string info();
    bool hasId(string id);
    void assignTeacher(class Teacher* teacher);
    int getEnrollmentCount();
    bool isFull();
    

private:
    string m_name;
    string m_id;
    short m_credit;
    static int cm_totalCount;
    vector<class Student*> _students;
    class Teacher* _teacher;
};

int Course::cm_totalCount = 0;

Course::Course(string id, string name)
    :m_name(name)
    ,m_id(id)
    ,_teacher(nullptr)
{
    cm_totalCount++;
}

bool Course::acceptEnrollment(Student *student)
{
    if(_students.size() < 80){
        _students.push_back(student);
        print("\"{}\" 选课成功！目前选择人数:{}\n",m_name,_students.size());
        return true;
    }
    return false;
}

bool Course::removeStudent(Student *student){
    auto it = std::find(_students.begin(), _students.end(), student);
    if(it != _students.end()){
        _students.erase(it);
        print(" \"{}\" 退课成功！目前选择人数:{}\n",m_name,_students.size());
        return true;
    }
    print("\"{}\" 退课失败！该学生未选此课程\n",m_name);
    return false;
}

string Course::info(){
    return format("{}  {}\n",m_id,m_name);
}

bool Course::hasId(string id){
    return id == m_id;
}

void Course::assignTeacher(Teacher* teacher){
    _teacher = teacher;
}

int Course::getEnrollmentCount(){
    return _students.size();
}

bool Course::isFull(){
    return _students.size() >= 80;
}


