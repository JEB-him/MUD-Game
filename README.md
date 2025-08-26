# MUD-Game

## 文件构成

```
.
├── CMakelists.txt    # 制定编译文件
├── README.md         # 项目说明
├── src               # 项目代码
└── tests             # 测试代码
```

## 规范

### 注释

注释使用 [Doxygen](https://doxygen.nl/manual/) 规范，这个框架可以方便地生成项目文档供其他成员使用，而且注释的可读性很高，上手简单，用 AI 生成几个例子就会了。

### 测试框架

测试框架我以前用过一个更小型的，用起来很香，可以非常快速的测试代码。他可以让我们预先设定好什么场景下程序应该有怎样的行为，如果我们设置了若干个场景，那么每次修改完代码就可以通过测试框架测试现在的代码对所有场景是否都适用。我们这个项目就使用 Catch (比较轻量化)

### 编码规范

大家可以先去读一下 Google 的 Style document, 讲了各种场景下的编码规范，按照规范写的代码可读性一般都很高。

#### 命名

然后我就确定一下命名规范了，大家觉得这种格式不合适可以提一下建议：

- 变量：变量采用蛇形命名法，即 `min_value` 的形式

- 函数：函数采用小驼峰命名法，即 `getValue` 的形式

- 类：采用大驼峰命名法，即 `MapLoader` 的形式

- 常量：采用全大写加下划线分割，即 `MATH_PI` 的形式

大家可以自行补充

#### include

这个地方强烈建议**不要**使用 `using namespace std;` 的形式直接导入命名空间，这会导致命名冲突问题，更好的方案是导入常用的函数、变量等：

```cpp
// 划分成多行只是为了格式美观
using std::cin, std::cout;
using std::string, std::endl;

// 如果想使用 vector
#include <vector>
// 如果需要频繁用到
using std::vector;
vector<int> my_vector;
// 不需要频繁使用
// std::vector<int> my_vecotr;
```
