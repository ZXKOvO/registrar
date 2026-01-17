// File: main_ui.cpp   Version: 0.1.0   License: AGPLv3
// Created: 2026-01-18
// Description: 交互式UI入口程序
//
import std;
import registrar;
import :ui;

int main()
{
    // 初始化系统
    auto& system = Registrar::system();
    system.initialize();

    // 创建UI实例并显示主菜单
    UI ui;
    ui.showMainMenu();

    return 0;
}