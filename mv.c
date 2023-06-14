/**
 * @file mv.c
 * @brief windows move命令
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "include/copy.h"
#include <getopt.h>
#include <stdio.h>

#include "common_data.h"

// 用户有参数与cp的参数是一样的
struct cp2mv_option g_option;

int main(int argc, char *argv[]) {
  int c;
  // 除开参数外，要传入SOURCE DST的个数
  const int diff_optind = 2;
  char *psrc, *pdst;

  static const struct option long_options[] = {{"help", 0, NULL, 'h'},
                                               {"recursive", 0, NULL, 'r'},
                                               {"link", 0, NULL, 'l'},
                                               {"fource", 0, NULL, 'f'},
                                               {NULL, 0, NULL, 0}};

  program_name = argv[0];

  memset(&g_option, 0, sizeof(g_option));
  while ((c = getopt_long(argc, argv, "hHrRlLfF", long_options, NULL)) != -1) {
    switch (c) {
    case 'h':
    case 'H':
      cp2mv_usage_exit(EXIT_SUCCESS);
      break;
    case 'r':
    case 'R':
      g_option.r = 1;
      break;
    case 'l':
    case 'L':
      g_option.l = 1;
      break;
    case 'f':
    case 'F':
      g_option.f = 1;
      break;
    default:
      cp2mv_usage_exit(EXIT_FAILURE);
      break;
    }
  }

  if (argc - optind != diff_optind)
    cp2mv_usage_exit(EXIT_FAILURE);

  psrc = argv[optind++];
  pdst = argv[optind++];
  assert(psrc != NULL && psrc[0] != 0);
  assert(pdst != NULL && pdst[0] != 0);

  cp(psrc, pdst, &g_option, 1);

  exit_status = EXIT_SUCCESS;
  exit(exit_status);
}
