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

    return 0;
}

