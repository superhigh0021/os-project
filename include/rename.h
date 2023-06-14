#ifndef _RENAME_H_
#define _RENAME_H_

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "file_isexist.h"

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

#endif
