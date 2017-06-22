# Qmemo

## Qmemo
#### 是一个简单的QT日历记事本应用。
***
#### 编写记录

 0. Idea: 版面参照金山词霸的设计，词霸的界面很好看( ⊙ o ⊙ )！
 1. TODO LIST: 1.构建日历记事本的各个组件类。2.组件这些类
 2. TODO LIST: 1.实现几个槽函数的连接，菜单的关于，退出。留坑：maybeSave()事件
 3. TODO LIST: 1.顶部Title,时间跟新显示(QTimer定时更新)
 4. TODO LIST: 1.MidLeftWidget: DateWidget=>日历框，QLabel显示年月，格子布局周几
      2.计算日期函数，42个labelday的日期，相关属性（参见CalendarWidget样例的构建）。
 5. TODO LIST: 1.实现下面的跳转按钮，年月日编辑按钮。2.整理好排版
 6. TODO LIST: 1.RightUpWidget显示图片区，继承自QWidget的widget，设置大小，载入图像和重绘
 7. TODO LIST: 1.RightDownWiget文本编辑区，实现QTextEdit和一个label，垂直布局。
     2.设置字体，保存状态，数据保存
 
 8. -- fixed above.
 9. TODO LIST: 1.实现数据库保存，2.字体选择		--fixed 2017/04/22
 10. TODO LIST: 1.当双击选择某日期时，在文本框显示这个日期的日历内容。         2.修复一些bug。	--fixed 2017/04/24
		3.额外：实现了QApplicationIcon蛮好看得啦。bug还没修。。。 --fixed 2017/04/23 17:19
 11. TODO LIST: 1.实现点击时出现一个红方框		   --fixed 2017/04/23 23:37
      2.实现定时播放音乐，新增类，slot函数	--fixed 2017/04/25
 12. TODO LIST: 1.在DataWidget上再添加一个现实日子的按钮，颜色同一的灰#646464大小要合适。--fixed 2017/04/24 13:52
      2.额外：修复le数据库路径bug.
 13. TODO LIST: 1.编写使用帮助说明，简单描述一下。。。集成到Title上的QAction上。。。
        2.开源使用说明，将代码放上github上，将一些图片的来源写清，参考的代码说清。。。
	    3.
 14. TODO LIST: 1.将设置信息记录，重用。QSettings 类运用。主要是1.图片路径的保存
        2.字体样式的保存
        2.1字体很难搞额。 				--partly fixed 2017/04/27
 15. TODO LIST: 1.学习QT的联网，可以实现图片的爬虫，每日一图。	--暂时没计划。。。
 16. 算是完成部分了。其他的是修饰了，定版。提交的代码要将debug等不需要的代码去掉。在ubuntu下进行终版修改。  --2017/04/27
 17. 调整了一些布局，重新使用最大化，把双击改为了单击，操作的感觉更好些。-- 2017/06/22，作业检查前的代码检查。
	 

    
