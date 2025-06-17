# Chroot_Debian12 - Magisk 模块版

> 由 Chroot 驱动的 Debian 12 环境，支持通过 SSH 登录与本地挂载访问，适合在 Android 上运行原生 Linux 环境。

---

## 📌 模块简介

本模块提供一个完整的 **Debian 12** 系统环境，基于 Chroot 技术实现，可直接在 Android 系统中以 Magisk 模块运行。支持 SSH 登录、root 权限访问、本地挂载 sdcard 目录等功能，是开发者、Linux 爱好者的移动利器。

---

## 🚀 使用说明

### 📂 容器安装目录

- **根路径**：`/data/debian12`

### ✅ 启动后默认配置

- **SSH 地址**：`ssh root@127.0.0.1 -p 22`
- **用户名**：`root`
- **密码**：`root`
- **端口**：`22`

### 📂 本机挂载访问

默认未挂载`/sdcard`，更改配置文件`config.prop`的内容`SDCARD=/mnt/sdcard`，启动后会将 Android 系统的 `/sdcard` 目录挂载至 Debian 系统的 `/mnt/sdcard`，方便双向数据交换。

⚠️ **注意事项**：  
**卸载或删除模块前**，请先手动取消挂载，避免清空 `/mnt/sdcard` 中的数据！
**未取消挂载，不要手动删除容器目录，直接在Magisk里面点删除模块，重启即可**
---

## 🧩 模块信息

| 字段         | 内容                                               |
|--------------|----------------------------------------------------|
| 模块 ID      | `chroot_debian12`                                  |
| 名称         | `Debian12`                                         |
| 作者         | [@zryyoung](https://github.com/zryyoung)           |
| 版本         | `1.2`                                              |
| versionCode  | `12`                                               |
| 描述         | Chroot 驱动的 Debian 12，支持 SSH 与本机挂载访问 |

---

## 🗂 项目地址

GitHub 仓库地址：  
🔗 [https://github.com/zryyoung/MagiskModules](https://github.com/zryyoung/MagiskModules)

---

## 📄 开源协议

---

Made with ❤️ by zryyoung
