/**
 * @file copy.h
 * @brief 复制文件
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _COMMAND_COPY_H_
#define _COMMAND_COPY_H_

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common_data.h"
#include "file_isexist.h"
#include "get_user_confirmation.h"
#include "my_rm.h"

/**
 * @brief cp和mv的参数
 */
struct cp2mv_option {
  char r; // 是否递归
  char l; // 是否是复制链接文件，而非实现
  char f; // 当目标文件存在，不提示直接覆盖，默认是要提示的
};

#define MAX_NAME_LEN 256

/**
 * @brief 复制函数
 *
 * @param src_file 指向需要复制的源文件
 * @param dst_file 指向复制文件的目标文件
 * @param is_force 当目标文件存在时，是否是强制覆盖
 * @param deal_link 是否复制链接文件
 * @param need_del 是否删除源文件
 * @return int
 */
int copy(const char *src_file, const char *dst_file, int is_force,
         int deal_link, int need_del);

/**
 * @brief 由于参数错误退出cp命令和mv命令，附带正确用法的提示
 *
 * @param status
 */
void cp2mv_usage_exit(int status);

/**
 * @brief 复制文件
 *
 * @param psrc 源文件
 * @param pdst 目的地
 * @param option 参数选项
 * @param needdel
 */
void cp_file(char *psrc, char *pdst, struct cp2mv_option *option, int needdel);

/**
 * @brief 递归复制
 *
 * @param psrc 源文件夹
 * @param pdst 目的地
 * @param option 参数选项
 * @param need_del
 */
void cp_r(char *psrc, char *pdst, struct cp2mv_option *option, int need_del);

/**
 * @brief 对外开放的cp、mv接口
 *
 * @param psrc 源
 * @param pdst 目的地
 * @param option 选项
 * @param needdel 是否删除源
 */
void cp(char *psrc, char *pdst, struct cp2mv_option *option, int needdel);

#endif
