/**
 * @file sort.c
 * @brief windows sort命令
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <getopt.h>

#include "include/my_sort.h"

extern struct sort_option g_op;
extern struct all_data g_filedata;

int main(int argc, char *argv[]) {
  int c;

  static const struct option long_options[] = {{"help", 0, NULL, 'h'},
                                               {NULL, 0, NULL, 0}};
  init_option(&g_op);
  exit_status = EXIT_SUCCESS;
  program_name = argv[0];
  while ((c = getopt_long(argc, argv, "hHnrt:k:f", long_options, NULL)) != -1) {
    switch (c) {
    case 'h':
    case 'H':
      sort_usage_exit(EXIT_SUCCESS);
      break;
    case 'n':
      g_op.n = 1;
      break;
    case 'r':
      g_op.r = 1;
      break;
    case 't':
      if (optarg)
        fill_t(&g_op, optarg);
      break;
    case 'k':
      if (optarg)
        fill_k(&g_op, optarg);
      break;
    case 'f':
      g_op.f = 1;
      break;
    default:
      sort_usage_exit(EXIT_FAILURE);
      break;
    }
  }

  if (argc - optind != 1)
    sort_usage_exit(EXIT_FAILURE);

  if (sort(argv[optind]) != 0)
    exit_status = EXIT_FAILURE;

  exit(exit_status);
}
