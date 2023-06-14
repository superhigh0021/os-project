/**
 * @file common_data.h
 * @brief 整个project共用的参数
 * @version 0.1
 * @date 2023-05-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _COMMAND_COMM_H_
#define _COMMAND_COMM_H_
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_BUF_SIZE 1024 // 默认的行缓冲大小

// 程序退出状态

static char *program_name = NULL;         // 程序名
static size_t exit_status = EXIT_FAILURE; // 程序退出状态

#endif
