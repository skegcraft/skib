# skib
A basic build system for the [Skript](https://github.com/SkriptLang/Skript) scripting language.

## Features
- `#include` other files recursively
- `#define` preprocessor symbols, and use them in code

## Usage
```
$ gcc main.c -o skib; cp skib /bin/
$ cd test
$ skib main.sk ~/MyServ/plugins/Skript/scripts/main.sk
```

## TODO:
- Macros
- Python to Skript transpiler?
