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

    // 获取成绩的详细信息
    // 返回：格式化的成绩信息字符串
    string info() const;

    // 更新成绩信息
    void updateInfo(double score, string comment);

    // 检查是否匹配指定的学生ID和课程ID
    bool matches(string studentId, string courseId) const;

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

// 获取成绩详细信息
string Grade::info() const
{
    return format("学生: {} | 课程: {} | 教师: {} | 成绩: {:.1f} | 评语: {}\n",
                  _studentId, _courseId, _teacherId, _score, _comment);
}

// 更新成绩信息
void Grade::updateInfo(double score, string comment)
{
    _score = score;
    _comment = comment;
}

// 检查是否匹配指定的学生ID和课程ID
bool Grade::matches(string studentId, string courseId) const
{
    return _studentId == studentId && _courseId == courseId;
}
