仓库原名字为mtemplate，原本是为了制作模板工程，但是现在为了剥离驱动代码和项目模板，再加上需要做多个不同单片机版本的支持，所以就把这些项目模板分开来。

# mylib

移植和整合各种库和外设驱动，封装为易用的接口。

目前计划是需要做对stm32和msp432的支持。



现在当前先支持STM32F103C8T6，然后STM32F401CCU6，之后是MSP432P401R。

为了减少工作量，STM32的需要手动用CubeMX生成初始化相应硬件的代码。



# 获取完整项目

打开shell

```shell
git clone https://gitee.com/Canrad/mtemplate.git --recursive -submodules
```

获取子模块的更新

```shell
git submodule update --remote
```



# TODO List

- [ ] 软件IIC
- [ ] 软件SPI
- [ ] 普通小车电机（驱动板为TB6612）：使电机可以以一个给定速度正反转
- [ ] 舵机（CA6496）：舵机转到指定角度
- [ ] 红外测距（TOF200）：返回测量的距离值
- [ ] 陀螺仪（MPU9250磁场姿态角度传感器JY61p）：读取三轴角度和四元数信息
- [ ] TF卡
- [ ] 移植oled库：[https://github.com/hello-myj/stm32_oled](https://github.com/hello-myj/stm32_oled)
- [ ] 移植按键驱动模块：[MultiButton](https://github.com/0x1abin/MultiButton)
- [ ] 移植定时器扩展模块：[MultiTimer](https://github.com/0x1abin/MultiTimer)



# 使用到的库和中间件

无标注是因为原本的库不更新了，使用的是最后的版本。

oled库：[https://github.com/hello-myj/stm32_oled](https://github.com/hello-myj/stm32_oled)

按键驱动库：[https://github.com/0x1abin/MultiButton](https://github.com/0x1abin/MultiButton)

定时器扩展库：[https://github.com/0x1abin/MultiTimer](https://github.com/0x1abin/MultiTimer)

FreeRTOS-Kernel（202210.01 LTS）：[https://www.freertos.org/](https://www.freertos.org/)



# 库的使用方法

1. 把当前仓库的`/src`目录下的`mylib`整个目录复制到自己的项目中（挑选合适的路径），或者是把`mylib`项目作为子模块加入到项目中（这可以参考[Git Submodule的使用方法](/doc/submodule.md)）。
2. 在构建工具中添加`mylib`目录的上一级目录到头文件，这是因为`mylib`为了良好的文件组织结构，包含头文件均为`#include <mylib/xxx/xxx.h>`此格式。
3. 选择需要的模块代码添加到项目中。



# 项目的结构

项目目前分为多个层级，包括以下内容。

- [app](/doc/app.md)：应用开发，例如oled菜单等。
- [driver](/doc/driver.md)：对一些驱动的封装，例如软件iic，systick等。
- [device](/doc/device.md)：常用外设设备的驱动代码，例如电机、编码器、陀螺仪等。
- [utility](/doc/utility.md)：一些实用的辅助组件，例如pid算法，环形缓冲区等。

