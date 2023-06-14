/**
 * @file rm.c
 * @brief windows rd命令
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "include/remove.h"
#include <getopt.h>
#include <stdlib.h>

#include "common_data.h"

static void rm_usage_exit(int status) {
  if (status != EXIT_SUCCESS) {
    fprintf(stderr, "Try '%s --help' for more infomation\n", program_name);
  } else {
    printf("Usage: %s  FILE[DIRECOTORY] OPTION\n", program_name);
    printf("-h  --help(--h) show the help infomation\n");
    printf("-f[-F]  --force(--f) force to remove without prompt\n");
    printf("-r[-R]  --recure(--r) remove directories and their contents "
           "recursively\n");
  }

  exit(status);
}

int main(int argc, char *argv[]) {
  int c;
  struct rm_option rmoption;

  memset(&rmoption, 0, sizeof(rmoption));

  exit_status = 0;
  static const struct option long_options[] = {{"help", 0, NULL, 'h'},
                                               {"recure", 0, NULL, 'r'},
                                               {"fource", 0, NULL, 'f'},
                                               {NULL, 0, NULL, 0}};

  program_name = argv[0];
  while ((c = getopt_long(argc, argv, "hHrRfF", long_options, NULL)) != -1) {
    switch (c) {
    case 'h':
    case 'H':
      rm_usage_exit(EXIT_SUCCESS);
      break;
    case 'r':
    case 'R':
      rmoption.r = 1;
      break;
    case 'f':
    case 'F':
      rmoption.f = 1;
      break;
    default:
      rm_usage_exit(EXIT_FAILURE);
      break;
    }
  }

  if (optind == argc) {
    rm_usage_exit(1);
  }

  while (optind < argc) {
    exit_status |= rm(argv[optind++], &rmoption);
  }

  exit(exit_status);
}
