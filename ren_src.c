#include "include/my_ren.h"

void rename_usage_exit(int status) {
  if (status != EXIT_SUCCESS) {
    fprintf(stderr, "Try '%s --help' for more infomation\n", program_name);
  } else {
    printf("Usage: %s oldname newname\n", program_name);
  }

  exit(status);
}

/**
 * @brief 重命名函数
 *
 * @param old_name
 * @param new_name
 * @return int  =0为成功
 *              其他为失败
 */
int _rename(char *old_name, char *new_name) {
  assert(old_name != NULL && old_name[0] != 0);
  assert(new_name != NULL && new_name[0] != 0);

  if (file_isexist(new_name)) {
    fprintf(stderr, "dstfile [%s] is existed\n", new_name);

    return EEXIST;
  }

  if (rename(old_name, new_name) != 0) {
    fprintf(stderr, "rename [%s] to [%s] error: [%s]\n", old_name, new_name,
            strerror(errno));

    return errno;
  }

  return 0;
}