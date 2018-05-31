课程设计 
=================


准备
------------
准备采用gtkmm3作为UI库。首先安装gtkmm开发库：
sudo apt-get install libgtk3.0-dev
文档：
sudo apt-get install gtkmm-documentation
在这也能看到在线的：http://www.gtkmm.org/zh_CN/documentation.html
https://developer.gnome.org/gtkmm-tutorial/stable/
file:///usr/share/doc/gtkmm-documentation/tutorial/html/index.html
上面类似于书籍的指导，并提供大量的示例和截图效果。

使用时，g++的编译选项需要添加一些东西：
g++ main.cpp -o simple `pkg-config gtkmm-3.0 --cflags --libs`


gtkmm
---------------------
### css
http://stackoverflow.com/questions/6666387/how-to-use-cssstyleprovider-in-gtkmm3
https://developer.gnome.org/gtkmm/stable/classGtk_1_1CssProvider.html#details
https://developer.gnome.org/gtk3/unstable/GtkCssProvider.html#GtkCssProvider.description
http://erikos.buchoase.de/2011/11/24/gtk-3-theme-style-your-applications-2/

最关键的一点，css具体的细节找gtk文档是找不到的！要去找css文档。


### expand
gtk Widget有个expand属性，设置在窗口大小改变是是否跟着改变。


### 焦点
Gtk::Widget::grab_focus();
把当前的组件设为键盘焦点

### toolbar
只有toolItem的子类才能添加进去，dev手册中搜索Gtk::ToolItem


### switch 切换时的发射的信号
竟然没找到C++的API！！！！！
于是决定改用C的API。
1. 调用控件对象的gobj()方法获得C语言层次的对象指针，用G_OBJECT宏转换成C语言层次的基类指针（否则类型检查通不过）。
2. 为了能契合C++ OOP的方式，将回调函数定义为类静态对象，第二个用于传递数据的指针拿来传递this指针。

注：C的API是用字符串当信号
```
    g_signal_connect(G_OBJECT(switch.gobj()), "notify::active",
                G_CALLBACK(&(MainWindow::on_switch_button_toggle)), this);
    MainWindow类里面：
    static void on_switch_button_toggle(GtkWidget *widget, void* instance_);
```

http://stackoverflow.com/questions/21478803/member-function-as-callback-function-to-g-signal-connect
http://stackoverflow.com/questions/1636987/compiling-gtk-application-with-g-compiler

可是我发现在我这样用之后会出现段错误（debian8.1, gcc4.9）。
然后把回调函数改成普通函数，没卵用。

查阅C API文档得知，G_CALLBACK宏将回调函数指针转换成GCallback（`typdef void (*GCallback)(void);`）类型。
目前猜测原因是，C++标准中将函数指针乱转换的行为是未定义的。
1. C++ primer 简体中文版，P221，写着"指向不同函数类型的指针间不存在转换规则"
2. http://www.sunistudio.com/cppfaq/pointers-to-members.html#faq-33.7  枪毙了成员函数指针转`void*`
3. http://www.sunistudio.com/cppfaq/pointers-to-members.html#faq-33.8  枪毙了普通函数指针转`void*`
可是能不能把函数指针转`(void(*)(void))`? 看完了上面链接所在的FAQ，可以有很大的把握认为是不行的了。

最后，python更适合写GTK。

C++
------------
&(ClassName::MemberFunctionName) 和 &ClassName::MemberFunctionName 似乎不太一样。在sigc::mem_fun()中，前面的写法会报错。

命名空间内类的前置声明
-----------------
我原以为是这样写：
class NamespaceName::ClassName;
但是其实是这样写的：
namespace NamespaceName
{
class ClassName;
}



内存泄露工具
------------
有memwatch，valgrind等。
对于valgrind有gnome的图形前端alleyoop，qt前端valkyrie
http://blog.csdn.net/21aspnet/article/details/8172124
http://blog.chinaunix.net/uid-23629988-id-3033741.html


问题
--------------
Q. 构造函数中调用其它构造函数，试图复用代码，为什么没有作用？
A: 确实没有作用！！编辑器连警告都没有！！！但是构造函数中是可以调用其它普通的成员函数。
    http://www.cnblogs.com/chio/archive/2007/10/20/931043.html
    实际上，假设类名是ExampleClass，
    以下这种写法会创建一个ExampleClass对象并返回：
    ExampleClass();
    以下这种写法是不行的，不能编译：
    this->ExampleClass();

    解决方法是把构造函数直接想复用的代码的放到另外一个成员函数里面，然后构造函数调用它（推荐方案）。
    或者使用 placement new 运算符显式调用构造函数（太奇怪的方法，貌似不是主流用法？）。


Q: 函数设置默认参数时，定义中如果写上默认参数貌似会报错。



Q: 在类中重载new delete运算符的具体行为？
A：原型是这样的：
    ```
    void* operator new(size_t size);
    void operator delete(void* p);
    void* operator new[](size_t size);
    void  operator delete[](void* p);
    ```
误解：类中重载new与delete之后，并不是类成员函数的new与delete操作被换成了定义的版本！！！
正解：当用new和delete操作该类对象时，使用定义的版本来控制内存分配。

C++一些必要的规范，遵守规范，避免踩坑：
http://www.sunistudio.com/cppfaq/coding-standards.html#[27.3]


Q: unix中，popen()执行shell命令后，如何等待命令执行结束？
A: 发现使用fclose()关闭管道时会先等待其执行完毕。


Q: 能通过基类指针delete掉派生类对象吗？
A: 可以。但是注意把基类析构函数设为virtual，设为virtual，设为virtual！！！
    http://www.sunistudio.com/cppfaq/virtual-functions.html#[20.4]

    但是要是new出来派生类对象数组，再把返回的派生类指针转换成基类指针，再试图delete []，估计是不行的。
    很显然，基类对象所占字节数和派生类可能不一样。而delete[]一般根据指针所指的对象类型推算出每个对象所占的字节数，以进一步推算数组中每个对象的首地址。（个人推测，未找到权威资料）


Q: 模板函数的声明放入.h中，模板函数的定义放入.cpp中，你看多么规范的写法。为什么不行？！
A: 模板函数和模板类不能分离编译，不能分离编译，不能分离编译！！！先记住就行，以后再问为什么。


