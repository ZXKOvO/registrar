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
using std::format;
using std::print;

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
    void loginInterface(UserTypes userType);
    void displayReport(string report);
    void displayMessage(string message);

private:
    Registrar& _registrar;
    StudentController _studentController;
    TeacherController _teacherController;
    SecretaryController _secretaryController;
    AuthenticationController _authController;

    void clearScreen();
    void pause();
    void clearInputBuffer();
};

UI::UI()
    : _registrar(Registrar::system())
    , _studentController(_registrar)
    , _teacherController(_registrar)
    , _secretaryController(_registrar, "A001")
    , _authController(_registrar)
{}

void UI::clearScreen()
{
    // 清屏操作
    print("\033[2J\033[H");
}

void UI::pause()
{
    print("\n按回车键继续...");
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
    print("\n{}\n", message);
}

void UI::displayReport(string report)
{
    print("\n{}\n", report);
}

void UI::showMainMenu()
{
    while (true)
    {
        clearScreen();
        print("========================================\n");
        print("        教务系统 - 主菜单\n");
        print("========================================\n");
        print("1. 学生登录\n");
        print("2. 教师登录\n");
        print("3. 教学秘书登录\n");
        print("0. 退出系统\n");
        print("========================================\n");
        print("请选择: ");
        
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
                loginInterface(UserTypes::STUDENT);
                break;
            case 2:
                loginInterface(UserTypes::TEACHER);
                break;
            case 3:
                loginInterface(UserTypes::SECRETARY);
                break;
            case 0:
                print("感谢使用，再见！\n");
                return;
            default:
                displayMessage("无效的选择，请重新输入！");
                pause();
        }
    }
}

void UI::loginInterface(UserTypes userType)
{
    clearScreen();
    print("========================================\n");
    print("              用户登录\n");
    print("========================================\n");
    
    string id, password;
    print("请输入ID: ");
    std::getline(std::cin, id);
    print("请输入密码: ");
    std::getline(std::cin, password);
    
    if (_authController.login(id, password, userType))
    {
        displayMessage("登录成功！");
        pause();
        
        // 清除输入缓冲区
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (userType)
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

void UI::showStudentMenu()
{
    while (true)
    {
        clearScreen();
        print("========================================\n");
        print("          学生菜单\n");
        print("========================================\n");
        print("1. 选课\n");
        print("2. 退课\n");
        print("3. 查看课程表\n");
        print("4. 查看可选课程\n");
        print("5. 查看成绩\n");
        print("0. 返回主菜单\n");
        print("========================================\n");
        print("请选择: ");

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
                _studentController.displayAvailableCourses();
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
    print("========================================\n");
    print("              选课界面\n");
    print("========================================\n");
    
    _studentController.displayAvailableCourses();
    
    string courseId;
    print("\n请输入要选修的课程ID: ");
    std::getline(std::cin, courseId);
    
    if (_studentController.enrollCourse(_authController.getCurrentUserId(), courseId))
    {
        displayMessage("选课成功！");
    }
    else
    {
        displayMessage("选课失败！");
    }
    pause();
}

void UI::studentDropInterface()
{
    clearScreen();
    print("========================================\n");
    print("              退课界面\n");
    print("========================================\n");
    
    // 显示当前已选课程
    print("当前已选课程:\n");
    _studentController.displaySchedule(_authController.getCurrentUserId());
    
    string courseId;
    print("\n请输入要退选的课程ID: ");
    std::getline(std::cin, courseId);
    
    if (_studentController.dropCourse(_authController.getCurrentUserId(), courseId))
    {
        displayMessage("退课成功！");
    }
    else
    {
        displayMessage("退课失败！");
    }
    pause();
}

void UI::studentScheduleInterface()
{
    clearScreen();
    print("========================================\n");
    print("            课程表界面\n");
    print("========================================\n");
    
    _studentController.displaySchedule(_authController.getCurrentUserId());
    pause();
}

void UI::studentGradeInterface()
{
    clearScreen();
    print("========================================\n");
    print("            学生成绩查询\n");
    print("========================================\n");
    
    _studentController.displayGrades(_authController.getCurrentUserId());
    pause();
}

void UI::showTeacherMenu()
{
    while (true)
    {
        clearScreen();
        print("========================================\n");
        print("          教师菜单\n");
        print("========================================\n");
        print("1. 查看课程表\n");
        print("2. 查看学生名单\n");
        print("3. 成绩评定\n");
        print("0. 返回主菜单\n");
        print("========================================\n");
        print("请选择: ");

        int choice;
        if (!(std::cin >> choice))
        {
            // 输入失败，可能是EOF，返回主菜单
            clearInputBuffer();
            return;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
    print("========================================\n");
    print("          教师课程表\n");
    print("========================================\n");
    
    _teacherController.displaySchedule(_authController.getCurrentUserId());
    pause();
}

void UI::teacherRosterInterface()
{
    clearScreen();
    print("========================================\n");
    print("          学生名单\n");
    print("========================================\n");
    
    // 首先显示该教师教授的所有课程
    print("您教授的课程:\n");
    _registrar.teacherSchedule(_authController.getCurrentUserId());
    
    print("\n请输入要查看学生名单的课程ID: ");
    string courseId;
    std::getline(std::cin, courseId);
    
    // 显示课程学生名单
    _teacherController.displayCourseRoster(_authController.getCurrentUserId(), courseId);
    pause();
}

void UI::showSecretaryMenu()
{
    while (true)
    {
        clearScreen();
        print("========================================\n");
        print("        教学秘书菜单\n");
        print("========================================\n");
        print("1. 学生管理\n");
        print("2. 教师管理\n");
        print("3. 课程管理\n");
        print("4. 报表生成\n");
        print("0. 返回主菜单\n");
        print("========================================\n");
        print("请选择: ");
        
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
        print("========================================\n");
        print("        学生管理\n");
        print("========================================\n");
        print("1. 查看学生列表\n");
        print("2. 添加学生\n");
        print("3. 删除学生\n");
        print("0. 返回\n");
        print("========================================\n");
        print("请选择: ");
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice)
        {
            case 1:
                _secretaryController.displayStudentList();
                pause();
                break;
            case 2:
            {
                string id, name;
                print("请输入学生ID: ");
                std::getline(std::cin, id);
                print("请输入学生姓名: ");
                std::getline(std::cin, name);
                _secretaryController.addStudent(id, name);
                pause();
                break;
            }
            case 3:
            {
                string id;
                print("请输入要删除的学生ID: ");
                std::getline(std::cin, id);
                _secretaryController.removeStudent(id);
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
        print("========================================\n");
        print("        教师管理\n");
        print("========================================\n");
        print("1. 查看教师列表\n");
        print("2. 添加教师\n");
        print("3. 删除教师\n");
        print("0. 返回\n");
        print("========================================\n");
        print("请选择: ");
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice)
        {
            case 1:
                _secretaryController.displayTeacherList();
                pause();
                break;
            case 2:
            {
                string id, name;
                print("请输入教师ID: ");
                std::getline(std::cin, id);
                print("请输入教师姓名: ");
                std::getline(std::cin, name);
                _secretaryController.addTeacher(id, name);
                pause();
                break;
            }
            case 3:
            {
                string id;
                print("请输入要删除的教师ID: ");
                std::getline(std::cin, id);
                _secretaryController.removeTeacher(id);
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
        print("========================================\n");
        print("        课程管理\n");
        print("========================================\n");
        print("1. 查看课程列表\n");
        print("2. 添加课程\n");
        print("3. 删除课程\n");
        print("0. 返回\n");
        print("========================================\n");
        print("请选择: ");
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice)
        {
            case 1:
                _secretaryController.displayCourseList();
                pause();
                break;
            case 2:
            {
                string id, name;
                print("请输入课程ID: ");
                std::getline(std::cin, id);
                print("请输入课程名称: ");
                std::getline(std::cin, name);
                _secretaryController.addCourse(id, name);
                pause();
                break;
            }
            case 3:
            {
                string id;
                print("请输入要删除的课程ID: ");
                std::getline(std::cin, id);
                _secretaryController.removeCourse(id);
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
        print("========================================\n");
        print("        报表生成\n");
        print("========================================\n");
        print("1. 选课统计报告\n");
        print("2. 课程详细报告\n");
        print("3. 教师工作量报告\n");
        print("0. 返回\n");
        print("========================================\n");
        print("请选择: ");
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice)
        {
            case 1:
            {
                _secretaryController.generateEnrollmentReport();
                pause();
                break;
            }
            case 2:
            {
                _secretaryController.generateCourseReport();
                pause();
                break;
            }
            case 3:
            {
                _secretaryController.generateTeacherWorkloadReport();
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

void UI::teacherGradeInterface()
{
    while (true)
    {
        clearScreen();
        print("========================================\n");
        print("            教师成绩评定\n");
        print("========================================\n");
        print("1. 录入成绩\n");
        print("2. 修改成绩\n");
        print("3. 查看所有成绩\n");
        print("0. 返回\n");
        print("========================================\n");
        print("请选择: ");

        int choice;
        if (!(std::cin >> choice))
        {
            // 输入失败，可能是EOF，返回教师菜单
            clearInputBuffer();
            return;
        }
        std::cin.ignore();

        switch (choice)
        {
            case 1:
            {
                // 录入成绩
                clearScreen();
                print("========================================\n");
                print("            录入成绩\n");
                print("========================================\n");
                
                print("您教授的课程:\n");
                _teacherController.displaySchedule(_authController.getCurrentUserId());
                
                string studentId, courseId;
                double score;
                string comment;
                
                print("\n请输入学生ID: ");
                std::getline(std::cin, studentId);
                
                print("请输入课程ID: ");
                std::getline(std::cin, courseId);
                
                // 验证输入
                if (studentId.empty() || courseId.empty()) {
                    print("错误：学生ID和课程ID不能为空！\n");
                    pause();
                    break;
                }
                
                print("请输入成绩(0-100): ");
                if (!(std::cin >> score)) {
                    print("错误：成绩输入无效！\n");
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    pause();
                    break;
                }
                std::cin.ignore();
                
                print("请输入评语(可选): ");
                std::getline(std::cin, comment);
                
                // 录入成绩
                if (_teacherController.addGrade(_authController.getCurrentUserId(), studentId, courseId, score, comment)) {
                    print("成绩录入成功！\n");
                }
                pause();
                break;
            }
            case 2:
            {
                // 修改成绩
                clearScreen();
                print("========================================\n");
                print("            修改成绩\n");
                print("========================================\n");
                
                print("您教授的课程:\n");
                _teacherController.displaySchedule(_authController.getCurrentUserId());
                
                print("\n已有成绩记录:\n");
                _teacherController.displayAllGrades(_authController.getCurrentUserId());
                
                string studentId, courseId;
                double newScore;
                string newComment;
                
                print("\n请输入学生ID: ");
                std::getline(std::cin, studentId);
                
                print("请输入课程ID: ");
                std::getline(std::cin, courseId);
                
                // 验证输入
                if (studentId.empty() || courseId.empty()) {
                    print("错误：学生ID和课程ID不能为空！\n");
                    pause();
                    break;
                }
                
                print("请输入新成绩(0-100): ");
                if (!(std::cin >> newScore)) {
                    print("错误：成绩输入无效！\n");
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    pause();
                    break;
                }
                std::cin.ignore();
                
                print("请输入新评语(可选): ");
                std::getline(std::cin, newComment);
                
                // 修改成绩
                if (_teacherController.updateGrade(_authController.getCurrentUserId(), studentId, courseId, newScore, newComment)) {
                    print("成绩修改成功！\n");
                }
                pause();
                break;
            }
            case 3:
            {
                // 查看所有成绩
                clearScreen();
                print("========================================\n");
                print("            查看所有成绩\n");
                print("========================================\n");
                
                _teacherController.displayAllGrades(_authController.getCurrentUserId());
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


