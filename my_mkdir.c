/**
 * @file my_mkdir.c
 * @brief windows MD创建目录
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/my_mkdir.h"

#include <getopt.h>

int main(int argc, char *argv[]) {
  int c;
  int is_p = 0; // 是否递归创建目录
  mode_t mode;  // 创建目录的权限

  static const struct option long_options[] = {{"help", 0, NULL, 'h'},
                                               {NULL, 0, NULL, 0}};

  program_name = argv[0];
  while ((c = getopt_long(argc, argv, "hHpP", long_options, NULL)) != -1) {
    switch (c) {
    case 'h':
    case 'H':
      mkdir_usage_exit(EXIT_SUCCESS);
      break;
    case 'p':
    case 'P':
      is_p = 1;
      break;
    default:
      mkdir_usage_exit(EXIT_FAILURE);
      break;
    }
  }

  // 最终得到mode值为511，即二进制的111111111，可以使mode包含所有权限
  mode = 0;
  for (c = 0; c < 9; ++c) {
    mode |= 1 << c;
  }

  if (optind < argc) {
    while (optind < argc) {

      exit_status = my_mkdir(argv[optind++], mode);

      if (exit_status != 0)
        exit(exit_status);
    }
  } else {
    mkdir_usage_exit(EXIT_FAILURE);
  }

  exit_status = EXIT_SUCCESS;
  exit(exit_status);
}
