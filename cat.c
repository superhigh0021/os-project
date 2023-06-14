/**
 * @file cat.c
 * @brief windows type命令
 * @version 0.1
 * @date 2023-05-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common_data.h"

static char *output_fd = NULL; // 指向输入文件
static int input_fd = -1;      // 输入文件描述符

/**
 * @brief 提示该命令的用法
 *
 * @param status
 */
static void usage(int status) {
  if (status != 0) {
    fprintf(stderr, "Try '%s --help' for more infomation\n", program_name);
  } else {
    fprintf(stderr, "Usage: %s  [FILE] ...\n", program_name);
    fprintf(stderr, "with no FILE or when FILE is -, %s will read from stdin\n",
            program_name);
  }

  exit(status);
}

/**
 * @brief 输出到屏幕
 *
 * @param fileno
 * @param needexit
 */
static void _cat(int fileno, int needexit) {
  char buf[MAX_LINE_BUF_SIZE];
  int n;

  while ((n = read(fileno, buf, MAX_LINE_BUF_SIZE)) > 0) {
    if (write(STDOUT_FILENO, buf, n) != n) {
      perror("write error");
      exit(exit_status);
    }
  }

  if (n < 0) {
    perror("in read");
    if (!needexit)
      exit(exit_status);
  }

  if (needexit) {
    exit_status = EXIT_SUCCESS;
    exit(exit_status);
  }
}

static void simple_cat(char *file) {
  struct stat statbuf;

  if (file == NULL || file[0] == 0) {
    // sprintf(stderr, "some asset error happend\n");
    exit(exit_status);
  }

  input_fd = open(file, O_RDONLY);
  if (input_fd < 0) {
    fprintf(stderr, "open file '%s' error: %s\n", file, strerror(errno));
    exit(exit_status);
  }

  if (fstat(input_fd, &statbuf) < 0) {
    fprintf(stderr, "file state'%s' error: %s\n", file, strerror(errno));
    close(input_fd);
    exit(exit_status);
  }

  if (S_ISREG(statbuf.st_mode) == 0) {
    fprintf(stderr, "file '%s' is not a text document\n", file);
    close(input_fd);
    exit(exit_status);
  }

  _cat(input_fd, 0);
  close(input_fd);
}

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
