// Module
// File: student.cppm   Version: 0.1.0   License: AGPLv3
// Created: XinKang Zheng  Id:2024051604022  2026-01-07 10:47:00
// Description:
//
export module registrar:student;
import std;
using std::string;
using std::vector;

export class Student
{
public:
    Student(string id, string name);
    void enrollsIn(class Course* course);
    void dropCourse(class Course* course);
    string schedule();
    string info();
    bool hasId(string id);

private:
    string m_name;
    string m_id;
    vector<class Course*> _courses;
};

Student::Student(string id, string name)
    :m_name(name)
    ,m_id(id)
{}

string Student::info()
{
    return format("{}   {}\n", m_id, m_name);
}

bool Student::hasId(string id)
{
    return id == m_id;
}
