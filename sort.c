/**
 * @file sort.c
 * @brief windows sort命令
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common_data.h"
#include "include/is_digit.h"
#include "include/str_match.h"

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
  size_t used; // 用于统计行数
};

struct sort_option g_op;
struct all_data g_filedata;

static void sort_usage_exit(int status) {
  if (status != 0) {
    fprintf(stderr, "Try '%s --help' for more infomation\n", program_name);
  } else {
    printf("Usage: %s   [OPTION] FILE\n", program_name);
    printf("-n              Sort by number instead of string\n");
    printf("-r              Reverse sort\n");
    printf("-t,             Separator, default is '\\t'\n");
    printf("-k,             Sort by which sector, default is 0\n");
    printf("-f,             Ignore case\n");
    printf("-h(-H)  --help  Show help information\n");
  }

  exit(status);
}

static void init_data() { memset(&g_filedata, 0, sizeof(g_filedata)); }

static void init_option(struct sort_option *op) {
  op->n = 0;
  op->r = 0;
  op->t = '\t';
  op->k = 0;
  op->f = 0;
}

static void fill_t(struct sort_option *op, const char *str) {
  if (strlen(str) > 1) {
    fprintf(stderr, "[%s]is error, only set one charactor\n", str);
    exit(1);
  }
  op->t = str[0];
}

static void fill_k(struct sort_option *op, char *str) {
  if (!is_digit(str, strlen(str))) {
    fprintf(stderr, "[%s] is not a number\n", str);
    exit(1);
  }

  op->k = atoi(str);
}

/**
 * @brief 获取普通文件的大小
 *
 * @param file
 * @return size_t
 */
static size_t get_simple_file_size(const char *file) {
  struct stat stabuf;

  if (stat(file, &stabuf) != 0) {
    fprintf(stderr, "stat [%s] error[%s]\n", file, strerror(errno));
    exit(1);
  }

  if (!S_ISREG(stabuf.st_mode)) {
    fprintf(stderr, "file [%s] is not a simple file\n", file);
    exit(1);
  }

  return stabuf.st_size;
}

static int fill_line(const char *line_data) {
  size_t index = g_filedata.used;

  if (index == MAX_LINE_NUM) {
    return 1;
  }

  g_filedata.data_arr[index].base = (char *)line_data;
  g_filedata.data_arr[index].size = strlen(line_data);
  g_filedata.data_arr[index].line_no = g_filedata.used++;

  return 0;
}

/**
 * @brief 从mmap的映射区域取走文本内容并按行分割，最后存入全局变量g_filedata中
 *
 * @param pdata
 * @return int
 */
static int fill_data(char *pdata) {
  char *line_data;

  init_data();
  line_data = strtok(pdata, "\n"); // 按行分割
  while (line_data != NULL) {
    if (fill_line(line_data)) {
      break;
    }
    line_data = strtok(NULL, "\n");
  }

  return 0;
}

static void pr_sorteddata() {
  int i;

  for (i = 0; i < g_filedata.used; ++i) {
    if (write(STDOUT_FILENO, g_filedata.data_arr[i].base,
              g_filedata.data_arr[i].size) != g_filedata.data_arr[i].size)
      perror("write");
    if (write(STDOUT_FILENO, "\n", 1) != 1)
      perror("write2");
  }
}

static void get_field(char *buf) {
  char sep[2];
  char *line_data;
  int field_num = (int)g_op.k;

  sprintf(sep, "%c", g_op.t);

  line_data = strtok(buf, sep);
  while (line_data != NULL && field_num-- > 0) {
    line_data = strtok(NULL, sep);
  }

  if (line_data) {
    memmove(buf, line_data, strlen(line_data));
    buf[strlen(line_data)] = 0;
  } else
    buf[0] = 0;
}

/**
 * @brief 排序依据
 *
 * @param arg1
 * @param arg2
 * @return int
 */
static int cmp_str(const void *arg1, const void *arg2) {
  int rc;
  char buf1[MAX_LINE_BUF_SIZE + 1], buf2[MAX_LINE_BUF_SIZE + 1];

  memset(buf1, 0, sizeof(buf1));
  memset(buf2, 0, sizeof(buf2));

  snprintf(buf1, MAX_LINE_BUF_SIZE, "%s", *(char **)arg1);
  snprintf(buf2, MAX_LINE_BUF_SIZE, "%s", *(char **)arg2);

  // 更新buf1, buf2中内容为需要比较的行
  get_field(buf1);
  get_field(buf2);

  if (g_op.n && is_digit(buf1, strlen(buf1)) && is_digit(buf2, strlen(buf2)))
    rc = atoi(buf1) - atoi(buf2);
  else if (g_op.f)
    rc = strcasecmp(buf1, buf2);
  else
    rc = strcmp(buf1, buf2);

  return g_op.r ? -rc : rc;
}

/**
 * @brief 对外开放的排序接口
 *
 */
static void sort_data() {
  qsort(g_filedata.data_arr, g_filedata.used, sizeof(struct one_line), cmp_str);
}

/**
 * @brief 对指定文本文件进行排序
 *
 * @param file
 * @return int
 */
static int sort(const char *file) {
  size_t file_size = get_simple_file_size(file);
  char *p_map_file = NULL;
  int fd = -1, err = 0;

  if ((fd = open(file, O_RDONLY)) < 0) {
    // O_RDONLY只读模式
    perror("open file");
    exit(1);
  }

  if ((p_map_file = (char *)mmap(0, file_size, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
    // 返回void*,应强制转换
    perror("mmap");
    exit(1);
  }
  if ((err = fill_data(p_map_file)) != 0) {
    goto end;
  }

  sort_data();
  pr_sorteddata();

end:
  if (p_map_file != NULL) {
    munmap(p_map_file, file_size);
  }
  if (fd != -1) {
    close(fd);
  }

  return err;
}

int main(int argc, char *argv[]) {
  int c;

  static const struct option long_options[] = {{"help", 0, NULL, 'h'},
                                               {NULL, 0, NULL, 0}};
  init_option(&g_op);
  exit_status = EXIT_SUCCESS;
  program_name = argv[0];
  while ((c = getopt_long(argc, argv, "hHnrt:k:f", long_options, NULL)) != -1) {
    switch (c) {
    case 'h':
    case 'H':
      sort_usage_exit(EXIT_SUCCESS);
      break;
    case 'n':
      g_op.n = 1;
      break;
    case 'r':
      g_op.r = 1;
      break;
    case 't':
      if (optarg)
        fill_t(&g_op, optarg);
      break;
    case 'k':
      if (optarg)
        fill_k(&g_op, optarg);
      break;
    case 'f':
      g_op.f = 1;
      break;
    default:
      sort_usage_exit(EXIT_FAILURE);
      break;
    }
  }

  if (argc - optind != 1)
    sort_usage_exit(EXIT_FAILURE);

  if (sort(argv[optind]) != 0)
    exit_status = EXIT_FAILURE;

  exit(exit_status);
}
