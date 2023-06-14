#include "include/err_msg.h"


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