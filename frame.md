LinuxCarProject/
├── app/                # 【应用层】用户空间业务逻辑
│   ├── ui_sim.c        # UI 模拟进程（负责接收传感器数据并准备对接 LVGL）
│   └── main.c          # 应用程序入口
├── bsp/                # 【驱动层】内核空间 / 板级支持包
│   ├── drivers/        # 存放内核驱动源码（如红外、光线传感器驱动 .ko）
│   ├── dts/            # 存放 RK3566 设备树文件 (.dts/.dtsi)
│   └── sensor_sim.c    # 模拟传感器进程（目前用于测试 IPC 管道）
├── component/          # 【应用层】通用软件组件/中间件
│   ├── log/            # 产品级彩色 Log 框架
│   │   ├── log.c
│   │   └── log.h
│   ├── sqlite/         # SQLite 3 数据库组件（后续用于缓存音乐/天气）
│   └── mqtt/           # MQTT 通信组件
├── lvgl/               # 【应用层】LVGL 图形库源码（正在下载/集成中）
├── platform/           # 【应用层】平台抽象层（封装 Linux 线程、定时器等）
├── docx/               # 项目文档与原理图资料
├── readme.md           # 项目自述文件
├── CMakeLists.txt      # 顶层构建脚本（管理整个项目的编译流程）
└── .vscode/            # VS Code 配置（包含 c_cpp_properties.json）