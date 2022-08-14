#include "src/argparse.h"

static CommandArg list_args[] = {{.flag = "file"},
                                 {.flag = "kmer"},
                                 {.flag = "--help"},
                                 {.flag = "--size"},
                                 {0}};

void *handle_list(int nargs, CommandArg *args) {
  CommandArg arg;
  for (int i = 0; i < nargs; i++) {
    arg = args[i];
    printf("%s\n", arg.flag);
  }
}

int main(int argc, char *argv[]) {
  argparse_init_parser("Test\n", "This is a test\n", "This is a usage\n");

  argparse_add_command("list", "This is a list command.", "This is a usage.",
                       handle_list, list_args);
  argparse_parse_args(argc, argv);
}

