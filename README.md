# ZOZ

一个玩具脚本语言，受到 [CraftingInterpreters](https://craftinginterpreters.com) 的启发

## 示例

### 交互模式

只需执行 `zoz` 即可:

```bash
$ zoz
:) >> version
zoz version 0.0.1
```

接下来你就可以在交互模式中和 `zoz` 玩一玩

:star: **注意**

- `:) >` 表示上条命令成功执行
- `:( >` 表示上条命令执行失败

### 执行文件

> **Note**
> 下面的例子可能还没有实现

一个简单的 hello world 示例程序如下所示：

```z
// hello.z
import sys

func main() {
  str = "";
  sys.io.in(str);
  sys.io.out("Your input is: '{}'\n", str);
}
```

你可以通过如下命令运行它:

```z
$ zoz hello.z
Hello World!
Your input is 'Hello World!'
$
```

## 构建

本项目未使用第三方库并且通过 cmake 进行构建，所以只需如下命令即可构建项目：

```bash
cmake . -B build && cmake --build build
```

如果想运行测试，你可以使用 `ctest`:

```bash
cd build/test && ctest
```

## 文档

见 [这里](./docs/README.md)

## 协议

[MIT](./LICENSE)
