#ifndef _MK_DIR_H_
#define _MK_DIR_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common_data.h"
#include "file_isexist.h"

/**
 * @brief 创建目录时参数选项出现问题时的提示
 *
 * @param status
 */
void mkdir_usage_exit(int status);

/**
 * @brief 创建目录
 *
 * @param path 路径名
 * @param mode 目录权限
 * @return int  =0   成功
 *              >0   操作系统的失败(返回errno)
 *              <0   用户层的失败
 */
int my_mkdir(const char *path, const mode_t mode);

#endif
