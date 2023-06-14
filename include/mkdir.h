#ifndef _MK_DIR_H_
#define _MK_DIR_H_

#include "./file_isexist.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * @brief 创建单层�?�?
 *
 * @param path �?径名
 * @param mode �?录权�?
 * @return int  =0   成功
 *              >0   操作系统的失�?(返回errno)
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

#endif
