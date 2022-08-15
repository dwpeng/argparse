#include "src/argparse.h"

static CommandArg list_args[] = {
    {.flag = "file"},
    {.flag = "kmer"},
    {.flag = "--help", .required = 1, .no_value = 1},
    {.flag = "--size", .no_value = 1},
    {0}};

void *handle_list(Command *cmd1, Command *cmd2) {

  //   if (cmd1) {
  //     argparse_print_command(cmd1);
  //   }
  if (cmd2) {
    for (int i = 0; i < cmd2->nargs; i++) {
      if (cmd2->args[i].no_value) {
        printf("%d\n", BOOL(cmd2->args[i].value));
      } else {
        printf("%s\n", cmd2->args[i].value);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  argparse_init_parser("Test\n", "This is a test\n", "This is a usage\n");

  argparse_add_command("list", "This is a list command.", "This is a usage.",
                       handle_list, list_args);
  //   argparse_add_command("", "22222222222222This is a list command.",
  //                        "This is a usage.", handle_list, NULL);
  argparse_parse_args(argc, argv);
}
