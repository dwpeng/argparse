#include "argparse.h"

/* 初始化全局唯一的解析器 */
struct ArgumentParser _parser = {0};
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
  for (int i = 0; i < n; i++) {
    ncmd[i] = __create_command();
    if (!ncmd[i]) {
      goto clean;
    }
  }
  return ncmd;

clean:
  for (int i = 0; i < n; i++) {
    if (ncmd[i]) {
      free(ncmd[i]);
    }
  }
  free(ncmd);
}

static inline void __update_parser_by_command(Command *cmd) {
  /* 更新parser */
  /* 检查是否还有空间添加新的命令 */
  if (!__parser->fcmd) {
    // 每次扩大4个空间
    Command **cmds = __create_ncommand(__parser->ncmd + 4);
    memcpy(cmds, __parser->cmd, sizeof(Command *) * __parser->ncmd);
    __parser->cmd = cmds;
    __parser->fcmd += 4;
    free(__parser->cmd);
  }
  __parser->cmd[__parser->ncmd] = cmd;
  __parser->ncmd++;
  __parser->fcmd--;
}

/**
 * 打印子命令信息
 */
void argparse_print_command() {}

/***
 * 打印解析器
 */
void argparse_print_parser() {
  printf("%s", __parser->prog);
  printf("%s", __parser->description);
  printf("%s", __parser->usage);
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
}

void *argparse_add_command(char *command_name, char *description, char *usage,
                           callback_t callback, CommandArg *args) {

  Command *cmd;
  cmd = __create_command();
  if (!cmd) {
    return NULL;
  }

  cmd->name = command_name;
  cmd->description = description;
  cmd->callback = callback;
  cmd->args = args;
  cmd->required_nargs = 0;

  /* 检查每一个参数的情况 */
  CommandArg arg;
  int c = 0;
  while (1) {
    arg = args[0];
    if (!arg.flag) {
      break;
    }

    if (arg.flag[0] != '-') {
      cmd->required_nargs++;
    } else {
      /* 检查参数的唯一性 */
      // TODO
    }
    c++;
  }
  cmd->nargs = c - 1;
  __update_parser_by_command(cmd);
}

void argparse_parse_args(int argc, char *argv[]) {
  if (argc < 2) {
    argparse_print_parser();
    return;
  }
  for (int i = 1; i < argc; i++) {
    while (1) {
    }
  }
}
