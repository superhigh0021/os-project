/**
 * @file file_isexist.h
 * @brief 判断文件是否存在
 * @version 0.1
 * @date 2023-05-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _COMMAND_FILE_ISEXIST_H_
#define _COMMAND_FILE_ISEXIST_H_

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @brief 判断文件（目录）是否存在
 *
 * @param filename
 * @return int
 */
bool file_isexist(const char *filename);

/*
    函数功能:
        判断文件是否存在,并且是普通文件
    返回值:
        ==  0   不存在
        ==  1   存在
*/
int file_isreg(const char *filename);

/**
 * @brief 判断是否是目录
 *
 * @param filename 文件名
 * @return int
 */
bool file_isdir(const char *filename);

/*
    得到文件大小
    返回值:
    <   0     出错
    >=  0     文件大小
*/
int get_file_size(int fd);

#endif
