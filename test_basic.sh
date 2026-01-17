#!/bin/bash
# 基本功能测试脚本

cd /root/shixun/course_selection/build

echo "=== 测试1: 程序启动和退出 ==="
echo "0" | timeout 3 ./registrar_ui 2>&1 | grep -A 3 "主菜单" | head -5
echo ""

echo "=== 测试2: 检查可执行文件 ==="
ls -lh registrar_ui
echo ""

echo "=== 测试3: 编译状态 ==="
echo "编译时间: $(stat -c %y registrar_ui)"
echo "文件大小: $(stat -c %s registrar_ui) bytes"
echo ""

echo "=== 测试完成 ==="