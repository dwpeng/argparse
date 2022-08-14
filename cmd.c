#include "src/argparse.h"

static CommandArg list_args[] = {{.flag = "file"}, {.flag = "kmer"}, {0}};

void* handle_list(int nargs, CommandArg* args) {
  CommandArg arg;
  for (int i = 0; i < nargs; i++) {
    arg = args[i];
    printf("%s\n", arg.flag);
  }
}

int main(int argc, char* argv[]) {
  //   init_parser("ROA", "test");
  argparse_init_parser("Test\n", "This is a test\n", "This is a usage\n");

  //   add_command("list", "This is a list command.", handle_list, list_args);
  //   parse_args(argc, argv);
  argparse_print_parser();
}
