# argparse

A simple argparser that support subcommand.


## 定义参数
参数定义使用结构体数据进行定义，每个参数的首字母不允许重复，结构体中的具体定义方法如下：

```c
typedef struct CommandArg {
  /* 参数名
   * 1. 使用--或者-作为前缀的参数
   * 2. 位置参数
   */
  char *flag;
  /* 参数是否为必须参数 */
  int required;
  /* 参数是否需要跟值 */
  int no_value;
  /* 参数的初始值 */
  void *init;
  /* 用于存放参数的真值 */
  void *value;
  /* 对参数的说明 */
  char *help;
} CommandArg;
```

- `char* flag`: 参数名，可以为`-s`、`--size`和`size`，其中以`-`开头的参数默认为可选参数，没有以`-`开头的参数为位置参数
- `int required`: 是否为必须参数，使用可选值0/1，默认0
- `int no_value`: 参数后是否跟随参数值，默认为0
- `void* init`: 参数的默认值
- `void* value`: 参数的值（重构：需要进行变量隐藏）
- `char* help`: 对该参数值的说明


## 函数原型
```c
/* 回调函数 */
typedef void (*callback_t)(void);
/* 添加一个命令 */
/* 当command_name为一个空字符串或者为NULL时，为通用命令
 * 否则为一个子命令
 * 每个解析器只能包含一个通用命令
 */
void *argparse_add_command(char *command_name, char *description, char *usage,
                           callback_t callback, CommandArg *args);
/**
 * 初始化解析器
 * prog：解析器名
 * description: 解析器描述
 * usage: 使用方法
 */
void argparse_init_parser(char *prog, char *description, char *usage);
void argparse_parse_args(int argc, char *argv[]);
/**
 * 获取一个参数的值，如果找不到就返回NULL
 */
void* get_arg(char* name);
/* 打印解析器以及所有的子命令参数 */
void argparse_print_parser();
/**
 * 打印一个子命令的参数
 * 传递的值为NULL时，打印当前子命令
 * 最好是在回调函数里面进行使用
 */
void argparse_print_command(Command *cmd);
```


## 使用方法

```c
#include "argparse.h"

/* 定义参数 */
/* 一定要在最后加一个空的结构体 */
static CommandArg list_args[] = {
    {.flag = "file"},
    {.flag = "kmer"},
    {.flag = "--help", .required = 1, .no_value = 1},
    {.flag = "--size", .no_value = 1},
    {0}
};

/* 定义回调函数 */
/* 回调函数原型为 */
// void callback(void);

void callback(void){
    /* 解析参数 */
    void* help_value = get_arg("help");
    /* something else */
    // 
}

/*创建解析器*/
int main(int argc, char* argv[]){
    /* 初始化一个解析器 */
    argparse_init_parser("Parser name\n", "This is a description\n", "This is a usage\n");
    /* 添加子命令 */
    argparse_add_command("list", "This is a list command.", "This is a usage.",
                    handle_list, list_args);
    /* 添加通用命令 */
    argparse_add_command("", "22222222222222This is a list command.",
                       "This is a usage.", handle_list, NULL);
    /* 解析参数 */
    argparse_parse_args(argc, argv);
}
```

一个小例子
```c
#include "argparse.h"

static CommandArg list_args[] = {
    {.flag = "file", .help="file help"},
    {.flag = "kmer", .help="kmer help"},
    {.flag = "--help", .required = 1, .no_value = 1, .help="--help arg does not need value"},
    {.flag = "--size", .no_value = 1, .init = "ssss", .help="--size arg does not need value"},
    {0}};

static CommandArg global_args[] = {
    {.flag="-k", .help="kmer size"},
    {.flag="-s", .help="the size"},
    {0}
};

void handle_list() {
  printf("%s\n", get_arg("file"));
  printf("%s\n", get_arg("kmer"));
  printf("%s\n", get_arg("help"));
  printf("%s\n", get_arg("size"));
}

int main(int argc, char *argv[]) {
  argparse_init_parser("Test\n", "This is a test\n", "This is a usage\n");

  argparse_add_command("list", "This is a subcommand.\n", "This is a usage.",
                       handle_list, list_args);
  argparse_add_command("", "This is a list command.\n",
                       "This is a usage.", NULL, global_args);
  argparse_parse_args(argc, argv);
}
```
## 命令行传参方式

### 使用首字母进行传参
定义时使用了`--size`作为参数名，那么传参时既可以使用`-s`也可以使用`--size`进行传参

### 特殊命令
支持`-h`和`--help`来打印命令行的帮助

```bash
./a.out -h       # 调用argparse_print_parser()
./a.out list -h  # 调用argparse_print_command(NULL)
```
