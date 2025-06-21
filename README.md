# 6.0s1

6.0s helper, Level 1 (Beginner Tier)

# 简介


## 目标人群
这个单头文件库主要用于C语言初学者的基本断言，跟踪和读写操作。
也可以适用于想要重新拿起来C语言的人群。

## 限制
由于这个头文件面向的是Level 1的C语言编程，它只提供几个类型的读写，不包括字符串，因为这是后面的Level要针对的，同时，这个头文件不提供选项分离编译，它只能被单文件c程序直接`#include`使用，毕竟这个Level也不需要处理多文件编译。

## 功能简介
这个头文件直接包含即可，提供了基本的断言，运行跟踪，以及几个常见类型的读写操作，避免
scanf/printf在学习初期引入额外的复杂性。但同时，有余力的读者也可以通过自己实现类似的接口来加深自己对scanf/printf的理解。

## 简要实例

先把这个头文件放到你要写的C代码同一个目录下， 然后:

```c
#include "6.0s1.h"

int main(void) {
    write_int(42);
}
```

注意，`6.0s1.h`已经包含了`<stdio.h>` `<stdlib.h>`，你可以选择重复包含他们，但是不要在包含`6.0s1.h`之前包含它们，尤其在MSVC环境上，因为要自动处理`scanf`的一些历史包袱，这个顺序尤其重要。

# 设计原则

## 位置导向的编程
这个头文件涉及到的所有功能，包括断言，跟踪，以及输入输出函数（的失败情况），都会输出最后调用的文件名，行号以及函数名。这方便用户和群友在调试的时候，快速定位到问题所在。

例如，如下输出

```
Trace: test.c:4: [in main] Hello
test.c:5 [in main] read_int: IOError
```

清楚的显示了是`test.c`的第四行被执行到，当前函数是main，以及一个自定义的"Hello"信息。
同时，`test.c`的第五行， `read_int`函数读入失败，打印了一个错误信息，这表明读取输入失败。

## 防御性编程
防御性编程指的是尽可能描述程序运行的正确情况，并且在错误情况下，尽可能地提供错误位置以及尽早退出。

例如上述例子，`read_int` 函数在失败时，会打印一个错误信息，然后退出，而不是看上去没有任何问题，但是用来读的变量却没有正确赋值。 

虽然，IO失败并不一定是致命性错误，但是处理IO失败需要更多的知识和技能，作为Level 1，我们唯一能做的就是报错退出。




# 具体功能

## 断言和跟踪

初学者一上来就需要了解如何正确决定程序行为是否正确，而不仅仅是用肉眼观察，因此这个头文件提供了基本的断言和跟踪功能。

```C
void s1_assert(int value, const char *reason);
```
如果value为真，则继续执行，否则打印reason并退出。


```
void s1_trace(const char* text);
```
打印text和相关位置，并继续执行。


例子:
```C
#include "6.0s1.h"
int main(void) {
    s1_trace("Line 1");
    write_int(42);
    write_char('\n');
    s1_trace("Line 2");
    write_int(84);
    write_char('\n');
    s1_trace("Line 3");
}
```

输出
```
Trace: test1.c:3: [in main] Line 1
42
Trace: test1.c:6: [in main] Line 2
84
Trace: test1.c:9: [in main] Line 3
```

例子:
```C
#include "6.0s1.h"
int main(void) {
    int a = 4;
    int b = 6;
    int c = 0; // 假设我们要算的是a - b，但这里算成了b - a
    // 假设对C进行复杂的计算，但是我们可以简单验证他应该等于a - b
    c = b - a;
    // 这里简单验证我们写对了
    s1_assert(c == a - b, "c should be a - b");
}
```


输出
```
test1.c:9 [in main] c should be a - b
```
这里我们是对`int c = 0`直到`s1_assert`的上一行进行验证，因此这里出错了就说明只有这一块错了。初学者应当学会精确控制断言的位置和内容，从而更加细致地定位问题，毕竟调试的艺术就是在出问题的时候，让报告（位置等信息）变得更加显著。

这里我们只是一个假想的例子，如果变量`c`的计算过程更为复杂，除了需要拆分步骤外，对每个步骤之间和里面也应当进行断言，这样在调试的时候，可以快速定位到问题所在。

## 输入输出

首先，我们重命名了四个常用的基本类型，在Level1我们不包含数组或字符串等更加深入的知识。

```C
typedef int32_t   s1_int;
typedef int64_t   s1_long;
typedef double    s1_double;
typedef char      s1_char;
```

注意我们不用`float`，C语言的一般学习过程不需要`float`。无论是精度还是速度，一般情况`double`就够用了。但是在特殊场合，`float`仍然有用，但是我们在这里不需要涉及`float`的细节，当你学到具体的框架或库的时候，自然就用上了。

然后我们提供了更不容易出错的输入输出函数，防止过早引入printf/scanf的无关细节:


```C
void write_int(s1_int value);
void write_long(s1_long value);
void write_double(s1_double value);
void write_char(s1_char value);
```

```C
s1_int read_int(void);
s1_long read_long(void);
s1_double read_double(void);
s1_char read_char(void);
void read_pnl(void);
```

输入输出的函数失败情况，都将输出一个错误信息标明所在的文件，行数以及函数名，然后退出。

其中`read_pnl`表示消耗这一行剩下的字符直到换行符为止，包括换行符本身。

例子:

```C
#include "6.0s1.h"
int main(void) {
    s1_int a = read_int();
    s1_trace("Line 1");
    write_int(a * 2);
    write_char('\n');
}
```


输入`42`并回车

输出
```
Trace: test1.c:4: [in main] Line 1
84
```

`read_pnl`例子
```C
#include "6.0s1.h"
int main(void) {
    s1_int a = read_int();
    write_int(a * 2);
    write_char('\n');
    read_pnl(); // 读入这一行剩下的字符直到换行符为止包括换行符本身
    s1_char c = read_char(); // 读入一个字符，这里才不会读入换行符，而是下一行的第一个字符
    write_char(c);
    write_char('\n');
}
```

输入: `42`回车`*`回车
输出:
```
84
*
```

