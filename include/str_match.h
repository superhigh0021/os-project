#ifndef _STR_MATCH_H_
#define _STR_MATCH_H_

#include "./err_msg.h"
#include <pcre.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define OVECCOUNT 30

struct finded_str {
  unsigned int begin;
  unsigned int end;
};

/**
 * @brief
 *
 * @param match_str 模式串
 * @param src_str 源字符串
 * @return int
 */
int is_matched(const char *match_str, const char *src_str);

/*
    返回�?:
    <   0   出错
    ==  0   没找�?
    >   0   找到了几�?结果
*/
int find_str(char *match_str,           /*  待查找的字�?�串  */
             char *str_src,             /*  源字符串        */
             struct finded_str *result, /*  打到的结�?      */
             size_t finded_str_count)   /*  存放结果的长�?  */
    ;

#endif
