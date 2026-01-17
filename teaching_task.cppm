// Module
// File: teaching_task.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-17   Id:2024051604015   xxh
// Description: 教学任务模块 - 管理教师教学任务安排
//
export module registrar:teaching_task;
import std;
using std::string;

// 教学任务类 - 用于存储和管理教师的教学任务安排信息
export class TeachingTask
{
public:
    // 构造函数 - 创建教学任务对象
    // 参数：teacherId（教师ID）、courseId（课程ID）、semester（学期）、timeSlot（时间段）、classroom（教室）
    TeachingTask(string teacherId, string courseId, string semester, string timeSlot, string classroom);
    
    // 获取教师ID
    string getTeacherId() const;
    
    // 获取课程ID
    string getCourseId() const;
    
    // 获取学期
    string getSemester() const;
    
    // 获取时间段
    string getTimeSlot() const;
    
    // 获取教室
    string getClassroom() const;
    
    // 设置时间段
    // 参数：timeSlot（新的时间段）
    void setTimeSlot(string timeSlot);
    
    // 设置教室
    // 参数：classroom（新的教室）
    void setClassroom(string classroom);
    
    // 获取教学任务的详细信息
    // 返回：格式化的教学任务信息字符串
    string info() const;

private:
    string _teacherId;      // 教师ID
    string _courseId;       // 课程ID
    string _semester;       // 学期
    string _timeSlot;       // 时间段
    string _classroom;      // 教室
};

// 构造函数实现
TeachingTask::TeachingTask(string teacherId, string courseId, string semester, string timeSlot, string classroom)
    : _teacherId(teacherId)
    , _courseId(courseId)
    , _semester(semester)
    , _timeSlot(timeSlot)
    , _classroom(classroom)
{}

// 获取教师ID
string TeachingTask::getTeacherId() const
{
    return _teacherId;
}

// 获取课程ID
string TeachingTask::getCourseId() const
{
    return _courseId;
}

// 获取学期
string TeachingTask::getSemester() const
{
    return _semester;
}

// 获取时间段
string TeachingTask::getTimeSlot() const
{
    return _timeSlot;
}

// 获取教室
string TeachingTask::getClassroom() const
{
    return _classroom;
}

// 设置时间段
void TeachingTask::setTimeSlot(string timeSlot)
{
    _timeSlot = timeSlot;
}

// 设置教室
void TeachingTask::setClassroom(string classroom)
{
    _classroom = classroom;
}

// 获取教学任务详细信息
string TeachingTask::info() const
{
    return format("教师: {} | 课程: {} | 学期: {} | 时间: {} | 教室: {}\n", 
                  _teacherId, _courseId, _semester, _timeSlot, _classroom);
}