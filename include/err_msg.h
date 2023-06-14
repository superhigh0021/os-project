/**
 * @file err_msg.h
 * @brief 错误信息
 * @version 0.1
 * @date 2023-05-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _ERR_MSG_H_
#define _ERR_MSG_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pr_msg(const char *fmt, ...);

/*
    FIXME:
        这里看起来可能调pr_msg的，但这�?�?变参数我还不清�?�具体去调用
*/
void pr_msg2exit(const int exit_status, const char *fmt, ...);

void pr_errno_msg(const int err_no);

void pr_errno_msg2exit(const int err_no);

#endif
