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
    void displayEnrollmentInfo(int& count, bool& full);

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

void Course::displayEnrollmentInfo(int& count, bool& full){
    count = _students.size();
    full = _students.size() >= 80;
}

string Course::roster(){
    string result = format("{} - 学生名单 (共{}人):\n", m_name, _students.size());
    if (_students.empty()) {
        result += "  (暂无学生选课)\n";
    } else {
        result += "  学生ID\t姓名\n";
        result += "  ----------------\n";
        // 由于循环依赖问题，这里只显示基本信息
        // 实际应用中可以通过其他方式获取学生详细信息
        result += "  (学生详细信息需要通过Registrar获取)\n";
    }
    return result;
}




