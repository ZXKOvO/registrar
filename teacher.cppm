// Module
// File: teacher.cppm   Version: 0.1.0   License: AGPLv3
// Created: XinKang Zheng  Id:2024051604022  2026-01-07 10:47:15
// Description:安排授课
export module registrar:teacher;
import :course;
import std;
using std::string;
using std::vector;

export class Teacher
{
public:
    Teacher(string id, string name);
    void assignToCourse(class Course* course);
    string info();
    bool hasId(string id);
    bool hasCourse(string courseId);
    string schedule();
    string roster();
    int getCourseCount();

private:
    string m_name;
    string m_id;
    vector<class Course*> _courses;
};

Teacher::Teacher(string id, string name)
    : m_name(name)
    , m_id(id)
{}

string Teacher::info()
{
    return std::format("{}   {}\n", m_id, m_name);
}

bool Teacher::hasId(string id)
{
    return id == m_id;
}

void Teacher::assignToCourse(class Course* course)
{
    _courses.push_back(course);
}



bool Teacher::hasCourse(string courseId)
{
    for (auto& c : _courses)
    {
        if (c->hasId(courseId))
        {
            return true;
        }
    }
    return false;
}

string Teacher::schedule()
{
    auto result = std::format("{}'s teaching schedule:\n", m_name);
    for (auto& c : _courses)
    {
        result += c->info();
    }
    return result;
}

string Teacher::roster()
{
    auto result = std::format("{}'s student rosters:\n", m_name);
    for (auto& c : _courses)
    {
        result += std::format("{}:\n", c->info());
        result += c->roster();
    }
    return result;
}

int Teacher::getCourseCount()
{
    return _courses.size();
}
