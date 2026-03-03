# X-Lab 🧪 | 智能科研实验室综合管理平台

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Qt](https://img.shields.io/badge/Qt-6.9.0-green.svg)](https://www.qt.io/)
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)

**欢迎来到X-Lab! 拒绝成为牛马，领先世界一万年[doge]**
<img width="1280" height="720" alt="image_973349899564201" src="https://github.com/user-attachments/assets/79f71fc5-abf2-454a-8125-bfcdfad1d81c" />


> **X-Lab** 是一个为所有科研工作者、热爱/向往科研的人打造的“科研百宝箱”平台。它集成了多学科常用工具、虚拟实验室管理、文献检索及协同办公功能，旨在通过一体化的平台提升科研效率，优化实验体验。

---

## 🌟 项目简介

在科研过程中，研究人员往往需要在多个工具和平台之间频繁切换。**X-Lab** 致力于解决这一痛点，提供一个统一的集成环境。本项目由 **"实验不与理论作队"** 开发，作为《程序设计实习》2025 Spring 大作业，实现了从用户管理、虚拟实验仿真到学术资源检索的全流程支持。

### 核心愿景
- **一站式服务**：整合分散的科研工具，降低学习成本。
- **灵活扩展**：基于插件化架构，支持按需加载新功能。
- **安全协作**：提供完善的用户权限管理与团队协作机制。

---

## 🚀 主要功能特性

### 1. 🧪 云实验室 (Cloud Lab)，especially for physics
- **虚拟仿真**：支持自由增删、搭配及移动实验仪器，模拟真实实验环境。
- **多领域适配**：架构设计兼容多学科实验场景，具备高度的可扩展性。
- **参数配置**：支持自定义仪器参数与数量，满足不同实验需求。

### 2. 📚 学术资源与扩展
- **文献检索**：集成 **Google Serp API**，直接访问 Google Scholar，解析并展示高质量学术文献。
- **LaTeX 编辑器**：内置 LaTeX 编写与编译工具，支持实时预览、HTML 生成及论文排版，助力科研写作。
- **API 查阅**：提供便捷的接口文档查阅功能。

### 3. 👥 用户与权限管理
- **安全认证**：基于 SQL 数据库的用户系统，支持注册/登录、邮箱验证码机制（SMTP）及密码找回功能。
- **个人中心**：用户可自定义头像、修改个人资料及密码。
- **管理员后台**：专属管理员界面 (`xlab@xlab.com`)，实现用户信息的闭环管理（编辑、权限控制）。

### 4. 🎨 交互体验
- **流畅导航**：采用 `QStackedWidget` 架构，实现首页、欢迎页、实验室、文献页等多界面的无缝切换。
- **现代化 UI**：
  - 卡片式布局展示项目与插件。
  - 悬停阴影特效与圆角设计，提升视觉质感。
  - 响应式侧边栏与弹窗设计。

---

## 💻 安装与运行

### 前置要求
- Qt Creator (推荐版本 6.9.0 及以上)
- MinGW 64-bit 或 MSVC 编译器
- 有效的 Google Serp API Key (用于文献检索功能)
- SMTP 邮箱配置 (用于注册/找回密码功能)

### 编译步骤
1. **克隆仓库**
   ```bash
   git clone https://github.com/xisenal/Qt_teamwork.git
   cd Qt_teamwork
   ```
2. **配置项目**
使用 Qt Creator 打开 X-Lab.pro 文件。
在 config.h 或相应配置文件中填入您的 API Key 和 SMTP 凭证。
构建与运行:

```bash
qmake
make
./X-Lab executable
```
(注：具体构建命令可能因操作系统和编译器而异)



## 如有任何疑问或建议，欢迎多多提issue或request！~

Jiaming Yang, Yunxi Wang, Haorun Li @ Copyright 2025-2026
