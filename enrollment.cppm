export module registrar:enrollment;
import std;

export class Enrollment {
public:
    Enrollment(const std::string& studentId, const std::string& courseId)
        : _studentId(studentId), _courseId(courseId) {}
    
    // 提供行为而非直接的数据访问
    bool involvesStudent(const std::string& studentId) const {
        return _studentId == studentId;
    }
    
    bool involvesCourse(const std::string& courseId) const {
        return _courseId == courseId;
    }
    
    bool matches(const std::string& studentId, const std::string& courseId) const {
        return involvesStudent(studentId) && involvesCourse(courseId);
    }
    
    // 提供业务相关的信息
    std::string enrolledStudentId() const { return _studentId; }
    std::string enrolledCourseId() const { return _courseId; }
    
    std::string description() const {
        return std::format("学生 {} 选修课程 {}", _studentId, _courseId);
    }
    
private:
    std::string _studentId;
    std::string _courseId;
};
