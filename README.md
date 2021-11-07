# skib
A basic build system for the [Skript](https://github.com/SkriptLang/Skript) scripting language.

## Features
- `#include` other files recursively
- `#define` preprocessor symbols and macros

## Usage
```
# Install
sudo gcc main.c -o /bin/skib
cd test
skib main.sk ~/MyServ/plugins/Skript/scripts/test.sk
```

## Syntax
It's pretty similar to the C preprocessor.  
`test/` should showcase all you need to know.  
- `#include` does not need double quotes around filename
- macro declaration don't have parameter names, use `$0`, $1`..
```
#define MY_MACRO $0 times
loop RANGE(10):
  broadcast "Foo"
```
