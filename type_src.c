#include "include/my_type.h"

char *output_fd = NULL; // 指向输入文件
int input_fd = -1;      // 输入文件描述符

/**
 * @brief 提示该命令的用法
 *
 * @param status
 */
void usage(int status) {
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
void _cat(int fileno, int needexit) {
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

void simple_cat(char *file) {
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
