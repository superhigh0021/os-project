/**
 * @file rename.c
 * @brief windows ren命令
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/rename.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_data.h"

void rename_usage_exit(int status) {
  if (status != EXIT_SUCCESS) {
    fprintf(stderr, "Try '%s --help' for more infomation\n", program_name);
  } else {
    printf("Usage: %s oldname newname\n", program_name);
  }

  exit(status);
}

int main(int argc, char *argv[]) {
  int c;
  // 除开选项参数外，要传入的参数个数
  const int diff_optind = 2;
  char *pold, *pnew;

  static const struct option long_options[] = {{"help", 0, NULL, 'h'},
                                               {NULL, 0, NULL, 0}};

  program_name = argv[0];
  while ((c = getopt_long(argc, argv, "hH", long_options, NULL)) != -1) {
    switch (c) {
    case 'h':
    case 'H':
      rename_usage_exit(EXIT_SUCCESS);
      break;
    default:
      rename_usage_exit(EXIT_FAILURE);
      break;
    }
  }

  if (argc - optind != diff_optind)
    rename_usage_exit(EXIT_FAILURE);

  pold = argv[optind++];
  pnew = argv[optind++];

  exit_status = _rename(pold, pnew);

  exit(exit_status);
}
