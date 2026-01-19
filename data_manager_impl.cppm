module;
#include "db_config.h"

module registrar:data_manager_impl;
import :data_manager;
import :student;
import :teacher;
import :course;
import :grade;
import :teaching_task;
import std;
using std::string;
using std::vector;
using std::unique_ptr;
using std::find_if;
using std::format;
using std::print;
using std::size_t;

// PostgreSQL C接口
extern "C" {
    // 前向声明
    struct pg_conn;
    struct pg_result;
    
    // 函数声明
    pg_conn* PQconnectdb(const char* conninfo);
    int PQstatus(const pg_conn* conn);
    char* PQerrorMessage(const pg_conn* conn);
    void PQfinish(pg_conn* conn);
    pg_result* PQexec(pg_conn* conn, const char* command);
    int PQresultStatus(const pg_result* res);
    void PQclear(pg_result* res);
    int PQntuples(const pg_result* res);
    char* PQgetvalue(const pg_result* res, int tup_num, int field_num);
    char* PQcmdTuples(const pg_result* res);
    
    // 常量
    const int CONNECTION_OK = 0;
    const int PGRES_COMMAND_OK = 1;
    const int PGRES_TUPLES_OK = 2;
}

// InMemoryStorage实现
class InMemoryStorage : public IDataAccess {
public:
    InMemoryStorage();
    ~InMemoryStorage() override;
    
    void saveStudent(Student* student) override;
    vector<Student*> loadStudents() override;
    bool removeStudent(const string& id) override;
    
    void saveTeacher(Teacher* teacher) override;
    vector<Teacher*> loadTeachers() override;
    bool removeTeacher(const string& id) override;
    
    void saveCourse(Course* course) override;
    vector<Course*> loadCourses() override;
    bool removeCourse(const string& id) override;
    
    void saveGrade(Grade* grade) override;
    vector<Grade*> loadGrades() override;
    bool removeGrade(const string& studentId, const string& courseId) override;
    
    void saveTeachingTask(TeachingTask* task) override;
    vector<TeachingTask*> loadTeachingTasks() override;
    bool removeTeachingTask(const string& courseId, const string& teacherId) override;
    
    bool saveEnrollment(const string& studentId, const string& courseId) override;
    bool removeEnrollment(const string& studentId, const string& courseId) override;
    vector<Enrollment*> loadEnrollments() override;
    
    bool beginTransaction() override { return true; }
    bool commit() override { return true; }
    bool rollback() override { return true; }
    
private:
    void clearAll();
    
    vector<Student*> _students;
    vector<Teacher*> _teachers;
    vector<Course*> _courses;
    vector<Grade*> _grades;
    vector<TeachingTask*> _teachingTasks;
    vector<Enrollment> _enrollments;
};

// PostgreSQLAdapter实现
class PostgreSQLAdapter : public IDataAccess {
public:
    PostgreSQLAdapter();
    ~PostgreSQLAdapter() override;
    
    bool connect();
    void disconnect();
    
    void saveStudent(Student* student) override;
    vector<Student*> loadStudents() override;
    bool removeStudent(const string& id) override;
    
    void saveTeacher(Teacher* teacher) override;
    vector<Teacher*> loadTeachers() override;
    bool removeTeacher(const string& id) override;
    
    void saveCourse(Course* course) override;
    vector<Course*> loadCourses() override;
    bool removeCourse(const string& id) override;
    
    void saveGrade(Grade* grade) override;
    vector<Grade*> loadGrades() override;
    bool removeGrade(const string& studentId, const string& courseId) override;
    
    void saveTeachingTask(TeachingTask* task) override;
    vector<TeachingTask*> loadTeachingTasks() override;
    bool removeTeachingTask(const string& courseId, const string& teacherId) override;
    
    bool saveEnrollment(const string& studentId, const string& courseId) override;
    bool removeEnrollment(const string& studentId, const string& courseId) override;
    vector<Enrollment*> loadEnrollments() override;
    
    bool beginTransaction() override;
    bool commit() override;
    bool rollback() override;
    
private:
    bool createTables();
    pg_result* executeQuery(const string& sql);
    
    pg_conn* _conn;
    bool _isConnected;
};

// InMemoryStorage实现
InMemoryStorage::InMemoryStorage() {
    // 初始化空的存储
}

InMemoryStorage::~InMemoryStorage() {
    clearAll();
}

void InMemoryStorage::clearAll() {
    // 清理学生
    for (auto student : _students) {
        delete student;
    }
    _students.clear();
    
    // 清理教师
    for (auto teacher : _teachers) {
        delete teacher;
    }
    _teachers.clear();
    
    // 清理课程
    for (auto course : _courses) {
        delete course;
    }
    _courses.clear();
    
    // 清理成绩
    for (auto grade : _grades) {
        delete grade;
    }
    _grades.clear();
    
    // 清理教学任务
    for (auto task : _teachingTasks) {
        delete task;
    }
    _teachingTasks.clear();
}

void InMemoryStorage::saveStudent(Student* student) {
    _students.push_back(student);
}

vector<Student*> InMemoryStorage::loadStudents() {
    vector<Student*> result;
    result.insert(result.end(), _students.begin(), _students.end());
    return result;
}

bool InMemoryStorage::removeStudent(const string& id) {
    auto it = find_if(_students.begin(), _students.end(),
        [&id](Student* s) { return s->hasId(id); });
    
    if (it != _students.end()) {
        delete *it;
        _students.erase(it);
        return true;
    }
    return false;
}

void InMemoryStorage::saveTeacher(Teacher* teacher) {
    _teachers.push_back(teacher);
}

vector<Teacher*> InMemoryStorage::loadTeachers() {
    vector<Teacher*> result;
    result.insert(result.end(), _teachers.begin(), _teachers.end());
    return result;
}

bool InMemoryStorage::removeTeacher(const string& id) {
    auto it = find_if(_teachers.begin(), _teachers.end(),
        [&id](Teacher* t) { return t->hasId(id); });
    
    if (it != _teachers.end()) {
        delete *it;
        _teachers.erase(it);
        return true;
    }
    return false;
}

void InMemoryStorage::saveCourse(Course* course) {
    _courses.push_back(course);
}

vector<Course*> InMemoryStorage::loadCourses() {
    vector<Course*> result;
    result.insert(result.end(), _courses.begin(), _courses.end());
    return result;
}

bool InMemoryStorage::removeCourse(const string& id) {
    auto it = find_if(_courses.begin(), _courses.end(),
        [&id](Course* c) { return c->hasId(id); });
    
    if (it != _courses.end()) {
        delete *it;
        _courses.erase(it);
        return true;
    }
    return false;
}

void InMemoryStorage::saveGrade(Grade* grade) {
    _grades.push_back(grade);
}

vector<Grade*> InMemoryStorage::loadGrades() {
    vector<Grade*> result;
    result.insert(result.end(), _grades.begin(), _grades.end());
    return result;
}

bool InMemoryStorage::removeGrade(const string& studentId, const string& courseId) {
    auto it = find_if(_grades.begin(), _grades.end(),
        [&studentId, &courseId](Grade* g) { return g->matches(studentId, courseId); });
    
    if (it != _grades.end()) {
        delete *it;
        _grades.erase(it);
        return true;
    }
    return false;
}

void InMemoryStorage::saveTeachingTask(TeachingTask* task) {
    _teachingTasks.push_back(task);
}

vector<TeachingTask*> InMemoryStorage::loadTeachingTasks() {
    vector<TeachingTask*> result;
    result.insert(result.end(), _teachingTasks.begin(), _teachingTasks.end());
    return result;
}

bool InMemoryStorage::removeTeachingTask(const string& courseId, const string& teacherId) {
    auto it = find_if(_teachingTasks.begin(), _teachingTasks.end(),
        [&courseId, &teacherId](TeachingTask* t) { return t->matches(courseId, teacherId); });
    
    if (it != _teachingTasks.end()) {
        delete *it;
        _teachingTasks.erase(it);
        return true;
    }
    return false;
}

bool InMemoryStorage::saveEnrollment(const string& studentId, const string& courseId) {
    _enrollments.emplace_back(studentId, courseId);
    return true;
}

bool InMemoryStorage::removeEnrollment(const string& studentId, const string& courseId) {
    auto it = find_if(_enrollments.begin(), _enrollments.end(), 
                      [&studentId, &courseId](const Enrollment& e) {
                          return e.matches(studentId, courseId);
                      });
    if (it != _enrollments.end()) {
        _enrollments.erase(it);
        return true;
    }
    return false;
}

vector<Enrollment*> InMemoryStorage::loadEnrollments() {
    vector<Enrollment*> enrollments;
    for (auto& e : _enrollments) {
        enrollments.push_back(new Enrollment(e.enrolledStudentId(), e.enrolledCourseId()));
    }
    return enrollments;
}

// PostgreSQLAdapter实现
PostgreSQLAdapter::PostgreSQLAdapter() : _isConnected(false), _conn(nullptr) {
}

PostgreSQLAdapter::~PostgreSQLAdapter() {
    if (_isConnected) {
        disconnect();
    }
}

bool PostgreSQLAdapter::connect() {
    // 使用头文件中的配置
    const char* conninfo = DBConfig::getConnectionString();
    
    _conn = PQconnectdb(conninfo);
    if (PQstatus(_conn) != CONNECTION_OK) {
        print("连接数据库失败: {}\n", PQerrorMessage(_conn));
        PQfinish(_conn);
        _conn = nullptr;
        _isConnected = false;
        return false;
    }
    
    _isConnected = true;
    print("成功连接到PostgreSQL数据库\n");
    
    // 创建数据库表
    if (!createTables()) {
        print("创建数据库表失败\n");
        return false;
    }
    
    return true;
}

void PostgreSQLAdapter::disconnect() {
    if (_conn) {
        PQfinish(_conn);
        _conn = nullptr;
    }
    _isConnected = false;
}

// 创建数据库表
bool PostgreSQLAdapter::createTables() {
    // 创建学生表
    const char* createStudentsTable = R"(
        CREATE TABLE IF NOT EXISTS students (
            id VARCHAR(20) PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            password VARCHAR(100) NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";
    
    pg_result* res = executeQuery(createStudentsTable);
    if (!res) return false;
    PQclear(res);
    
    // 创建教师表
    const char* createTeachersTable = R"(
        CREATE TABLE IF NOT EXISTS teachers (
            id VARCHAR(20) PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            password VARCHAR(100) NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";
    
    res = executeQuery(createTeachersTable);
    if (!res) return false;
    PQclear(res);
    
    // 创建课程表
    const char* createCoursesTable = R"(
        CREATE TABLE IF NOT EXISTS courses (
            id VARCHAR(20) PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            credit INTEGER DEFAULT 3,
            teacher_id VARCHAR(20) REFERENCES teachers(id),
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";
    
    res = executeQuery(createCoursesTable);
    if (!res) return false;
    PQclear(res);
    
    // 创建成绩表
    const char* createGradesTable = R"(
        CREATE TABLE IF NOT EXISTS grades (
            id SERIAL PRIMARY KEY,
            student_id VARCHAR(20) NOT NULL REFERENCES students(id),
            course_id VARCHAR(20) NOT NULL REFERENCES courses(id),
            teacher_id VARCHAR(20) NOT NULL REFERENCES teachers(id),
            score DECIMAL(5,2) CHECK (score >= 0 AND score <= 100),
            comment TEXT,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(student_id, course_id)
        )
    )";
    
    res = executeQuery(createGradesTable);
    if (!res) return false;
    PQclear(res);
    
    // 创建教学任务表
    const char* createTeachingTasksTable = R"(
        CREATE TABLE IF NOT EXISTS teaching_tasks (
            id SERIAL PRIMARY KEY,
            teacher_id VARCHAR(20) NOT NULL REFERENCES teachers(id),
            course_id VARCHAR(20) NOT NULL REFERENCES courses(id),
            semester VARCHAR(20) NOT NULL,
            time_slot VARCHAR(50) NOT NULL,
            classroom VARCHAR(50),
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(teacher_id, course_id, semester)
        )
    )";
    
    res = executeQuery(createTeachingTasksTable);
    if (!res) return false;
    PQclear(res);
    
    // 创建学生选课表
    const char* createStudentEnrollmentsTable = R"(
        CREATE TABLE IF NOT EXISTS student_enrollments (
            id SERIAL PRIMARY KEY,
            student_id VARCHAR(20) NOT NULL REFERENCES students(id),
            course_id VARCHAR(20) NOT NULL REFERENCES courses(id),
            enrolled_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(student_id, course_id)
        )
    )";
    
    res = executeQuery(createStudentEnrollmentsTable);
    if (!res) return false;
    PQclear(res);
    
    print("数据库表创建成功\n");
    return true;
}

// 执行SQL语句的辅助函数
pg_result* PostgreSQLAdapter::executeQuery(const string& sql) {
    if (!_isConnected || !_conn) {
        print("数据库未连接\n");
        return nullptr;
    }
    
    pg_result* res = PQexec(_conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
        print("SQL执行失败: {}\n", PQerrorMessage(_conn));
        PQclear(res);
        return nullptr;
    }
    
    return res;
}

// PostgreSQLAdapter的具体实现
void PostgreSQLAdapter::saveStudent(Student* student) {
    string info = student->info();
    size_t end = info.find("   ");
    string id = info.substr(0, end);
    size_t start = end + 3;
    end = info.find("\t", start);
    string name = info.substr(start, end - start);
    string password = "1234";
    
    string sql = format("INSERT INTO students (id, name, password) VALUES ('{}', '{}', '{}') "
                        "ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name, password = EXCLUDED.password",
                        id, name, password);
    
    pg_result* res = executeQuery(sql);
    if (res) {
        PQclear(res);
    }
}

vector<Student*> PostgreSQLAdapter::loadStudents() {
    vector<Student*> students;
    const char* sql = "SELECT id, name, password FROM students";
    
    pg_result* res = executeQuery(sql);
    if (!res) return students;
    
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        string id = PQgetvalue(res, i, 0);
        string name = PQgetvalue(res, i, 1);
        string password = PQgetvalue(res, i, 2);
        
        students.push_back(new Student(id, name, password));
    }
    
    PQclear(res);
    return students;
}

bool PostgreSQLAdapter::removeStudent(const string& id) {
    string sql = format("DELETE FROM students WHERE id = '{}'", id);
    pg_result* res = executeQuery(sql);
    if (!res) return false;
    
    bool affected = (PQcmdTuples(res)[0] != '0');
    PQclear(res);
    return affected;
}

void PostgreSQLAdapter::saveTeacher(Teacher* teacher) {
    string info = teacher->info();
    size_t end = info.find("   ");
    string id = info.substr(0, end);
    string name = info.substr(end + 3);
    string password = "123";
    
    string sql = format("INSERT INTO teachers (id, name, password) VALUES ('{}', '{}', '{}') "
                        "ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name, password = EXCLUDED.password",
                        id, name, password);
    
    pg_result* res = executeQuery(sql);
    if (res) {
        PQclear(res);
    }
}

vector<Teacher*> PostgreSQLAdapter::loadTeachers() {
    vector<Teacher*> teachers;
    const char* sql = "SELECT id, name, password FROM teachers";
    
    pg_result* res = executeQuery(sql);
    if (!res) return teachers;
    
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        string id = PQgetvalue(res, i, 0);
        string name = PQgetvalue(res, i, 1);
        string password = PQgetvalue(res, i, 2);
        
        teachers.push_back(new Teacher(id, name, password));
    }
    
    PQclear(res);
    return teachers;
}

bool PostgreSQLAdapter::removeTeacher(const string& id) {
    string sql = format("DELETE FROM teachers WHERE id = '{}'", id);
    pg_result* res = executeQuery(sql);
    if (!res) return false;
    
    bool affected = (PQcmdTuples(res)[0] != '0');
    PQclear(res);
    return affected;
}

void PostgreSQLAdapter::saveCourse(Course* course) {
    string info = course->info();
    size_t end = info.find("  ");
    string id = info.substr(0, end);
    string name = info.substr(end + 2);
    int credit = 3;
    
    string sql = format("INSERT INTO courses (id, name, credit) VALUES ('{}', '{}', {}) "
                        "ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name, credit = EXCLUDED.credit",
                        id, name, credit);
    
    pg_result* res = executeQuery(sql);
    if (res) {
        PQclear(res);
    }
}

vector<Course*> PostgreSQLAdapter::loadCourses() {
    vector<Course*> courses;
    const char* sql = "SELECT id, name FROM courses";
    
    pg_result* res = executeQuery(sql);
    if (!res) return courses;
    
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        string id = PQgetvalue(res, i, 0);
        string name = PQgetvalue(res, i, 1);
        
        courses.push_back(new Course(id, name));
    }
    
    PQclear(res);
    return courses;
}

bool PostgreSQLAdapter::removeCourse(const string& id) {
    string sql = format("DELETE FROM courses WHERE id = '{}'", id);
    pg_result* res = executeQuery(sql);
    if (!res) return false;
    
    bool affected = (PQcmdTuples(res)[0] != '0');
    PQclear(res);
    return affected;
}

void PostgreSQLAdapter::saveGrade(Grade* grade) {
    string info = grade->info();
    size_t start = info.find("学生: ") + 4;
    size_t end = info.find(" | 课程: ", start);
    string studentId = info.substr(start, end - start);
    
    start = info.find("课程: ", end) + 4;
    end = info.find(" | 教师: ", start);
    string courseId = info.substr(start, end - start);
    
    start = info.find("教师: ", end) + 4;
    end = info.find(" | 成绩: ", start);
    string teacherId = info.substr(start, end - start);
    
    start = info.find("成绩: ", end) + 4;
    end = info.find(" | 评语: ", start);
    string scoreStr = info.substr(start, end - start);
    double score = std::stod(scoreStr);
    
    start = info.find("评语: ", end) + 4;
    string comment = info.substr(start);
    
    string sql = format("INSERT INTO grades (student_id, course_id, teacher_id, score, comment) "
                        "VALUES ('{}', '{}', '{}', {}, '{}') "
                        "ON CONFLICT (student_id, course_id) DO UPDATE SET "
                        "teacher_id = EXCLUDED.teacher_id, score = EXCLUDED.score, comment = EXCLUDED.comment",
                        studentId, courseId, teacherId, score, comment);
    
    pg_result* res = executeQuery(sql);
    if (res) {
        PQclear(res);
    }
}

vector<Grade*> PostgreSQLAdapter::loadGrades() {
    vector<Grade*> grades;
    const char* sql = "SELECT student_id, course_id, teacher_id, score, comment FROM grades";
    
    pg_result* res = executeQuery(sql);
    if (!res) return grades;
    
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        string studentId = PQgetvalue(res, i, 0);
        string courseId = PQgetvalue(res, i, 1);
        string teacherId = PQgetvalue(res, i, 2);
        double score = std::stod(PQgetvalue(res, i, 3));
        string comment = PQgetvalue(res, i, 4);
        
        grades.push_back(new Grade(studentId, courseId, teacherId, score, comment));
    }
    
    PQclear(res);
    return grades;
}

bool PostgreSQLAdapter::removeGrade(const string& studentId, const string& courseId) {
    string sql = format("DELETE FROM grades WHERE student_id = '{}' AND course_id = '{}'", 
                        studentId, courseId);
    pg_result* res = executeQuery(sql);
    if (!res) return false;
    
    bool affected = (PQcmdTuples(res)[0] != '0');
    PQclear(res);
    return affected;
}

void PostgreSQLAdapter::saveTeachingTask(TeachingTask* task) {
    string info = task->info();
    size_t start = info.find("Teacher: ") + 9;
    size_t end = info.find(" | Course: ", start);
    string teacherId = info.substr(start, end - start);
    
    start = info.find("Course: ", end) + 8;
    end = info.find(" | Semester: ", start);
    string courseId = info.substr(start, end - start);
    
    start = info.find("Semester: ", end) + 10;
    end = info.find(" | Time: ", start);
    string semester = info.substr(start, end - start);
    
    start = info.find("Time: ", end) + 6;
    end = info.find(" | Classroom: ", start);
    string timeSlot = info.substr(start, end - start);
    
    start = info.find("Classroom: ", end) + 11;
    string classroom = info.substr(start);
    
    string sql = format("INSERT INTO teaching_tasks (teacher_id, course_id, semester, time_slot, classroom) "
                        "VALUES ('{}', '{}', '{}', '{}', '{}') "
                        "ON CONFLICT (teacher_id, course_id, semester) DO UPDATE SET "
                        "time_slot = EXCLUDED.time_slot, classroom = EXCLUDED.classroom",
                        teacherId, courseId, semester, timeSlot, classroom);
    
    pg_result* res = executeQuery(sql);
    if (res) {
        PQclear(res);
    }
}

vector<TeachingTask*> PostgreSQLAdapter::loadTeachingTasks() {
    vector<TeachingTask*> tasks;
    const char* sql = "SELECT teacher_id, course_id, semester, time_slot, classroom FROM teaching_tasks";
    
    pg_result* res = executeQuery(sql);
    if (!res) return tasks;
    
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        string teacherId = PQgetvalue(res, i, 0);
        string courseId = PQgetvalue(res, i, 1);
        string semester = PQgetvalue(res, i, 2);
        string timeSlot = PQgetvalue(res, i, 3);
        string classroom = PQgetvalue(res, i, 4);
        
        tasks.push_back(new TeachingTask(teacherId, courseId, semester, timeSlot, classroom));
    }
    
    PQclear(res);
    return tasks;
}

bool PostgreSQLAdapter::removeTeachingTask(const string& courseId, const string& teacherId) {
    string sql = format("DELETE FROM teaching_tasks WHERE course_id = '{}' AND teacher_id = '{}'", 
                        courseId, teacherId);
    pg_result* res = executeQuery(sql);
    if (!res) return false;
    
    bool affected = (PQcmdTuples(res)[0] != '0');
    PQclear(res);
    return affected;
}

bool PostgreSQLAdapter::saveEnrollment(const string& studentId, const string& courseId) {
    string sql = format("INSERT INTO student_enrollments (student_id, course_id) VALUES ('{}', '{}') "
                        "ON CONFLICT (student_id, course_id) DO NOTHING",
                        studentId, courseId);
    
    pg_result* res = executeQuery(sql);
    if (res) {
        PQclear(res);
        return true;
    }
    return false;
}

bool PostgreSQLAdapter::removeEnrollment(const string& studentId, const string& courseId) {
    string sql = format("DELETE FROM student_enrollments WHERE student_id = '{}' AND course_id = '{}'", 
                        studentId, courseId);
    pg_result* res = executeQuery(sql);
    if (!res) return false;
    
    bool affected = (PQcmdTuples(res)[0] != '0');
    PQclear(res);
    return affected;
}

vector<Enrollment*> PostgreSQLAdapter::loadEnrollments() {
    vector<Enrollment*> enrollments;
    const char* sql = "SELECT student_id, course_id FROM student_enrollments";
    
    pg_result* res = executeQuery(sql);
    if (!res) return enrollments;
    
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        string studentId = PQgetvalue(res, i, 0);
        string courseId = PQgetvalue(res, i, 1);
        
        enrollments.push_back(new Enrollment(studentId, courseId));
    }
    
    PQclear(res);
    return enrollments;
}

bool PostgreSQLAdapter::beginTransaction() {
    pg_result* res = executeQuery("BEGIN");
    if (!res) return false;
    PQclear(res);
    return true;
}

bool PostgreSQLAdapter::commit() {
    pg_result* res = executeQuery("COMMIT");
    if (!res) return false;
    PQclear(res);
    return true;
}

bool PostgreSQLAdapter::rollback() {
    pg_result* res = executeQuery("ROLLBACK");
    if (!res) return false;
    PQclear(res);
    return true;
}

// DataManager实现
DataManager::DataManager(unique_ptr<IDataAccess> dataAccess) 
    : _dataAccess(std::move(dataAccess)) {
}

DataManager::~DataManager() {
    // unique_ptr会自动清理资源
}

void DataManager::switchToInMemory() {
    _dataAccess = std::make_unique<InMemoryStorage>();
}

void DataManager::switchToPostgreSQL() {
    auto pgAdapter = std::make_unique<PostgreSQLAdapter>();
    if (pgAdapter->connect()) {
        _dataAccess = std::move(pgAdapter);
        print("已切换到PostgreSQL数据库\n");
    } else {
        print("PostgreSQL连接失败，切换到内存存储\n");
        _dataAccess = std::make_unique<InMemoryStorage>();
    }
}

// 代理所有数据访问操作
void DataManager::saveStudent(Student* student) {
    _dataAccess->saveStudent(student);
}

vector<Student*> DataManager::loadStudents() {
    return _dataAccess->loadStudents();
}

bool DataManager::removeStudent(const string& id) {
    return _dataAccess->removeStudent(id);
}

void DataManager::saveTeacher(Teacher* teacher) {
    _dataAccess->saveTeacher(teacher);
}

vector<Teacher*> DataManager::loadTeachers() {
    return _dataAccess->loadTeachers();
}

bool DataManager::removeTeacher(const string& id) {
    return _dataAccess->removeTeacher(id);
}

void DataManager::saveCourse(Course* course) {
    _dataAccess->saveCourse(course);
}

vector<Course*> DataManager::loadCourses() {
    return _dataAccess->loadCourses();
}

bool DataManager::removeCourse(const string& id) {
    return _dataAccess->removeCourse(id);
}

void DataManager::saveGrade(Grade* grade) {
    _dataAccess->saveGrade(grade);
}

vector<Grade*> DataManager::loadGrades() {
    return _dataAccess->loadGrades();
}

bool DataManager::removeGrade(const string& studentId, const string& courseId) {
    return _dataAccess->removeGrade(studentId, courseId);
}

void DataManager::saveTeachingTask(TeachingTask* task) {
    _dataAccess->saveTeachingTask(task);
}

vector<TeachingTask*> DataManager::loadTeachingTasks() {
    return _dataAccess->loadTeachingTasks();
}

bool DataManager::removeTeachingTask(const string& courseId, const string& teacherId) {
    return _dataAccess->removeTeachingTask(courseId, teacherId);
}

bool DataManager::saveEnrollment(const string& studentId, const string& courseId) {
    return _dataAccess->saveEnrollment(studentId, courseId);
}

bool DataManager::removeEnrollment(const string& studentId, const string& courseId) {
    return _dataAccess->removeEnrollment(studentId, courseId);
}

vector<Enrollment*> DataManager::loadEnrollments() {
    return _dataAccess->loadEnrollments();
}

bool DataManager::beginTransaction() {
    return _dataAccess->beginTransaction();
}

bool DataManager::commit() {
    return _dataAccess->commit();
}

bool DataManager::rollback() {
    return _dataAccess->rollback();
}