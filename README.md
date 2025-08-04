# IEC-Lab-NBUT

这是一个实验室项目和比赛代码的中央仓库。

## 项目结构

本仓库包含以下主要目录，其中的模板工程是用于编写和调试各种模块的代码，集成已经测试过的模块代码。

- [robot](robot/) - 基于STM32F767的机器人比赛的代码

- [mspm0g3507_template_project](mspm0g3507_template_project/) - MSPM0G3507的模板工程

- [stm32f407vgt6_template_project](stm32f407vgt6_template_project/) - STM32F407VGT6的模板项目

## 注意事项

1. 禁止直接在main推送代码，除非特殊情况
2. 提交前使用clangformat格式化代码
3. 模块驱动代码编写完成以后，提供一个`test_xxx`的函数，用于测试此模块
4. 每次更改的文件数量尽可能不要太多
5. 编写一定的代码就提交一次，不要把多个功能的修改放在一起提交
6. commit的信息要明确，禁止使用意义不明的`.`这种垃圾信息

## 开发流程

1. 明确功能，开一个issue说明功能
2. 创建一个分支，分支命名格式为`issue_xxx`，xxx为issue的编号
3. 关联分支到issue，指定开发者，并且打issue的标签，说明此issue是做什么的
4. 编写issue相关代码，并提交
5. 编写测试代码，并提交
6. 开pull request，编写合适的描述，并指定别人review
7. 确定代码没有问题，再合并到主分支
