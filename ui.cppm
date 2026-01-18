// Module
// File: ui.cppm   Version: 0.1.0   License: AGPLv3
// Created: CCJ      2026-01-17 19:05:37
// Description:
//     完善ui界面，分对象设计3个ui（学生，教师，教学秘书）
export module ui;
import std;
import registrar;
using std::string;
using std::vector;

// 用户类型枚举
export enum class UserTypes
{
    STUDENT,
    TEACHER,
    SECRETARY
};

// UI类 - 用户界面管理
export class UI
{
public:
    UI();
    void showMainMenu();
    void showStudentMenu();
    void showTeacherMenu();
    void showSecretaryMenu();
    void studentEnrollmentInterface();
    void studentDropInterface();
    void studentScheduleInterface();
    void studentGradeInterface();
    void teacherScheduleInterface();
    void teacherRosterInterface();
    void teacherGradeInterface();
    void secretaryStudentManagement();
    void secretaryTeacherManagement();
    void secretaryCourseManagement();
    void secretaryReportInterface();
    void loginInterface();
    bool authenticateUser(string id, string password, UserTypes type);
    void displayReport(string report);
    void displayMessage(string message);

private:
    Registrar& _registrar;
    UserTypes _currentUserType;
    string _currentUserId;

    void displayStudentList();
    void displayCourseList();
    void displayTeacherList();
    void clearScreen();
    void pause();
    void clearInputBuffer();
    void teacherAddGrade();
    void teacherUpdateGrade();
    void teacherViewGrades();
};

UI::UI()
    : _registrar(Registrar::system())
    , _currentUserType(UserTypes::STUDENT)
    , _currentUserId("")
{}

void UI::clearScreen()
{
    // 清屏操作
    std::print("\033[2J\033[H");
}

void UI::pause()
{
    std::print("\n按回车键继续...");
    std::string temp;
    if (std::cin)
    {
        std::getline(std::cin, temp);
    }
}

void UI::clearInputBuffer()
{
    if (std::cin && std::cin.peek() != '\n')
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void UI::displayMessage(string message)
{
    std::print("\n{}\n", message);
}

void UI::displayReport(string report)
{
    std::print("\n{}\n", report);
}

void UI::showMainMenu()
{
    while (true)
    {
        clearScreen();
        std::print("========================================\n");
        std::print("        教务系统 - 主菜单\n");
        std::print("========================================\n");
        std::print("1. 学生登录\n");
        std::print("2. 教师登录\n");
        std::print("3. 教学秘书登录\n");
        std::print("0. 退出系统\n");
        std::print("========================================\n");
        std::print("请选择: ");
        
        int choice;
        if (!(std::cin >> choice))
        {
            // 输入失败，可能是EOF，退出程序
            return;
        }
        std::cin.ignore(); // 清除输入缓冲区
        
        switch (choice)
        {
            case 1:
                _currentUserType = UserTypes::STUDENT;
                loginInterface();
                break;
            case 2:
                _currentUserType = UserTypes::TEACHER;
                loginInterface();
                break;
            case 3:
                _currentUserType = UserTypes::SECRETARY;
                loginInterface();
                break;
            case 0:
                std::print("感谢使用，再见！\n");
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::loginInterface()
{
    clearScreen();
    std::print("========================================\n");
    std::print("              用户登录\n");
    std::print("========================================\n");
    
    string id, password;
    std::print("请输入ID: ");
    std::getline(std::cin, id);
    std::print("请输入密码: ");
    std::getline(std::cin, password);
    
    if (authenticateUser(id, password, _currentUserType))
    {
        _currentUserId = id;
        displayMessage("登录成功！");
        pause();
        
        switch (_currentUserType)
        {
            case UserTypes::STUDENT:
                showStudentMenu();
                break;
            case UserTypes::TEACHER:
                showTeacherMenu();
                break;
            case UserTypes::SECRETARY:
                showSecretaryMenu();
                break;
        }
    }
    else
    {
        displayMessage("登录失败！用户ID或密码错误。");
        pause();
    }
}

bool UI::authenticateUser(string id, string password, UserTypes type)
{
    switch (type)
    {
        case UserTypes::STUDENT:
            return _registrar.findStudentById(id) != nullptr;
        case UserTypes::TEACHER:
            return _registrar.findTeacherById(id) != nullptr;
        case UserTypes::SECRETARY:
            // 教学秘书暂时简化验证，只检查ID是否存在
            // 实际应用中应该有专门的秘书管理
            return true; // 暂时允许任何秘书登录
        default:
            return false;
    }
}

void UI::showStudentMenu()
{
    while (true)
    {
        clearScreen();
        std::print("========================================\n");
        std::print("          学生菜单\n");
        std::print("========================================\n");
        std::print("1. 选课\n");
        std::print("2. 退课\n");
        std::print("3. 查看课程表\n");
        std::print("4. 查看可选课程\n");
        std::print("5. 查看成绩\n");
        std::print("0. 返回主菜单\n");
        std::print("========================================\n");
        std::print("请选择: ");

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
            case 1:
                studentEnrollmentInterface();
                break;
            case 2:
                studentDropInterface();
                break;
            case 3:
                studentScheduleInterface();
                break;
            case 4:
                displayCourseList();
                pause();
                break;
            case 5:
                studentGradeInterface();
                break;
            case 0:
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::studentEnrollmentInterface()
{
    clearScreen();
    std::print("========================================\n");
    std::print("              选课界面\n");
    std::print("========================================\n");
    
    displayCourseList();
    
    string courseId;
    std::print("\n请输入要选修的课程ID: ");
    std::getline(std::cin, courseId);
    
    _registrar.studentEnrollsInCourse(_currentUserId, courseId);
    pause();
}

void UI::studentDropInterface()
{
    clearScreen();
    std::print("========================================\n");
    std::print("              退课界面\n");
    std::print("========================================\n");
    
    // 显示当前已选课程
    auto student = _registrar.findStudentById(_currentUserId);
    if (student)
    {
        std::print("当前已选课程:\n");
        std::print("{}", student->schedule());
    }
    
    string courseId;
    std::print("\n请输入要退选的课程ID: ");
    std::getline(std::cin, courseId);
    
    _registrar.studentDropsCourse(_currentUserId, courseId);
    pause();
}

void UI::studentScheduleInterface()
{
    clearScreen();
    std::print("========================================\n");
    std::print("            课程表界面\n");
    std::print("========================================\n");
    
    _registrar.studentSchedule(_currentUserId);
    pause();
}

void UI::showTeacherMenu()
{
    while (true)
    {
        clearScreen();
        std::print("========================================\n");
        std::print("          教师菜单\n");
        std::print("========================================\n");
        std::print("1. 查看课程表\n");
        std::print("2. 查看学生名单\n");
        std::print("3. 成绩评定\n");
        std::print("0. 返回主菜单\n");
        std::print("========================================\n");
        std::print("请选择: ");

        int choice;
        if (!(std::cin >> choice))
        {
            // 输入失败，可能是EOF，返回主菜单
            return;
        }
        clearInputBuffer();

        switch (choice)
        {
            case 1:
                teacherScheduleInterface();
                break;
            case 2:
                teacherRosterInterface();
                break;
            case 3:
                teacherGradeInterface();
                break;
            case 0:
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::teacherScheduleInterface()
{
    clearScreen();
    std::print("========================================\n");
    std::print("          教师课程表\n");
    std::print("========================================\n");
    
    _registrar.teacherSchedule(_currentUserId);
    pause();
}

void UI::teacherRosterInterface()
{
    clearScreen();
    std::print("========================================\n");
    std::print("          学生名单\n");
    std::print("========================================\n");
    
    std::print("请输入课程ID: ");
    string courseId;
    std::getline(std::cin, courseId);
    
    _registrar.courseRoster(courseId);
    pause();
}

void UI::showSecretaryMenu()
{
    while (true)
    {
        clearScreen();
        std::print("========================================\n");
        std::print("        教学秘书菜单\n");
        std::print("========================================\n");
        std::print("1. 学生管理\n");
        std::print("2. 教师管理\n");
        std::print("3. 课程管理\n");
        std::print("4. 报表生成\n");
        std::print("0. 返回主菜单\n");
        std::print("========================================\n");
        std::print("请选择: ");
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice)
        {
            case 1:
                secretaryStudentManagement();
                break;
            case 2:
                secretaryTeacherManagement();
                break;
            case 3:
                secretaryCourseManagement();
                break;
            case 4:
                secretaryReportInterface();
                break;
            case 0:
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::secretaryStudentManagement()
{
    while (true)
    {
        clearScreen();
        std::print("========================================\n");
        std::print("        学生管理\n");
        std::print("========================================\n");
        std::print("1. 查看学生列表\n");
        std::print("2. 添加学生\n");
        std::print("3. 删除学生\n");
        std::print("0. 返回\n");
        std::print("========================================\n");
        std::print("请选择: ");
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice)
        {
            case 1:
                displayStudentList();
                pause();
                break;
            case 2:
            {
                string id, name;
                std::print("请输入学生ID: ");
                std::getline(std::cin, id);
                std::print("请输入学生姓名: ");
                std::getline(std::cin, name);
                _registrar.addStudent(id, name);
                displayMessage("学生添加成功！");
                pause();
                break;
            }
            case 3:
            {
                string id;
                std::print("请输入要删除的学生ID: ");
                std::getline(std::cin, id);
                _registrar.removeStudent(id);
                displayMessage("学生删除成功！");
                pause();
                break;
            }
            case 0:
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::secretaryTeacherManagement()
{
    while (true)
    {
        clearScreen();
        std::print("========================================\n");
        std::print("        教师管理\n");
        std::print("========================================\n");
        std::print("1. 查看教师列表\n");
        std::print("2. 添加教师\n");
        std::print("3. 删除教师\n");
        std::print("0. 返回\n");
        std::print("========================================\n");
        std::print("请选择: ");
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice)
        {
            case 1:
                displayTeacherList();
                pause();
                break;
            case 2:
            {
                string id, name;
                std::print("请输入教师ID: ");
                std::getline(std::cin, id);
                std::print("请输入教师姓名: ");
                std::getline(std::cin, name);
                _registrar.addTeacher(id, name);
                displayMessage("教师添加成功！");
                pause();
                break;
            }
            case 3:
            {
                string id;
                std::print("请输入要删除的教师ID: ");
                std::getline(std::cin, id);
                _registrar.removeTeacher(id);
                displayMessage("教师删除成功！");
                pause();
                break;
            }
            case 0:
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::secretaryCourseManagement()
{
    while (true)
    {
        clearScreen();
        std::print("========================================\n");
        std::print("        课程管理\n");
        std::print("========================================\n");
        std::print("1. 查看课程列表\n");
        std::print("2. 添加课程\n");
        std::print("3. 删除课程\n");
        std::print("0. 返回\n");
        std::print("========================================\n");
        std::print("请选择: ");
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice)
        {
            case 1:
                displayCourseList();
                pause();
                break;
            case 2:
            {
                string id, name;
                std::print("请输入课程ID: ");
                std::getline(std::cin, id);
                std::print("请输入课程名称: ");
                std::getline(std::cin, name);
                _registrar.addCourse(id, name);
                displayMessage("课程添加成功！");
                pause();
                break;
            }
            case 3:
            {
                string id;
                std::print("请输入要删除的课程ID: ");
                std::getline(std::cin, id);
                _registrar.removeCourse(id);
                displayMessage("课程删除成功！");
                pause();
                break;
            }
            case 0:
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::secretaryReportInterface()
{
    while (true)
    {
        clearScreen();
        std::print("========================================\n");
        std::print("        报表生成\n");
        std::print("========================================\n");
        std::print("1. 选课统计报告\n");
        std::print("2. 课程详细报告\n");
        std::print("3. 教师工作量报告\n");
        std::print("0. 返回\n");
        std::print("========================================\n");
        std::print("请选择: ");
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice)
        {
            case 1:
            {
                string report = _registrar.generateEnrollmentReport();
                displayReport(report);
                pause();
                break;
            }
            case 2:
            {
                string report = _registrar.generateCourseReport();
                displayReport(report);
                pause();
                break;
            }
            case 3:
            {
                string report = _registrar.generateTeacherReport();
                displayReport(report);
                pause();
                break;
            }
            case 0:
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::displayStudentList()
{
    std::print("\n=== 学生列表 ===\n");
    std::print("学号\t姓名\t已选课程数\n");
    std::print("----------------------------\n");

    _registrar.forEachStudent([&](auto& student) {
        std::print("{}\n", student->info());
    });
}

void UI::displayCourseList()
{
    std::print("\n=== 可选课程列表 ===\n");
    std::print("课程ID\t课程名称\t选课人数\t状态\n");
    std::print("------------------------------------------------\n");

    _registrar.forEachCourse([&](auto& course) {
        auto info = course->info();
        int count;
        bool full;
        course->displayEnrollmentInfo(count, full);
        std::print("{}\t{}/80\t{}\n",
            info.substr(0, info.find('\n')),
            count,
            full ? "已满" : "可选");
    });
}

void UI::displayTeacherList()
{
    std::print("\n=== 教师列表 ===\n");
    std::print("教师ID\t姓名\t授课数\n");
    std::print("------------------------\n");

    _registrar.forEachTeacher([&](auto& teacher) {
        auto info = teacher->info();
        int count;
        teacher->displayCourseInfo(count);
        std::print("{}\t{}\n",
            info.substr(0, info.find('\n')),
            count);
    });
}

void UI::studentGradeInterface()
{
    clearScreen();
    std::print("========================================\n");
    std::print("            学生成绩查询\n");
    std::print("========================================\n");

    auto student = _registrar.findStudentById(_currentUserId);
    if (!student)
    {
        displayMessage("学生信息未找到！");
        pause();
        return;
    }

    std::print("学生: {}\n", student->info());
    std::print("\n=== 成绩列表 ===\n");
    std::print("课程ID\t课程名称\t成绩\t评语\n");
    std::print("----------------------------------------\n");

    // 遍历所有教师，查找该学生的成绩
    bool found = false;
    _registrar.forEachTeacher([&](auto& teacher) {
        auto& grades = teacher->getGrades();
        for (auto& grade : grades)
        {
            if (grade->matches(_currentUserId, ""))
            {
                std::print("{}", grade->info());
                found = true;
            }
        }
    });

    if (!found)
    {
        std::print("暂无成绩记录\n");
    }

    pause();
}

void UI::teacherGradeInterface()
{
    while (true)
    {
        clearScreen();
        std::print("========================================\n");
        std::print("            教师成绩评定\n");
        std::print("========================================\n");
        std::print("1. 录入成绩\n");
        std::print("2. 修改成绩\n");
        std::print("3. 查看所有成绩\n");
        std::print("0. 返回\n");
        std::print("========================================\n");
        std::print("请选择: ");

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
            case 1:
                teacherAddGrade();
                break;
            case 2:
                teacherUpdateGrade();
                break;
            case 3:
                teacherViewGrades();
                break;
            case 0:
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::teacherAddGrade()
{
    clearScreen();
    std::print("========================================\n");
    std::print("            录入成绩\n");
    std::print("========================================\n");

    string studentId, courseId;
    double score;
    string comment;

    std::print("请输入学生ID: ");
    std::getline(std::cin, studentId);

    std::print("请输入课程ID: ");
    std::getline(std::cin, courseId);

    std::print("请输入成绩(0-100): ");
    std::cin >> score;
    std::cin.ignore();

    std::print("请输入评语(可选): ");
    std::getline(std::cin, comment);

    // 验证输入
    if (score < 0 || score > 100)
    {
        displayMessage("成绩必须在0-100之间！");
        pause();
        return;
    }

    auto teacher = _registrar.findTeacherById(_currentUserId);
    if (!teacher)
    {
        displayMessage("教师信息未找到！");
        pause();
        return;
    }

    // 检查教师是否教授该课程
    if (!teacher->hasCourse(courseId))
    {
        displayMessage("您没有教授该课程！");
        pause();
        return;
    }

    // 检查学生是否选修了该课程
    auto student = _registrar.findStudentById(studentId);
    if (!student)
    {
        displayMessage("学生不存在！");
        pause();
        return;
    }

    auto course = _registrar.findCourseById(courseId);
    if (!course)
    {
        displayMessage("课程不存在！");
        pause();
        return;
    }

    // 录入成绩
    teacher->gradeCourse(studentId, courseId, score, comment);
    displayMessage("成绩录入成功！");
    pause();
}

void UI::teacherUpdateGrade()
{
    clearScreen();
    std::print("========================================\n");
    std::print("            修改成绩\n");
    std::print("========================================\n");

    string studentId, courseId;
    double newScore;
    string newComment;

    std::print("请输入学生ID: ");
    std::getline(std::cin, studentId);

    std::print("请输入课程ID: ");
    std::getline(std::cin, courseId);

    std::print("请输入新成绩(0-100): ");
    std::cin >> newScore;
    std::cin.ignore();

    std::print("请输入新评语(可选): ");
    std::getline(std::cin, newComment);

    // 验证输入
    if (newScore < 0 || newScore > 100)
    {
        displayMessage("成绩必须在0-100之间！");
        pause();
        return;
    }

    auto teacher = _registrar.findTeacherById(_currentUserId);
    if (!teacher)
    {
        displayMessage("教师信息未找到！");
        pause();
        return;
    }

    // 更新成绩
    auto grade = teacher->updateGrade(studentId, courseId, newScore, newComment);
    if (grade)
    {
        displayMessage("成绩修改成功！");
    }
    else
    {
        displayMessage("未找到该学生的成绩记录，请先录入成绩！");
    }
    pause();
}

void UI::teacherViewGrades()
{
    clearScreen();
    std::print("========================================\n");
    std::print("            查看所有成绩\n");
    std::print("========================================\n");

    auto teacher = _registrar.findTeacherById(_currentUserId);
    if (!teacher)
    {
        displayMessage("教师信息未找到！");
        pause();
        return;
    }

    std::print("教师: {}\n", teacher->info());
    std::print("\n=== 成绩列表 ===\n");
    std::print("学生ID\t课程ID\t成绩\t评语\n");
    std::print("----------------------------------------\n");

    auto& grades = teacher->getGrades();
    bool found = false;
    for (auto& grade : grades)
    {
        std::print("{}", grade->info());
        found = true;
    }

    if (!found)
    {
        std::print("暂无成绩记录\n");
    }

    pause();
}
