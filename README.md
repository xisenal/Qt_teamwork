# Qt_teamwork


### 如何通过git合作?

**注意: 每次开始修改代码前先git pull**

目前只用一个branch就是main, 我们自己都在main上面改. 如果自己在修改的过程中, 远程的main发生了更新, 那么操作是: 先git stash,再git pull, 再git stash apply, 参考下面的命令.
```
git stash # 保存当前修改
git stash list #列出所有的stash
git stash apply stash@{1} #恢复指定的 stash，例如 stash@{1}
git stash drop # 删除最近的stash
```

也可以用IDE/vscode提供的集成推送功能，使用会更加安逸

### 发送验证码的邮箱

账户：xlabmailsystem@163.com

密码：看群吧

### 管理员账户

账号：xlab@xlab.com

密码：XLab

登录这个账户不会进入主页面，而是进入账户管理界面（不会显示密码之类的东西的），可以编辑账户姓名、学号等信息，后面会继续更新功能。

### To Do List:

请各位把自己接下来需要完成的任务写在这里，可以激励你完成任务

wyx:

1. 完成忘记密码的功能
2. 完成主页面的用户信息显示、修改、头像上传等功能








*****Qt真难用
