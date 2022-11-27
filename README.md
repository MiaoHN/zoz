# ZOZ

A toy script language inspired by [CraftingInterpreters](https://craftinginterpreters.com)

## Build

This project don't use any third-party library, and build tool is cmake. So you just need to setup basic cmake environment and build with cmake simply:

```bash
cmake . -B build && cmake --build build
```

And if you want to run tests, you can use `ctest`:

```bash
cd build/test && ctest
```

## Examples

### Repl

Just type like follow:

```bash
$ zoz
:) >
```

Then you can play with zoz in repl-mode

:star: **Notice**

- `:) >` means everything is ok
- `:( >` means everything is ok

## License

[MIT](./LICENSE)
