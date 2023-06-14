#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief error错误处理
 *
 * @param ptr
 */
void derror(const char *ptr);

/**
 * @brief 读取目录中的文件
 *
 * @param pathname
 * @param flag
 */
void dirs(const char *pathname, bool flag[]);

/**
 * @brief 判断并打印文件类型
 *
 * @param get_message
 */
void ftype(const struct stat *get_message);
/**
 * @brief 打印文件权限
 *
 * @param mode
 * @param type
 * @param ch
 */
void fpower_char(mode_t mode, mode_t type, const char ch);

/**
 * @brief 用户id与组id转换成用户名与组名并打印
 *
 * @param get_message
 */
void getid(const struct stat *get_message);

/**
 * @brief 将GMT时间的秒数转换成标准时间格式输出
 *
 * @param get_message
 */
void showtime(const struct stat *get_message);

void showlist(const char *filename, const struct stat *get_message);