# ZOZ

A toy script language inspired by [CraftingInterpreters](https://craftinginterpreters.com)

## Examples

### Repl

Just type like follow:

```bash
$ zoz
:) >> version
zoz version 0.0.1
```

Then you can play with zoz in repl-mode

:star: **Notice**

- `:) >` means everything is ok
- `:( >` means everything is ok

### File

> **Note**
> examples below may not be implemented.

Below is a simple hello-world program

```z
// hello.z
import sys

func main() {
  str = "";
  sys.io.in(str);
  sys.io.out("Your input is: '{}'\n", str);
}
```

you can run it with:

```z
$ zoz hello.z
Hello World!
Your input is 'Hello World!'
$
```

## Build

This project don't use any third-party library, and build tool is cmake. So you just need to setup basic cmake environment and build with cmake simply:

```bash
cmake . -B build && cmake --build build
```

And if you want to run tests, you can use `ctest`:

```bash
cd build/test && ctest
```

## Documentation

Please see [here](./docs/README.md)

## License

[MIT](./LICENSE)
