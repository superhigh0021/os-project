#include "include/my_dir.h"

bool flag[4]; // 确定命令参数选项的全局变量

/**
 * @brief error错误处理
 *
 * @param ptr
 */
void derror(const char *ptr) {
  perror(ptr);
  exit(1);
}

/**
 * @brief 读取目录中的文件
 *
 * @param pathname
 * @param flag
 */
void dirs(const char *pathname, bool flag[]) {
  char filename[256], tname[256];
  char sort_name[256][256];
  struct stat var, tmp;
  struct stat sort_t[256];
  struct dirent *rreaddir = NULL; // 定义接收readdir函数返回的结构体变量

  DIR *ropendir = opendir(pathname); // 打开传递的目录
  if (ropendir == NULL) {
    derror("opendir");
  }

  if (flag[2] == 1) {
    // -t输出
    int num = 0;
    while ((rreaddir = readdir(ropendir)) != NULL) {
      if (strcmp(rreaddir->d_name, ".") && strcmp(rreaddir->d_name, "..")) {
        memset(filename, 0, 256);
        strcpy(filename, pathname);
        strcat(filename, "/");
        strcat(filename, rreaddir->d_name); // filename中是文件路径

        // 用stat函数读取fiename文件信息，并将结果写到sort_t[num]中
        int rstat = stat(filename, &sort_t[num]);

        if (rstat == -1) {
          printf("stat %s", filename);
          exit(1);
        }
        strcpy(sort_name[num], rreaddir->d_name); // 将文件名放入缓存
        num++;
      }
    }

    // 将sort中文件按st_mtime排序
    int i, j, t;
    for (i = 0; i < num - 1; i++) {
      t = i;
      for (j = i + 1; j < num; j++) {
        if (sort_t[t].st_mtime < sort_t[j].st_mtime)
          t = j;
      }
      tmp = sort_t[i];
      sort_t[i] = sort_t[t];
      sort_t[t] = tmp;
      // sort_name[i],sort_name[j]交换
      strcpy(tname, sort_name[i]);
      strcpy(sort_name[i], sort_name[t]);
      strcpy(sort_name[t], tname);
    }

    // 将排序后的文件输出
    for (i = 0; i < num; i++) {
      if (flag[1] == 1)
        printf("%ld\t", sort_t[i].st_ino);
      if (flag[0] == 1) {
        memset(filename, 0, 256);
        strcpy(filename, pathname);
        strcat(filename, "/");
        strcat(filename, sort_name[i]); // filename中是文件路径

        // 用stat函数读取fiename文件信息，并将结果写到var中
        int rstat = stat(filename, &var);
        if (rstat == -1) {
          perror(filename);
          exit(1);
        }
        showlist(filename, &var);
      }
      printf("%s", sort_name[i]);
      if (flag[0] == 1)
        printf("\n");
      else
        printf("\t");
    }
    printf("\n");
  } else {
    while ((rreaddir = readdir(ropendir)) != NULL) {
      if (strcmp(rreaddir->d_name, ".") && strcmp(rreaddir->d_name, "..")) {
        memset(filename, 0, 256);
        strcpy(filename, pathname);
        strcat(filename, "/");
        strcat(filename, rreaddir->d_name);

        // 用stat函数读取fiename文件信息，并将结果写到var中
        int rstat = stat(filename, &var);

        if (rstat == -1) {
          printf("stat %s", filename);
          exit(1);
        }

        if (flag[1] == 1) {
          // -i输出
          printf("%ld\t", var.st_ino);
        }
        if (flag[0] == 1) {
          // -l输出
          showlist(filename, &var);
        }
        printf("%s\n", rreaddir->d_name);
      }
    }
  }
}

/**
 * @brief 判断并打印文件类型
 *
 * @param get_message
 */
void ftype(const struct stat *get_message) {
  mode_t mode = get_message->st_mode;
  if (S_ISREG(mode)) {
    // 普通文件
    printf("-");
  } else if (S_ISDIR(mode)) {
    // 目录文件
    printf("d");
  }
}

/**
 * @brief 打印文件权限
 *
 * @param mode
 * @param type
 * @param ch
 */
void fpower_char(mode_t mode, mode_t type, const char ch) {
  if ((mode & type) == type)
    printf("%c", ch);
  else
    printf("-");
}

/**
 * @brief 判断并打印文件权限
 *
 * @param get_message
 */
void fpower(const struct stat *get_message) {
  mode_t mode = get_message->st_mode & 07777; // 取后四位

  fpower_char(mode, S_IRUSR, 'r'); // 判断user有无读权限
  fpower_char(mode, S_IWUSR, 'w'); // 判断user有无写权限
  fpower_char(mode, S_IXUSR, 'x'); // 判断user有无可执行权限
  fpower_char(mode, S_IRGRP, 'r'); // 判断group有无读权限
  fpower_char(mode, S_IWGRP, 'w'); // 判断group有无写权限
  fpower_char(mode, S_IXGRP, 'x'); // 判断group有无可执行权限
  fpower_char(mode, S_IROTH, 'r'); // 判断other有无读权限
  fpower_char(mode, S_IWOTH, 'w'); // 判断other有无写权限
  fpower_char(mode, S_IXOTH, 'x'); // 判断other有无可执行权限

  printf(" ");
}

/**
 * @brief 用户id与组id转换成用户名与组名并打印
 *
 * @param get_message
 */
void getid(const struct stat *get_message) {
  struct passwd *pwd; // 根据用户id获取用户名
  pwd = getpwuid(get_message->st_uid);
  printf("%s ", pwd->pw_name);

  struct group *grp; // 根据组id获取组名
  grp = getgrgid(get_message->st_gid);
  printf("%s ", grp->gr_name);
}

/**
 * @brief 将GMT时间的秒数转换成标准时间格式输出
 *
 * @param get_message
 */
void showtime(const struct stat *get_message) {
  struct tm *chtm = localtime(&(get_message->st_mtime));
  if (chtm == NULL) {
    printf("localtime is error");
    exit(1);
  } else
    printf("%d年%d月%d日", chtm->tm_year + 1900, chtm->tm_mon + 1,
           chtm->tm_mday);
  if (chtm->tm_hour < 10) // 0-9改写为0X格式
    printf("0");
  printf("%d:", chtm->tm_hour);
  if (chtm->tm_min < 10)
    printf("0");
  printf("%d  ", chtm->tm_min);
}

void showlist(const char *filename, const struct stat *get_message) {
  ftype(get_message); // 判断并打印文件类型

  fpower(get_message); // 判断并打印文件权限

  fprintf(stdout, "%ld ", get_message->st_nlink); // 打印硬链接数

  getid(get_message); // 用户id与组id转换成用户名与组名并打印

  fprintf(stdout, "%ld\t", get_message->st_size); // 打印文件大小

  showtime(get_message); // 将GMT时间的秒数转换成标准时间格式输出
}