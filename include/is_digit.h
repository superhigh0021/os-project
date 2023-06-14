/**
 * @file is_digit.h
 * @brief 判断是否为数字
 * @version 0.1
 * @date 2023-05-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _IS_DIGIT_H_
#define _IS_DIGIT_H_

#include <stddef.h>

/*
    判断是否为满足要求
    返回值
    ==  0   不满足要求
    ==  1   满足要求
*/
int is_in(char *ptr,   /*  待检测字符串                   */
          size_t size, /*  字符串长度，不包含结尾符   */
          int min,     /*  最小值(包含)                */
          int max      /*最大值(包含)                  */
);

/*
    判断是否为数据字符串
    返回值
    ==  0   不全为数字
    ==  1   全为数字
*/
int is_digit(char *ptr,  /*  待检测字符串             */
             size_t size /*  字符串长度，不包含结尾符 */
);

#endif
