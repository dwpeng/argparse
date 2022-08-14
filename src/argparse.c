#include "argparse.h"

/* 初始化全局唯一的解析器 */
struct ArgumentParser _parser = {"", ""};
struct ArgumentParser *__parser = &_parser;

static inline Command *__create_command() {
  Command *cmd;
  if ((cmd = malloc(sizeof(Command))) == NULL) {
    return NULL;
  }
  return cmd;
}

static inline Command **__create_ncommand(int n) {
  Command **ncmd;
  if ((ncmd = malloc(sizeof(Command *) * n)) == NULL) {
    return NULL;
  }
  return ncmd;
}

static inline void __update_parser_by_command(Command *cmd) {
  /* 更新parser */
  /* 检查是否还有空间添加新的命令 */
  /* 为通用参数直接放到通用参数 */
  if ((void *)cmd->name == NULL || !strlen(cmd->name)) {
    __parser->generic_cmd = cmd;
  } else if (!__parser->fcmd) {
    // 每次扩大4个空间
    Command **cmds = __create_ncommand(__parser->ncmd + 4);
    if (__parser->cmd) {
      memcpy(cmds, __parser->cmd, sizeof(Command *) * __parser->ncmd);
      free(__parser->cmd);
    }
    __parser->cmd = cmds;
    __parser->fcmd += 4;
  }
  __parser->cmd[__parser->ncmd] = cmd;
  __parser->ncmd++;
  __parser->fcmd--;
}

/**
 * 打印子命令信息
 */
void argparse_print_command(Command *cmd) {
  printf("%s", cmd->name);
  printf("%s", SAFE_STR(cmd->description));
  printf("%s\n", SAFE_STR(cmd->usage));
  for (int i = 0; i < cmd->nargs; i++) {
    printf("    %s", cmd->args[i].flag);
    printf("\t%s", SAFE_STR(cmd->args[i].help));
    printf("\n");
  }
}

/***
 * 打印解析器
 */
void argparse_print_parser() {
  printf("\n");
  printf("%s", SAFE_STR(__parser->prog));
  printf("%s", SAFE_STR(__parser->description));
  printf("%s\n", SAFE_STR(__parser->usage));
  if (__parser->generic_cmd) {
    printf("Generic argument\n");
    argparse_print_command(__parser->generic_cmd);
    printf("\n");
  }
  if (__parser->ncmd) {
    printf("Commands:\n");
    for (int i = 0; i < __parser->ncmd; i++) {
      argparse_print_command(__parser->cmd[i]);
    }
  }
  printf("\n");
}

/***
 * 初始化参数解析器
 * 为解析器提供以下字段
 * 1. 解析器的名字
 * 2. 解析器的描述
 * 3. 解析器的使用方法
 */
void argparse_init_parser(char *prog, char *description, char *usage) {
  __parser->prog = prog;
  __parser->description = description;
  __parser->usage = usage;
  __parser->ncmd = 0;
  __parser->fcmd = 0;
  __parser->generic_cmd = NULL;
  __parser->cmd = NULL;
  __parser->now_command[0] = NULL;
  __parser->now_command[1] = NULL;
}

void *argparse_add_command(char *command_name, char *description, char *usage,
                           callback_t callback, CommandArg *args) {

  Command *cmd;
  if ((cmd = __create_command()) == NULL) {
    goto clean;
  }
  cmd->name = command_name;
  cmd->description = description;
  cmd->usage = usage;
  cmd->callback = callback;
  cmd->args = args;
  cmd->required_nargs = 0;
  cmd->nargs = 0;
  /* 检查每一个参数的情况 */
  CommandArg arg;
  int c = 0;
  int bitmap_args = 0x0000;
  char *arg_flag = NULL;
  while (1) {
    arg = args[c];
    if ((void *)arg.flag == NULL)
      break;
    if (arg.flag[0] != '-') {
      cmd->required_nargs++;
    } else {
      /* 检查参数的唯一性 */
      arg_flag = arg.flag;
      while (arg_flag[0] == '-')
        arg_flag++;
      if (!strlen(arg_flag)) {
        printf("Blank arg: <%s> found. Check again.\n", arg.flag);
        goto clean;
      }
      int bitmap_arg_pos = (0x0001 << (arg_flag[0] - 61));
      if ((bitmap_args & bitmap_arg_pos) == bitmap_arg_pos) {
        printf("Duplicated arg: <%s> found. Check again.\n", arg.flag);
        goto clean;
      }
      bitmap_args |= bitmap_arg_pos;
    }
    c++;
  }
  cmd->nargs = c;
  __update_parser_by_command(cmd);
  return cmd;

clean:
  for (int i = 0; i < __parser->ncmd; i++) {
    free(__parser->cmd[i]);
  }
  free(__parser->cmd);
  __parser->cmd = NULL;
  return NULL;
}

void argparse_parse_args(int argc, char *argv[]) {
  if (argc < 2) {
    argparse_print_parser();
    return;
  }
  /* 更新为当前的now_command */
  __parser->now_command[0] = __parser->generic_cmd;
  /* 查看是否为子命令 */
  if (argv[1][0] != '-') {
    for (int i = 0; i < __parser->ncmd; i++) {
      /* 这里和子命令做比对 */
      if (strcmp(argv[1], __parser->cmd[i]->name) == 0) {
        __parser->now_command[1] = __parser->cmd[i];
        break;
      }
    }
    
    /* 没有找到对应的子命令，可能是一个错误的子命令 */
    if (__parser->now_command[1] == NULL) {
      printf("Get Wrong subcommand: <%s>. Check again.\n", argv[1]);
      argparse_print_parser();
      goto clean;
    }
  }

//   for (int i = 1; i < argc; i++) {
//     while (1) {
//       /*先去通用参数里面去查找*/
//       if (__parser->generic_cmd) {
//       }
//       /* 解析子命令 */
//       if (__parser->ncmd) {
//         while (1) {
//         }
//       }
//     }
//   }
  goto clean;

clean:
  /* 释放所有的command */
  if (__parser->generic_cmd) {
    free(__parser->generic_cmd);
  }
  for (int i = 0; i < __parser->ncmd; i++) {
    free(__parser->cmd[i]);
  }
  free(__parser->cmd);
}
