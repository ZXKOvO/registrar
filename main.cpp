// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created: XinKang Zheng  Id:2024051604022   2026-01-07 10:47:55
// Description:选课系统
//
// Main program - demonstrates the registrar system
//     [v0.1.2]     2026-01-07 10:36:31
//         *add class teacher
import registrar;
import std;

int main()
{
    auto system = &Registrar::system;
    system().initialize();

    // Student enrollment
    system().studentEnrollsInCourse("S001","CS101");
    system().studentEnrollsInCourse("S001","CS201");
    system().studentEnrollsInCourse("S001","MATH101");

    system().studentEnrollsInCourse("S002","CS101");
    system().studentEnrollsInCourse("S002","MATH101");

    system().studentEnrollsInCourse("S003","CS201");
    system().studentEnrollsInCourse("S003","MATH101");

    system().studentEnrollsInCourse("S004","CS101");
    system().studentEnrollsInCourse("S004","CS201");

    system().studentEnrollsInCourse("S005","CS201");

    std::println();
    
    // View course rosters
    system().courseRoster("CS101");
    system().courseRoster("CS201");
    system().courseRoster("MATH101");

    // View student schedules
    std::print("所有学生的课程表\n");
    system().studentSchedule("S001");
    system().studentSchedule("S002");
    system().studentSchedule("S003");
    system().studentSchedule("S004");
    system().studentSchedule("S005");

    // Student drops course
    std::println("S001退课CS101");
    system().studentDropsCourse("S001","CS101");
    
    std::println("S001退课后的课表");
    system().studentSchedule("S001");

    std::print("CS101的选课名单\n");
    system().courseRoster("CS101");

    // View teacher schedules
    std::println("教师授课安排");
    system().teacherSchedule("T001");
    system().teacherSchedule("T002");
    system().teacherSchedule("T003");

    // View teacher rosters
    std::println("教师授课学生名单");
    system().teacherRoster("T001");
    system().teacherRoster("T002");
    system().teacherRoster("T003");

    // === 测试新增功能 ===
    std::println("\n=== 测试新增功能 ===");
    
    // 1. 测试报告生成功能
    std::println("\n--- 系统报告 ---");
    std::println("{}", system().generateEnrollmentReport());
    std::println("\n{}", system().generateCourseReport());
    
    // 2. 测试添加新学生
    std::println("\n--- 添加新学生 ---");
    system().addStudent("S006", "张三");
    system().studentSchedule("S006");
    system().studentEnrollsInCourse("S006", "CS101");
    std::println("张三选课CS101后:");
    system().studentSchedule("S006");
    
    // 3. 测试添加新教师
    std::println("\n--- 添加新教师 ---");
    system().addTeacher("T004", "李老师");
    system().teacherSchedule("T004");
    
    // 4. 测试添加新课程
    std::println("\n--- 添加新课程 ---");
    system().addCourse("CS301", "算法设计");
    system().studentEnrollsInCourse("S001", "CS301");
    system().studentEnrollsInCourse("S002", "CS301");
    std::println("CS301课程选课情况:");
    system().courseRoster("CS301");
    
    // 5. 测试课程状态查询
    auto cs301 = system().findCourseById("CS301");
    if (cs301) {
        std::println("CS301选课人数: {}", cs301->getEnrollmentCount());
        std::println("CS301是否已满: {}", cs301->isFull() ? "是" : "否");
    }
    
    // 6. 测试教师工作量查询
    auto smith = system().findTeacherById("T001");
    if (smith) {
        std::println("\nT001 Smith老师授课数量: {}", smith->getCourseCount());
    }
    
    // 7. 测试删除功能
    std::println("\n--- 测试删除功能 ---");
    std::println("删除学生S006:");
    system().removeStudent("S006");
    
    std::println("删除课程CS301:");
    system().removeCourse("CS301");
    
    std::println("删除教师T004:");
    system().removeTeacher("T004");
    
    // 8. 最终系统报告
    std::println("\n--- 最终系统状态 ---");
    std::println("{}", system().generateTeacherReport());

    return 0;
}

