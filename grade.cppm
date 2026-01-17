// Module
// File: grade.cppm   Version: 0.1.0   License: AGPLv3
// Created:CCJ       2026-01-16 18:42:19
// Description:
//     添加成绩类，用于存储和管理学生的课程成绩信息
export module registrar:grade;
import std;
using std::string;

// 成绩类 - 用于存储和管理学生的课程成绩信息
export class Grade
{
public:
    // 构造函数 - 创建成绩对象
    // 参数：studentId（学生ID）、courseId（课程ID）、teacherId（教师ID）、score（分数）、comment（评语，默认为空）
    Grade(string studentId, string courseId, string teacherId, double score, string comment = "");
    
    // 获取学生ID
    string getStudentId() const;
    
    // 获取课程ID
    string getCourseId() const;
    
    // 获取教师ID
    string getTeacherId() const;
    
    // 获取分数
    double getScore() const;
    
    // 获取评语
    string getComment() const;
    
    // 设置分数
    // 参数：score（新的分数）
    void setScore(double score);
    
    // 设置评语
    // 参数：comment（新的评语）
    void setComment(string comment);
    
    // 获取成绩的详细信息
    // 返回：格式化的成绩信息字符串
    string info() const;

private:
    string _studentId;      // 学生ID
    string _courseId;       // 课程ID
    string _teacherId;      // 教师ID
    double _score;          // 分数
    string _comment;        // 评语
};

// 构造函数实现
Grade::Grade(string studentId, string courseId, string teacherId, double score, string comment)
    : _studentId(studentId)
    , _courseId(courseId)
    , _teacherId(teacherId)
    , _score(score)
    , _comment(comment)
{}

// 获取学生ID
string Grade::getStudentId() const
{
    return _studentId;
}

// 获取课程ID
string Grade::getCourseId() const
{
    return _courseId;
}

// 获取教师ID
string Grade::getTeacherId() const
{
    return _teacherId;
}

// 获取分数
double Grade::getScore() const
{
    return _score;
}

// 获取评语
string Grade::getComment() const
{
    return _comment;
}

// 设置分数
void Grade::setScore(double score)
{
    _score = score;
}

// 设置评语
void Grade::setComment(string comment)
{
    _comment = comment;
}

// 获取成绩详细信息
string Grade::info() const
{
    return format("学生: {} | 课程: {} | 教师: {} | 成绩: {:.1f} | 评语: {}\n", 
                  _studentId, _courseId, _teacherId, _score, _comment);
}
