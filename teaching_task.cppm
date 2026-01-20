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
    
    // 获取教学任务的详细信息
    // 返回：格式化的教学任务信息字符串
    string info() const;
    
    // 检查是否匹配指定的教师ID和课程ID
    bool matches(string teacherId, string courseId) const;
    
    // 检查是否匹配指定的教师ID
    bool matchesTeacher(string teacherId) const;
    
    // 检查是否匹配指定的课程ID
    bool matchesCourse(string courseId) const;
    
    // 检查是否匹配指定的学期
    bool matchesSemester(string semester) const;
    
    // 提供教学任务的相关信息，而非直接的数据访问
    string assignedTeacherId() const;
    string assignedCourseId() const;
    string scheduledSemester() const;
    string scheduledTimeSlot() const;
    string assignedClassroom() const;
    
    // 更新教学任务安排的方法，使用更具描述性的名称
    TeachingTask withUpdatedSchedule(string newTimeSlot) const;
    TeachingTask withUpdatedClassroom(string newClassroom) const;
    TeachingTask withUpdatedSemester(string newSemester) const;
    
    // 格式化的时间段信息
    string formattedSchedule() const;

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

// 获取教学任务详细信息
string TeachingTask::info() const
{
    return format("Teacher: {} | Course: {} | Semester: {} | Time: {} | Classroom: {}\n", 
                  _teacherId, _courseId, _semester, _timeSlot, _classroom);
}

// 检查是否匹配指定的教师ID和课程ID
bool TeachingTask::matches(string teacherId, string courseId) const
{
    return _teacherId == teacherId && _courseId == courseId;
}

// 检查是否匹配指定的教师ID
bool TeachingTask::matchesTeacher(string teacherId) const
{
    return _teacherId == teacherId;
}

// 检查是否匹配指定的课程ID
bool TeachingTask::matchesCourse(string courseId) const
{
    return _courseId == courseId;
}

// 检查是否匹配指定的学期
bool TeachingTask::matchesSemester(string semester) const
{
    return _semester == semester;
}



string TeachingTask::assignedTeacherId() const
{
    return _teacherId;
}

string TeachingTask::assignedCourseId() const
{
    return _courseId;
}

string TeachingTask::scheduledSemester() const
{
    return _semester;
}

string TeachingTask::scheduledTimeSlot() const
{
    return _timeSlot;
}

string TeachingTask::assignedClassroom() const
{
    return _classroom;
}

TeachingTask TeachingTask::withUpdatedSchedule(string newTimeSlot) const
{
    return TeachingTask(_teacherId, _courseId, _semester, newTimeSlot, _classroom);
}

TeachingTask TeachingTask::withUpdatedClassroom(string newClassroom) const
{
    return TeachingTask(_teacherId, _courseId, _semester, _timeSlot, newClassroom);
}

TeachingTask TeachingTask::withUpdatedSemester(string newSemester) const
{
    return TeachingTask(_teacherId, _courseId, newSemester, _timeSlot, _classroom);
}

string TeachingTask::formattedSchedule() const
{
    return format("{} | {} | {}", _timeSlot, _classroom, _semester);
}
