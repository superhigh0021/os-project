#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common_data.h"
#include "is_digit.h"
#include "str_match.h"

#define MAX_LINE_NUM 8192 // 能处理的最大行数

struct sort_option {
  unsigned char n; // 以数字进行排序(0:字母排序1:数据排序)
  unsigned char r; // 逆向排序
  unsigned char t; // 分隔符，默认为'\t'
  unsigned char k; // 以第几个区间排序，默认为0
  unsigned char f; // 忽略大小写，默认是要考虑大小写的
};

/**
 * @brief 一行文本的三项要素
 *
 */
struct one_line {
  char *base;     // 该行的文本内容
  size_t size;    // 该行长度
  size_t line_no; // 行号
};

/**
 * @brief 将文件的所有内容读入该struct
 *
 */
struct all_data {
  struct one_line data_arr[MAX_LINE_NUM]; // 行数组，存放所有行
  size_t used;                            // 用于统计行数
};

void sort_usage_exit(int status);

void init_data();

void init_option(struct sort_option *op);

void fill_t(struct sort_option *op, const char *str);

void fill_k(struct sort_option *op, char *str);

/**
 * @brief 获取普通文件的大小
 *
 * @param file
 * @return size_t
 */
size_t get_simple_file_size(const char *file);

int fill_line(const char *line_data);

/**
 * @brief 从mmap的映射区域取走文本内容并按行分割，最后存入全局变量g_filedata中
 *
 * @param pdata
 * @return int
 */
int fill_data(char *pdata);

void pr_sorteddata();

void get_field(char *buf);

/**
 * @brief 排序依据
 *
 * @param arg1
 * @param arg2
 * @return int
 */
int cmp_str(const void *arg1, const void *arg2);

/**
 * @brief 对外开放的排序接口
 *
 */
void sort_data();

/**
 * @brief 对指定文本文件进行排序
 *
 * @param file
 * @return int
 */
int sort(const char *file);