// Partition 'course' of module 'registrar' - defines Course class
// File: course.cppm   Version: 1.0      License: AGPLv3
// Created: Wei Gong open-src@qq.com      2025-12-12
//
// Description:
//     The interface and implementation of Course class are logically separated.

// Change Log:
//     [v1.1] Wei Gong open-src@qq.com   2025-12-12
//         * added static data member 'totalCount': counts all Course objects in the system.
//     [v1.2] Wei Gong open-src@qq.com   2025-12-12
//         * added the implementation of the Course class.
//     [v1.3] Wei Gong open-src@qq.com   2025-12-12 18:02:14
//         * added Course::info()

export module registrar:course;
import std;

using std::string; using std::vector;

export class Course
{
public:
    Course(string id, string name);

    bool acceptEnrollment(class Student* student);
    string roster();
    string info();
    bool hasId(string id);
private:
    string m_name;
    string m_id;
    short m_credit;
    static int cm_totalCount;  // static data member

    vector<class Student*> _students;
};

// ----- Partial implementation of class Course -----

int Course::cm_totalCount = 0; // initialize static data memeber

Course::Course(string id, string name)
    : m_name(name)
    , m_id(id)
{
    cm_totalCount++;
}

bool Course::acceptEnrollment(Student *student){
    if(_students.size() < 80){  // 假定某个课程的最大人数为80
        _students.push_back(student);
        print("\"{}\" 选课成功！目前选择该课程的人数: {}\n",
              m_name, _students.size());
        return true;
    }
    return false;
}

string Course::info(){
    return format("{}   {}\n", m_id, m_name);
}


bool Course::hasId(string id){
    return id == m_id;
}
