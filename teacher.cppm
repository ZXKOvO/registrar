// Module
// File: teacher.cppm   Version: 0.1.0   License: AGPLv3
// Created: XinKang Zheng  Id:2024051604022  2026-01-07 10:47:15
// Description:安排授课
export module registrar:teacher;
import :course;
import :grade;
import std;
using std::string;
using std::vector;
using std::print;

export class Teacher
{
public:
    Teacher(string id, string name, string password = "");
    void assignToCourse(class Course* course);
    string info();
    bool hasId(string id);
    bool hasCourse(string courseId);
    string schedule();
    string roster();
    void displayCourseInfo(int& count);
    void displayGrades();
    Grade* findGradesByStudent(string studentId);
    class Grade* gradeCourse(string studentId, string courseId, double score, string comment = "");
    class Grade* findGrade(string studentId, string courseId);
    class Grade* updateGrade(string studentId, string courseId, double score, string comment);
    bool authenticate(string password);

private:
    string m_name;
    string m_id;
    string m_password;
    vector<class Course*> _courses;
    vector<class Grade*> _grades;
};

Teacher::Teacher(string id, string name, string password)
    : m_name(name)
    , m_id(id)
    , m_password(password)
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
    if (_courses.empty())
    {
        result += "(未安排课程)\n";
    }
    else
    {
        for (auto& c : _courses)
        {
            result += c->info();
        }
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

void Teacher::displayCourseInfo(int& count)
{
    count = _courses.size();
}

class Grade* Teacher::gradeCourse(string studentId, string courseId, double score, string comment)
{
    auto grade = new Grade(studentId, courseId, m_id, score, comment);
    _grades.push_back(grade);
    return grade;
}

void Teacher::displayGrades()
{
    print("教师: {}\n", info());
    print("\n=== 成绩列表 ===\n");
    print("学生ID\t课程ID\t成绩\t评语\n");
    print("----------------------------------------\n");
    
    bool found = false;
    for (auto& grade : _grades) {
        print("{}", grade->info());
        found = true;
    }
    
    if (!found) {
        print("暂无成绩记录\n");
    }
}

Grade* Teacher::findGradesByStudent(string studentId)
{
    for (auto& grade : _grades) {
        if (grade->matches(studentId, "")) {
            return grade;
        }
    }
    return nullptr;
}

class Grade* Teacher::findGrade(string studentId, string courseId)
{
    for (auto& grade : _grades)
    {
        if (grade->matches(studentId, courseId))
        {
            return grade;
        }
    }
    return nullptr;
}

class Grade* Teacher::updateGrade(string studentId, string courseId, double score, string comment)
{
    auto grade = findGrade(studentId, courseId);
    if (grade)
    {
        grade->updateInfo(score, comment);
        return grade;
    }
    return nullptr;
}

bool Teacher::authenticate(string password)
{
    return m_password == password;
}
