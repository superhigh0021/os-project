#ifndef _RENAME_H_
#define _RENAME_H_

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "common_data.h"
#include "file_isexist.h"

void rename_usage_exit(int status);

/**
 * @brief 重命名函数
 *
 * @param old_name
 * @param new_name
 * @return int  =0为成功
 *              其他为失败
 */
int _rename(char *old_name, char *new_name);

#endif
