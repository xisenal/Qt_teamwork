# UserManager 类方法文档

## 构造函数和析构函数

### UserManager(QObject *parent = nullptr)
- **功能说明**：创建UserManager对象，初始化SQLite数据库连接
- **参数**：
  - parent：父QObject对象指针，默认为nullptr
- **返回值**：无

### ~UserManager()
- **功能说明**：析构函数，关闭数据库连接
- **参数**：无
- **返回值**：无

## 数据库相关方法

### bool initDatabase()
- **功能说明**：初始化数据库连接并创建必要的数据表
- **参数**：无
- **返回值**：
  - true：数据库初始化成功
  - false：数据库初始化失败

## 用户管理方法

### bool registerUser(const QString &email, const QString &studentId, const QString &name, const QString &password)
- **功能说明**：注册新用户，创建用户记录和用户目录
- **参数**：
  - email：用户邮箱（作为唯一标识）
  - studentId：学生ID
  - name：用户姓名
  - password：用户密码（将被加密存储）
- **返回值**：
  - true：用户注册成功
  - false：用户注册失败

### bool createUserDirectory(const QString &email)
- **功能说明**：创建用户专属目录结构，包括头像和个人信息文件
- **参数**：
  - email：用户邮箱
- **返回值**：
  - true：目录创建成功
  - false：目录创建失败

### bool validateLogin(const QString &email, const QString &password, bool &isAdmin)
- **功能说明**：验证用户登录信息
- **参数**：
  - email：用户邮箱
  - password：用户密码
  - isAdmin：输出参数，标识是否为管理员账户
- **返回值**：
  - true：登录验证成功
  - false：登录验证失败

### bool getUserInfo(const QString &email, QString &studentId, QString &name)
- **功能说明**：获取用户基本信息
- **参数**：
  - email：用户邮箱
  - studentId：输出参数，返回学生ID
  - name：输出参数，返回用户姓名
- **返回值**：
  - true：获取信息成功
  - false：获取信息失败

### bool getUserDetailInfo(const QString &email, QString &avatarPath, QString &schoolId, QString &description)
- **功能说明**：获取用户详细信息，包括头像路径和个人描述
- **参数**：
  - email：用户邮箱
  - avatarPath：输出参数，返回头像文件路径
  - schoolId：输出参数，返回学号
  - description：输出参数，返回用户描述
- **返回值**：
  - true：获取详细信息成功
  - false：获取详细信息失败

### QList<QMap<QString, QString>> getAllUsers()
- **功能说明**：获取所有用户的基本信息列表
- **参数**：无
- **返回值**：
  - 包含所有用户信息的列表，每个用户信息包含email、student_id和name字段

### bool deleteUser(const QString &email)
- **功能说明**：删除指定用户及其相关数据
- **参数**：
  - email：要删除的用户邮箱
- **返回值**：
  - true：用户删除成功
  - false：用户删除失败

### bool updateUserInfo(const QString &email, const QString &studentId, const QString &name)
- **功能说明**：更新用户基本信息
- **参数**：
  - email：用户邮箱
  - studentId：新的学生ID
  - name：新的用户姓名
- **返回值**：
  - true：信息更新成功
  - false：信息更新失败

### bool resetPassword(const QString &email, const QString &newPassword)
- **功能说明**：重置用户密码
- **参数**：
  - email：用户邮箱
  - newPassword：新密码
- **返回值**：
  - true：密码重置成功
  - false：密码重置失败