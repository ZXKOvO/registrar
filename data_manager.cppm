export module registrar:data_manager;

export import :student;
export import :teacher;
export import :course;
export import :grade;
export import :teaching_task;
export import :enrollment;
export import :academic_secretary;

export class IDataAccess {
public:
    virtual ~IDataAccess() = default;
    
    // 学生数据操作
    virtual void saveStudent(Student* student) = 0;
    virtual std::vector<Student*> loadStudents() = 0;
    virtual bool removeStudent(const std::string& id) = 0;
    
    // 教师数据操作
    virtual void saveTeacher(Teacher* teacher) = 0;
    virtual std::vector<Teacher*> loadTeachers() = 0;
    virtual bool removeTeacher(const std::string& id) = 0;
    
    // 课程数据操作
    virtual void saveCourse(Course* course) = 0;
    virtual std::vector<Course*> loadCourses() = 0;
    virtual bool removeCourse(const std::string& id) = 0;
    
    // 成绩操作
    virtual void saveGrade(Grade* grade) = 0;
    virtual std::vector<Grade*> loadGrades() = 0;
    virtual bool removeGrade(const std::string& studentId, const std::string& courseId) = 0;
    virtual bool removeGradeByStudent(const std::string& studentId) = 0;
    
    // 教学任务操作
    virtual void saveTeachingTask(TeachingTask* task) = 0;
    virtual std::vector<TeachingTask*> loadTeachingTasks() = 0;
    virtual bool removeTeachingTask(const std::string& courseId, const std::string& teacherId) = 0;
    
    // 选课记录操作
    virtual bool saveEnrollment(const std::string& studentId, const std::string& courseId) = 0;
    virtual bool removeEnrollment(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::vector<Enrollment*> loadEnrollments() = 0;
    
    // 教学秘书数据操作
    virtual void saveAcademicSecretary(AcademicSecretary* secretary) = 0;
    virtual std::vector<AcademicSecretary*> loadAcademicSecretaries() = 0;
    virtual bool removeAcademicSecretary(const std::string& id) = 0;
    
    // 事务支持
    virtual bool beginTransaction() = 0;
    virtual bool commit() = 0;
    virtual bool rollback() = 0;
};

export class DataManager {
public:
    explicit DataManager(std::unique_ptr<IDataAccess> dataAccess);
    ~DataManager();
    
    // 数据存储切换
    void switchToInMemory();
    void switchToPostgreSQL();
    
    // 代理所有数据访问操作
    void saveStudent(Student* student);
    std::vector<Student*> loadStudents();
    bool removeStudent(const std::string& id);
    
    void saveTeacher(Teacher* teacher);
    std::vector<Teacher*> loadTeachers();
    bool removeTeacher(const std::string& id);
    
    void saveCourse(Course* course);
    std::vector<Course*> loadCourses();
    bool removeCourse(const std::string& id);
    
    void saveGrade(Grade* grade);
    bool removeGrade(const std::string& studentId, const std::string& courseId);
    bool removeGradeByStudent(const std::string& studentId);
    std::vector<Grade*> loadGrades();
    
    void saveTeachingTask(TeachingTask* task);
    std::vector<TeachingTask*> loadTeachingTasks();
    bool removeTeachingTask(const std::string& courseId, const std::string& teacherId);
    
    // 选课记录操作
    bool saveEnrollment(const std::string& studentId, const std::string& courseId);
    bool removeEnrollment(const std::string& studentId, const std::string& courseId);
    std::vector<Enrollment*> loadEnrollments();
    
    // 教学秘书数据操作
    void saveAcademicSecretary(AcademicSecretary* secretary);
    std::vector<AcademicSecretary*> loadAcademicSecretaries();
    bool removeAcademicSecretary(const std::string& id);
    
    // 事务支持
    bool beginTransaction();
    bool commit();
    bool rollback();
    
private:
    std::unique_ptr<IDataAccess> _dataAccess;
};