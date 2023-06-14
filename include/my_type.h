#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common_data.h"

/**
 * @brief 提示该命令的用法
 *
 * @param status
 */
void usage(int status);

/**
 * @brief 输出到屏幕
 *
 * @param fileno
 * @param needexit
 */
void _cat(int fileno, int needexit);

void simple_cat(char *file);