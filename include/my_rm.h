#ifndef _REMOVE_H_
#define _REMOVE_H_

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common_data.h"
#include "file_isexist.h"
#include "get_user_confirmation.h"

/**
 * @brief rm参数选项
 *
 */
struct rm_option {
  int r; // 递归删除
  int f; // 强制删除
};

void rm_usage_exit(int status);

/**
 * @brief 删除普通文件
 *
 * @param file
 * @param option
 * @return int
 */
int rm_file(const char *file, const struct rm_option *option);

/**
 * @brief 删除目录（递归函数）
 *
 * @param file_pre
 * @param option
 * @return int
 */
int rm_dir(const char *file_pre, const struct rm_option *option);

/**
 * @brief 对外开放的rm接口
 *
 * @param file 文件名
 * @param option 参数选项
 * @return int
 */
int rm(char *file, const struct rm_option *option);

#endif
