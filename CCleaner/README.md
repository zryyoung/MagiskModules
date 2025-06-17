# CCleaner

> 一款基于 C++ 编写的 Android Magisk 模块，用于自定义清理指定路径中的垃圾文件，支持循环执行、日志输出等功能，灵活高效，配置简单。

---

## 📌 项目简介

CCleaner 是一个通过 **配置文件驱动** 的 **自动清理工具**，以 Magisk 模块形式运行，专为 Android 系统设计。支持定时清理指定文件/文件夹、自动删除空目录、输出日志，运行稳定，适合高级用户定制使用场景。

---

## 🛠️ 功能特点

- 📁 支持清理自定义路径中的文件或文件夹
- ♻️ 支持循环执行（定时清理）
- 📄 支持输出日志信息
- ⚙️ 配置文件驱动，易于修改，无需重新编译
- 🧩 作为 Magisk 模块运行，系统级权限无阻碍

---

## 🔧 配置说明（`config` 文件）

配置文件位于模块目录下的 `config` 文件中，格式如下：

### `[settings]` 配置项：

| 项目       | 说明                      | 示例          |
|------------|---------------------------|---------------|
| loop       | 是否循环执行              | `true` / `false` |
| interval   | 循环间隔，单位为秒        | `60`          |
| log        | 是否输出日志              | `true` / `false` |

### `[paths]` 清理路径列表：

逐行填写需删除的路径，例如：
```ini
[paths]
/sdcard/.duid
/sdcard/..ccvid
/sdcard/NP
```

完整示例：
```ini
[settings]
loop = true
interval = 60
log = true

[paths]
/sdcard/.duid
/sdcard/..ccvid
/sdcard/..ccdid
/sdcard/.SLOGAN
/sdcard/Documents/ringtone
/sdcard/download/gameresource
/sdcard/download/update
```

---

## 🚀 编译运行方式

该模块为 C++ 程序，请确保已在 Magisk 模块模板中正确嵌入并配置 `config` 文件。系统将自动在模块启用后运行清理程序。

编译命令（仅供参考）：
```bash
clang++ CCleaner.cpp -o CCleaner
```

---

## 👨‍💻 作者信息

- 作者：[@zryyoung](https://github.com/zryyoung)
- 项目地址：https://github.com/zryyoung/MagiskModules  
- 更新配置：https://github.com/zryyoung/MagiskModules/raw/main/CCleaner.json

---

## 📄 开源协议


---

Made with ❤️ by zryyoung
