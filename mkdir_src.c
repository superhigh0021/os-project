#include "include/my_mkdir.h"


/**
 * @brief 创建目录时参数选项出现问题时的提示
 *
 * @param status
 */
void mkdir_usage_exit(int status) {
  if (status != 0) {
    fprintf(stderr, "Try '%s --help' for more infomation\n", program_name);
  } else {
    printf("Usage: %s [OPTION]DIR ...\n", program_name);
  }

  exit(status);
}

/**
 * @brief 创建目录
 *
 * @param path 路径名
 * @param mode 目录权限
 * @return int  =0   成功
 *              >0   操作系统的失败(返回errno)
 *              <0   用户层的失败
 */
 int my_mkdir(const char *path, const mode_t mode) {
  int save_errno, ret;

  if (path == NULL || path[0] == 0) {
    return -1;
  }
  save_errno = errno;
  if (mkdir(path, mode) != 0) {
    ret = errno;
    errno = save_errno;
    fprintf(stderr, "mkdir [%s] error [%s]\n", path, strerror(ret));

    return ret;
  }

  return 0;
}