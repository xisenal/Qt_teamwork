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
