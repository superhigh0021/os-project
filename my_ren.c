/**
 * @file rename.c
 * @brief windows ren命令
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/my_ren.h"
#include <getopt.h>

int main(int argc, char *argv[]) {
  int c;
  const int diff_optind = 2; // 除开选项参数外，要传入的参数个数
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
