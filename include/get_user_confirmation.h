/**
 * @file get_user_confirmation.h
 * @brief 从标准输入读入是否是yes与no,直到读到yes或者no或者程序退出才返回
 * @version 0.1
 * @date 2023-06-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _COMMAND_YES_NO_H_
#define _COMMAND_YES_NO_H_

#include <ctype.h>
#include <stdio.h>

/*
    返回值:
        ==  1 从标准输入读了yes
        ==  0 从标准输入读入了no
*/
int getUserConfirmation();

#endif
