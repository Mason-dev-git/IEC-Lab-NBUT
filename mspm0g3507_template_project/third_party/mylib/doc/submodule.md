管理 Git submodule 的方法

1. **初始化子模块**： 如果你还没有初始化子模块，你需要先初始化它。

   ```shells
   git submodule init
   ```

2. **克隆子模块**： 克隆子模块的代码到本地仓库中。

   ```shell
   git submodule update --init --recursive
   ```

   –recursive 参数会递归地初始化和克隆所有嵌套的子模块。

3. **拉取子模块的更改**： 如果你需要从远程仓库拉取子模块的最新更改，你可以使用以下命令：

   ```shell
   git submodule update --remote
   ```

   这将从远程仓库拉取子模块的最新更改，并合并到本地子模块中。

4. **提交子模块的更改**： 如果你在子模块中进行了更改，并且想要将这些更改提交到子模块的本地仓库中，你可以使用以下命令：

   ```shell
   git commit -am "Submodule commit message"
   ```

5. **推送子模块的更改**： 如果你想要将子模块的更改推送到远程仓库，你可以使用以下命令：

   ```shell
   git push origin HEAD:submodule_path
   ```

   这里的 `submodule_path` 是子模块在父仓库中的路径。