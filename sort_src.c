#include "include/my_sort.h"

struct sort_option g_op;
struct all_data g_filedata;

void sort_usage_exit(int status) {
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

void init_data() { memset(&g_filedata, 0, sizeof(g_filedata)); }

void init_option(struct sort_option *op) {
  op->n = 0;
  op->r = 0;
  op->t = '\t';
  op->k = 0;
  op->f = 0;
}

void fill_t(struct sort_option *op, const char *str) {
  if (strlen(str) > 1) {
    fprintf(stderr, "[%s]is error, only set one charactor\n", str);
    exit(1);
  }
  op->t = str[0];
}

void fill_k(struct sort_option *op, char *str) {
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
size_t get_simple_file_size(const char *file) {
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

int fill_line(const char *line_data) {
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
int fill_data(char *pdata) {
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

void pr_sorteddata() {
  int i;

  for (i = 0; i < g_filedata.used; ++i) {
    if (write(STDOUT_FILENO, g_filedata.data_arr[i].base,
              g_filedata.data_arr[i].size) != g_filedata.data_arr[i].size)
      perror("write");
    if (write(STDOUT_FILENO, "\n", 1) != 1)
      perror("write2");
  }
}

void get_field(char *buf) {
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
int cmp_str(const void *arg1, const void *arg2) {
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
void sort_data() {
  qsort(g_filedata.data_arr, g_filedata.used, sizeof(struct one_line), cmp_str);
}

/**
 * @brief 对指定文本文件进行排序
 *
 * @param file
 * @return int
 */
int sort(const char *file) {
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