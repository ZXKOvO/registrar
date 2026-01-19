教务管理系统 (Registrar System)
项目概述
这是一个基于C++23模块化开发的教务管理系统，实现了学生选课、教师授课管理、成绩评定、教学任务分配等核心功能。系统采用四层架构设计，支持PostgreSQL数据库持久化存储，并提供了完整的命令行用户界面。
技术栈
语言: C++23 (使用模块系统)
构建工具: CMake
数据库: PostgreSQL
架构模式: 四层架构 (UI层、控制器层、领域层、数据访问层)
设计模式: 单例模式、数据访问对象(DAO)模式

registrar/
├── main.cpp                    程序入口点
├── registrar.cppm              主模块导出文件
├── registrar_core.cppm         核心业务逻辑
├── student.cppm                学生实体类
├── teacher.cppm                教师实体类
├── course.cppm                 课程实体类
├── grade.cppm                  成绩实体类
├── teaching_task.cppm          教学任务实体类
├── enrollment.cppm             选课记录实体类
├── academic_secretary.cppm     教学秘书实体类
├── data_manager.cppm           数据管理器接口
├── data_manager_impl.cppm      数据管理器实现
├── student_controller.cppm     学生控制器
├── teacher_controller.cppm     教师控制器
├── secretary_controller.cppm   教学秘书控制器
├── authentication_controller.cppm 认证控制器
├── ui.cppm                     用户界面
├── db_config.h                 数据库配置
├── CMakeLists.txt              构建配置
└── README.md                   项目说明文档
系统架构
四层架构设计
1. UI层 (ui.cppm)
   - 提供命令行交互界面
   - 处理用户输入和显示
   - 分为学生、教师、教学秘书三个角色界面
2. 控制器层
   - StudentController: 学生业务逻辑
   - TeacherController: 教师业务逻辑
   - SecretaryController: 教学秘书业务逻辑
   - AuthenticationController: 用户认证逻辑
3. 领域层 (registrar_core.cppm)
   - Registrar类: 核心业务协调
   - 实体类: Student, Teacher, Course, Grade等
   - 业务规则和领域逻辑

4. 数据访问层
   - DataManager: 数据访问统一接口
   - InMemoryStorage: 内存存储实现
   - PostgreSQLAdapter: PostgreSQL数据库适配器

核心功能

学生功能
- 选课/退课
- 查看课程表
- 查看可选课程
- 查看成绩

教师功能
- 查看授课安排
- 查看学生名单
- 录入/修改成绩
- 查看所有成绩

教学秘书功能
- 学生管理 (增删查)
- 教师管理 (增删查)
- 课程管理 (增删查)
- 教学任务分配
- 报表生成 (选课统计、课程详情、教师工作量)

数据库设计

系统使用PostgreSQL数据库，包含以下主要表:
- students (学生表)
- teachers (教师表)
- courses (课程表)
- grades (成绩表)
- teaching_tasks (教学任务表)
- student_enrollments (学生选课表)

默认数据

系统初始化时会创建以下默认数据:
- 学生: S001(Tomas), S002(Jerry), S003(Baker), S004(Tom), S005(Musk)
- 教师: T001(Smith), T002(Johnson), T003(Williams)
- 课程: CS101(C programming), CS201(Data structure), MATH101(Advanced Math)

开发特点

1. 模块化设计: 使用C++23模块系统，提高编译效率和代码组织
2. 分层架构: 清晰的四层架构，职责分离，易于维护
3. 数据存储抽象: 支持内存和PostgreSQL两种存储方式，可灵活切换
4. 完整业务流程: 覆盖教务管理的主要业务场景
5. 错误处理: 完善的输入验证和错误提示


作者信息

- 创建者: zhengxinkang,xiexianhao,cuichengjiang等
- 版本: 0.2.0
- 创建日期: 2026-01-07
- 最后更新: 2026-01-19
