# yaml-cpp学习

用于记录学习中的各类笔记，采用md格式记录。

* [yaml-cpp学习笔记](https://blog.csdn.net/weixin_43860854/article/details/109624576)
* [yaml-cpp介绍](https://blog.csdn.net/xhtchina/article/details/113868147)
* [yaml文件解析：c++篇](https://latelee.blog.csdn.net/article/details/111994720)

## YAML语法

YAML是专门用来写配置文件的语言，非常简洁和强大，比 JSON和xml格式要方便很多。

YAML语言的设计目标，就是方便人类读写。它实质上是一种通用的数据串行化格式。它的基本语法规则如下：

* 大小写敏感
* 使用缩进表示层级关系
* 缩进时不允许使用Tab键，只允许使用空格。
* 缩进的空格数目不重要，只要相同层级的元素左侧对齐即可
* #表示注释，从这个字符一直到行尾，都会被解析器忽略。

YAML支持的数据结构有三种。

* 对象：键值对的集合，又称为映射（mapping）/ 哈希（hashes） / 字典（dictionary）
* 数组：一组按次序排列的值，又称为序列（sequence）/ 列表（list）
* 纯量（scalars）：单个的、不可再分的值

## yaml的解析

Node 是 yaml-cpp 中的核心概念，是最重要的数据结构，它用于存储解析后的 yaml 信息。Node一共有以下几种type：

* Null 空节点
* Sequence 序列，类似于一个Vector,对应YAML格式中的数组
* Map 类似标准库中的Map，对应YAML格式中的对象
* Scalar 标量，对应YAML格式中的常量


**生成Node**
```
生成 Node 的形式有很多种， loadFile() 是最常见的一种。

Node LoadFile(const std::string& filename)
filename 就是yaml文件的路径。

有了 Node 之后，所有的信息都可以检索到。比如 name.

cout << "name:" << config["name"].as<string>() << endl;
as<string>()表示将解析的内容转换成 string 类型。
你也可以转换成其它类型。它是一个模板方法。
```

**yamlcpp中的迭代**

yaml-cpp 中也可以通过迭代的方式，访问 Node 中的内容。

比如，访问 skills 下面的各个元素。
```c++
for(YAML::const_iterator it= config["skills"].begin(); it != config["skills"].end();++it)
{
    cout << it->first.as<string>() << ":" << it->second.as<int>() << endl;
}
```

**NodeType类型**

yaml 支持 Scalar、List、Map 类型，yaml-cpp 通过 NodeType 定义了 Node 的可能类型。

```c++
namespace YAML {
  struct NodeType {
    enum value { Undefined, Null, Scalar, Sequence, Map };
  };
}
```

对应未定义、空、标量、序列、字典。

```c++
YAML::Node test1 = YAML::Load("[1,2,3,4]");
cout << " Type: " << test1.Type() << endl;
 
YAML::Node test2 = YAML::Load("1");
cout << " Type: " << test2.Type() << endl;
 
YAML::Node test3 = YAML::Load("{'id':1,'degree':'senior'}");
cout << " Type: " << test3.Type() << endl;
```

上面的代码是为了判断 NodeType。

结果如下：
```
 Type: 3
 Type: 2
 Type: 4
```

分别对应 Sequence、Scalar、Map。


**yaml-cpp写配置文件**

日常开发中，经常用yaml文件来做配置文件，除了读取配置参数，我们经常需要保存参数，yaml-cpp 自然也提供了相应的功能。

Node可以使用文件流的方式进行读写，前面已经使用过了，保存一个node可以用下面的方法：

```c++
std::ofstream fout("config.yaml");
//设置配置文件node数据
fout << node <<std::endl;
 
fout.close();
```

这样，上面打印到cout的内容会被输出到config.yaml文件。

