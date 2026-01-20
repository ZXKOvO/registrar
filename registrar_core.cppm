// Module
// File: registrar_core.cppm   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-17
// Description: Registrar核心模块 - 定义Registrar类
//
module;
#include <string>
#include <vector>
#include <format>
#include <algorithm>
#include <print>

export module registrar:registrar_core;
export import :student;
export import :course;
export import :teacher;
export import :grade;
export import :teaching_task;
export import :academic_secretary;
export import :data_manager;

using std::string;
using std::vector;
using std::format;
using std::print;
using std::println;

export class Registrar
{
public:
    static Registrar& system();
    void studentEnrollsInCourse(std::string sid, std::string cid);
    void studentDropsCourse(std::string sid, std::string cid);
    void studentSchedule(std::string sid);
    void courseRoster(std::string cid);
    void teacherSchedule(std::string tid);
    void teacherRoster(std::string tid);
    void initialize();
    void initializeDefaultData();
    void saveAllData();
    void addStudent(std::string id, std::string name);
    void removeStudent(std::string id);
    void addTeacher(std::string id, std::string name);
    void removeTeacher(std::string id);
    void addCourse(std::string id, std::string name);
    void removeCourse(std::string id);
    std::string generateEnrollmentReport();
    std::string generateCourseReport();
    std::string generateDetailedCourseReport();
    std::string generateTeacherReport();
    Course* findCourseById(const std::string& id);
    Teacher* findTeacherById(const std::string& id);
    Student* findStudentById(const std::string& id);
    void forEachStudent(auto&& func);
    void forEachTeacher(auto&& func);
    void forEachCourse(auto&& func);
    
    // 为教师设置课程信息（用于从数据库加载）
    void setTeacherCourses();
    
    // 为学生设置课程信息（用于从数据库加载）
    void setStudentCourses();
    
    // 教学任务管理
    void addTeachingTask(TeachingTask* task);
    bool removeTeachingTask(const std::string& teacherId, const std::string& courseId);
    std::vector<TeachingTask*> getTeachingTasks();
    
    // 数据管理
    void setDataManager(DataManager* dataManager);
    DataManager* getDataManager();
    void loadAllData();

private:
    Registrar();
    DataManager* _dataManager;

    std::vector<Course*> _courses;
    std::vector<Student*> _students;
    std::vector<Teacher*> _teachers;
    std::vector<TeachingTask*> _teachingTasks;
};

Registrar &Registrar::system(){
    static Registrar instance;
    return instance;
}

void Registrar::studentEnrollsInCourse(std::string sid, std::string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);
    if(!student || !course) {
        print("错误: 学生或课程不存在 - 学生: {}, 课程: {}\n", sid, cid);
        return;
    }
    
    // 检查是否已经选过该课程
    if (student->hasCourse(cid)) {
        print("错误: 学生 {} 已选修课程 {}\n", sid, cid);
        return;
    }
    
    // 使用事务确保数据一致性
    bool dbSuccess = false;
    if (_dataManager) {
        _dataManager->beginTransaction();
        try {
            dbSuccess = _dataManager->saveEnrollment(sid, cid);
            if (dbSuccess) {
                _dataManager->commit();
                print("选课数据已保存到数据库\n");
            } else {
                _dataManager->rollback();
                print("数据库保存失败\n");
            }
        } catch (const std::exception& e) {
            _dataManager->rollback();
            print("选课失败，数据已回滚: {}\n", e.what());
        }
    }
    
    // 只有数据库操作成功才更新内存状态
    if (dbSuccess || !_dataManager) {
        student->enrollsIn(course);
        print("学生 {} 成功选修课程 {}\n", sid, cid);
    }
}

void Registrar::studentDropsCourse(std::string sid, std::string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);
    if(!student || !course) {
        print("错误: 学生或课程不存在 - 学生: {}, 课程: {}\n", sid, cid);
        return;
    }
    
    // 检查是否选了该课程
    if (!student->hasCourse(cid)) {
        print("错误: 学生 {} 未选修课程 {}\n", sid, cid);
        return;
    }
    
    // 使用事务确保数据一致性
    bool dbSuccess = false;
    if (_dataManager) {
        _dataManager->beginTransaction();
        try {
            dbSuccess = _dataManager->removeEnrollment(sid, cid);
            if (dbSuccess) {
                _dataManager->commit();
                print("退课数据已从数据库删除\n");
            } else {
                _dataManager->rollback();
                print("数据库删除失败\n");
            }
        } catch (const std::exception& e) {
            _dataManager->rollback();
            print("退课失败，数据已回滚: {}\n", e.what());
        }
    }
    
    // 只有数据库操作成功才更新内存状态
    if (dbSuccess || !_dataManager) {
        student->dropCourse(course);
        print("学生 {} 成功退选课程 {}\n", sid, cid);
    }
}

void Registrar::courseRoster(std::string cid){
    auto c = findCourseById(cid);
    print("{}\n",c->roster());
}

void Registrar::studentSchedule(std::string sid){
    auto s = findStudentById(sid);
    if(s){
        print("{}\n",s->schedule());
    }
}

void Registrar::teacherSchedule(std::string tid){
    auto t = findTeacherById(tid);
    if(t){
        print("{}\n",t->schedule());
    }
}

void Registrar::teacherRoster(std::string tid){
    auto t = findTeacherById(tid);
    if(t){
        print("{}\n",t->roster());
    }
}

void Registrar::initialize(){
    // 尝试从数据库加载数据
    if (_dataManager) {
        // 使用loadAllData方法加载数据
        loadAllData();
        
        // 如果数据库中没有数据，则初始化默认数据
        if (_students.empty() && _teachers.empty() && _courses.empty()) {
            print("数据库为空，初始化默认数据...\n");
            initializeDefaultData();
            // 重新加载数据
            loadAllData();
        }
    } else {
        // 如果没有数据管理器，则初始化默认数据
        initializeDefaultData();
    }
}

void Registrar::initializeDefaultData(){
    // Create students
    _students.push_back(new Student("S001","Tomas", "1234"));
    _students.push_back(new Student("S002","Jerry", "2345"));
    _students.push_back(new Student("S003","Baker", "3456"));
    _students.push_back(new Student("S004","Tom", "4567"));
    _students.push_back(new Student("S005","Musk", "5678"));
        
    // Create teachers
    _teachers.push_back(new Teacher("T001","Smith", "123"));
    _teachers.push_back(new Teacher("T002","Johnson", "456"));
    _teachers.push_back(new Teacher("T003","Williams", "789"));
    
    // Create courses
    _courses.push_back(new Course("CS101","C programming"));
    _courses.push_back(new Course("CS201","Data structure"));
    _courses.push_back(new Course("MATH101","Advanced Math"));
    
    // Assign teachers to courses
    _courses[0]->assignTeacher(findTeacherById("T001"));
    _courses[1]->assignTeacher(findTeacherById("T002"));
    _courses[2]->assignTeacher(findTeacherById("T003"));
    
    // Update teacher schedules
    auto t1 = findTeacherById("T001");
    auto t2 = findTeacherById("T002");
    auto t3 = findTeacherById("T003");
    
    if (t1) t1->assignToCourse(_courses[0]);
    if (t2) t2->assignToCourse(_courses[1]);
    if (t3) t3->assignToCourse(_courses[2]);
    
    // 创建教学任务并保存到数据库
    if (_dataManager) {
        _dataManager->saveTeachingTask(new TeachingTask("T001", "CS101", "2024-Spring", "Mon 8:00-10:00", "A101"));
        _dataManager->saveTeachingTask(new TeachingTask("T002", "CS201", "2024-Spring", "Tue 10:00-12:00", "B202"));
        _dataManager->saveTeachingTask(new TeachingTask("T003", "MATH101", "2024-Spring", "Wed 14:00-16:00", "C303"));
        
        // 创建默认教学秘书
        _dataManager->saveAcademicSecretary(new AcademicSecretary("A001", "张秘书", "123"));
        // _dataManager->saveAcademicSecretary(new AcademicSecretary("secretary2", "李秘书", "123"));
    }
    
    print("course list:\n cid  course\n");
    for(auto &c:_courses){
            print("{}",c->info());
        }
        println();
}

Registrar::Registrar(){}

Student *Registrar::findStudentById(const std::string &id){
    for(auto& student :_students){
        if(student->hasId(id))
        return student;
    }
    return nullptr;
}

Course *Registrar::findCourseById(const std::string&id){
    for(auto& course :_courses){
        if(course->hasId(id))
        return course;
    }
    return nullptr;
}

Teacher *Registrar::findTeacherById(const std::string&id){
    for(auto& teacher :_teachers){
        if(teacher->hasId(id))
        return teacher;
    }
    return nullptr;
}

void Registrar::forEachStudent(auto&& func){
    for(auto& student : _students){
        func(student);
    }
}

void Registrar::forEachTeacher(auto&& func){
    for(auto& teacher : _teachers){
        func(teacher);
    }
}

void Registrar::forEachCourse(auto&& func){
    for(auto& course : _courses){
        func(course);
    }
}

void Registrar::addStudent(std::string id, std::string name) {
    // 检查是否已存在
    for (auto student : _students) {
        if (student->hasId(id)) {
            print("学生 {} 已存在\n", id);
            return;
        }
    }
    
    auto newStudent = new Student(id, name);
    
    // 先保存到数据库
    bool dbSuccess = false;
    if (_dataManager) {
        _dataManager->beginTransaction();
        try {
            _dataManager->saveStudent(newStudent);
            _dataManager->commit();
            dbSuccess = true;
            print("学生信息已保存到数据库\n");
        } catch (const std::exception& e) {
            _dataManager->rollback();
            print("学生信息保存失败: {}\n", e.what());
            delete newStudent;
            return;
        }
    }
    
    // 只有数据库操作成功才添加到内存
    if (dbSuccess || !_dataManager) {
        _students.push_back(newStudent);
        print("成功添加学生: {} ({})\n", name, id);
    }
}

void Registrar::removeStudent(std::string id) {
    auto it = std::find_if(_students.begin(), _students.end(),
        [&id](Student* s) { return s->hasId(id); });
    
    if (it == _students.end()) {
        print("学生 {} 不存在\n", id);
        return;
    }
    
    // 先从数据库删除
    bool dbSuccess = false;
    if (_dataManager) {
        _dataManager->beginTransaction();
        try {
            dbSuccess = _dataManager->removeStudent(id);
            if (dbSuccess) {
                _dataManager->commit();
                print("学生信息已从数据库删除\n");
            } else {
                _dataManager->rollback();
                print("数据库删除失败\n");
            }
        } catch (const std::exception& e) {
            _dataManager->rollback();
            print("学生信息删除失败: {}\n", e.what());
        }
    }
    
    // 只有数据库操作成功才从内存删除
    if (dbSuccess || !_dataManager) {
        delete *it;
        _students.erase(it);
        print("成功删除学生: {}\n", id);
    }
}

void Registrar::addTeacher(std::string id, std::string name) {
    // 检查是否已存在
    for (auto teacher : _teachers) {
        if (teacher->hasId(id)) {
            print("教师 {} 已存在\n", id);
            return;
        }
    }
    
    auto newTeacher = new Teacher(id, name);
    
    // 先保存到数据库
    bool dbSuccess = false;
    if (_dataManager) {
        _dataManager->beginTransaction();
        try {
            _dataManager->saveTeacher(newTeacher);
            _dataManager->commit();
            dbSuccess = true;
            print("教师信息已保存到数据库\n");
        } catch (const std::exception& e) {
            _dataManager->rollback();
            print("教师信息保存失败: {}\n", e.what());
            delete newTeacher;
            return;
        }
    }
    
    // 只有数据库操作成功才添加到内存
    if (dbSuccess || !_dataManager) {
        _teachers.push_back(newTeacher);
        print("成功添加教师: {} ({})\n", name, id);
    }
}

void Registrar::removeTeacher(std::string id) {
    auto it = std::find_if(_teachers.begin(), _teachers.end(),
        [&id](Teacher* t) { return t->hasId(id); });
    
    if (it == _teachers.end()) {
        print("教师 {} 不存在\n", id);
        return;
    }
    
    // 先从数据库删除
    bool dbSuccess = false;
    if (_dataManager) {
        _dataManager->beginTransaction();
        try {
            dbSuccess = _dataManager->removeTeacher(id);
            if (dbSuccess) {
                _dataManager->commit();
                print("教师信息已从数据库删除\n");
            } else {
                _dataManager->rollback();
                print("数据库删除失败\n");
            }
        } catch (const std::exception& e) {
            _dataManager->rollback();
            print("教师信息删除失败: {}\n", e.what());
        }
    }
    
    // 只有数据库操作成功才从内存删除
    if (dbSuccess || !_dataManager) {
        delete *it;
        _teachers.erase(it);
        print("成功删除教师: {}\n", id);
    }
}

void Registrar::addCourse(std::string id, std::string name) {
    // 检查是否已存在
    for (auto course : _courses) {
        if (course->hasId(id)) {
            print("课程 {} 已存在\n", id);
            return;
        }
    }
    
    auto newCourse = new Course(id, name);
    
    // 先保存到数据库
    bool dbSuccess = false;
    if (_dataManager) {
        _dataManager->beginTransaction();
        try {
            _dataManager->saveCourse(newCourse);
            _dataManager->commit();
            dbSuccess = true;
            print("课程信息已保存到数据库\n");
        } catch (const std::exception& e) {
            _dataManager->rollback();
            print("课程信息保存失败: {}\n", e.what());
            delete newCourse;
            return;
        }
    }
    
    // 只有数据库操作成功才添加到内存
    if (dbSuccess || !_dataManager) {
        _courses.push_back(newCourse);
        print("成功添加课程: {} ({})\n", name, id);
    }
}

void Registrar::removeCourse(std::string id) {
    auto it = std::find_if(_courses.begin(), _courses.end(),
        [&id](Course* c) { return c->hasId(id); });
    
    if (it == _courses.end()) {
        print("课程 {} 不存在\n", id);
        return;
    }
    
    // 先从数据库删除
    bool dbSuccess = false;
    if (_dataManager) {
        _dataManager->beginTransaction();
        try {
            dbSuccess = _dataManager->removeCourse(id);
            if (dbSuccess) {
                _dataManager->commit();
                print("课程信息已从数据库删除\n");
            } else {
                _dataManager->rollback();
                print("数据库删除失败\n");
            }
        } catch (const std::exception& e) {
            _dataManager->rollback();
            print("课程信息删除失败: {}\n", e.what());
        }
    }
    
    // 只有数据库操作成功才从内存删除
    if (dbSuccess || !_dataManager) {
        delete *it;
        _courses.erase(it);
        print("成功删除课程: {}\n", id);
    }
}

std::string Registrar::generateEnrollmentReport(){
    std::string report = "=== 选课统计报告 ===\n";
    report += std::format("总学生数: {}\n", _students.size());
    report += std::format("总课程数: {}\n", _courses.size());

    for(auto& course : _courses){
        int count;
        bool full;
        course->displayEnrollmentInfo(count, full);
        report += std::format("课程 {} - 选课人数: {}/{}\n",
            course->info().substr(0, course->info().find_last_of("\n")),
            count, 80);
    }
    return report;
}

std::string Registrar::generateCourseReport(){
    std::string report = "=== 课程详细报告 ===\n";
    for(auto& course : _courses){
        int count;
        bool full;
        course->displayEnrollmentInfo(count, full);
        report += course->info();
        report += std::format("选课人数: {}/{}\n", count, 80);
        report += "选课学生名单:\n";
        report += course->roster();
        report += "\n";
    }
    return report;
}

std::string Registrar::generateDetailedCourseReport(){
    std::string report = "=== 课程详细报告（含学生信息）===\n";
    for(auto& course : _courses){
        int count;
        bool full;
        course->displayEnrollmentInfo(count, full);
        report += course->info();
        report += std::format("选课人数: {}/{}\n", count, 80);
        report += "选课学生名单:\n";
        
        // 手动构建学生名单，包含ID和姓名
        report += "  学生ID\t姓名\n";
        report += "  ----------------\n";
        
        // 遍历所有学生，找到选修该课程的学生
        for (auto student : _students) {
            if (student->hasCourse(course->identifier())) {
                report += format("  {}\t{}\n", student->getId(), student->getName());
            }
        }
        
        report += "\n";
    }
    return report;
}

std::string Registrar::generateTeacherReport(){
    std::string report = "=== 教师工作量报告 ===\n";
    for(auto& teacher : _teachers){
        int count;
        teacher->displayCourseInfo(count);
        report += teacher->info();
        report += std::format("授课数量: {}\n", count);
        report += "授课安排:\n";
        report += teacher->schedule();
        report += "\n";
    }
    return report;
}

void Registrar::setDataManager(DataManager* dataManager) {
    _dataManager = dataManager;
}

DataManager* Registrar::getDataManager() {
    return _dataManager;
}

void Registrar::saveAllData() {
    if (!_dataManager) return;
    
    // 保存所有学生
    for (auto student : _students) {
        _dataManager->saveStudent(student);
    }
    
    // 保存所有教师
    for (auto teacher : _teachers) {
        _dataManager->saveTeacher(teacher);
    }
    
    // 保存所有课程
    for (auto course : _courses) {
        _dataManager->saveCourse(course);
    }
    
    // 保存所有教学任务
    for (auto task : _teachingTasks) {
        _dataManager->saveTeachingTask(task);
    }
}

void Registrar::loadAllData() {
    if (!_dataManager) return;
    
    print("正在从数据库加载数据...\n");
    
    // 加载所有学生
    auto students = _dataManager->loadStudents();
    _students.insert(_students.end(), students.begin(), students.end());
    
    // 加载所有教师
    auto teachers = _dataManager->loadTeachers();
    _teachers.insert(_teachers.end(), teachers.begin(), teachers.end());
    
    // 加载所有课程
    auto courses = _dataManager->loadCourses();
    _courses.insert(_courses.end(), courses.begin(), courses.end());
    
    // 加载教学任务
    auto tasks = _dataManager->loadTeachingTasks();
    _teachingTasks.insert(_teachingTasks.end(), tasks.begin(), tasks.end());
    
    // 加载教学秘书
    auto academicSecretaries = _dataManager->loadAcademicSecretaries();
    // 注意：教学秘书不需要存储在Registrar中，因为它们主要用于认证
    
    // 如果教学任务为空，则创建默认教学任务
    if (_teachingTasks.empty() && !_students.empty() && !_teachers.empty() && !_courses.empty()) {
        print("数据库中没有教学任务，初始化默认教学任务...\n");
        _dataManager->saveTeachingTask(new TeachingTask("T001", "CS101", "2024-Spring", "Mon 8:00-10:00", "A101"));
        _dataManager->saveTeachingTask(new TeachingTask("T002", "CS201", "2024-Spring", "Tue 10:00-12:00", "B202"));
        _dataManager->saveTeachingTask(new TeachingTask("T003", "MATH101", "2024-Spring", "Wed 14:00-16:00", "C303"));
        
        // 重新加载教学任务
        tasks = _dataManager->loadTeachingTasks();
        _teachingTasks.insert(_teachingTasks.end(), tasks.begin(), tasks.end());
    }
    
    // 检查是否需要初始化教学秘书
    if (academicSecretaries.empty()) {
        print("数据库中没有教学秘书，初始化默认教学秘书...\n");
        _dataManager->saveAcademicSecretary(new AcademicSecretary("A001", "王秘书", "123"));
    }
    
    // 基于教学任务设置教师的课程信息和课程的教师信息
    print("建立教师与课程关联...\n");
    for (auto task : _teachingTasks) {
        auto teacher = findTeacherById(task->assignedTeacherId());
        auto course = findCourseById(task->assignedCourseId());
        
        if (teacher && course) {
            // 建立双向引用
            teacher->addCourseInternal(course);
            course->assignTeacherInternal(teacher);
            print("  建立关联: 教师 {} -> 课程 {} (学期: {})\n", 
                  task->assignedTeacherId(), task->assignedCourseId(), task->scheduledSemester());
        } else {
            print("  警告: 教学任务关联失败 - 教师: {}, 课程: {}\n", 
                  task->assignedTeacherId(), task->assignedCourseId());
        }
    }
    
    // 设置学生的课程信息
    setStudentCourses();
    
    // 加载学生选课数据并更新学生和课程的双向引用
    auto enrollments = _dataManager->loadEnrollments();
    print("加载选课记录: {} 条\n", enrollments.size());
    for (auto enrollment : enrollments) {
        auto student = findStudentById(enrollment->enrolledStudentId());
        auto course = findCourseById(enrollment->enrolledCourseId());
        if (student && course) {
            // 使用内部方法建立双向引用，避免触发打印信息
            student->addCourseInternal(course);
            course->addStudentInternal(student);
            print("  建立关联: 学生 {} -> 课程 {}\n", 
                  enrollment->enrolledStudentId(), enrollment->enrolledCourseId());
        } else {
            print("  警告: 选课记录关联失败 - 学生: {}, 课程: {}\n", 
                  enrollment->enrolledStudentId(), enrollment->enrolledCourseId());
        }
    }
    
    // 加载成绩数据并更新教师的成绩列表
    auto grades = _dataManager->loadGrades();
    for (auto grade : grades) {
        auto teacher = findTeacherById(grade->getTeacherId());
        if (teacher) {
            teacher->addGradeInternal(grade);
        }
    }
    
    print("数据加载完成。共加载 {} 个学生, {} 个教师, {} 个课程, {} 个教学任务\n", 
          _students.size(), _teachers.size(), _courses.size(), _teachingTasks.size());
}

// ========== 教学任务管理实现 ==========

void Registrar::addTeachingTask(TeachingTask* task) {
    if (!task) {
        print("错误: 教学任务为空\n");
        return;
    }
    
    // 验证教师和课程是否存在
    auto teacher = findTeacherById(task->assignedTeacherId());
    auto course = findCourseById(task->assignedCourseId());
    
    if (!teacher) {
        print("错误: 教师 {} 不存在\n", task->assignedTeacherId());
        return;
    }
    
    if (!course) {
        print("错误: 课程 {} 不存在\n", task->assignedCourseId());
        return;
    }
    
    // 检查是否已存在相同的教学任务
    for (auto existingTask : _teachingTasks) {
        if (existingTask->matches(task->assignedTeacherId(), task->assignedCourseId())) {
            print("错误: 教师 {} 课程 {} 的教学任务已存在\n", 
                  task->assignedTeacherId(), task->assignedCourseId());
            return;
        }
    }
    
    // 保存到数据库
    bool dbSuccess = false;
    if (_dataManager) {
        _dataManager->beginTransaction();
        try {
            _dataManager->saveTeachingTask(task);
            _dataManager->commit();
            dbSuccess = true;
            print("教学任务已保存到数据库\n");
        } catch (const std::exception& e) {
            _dataManager->rollback();
            print("教学任务保存失败: {}\n", e.what());
        }
    }
    
    // 只有数据库操作成功才添加到内存
    if (dbSuccess || !_dataManager) {
        _teachingTasks.push_back(task);
        
        // 建立教师和课程之间的双向引用
        teacher->addCourseInternal(course);
        course->assignTeacherInternal(teacher);
        
        print("成功添加教学任务: 教师 {} -> 课程 {} (学期: {})\n", 
              task->assignedTeacherId(), task->assignedCourseId(), task->scheduledSemester());
    }
}

bool Registrar::removeTeachingTask(const std::string& teacherId, const std::string& courseId) {
    auto it = std::find_if(_teachingTasks.begin(), _teachingTasks.end(),
        [&teacherId, &courseId](TeachingTask* task) {
            return task->matches(teacherId, courseId);
        });
    
    if (it != _teachingTasks.end()) {
        // 先从数据库删除
        bool dbSuccess = false;
        if (_dataManager) {
            _dataManager->beginTransaction();
            try {
                dbSuccess = _dataManager->removeTeachingTask(courseId, teacherId);
                if (dbSuccess) {
                    _dataManager->commit();
                    print("教学任务已从数据库删除\n");
                } else {
                    _dataManager->rollback();
                    print("数据库删除失败\n");
                }
            } catch (const std::exception& e) {
                _dataManager->rollback();
                print("教学任务删除失败: {}\n", e.what());
                return false;
            }
        }
        
        // 只有数据库操作成功才从内存删除
        if (dbSuccess || !_dataManager) {
            delete *it;
            _teachingTasks.erase(it);
            print("成功删除教学任务: 教师 {} -> 课程 {}\n", teacherId, courseId);
            return true;
        }
    } else {
        print("错误: 未找到教师 {} 课程 {} 的教学任务\n", teacherId, courseId);
    }
    return false;
}

std::vector<TeachingTask*> Registrar::getTeachingTasks() {
    return _teachingTasks;
}

void Registrar::setTeacherCourses() {
    if (!_dataManager) return;
    
    // 为每个教师设置其教授的课程
    for (auto teacher : _teachers) {
        vector<Course*> teacherCourses;
        
        // 遍历所有教学任务，找到该教师的课程
        for (auto task : _teachingTasks) {
            if (task->matchesTeacher(teacher->getId())) {
                auto course = findCourseById(task->assignedCourseId());
                if (course) {
                    teacherCourses.push_back(course);
                }
            }
        }
        
        // 设置教师的课程列表
        teacher->setCourses(teacherCourses);
    }
}

// 设置学生的课程信息
void Registrar::setStudentCourses() {
    if (!_dataManager) return;
    
    // 注意：这个方法现在不需要做任何事情，因为选课记录的加载
    // 已经在loadAllData()的最后部分处理了，那里会正确建立双向引用
    
    // 保留这个方法是为了保持接口兼容性，但实际工作已经移到loadAllData()
}
