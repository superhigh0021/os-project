/**
 * @file my_type.c
 * @brief windows type命令
 * @version 0.1
 * @date 2023-05-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/my_type.h"

extern char *output_fd;
extern int input_fd;

int main(int argc, char *argv[]) {
  struct stat stat_buf;
  int c;
  const static struct option long_options[] = {{"help", no_argument, NULL, 'h'},
                                               {NULL, no_argument, NULL, 0}};

  program_name = argv[0];

  while ((c = getopt_long(argc, argv, "h", long_options, NULL)) != -1) {
    switch (c) {
    case 'h':
      usage(EXIT_SUCCESS);
      break;
    default:
      usage(EXIT_FAILURE);
      break;
    }
  }

  if (optind < argc) {
    while (optind < argc) {
      output_fd = argv[optind++];
      simple_cat(output_fd);
    }
  }

  exit_status = EXIT_SUCCESS;

  exit(exit_status);
}
