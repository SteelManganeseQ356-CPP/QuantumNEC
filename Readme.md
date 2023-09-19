# QuantumNEC

## 系统简述

- 名字:

    QuantumNEC = 量子死亡脉冲

- 名字意义:

    "Quantum"的意思是"量子"，意味着这个操作系统的运行速度和量子矩阵传播速度一样快(虽然几乎不可能),
    "NEC"是"死亡脉冲"，我也不知道怎么取得(哭笑),
    这个系统从2023-1-1日开始正式开发

- 系统桌面:

  ![ref](./images/Quantum.jpeg)

- 开发者:

    钢锰Q356-CPP

    bilibili - <https://space.bilibili.com/1226480503?spm_id_from=333.1007.0.0>

- 系统方面实现的东西:
  - 系统引导 - UEFI
  - 64位支持
  - 硬件驱动:
    - 键盘 - 驱动
    - 鼠标 - 驱动
    - 硬盘(IDE, SCSI, SATA, SAS, NVME(M.2/U.2)) - 驱动
    - 时钟 - 驱动
    - 显卡 - 驱动
    - 网卡 - 驱动 - DHCP协议栈
    - USB - 驱动
  - CPU - 多任务调度
  - 文件系统 - EXT4
  - 内存分页
  - 系统调用
  - Shell

## 是否开源

  为避免特殊影响，本系统将由我一个人开发，暂时不开源
  (后续完善至Build number达到50000再考虑开源)

## 特点

- 兼容的语言(编译器)
  - ASM(NASM)
  - C/C++(GCC)
  - lua
  - JAVA
  - Virtual Basic
- 各种小工具
  - 记事本
  - 计算器
  - 绘图
  - 截图工具
  - 写字板
  - 光盘刻录工具
  - 7z, zip, rar等压缩文件解压软件
  - 系统备份工具
- 自己的编程语言：TZX(编译型)
- 兼容的浏览器: Chrome (待开发)

## Other

- NTFS官网 - <https://www.ntfs.com/ntfs_basics.htm>

- 简介 - <https://www.baidu.com/link?url=0X9BYypEaZ1tqTGNTMu52NiDb22AiWsK8CUhra_-DasvbQAo6Ab-FDWJZxAvfNae&wd=&eqid=a2b23dba005ff8880000000463b2940c>\
         <https://www.cnblogs.com/yet-another-cnblogs-blog/p/16822573.html>
- 参考 - <https://wiki.osdev.org/Beginner_Mistakes> or <https://blog.csdn.net/weixin_44391390/article/details/113563215>

## 关于旧版

  这个系统有一个旧版本, build number 是 -1, 基于BIOS引导
  旧版将作为支线任务作为开发

## 彩蛋

  在系统PowerShell中输入SoNvidiaFuckYou有彩蛋 (doge)
