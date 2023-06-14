/**
 * @file my_copy.c
 * @brief windows xcopy命令
 * @version 0.1
 * @date 2023-06-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <dirent.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/my_copy.h"

// 用户参数说明
struct cp2mv_option g_optin;

static void cp_exit(void) {}

int main(int argc, char *argv[]) {
  int c;
  const int diff_optind = 2; // 除开参数外，要传入SOURCE DST的个数
  char *psrc, *pdst;

  static struct option const long_options[] = {{"help", 0, NULL, 'h'},
                                               {"recursive", 0, NULL, 'r'},
                                               {"link", 0, NULL, 'l'},
                                               {"fource", 0, NULL, 'f'},
                                               {NULL, 0, NULL, 0}};

  program_name = argv[0];
  atexit(cp_exit);

  memset(&g_optin, 0, sizeof(g_optin));
  while ((c = getopt_long(argc, argv, "hHrRlLfF", long_options, NULL)) != -1) {
    switch (c) {
    case 'h':
    case 'H':
      cp2mv_usage_exit(EXIT_SUCCESS);
      break;
    case 'r':
    case 'R':
      g_optin.r = 1;
      break;
    case 'l':
    case 'L':
      g_optin.l = 1;
      break;
    case 'f':
    case 'F':
      g_optin.f = 1;
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

  cp(psrc, pdst, &g_optin, 0);

  exit_status = EXIT_SUCCESS;
  exit(exit_status);
}
