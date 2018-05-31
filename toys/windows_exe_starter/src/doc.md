简单启动器记录
=================
2016-03-25

程序目标
-----------
windows下，许多脚本语言编写的程序，入口都不是一个exe文件（入口通常是一个bat文件里写上启动命令）。
bat做的程序入门，很丑，不能改图标，启动的时候还有个更丑的黑窗口一闪而过。而且bat做的入口没有exe地道。
因此，现在的目标是，写一个简单的exe启动器，双击它即可运行配置文件里的命令。

程序功能
------------
把启动器exe文件放在某个目录（下面以放在D:\myProgram为例子）。
这样配置文件位于D:\myProgram\starter\config.txt
在配置文件的第一行写上命令。现在我们写上：explorer（该命令是启动windows文件管理器）

好，现在双击我们的启动器，嘿！一个文件管理器弹了出来！

要注意的是，如果命令里有相对路径表示的文件路径，该相对路径是是相对于目录D:\myProgram（即启动器exe文件所在的目录）。

就这么多了，这个C语言编写的启动器功能很简单。



配合lua玩点复杂的
-------------
现在，我们想在启动某个命令之前，改变下环境变量。显然单单用前面的启动器是做不到的了。前面写的启动器只能简单的执行下配置文件里的命令就完事了。
我们可以这样，首先在config.txt中让启动器执行一个lua脚本。
接着，在lua脚本里，用lua改下环境变量，最后用lua的库函数启动我们真正想要启动的命令。

为什么用lua不用python？因为lua很小巧啊！几百k的文件。不然弄个50M启动器的多尴尬啊？
为什么不直接用C搞定？要是把这部分东西写死在C中，每次修改还得重新编译。要是放在配置文件里，设计配置文件格式还有解析配置文件很麻烦的。


看来用lua很不错！首先我们先下载lua5.1:http://luabinaries.sourceforge.net/
lua的标准库函数，功能少的可怜，不仅不能改环境变量，而且标准库运行命令的函数还有黑窗口闪过=_=
那lua有没有什么丰富的系统功能库？有的，像： http://peterodding.com/code/lua/apr
apr是一个c语言库，大名鼎鼎的apache的子项目。这个库是apr到lua的绑定。不是很大，1M左右。
下载地址：http://peterodding.com/code/lua/apr/downloads/

如果仅仅是为了更改环境变量加上无黑窗口运行命令，还有个小巧到极致的库luaex： https://github.com/LuaDist/luaex  （内含使用文档）
什么？刚才那个网址没现成的dll下载？不能忍啊。好在找到了这个：
https://github.com/rjpcomputing/luaforwindows/tree/master/files/clibs
直接下载里面的ex.dll即可。
要注意的是，前面的lua务必下载5.1版本的，因为这里的ex.dll是和5.1版本配套的。


下载完毕了，现在把下载的东西解压，各种dll，exe一股脑扔进D:\myProgram\starter\lua目录就行。
现在我们在config.txt里第一行写上：
starter\lua\wlua5.1.exe starter\start.lua
意思是，执行D:\myProgram\starter\start.lua这个lua脚本。
细心点的伙计肯定会发现了，这里写的是wlua5.1.exe。但是lua目录里还有个和这个很像的lua5.1.exe.
它们的区别是，前者是运行时不会出现黑窗口，后者运行的时候会出现黑窗口。
我们在实际使用时用前者，在调试时用后者。这是因为出现黑窗口能看到错误信息和print函数的输出信息。


最后一步是在lua脚本里设置环境变量，启动程序即可了。

多启动器并存
--------------------
为了支持多启动器并存，我们重新设计一下配置文件的规则。
假设我们有s1.exe, s2.exe, s3.exe，那么他们会分别去starter/s1，starter/s2, starter/s3文件夹去寻找配置文件。
