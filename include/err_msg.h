/**
 * @file err_msg.h
 * @brief 错�??信息
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

 void pr_msg(const char *fmt, ...) {
  va_list va;

  va_start(va, fmt);
  vfprintf(stderr, fmt, va);
  fflush(stderr);
  va_end(va);
}

/*
    FIXME:
        这里看起来可能调pr_msg的，但这�?�?变参数我还不清�?�具体去调用
*/
 void pr_msg2exit(const int exit_status, const char *fmt, ...) {
  va_list va;

  va_start(va, fmt);
  vfprintf(stderr, fmt, va);
  fflush(stderr);
  va_end(va);

  exit(exit_status);
}

 void pr_errno_msg(const int err_no) {
  if (err_no > 0) {
    fprintf(stderr, "%s\n", strerror(err_no));
    fflush(stderr);
  }
}

 void pr_errno_msg2exit(const int err_no) {
  pr_errno_msg(err_no);

  exit(err_no);
}

#endif
