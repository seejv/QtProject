# QtProject
项目地址来自 github: https://github.com/seejv/QtProject

自定义 qt 控件
工程：
1. QtVirtualKeyboard:
一个使用 QWiget 开发的 QT 虚拟键盘，理论上可以适用所有的 qt 版本，本项目使用数据库 sqlite3 来实现拼音词组。 解决需求：嵌入式或者其他场景需要用到 qt 键盘来完成汉字词组的输入，并且无法使用 qml。
![qt虚拟键盘演示](https://github.com/seejv/QtProject/blob/master/QtVirtualKeyboard/useKeyBoard_Demo/keyboard_show-how.gif)

2. QtRotatingInArmForQt5:
添加 qt5 旋转补丁包，以及修复会导致透明失效的问题。（环境 qt5.5.1）,可能由于补丁包是 qt5.4，所以才会导致打了补丁包后无法正常设置透明效果。
具体需要哪些内容请自行决定。

3. QtPagingToolBar: 
一个使用 QWidget 开发的分页栏，类似于网页分页栏的效果。
 ![qt分页栏演示](https://github.com/seejv/QtProject/blob/master/QtPagingToolBar/pagingtoolbar_show.gif)

4. QtSliptBotton: 
一个使用 QWidget 开发的单选滑块按钮。
 ![qt滑动按钮演示](https://github.com/seejv/QtProject/blob/master/QtSliptBotton/slipt_btn.png)

5. QtDrawingBoard: 
一个 Qt 画板，支持擦除、恢复，保存，可以用于嵌入式
 ![qt画板演示](https://github.com/seejv/QtProject/blob/master/QtDrawingBoard/drawingboard.gif)

6. QtTimer:
定时器, linux 不用再找定时器了                                                                                                
 ![qt定时器演示](https://github.com/seejv/QtProject/blob/master/QtTimer/mytimer.gif)
