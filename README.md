# Qt Installer Framework 概观

先说两个东西，Qt框架自己的安装维护也使用

- **installer**：安装器，份为在线和离线两种，负责程序的部署配置，安装向导
- **maintenancetool**：安装器安装好程序后，程序的暗中目录保留维护工具，有修复、更新、删除程序组件的功能。在控制面版中也会接管程序的修复和卸载工作

更全面的描述参看官方文档：https://doc.qt.io/qtinstallerframework/ifw-overview.html



# 我们的需求及场景分析

1. 程序打包工具，可能需要在线安装功能
2. 程序自检更新

对于1，QIF当然满足，对于2. 需要程序在合适的时间（启动时或其他）运行maintenacetool来完成



# 使用 QIF 准备

下载：https://download.qt.io/official_releases/qt-installer-framework/

建议 3.0.2，较新的版本有bug，在我的机器上影响 maintenancetool 部署。

安装好后将 <installdir>/bin 添加到环境变量PATH，这个目录中binarycreator.exe， repogen.exe 是我们将使用的主要工具，一个用来制作installer，一个生成repository。



# 参考文档及样例

- QIF 装好后携带的文档：

  `<installdir>/doc` 下的html文档可以添加到Qt Assistant查看

- 也可以查看在线文档：

  https://doc.qt.io/qtinstallerframework/index.html

- 有篇中文博客可以快速入门：

  https://pressc.cn/1041.html

  其中程序自动更新功能，我使用了他的代码（dummy_src/main.cpp），比较简单，可能稍微耽误程序的启动，有精力可以研究作者提到的开源项目https://github.com/Skycoder42/QtAutoUpdater，有更完善的自动检查并且支持QIF做后台

- QIF example：

  `<installdir>/examples`  

  建议先看tutorial、online、startmenu. 我的这个样例就是参考了这三个实现了基本功能



# 样例使用说明

假定您通过上述文档基本了解了 QIF 的使用方法，构造安装器的目录结构、配置文件等等。

## 各部分功能：

- **config**目录：binarycreator.exe 创建安装器的配置 （QIF文档和入门教程介绍了典型config）
- **packages**目录：第一版（v1.0.0）包含程序数据和安装器元数据的目录（QIF文档和入门教程介绍了典型packages）

- **package_update**目录：第二版 （v1.1.0）程序数据和安装器元数据目录（参考QIF examples/online）repogen.exe 用来更新repository
- **create_installer_v1.0.0_online.bat** 构造第一版在线安装器的命令脚本

- **create_installer_v1.0.0_offline.bat** 构造第一版离线安装器的命令脚本

- **create_repository.bat** 生成repository的命令脚本

- **py_http_srv.bat** 脚本开启一个简易webserver（前提是系统安装了python，并添加了PATH），使得 http://localhost/repository 可以被在线安装器及maintenancetool访问

- **update_repository.bat** 基于package_update 更新repository的命令脚本

## 试验

### 场景一：使用在线安装包安装

1. 运行`create_installer_v1.0.0_online.bat`，验证生成安装器`ebtrade_v.1.0.0_setup_online.exe`

2. 运行 `create_repository.bat`，验证生成目录`repository`

3. 运行 `py_http_srv.bat`，浏览器验证`http://localhost/repository/`可访问

4. 运行`ebtrade_v.1.0.0_setup_online.exe`，按向导完成安装，验证：

   - 安装过程无报错
- 安装完成后，指定安装目录（默认`C:\Program Files (x86)\EBSCN\EBTrade`）有程序文件以及`maintenancetool.exe`
   - 安装完成桌面有 `EBTrade`程序快捷方式，开始菜单有`EBTrade`目录和目录下的`EBTrade`快捷方式
- EBTrade程序可以启动
5. 控制面板/程序和功能，点击`EBTrade`，能弹出`maintenancetool`页面，选择“删除所有组件”，验证可以成功卸载程序

### 场景二：使用离线安装包

1. 卸载
2. 运行`create_installer_v1.0.0_offline.bat`，验证生成安装器`ebtrade_v.1.0.0_setup_offline.exe`
3. 运行`ebtrade_v.1.0.0_setup_offline.exe`，按向导完成安装，验证：

  - 安装过程无报错
  - 安装完成后，指定安装目录（默认`C:\Program Files (x86)\EBSCN\EBTrade`）有程序文件以及`maintenancetool.exe`
  - 安装完成桌面有 `EBTrade`程序快捷方式，开始菜单有`EBTrade`目录和目录下的`EBTrade`快捷方式
  - EBTrade程序可以启动
4. 控制面板/程序和功能，点击`EBTrade`，能弹出`maintenancetool`页面，选择“删除所有组件”，验证可以成功卸载程序

### 场景三：更新repository后，程序启动提示升级

1. 按场景一进行到4步骤，先关闭安装好的EBTrade程序
2. 在样例目录内运行`update_repository.bat`脚本，升级 repository，保持python开启的web服务器运行
3. 点击桌面的EBTrad图标打开程序后，程序会弹出`maintenancetool`的维护页面，默认停留在“更新组件”选项上
4. 连续进行“下一步”，完成更新。关闭EBTrade，再次点击桌面图标启动EBTrade，验证已经是v1.1.0版本



## 样例经验总结

在前述的网上教程基础上，本样例实践了QIF构建在线离线安装包，生成和更新repository等功能，并且在程序中加了一部分代码（参见dummy_src/main.cpp），使之能在启动时检查更新。

**教程之外的一点经验**：

1. config.xml 中配置安装目标目录采用了这样的方式`<TargetDir>@ApplicationsDir@/@Publisher@/EBTrade</TargetDir>` 其中`@ApplicationsDir@`变量一般是`C:\Program Files (x86)`，而`@Publisher@`由`<Publisher>EBSCN</Publisher>`配置，这种目录组织更符合一般软件的惯例
2. 生成桌面和开始菜单中的快捷方式，要借助qs脚本（`packages\main\meta\installscript.qs`）参考了QIF的`startmenu`这个样例
3. 安装器组件主程序部分默认必装，是在`packages\main\meta\package.xml`中设置 `<ForcedInstallation>true</ForcedInstallation>`标签实现

**将来探索**：

1. config.xml 和 package.xml有丰富的配置项研究
2. QIF 可以自定义ui，qs脚本，使得installer有更强的定制性和更丰富的功能，对安装有复杂需求可以深入研究