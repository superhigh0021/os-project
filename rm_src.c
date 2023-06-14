
#include "include/my_rm.h"

void rm_usage_exit(int status) {
  if (status != EXIT_SUCCESS) {
    fprintf(stderr, "Try '%s --help' for more infomation\n", program_name);
  } else {
    printf("Usage: %s  FILE[DIRECOTORY] OPTION\n", program_name);
    printf("-h  --help(--h) show the help infomation\n");
    printf("-f[-F]  --force(--f) force to remove without prompt\n");
    printf("-r[-R]  --recure(--r) remove directories and their contents "
           "recursively\n");
  }

  exit(status);
}

/**
 * @brief 删除普通文件
 *
 * @param file
 * @param option
 * @return int
 */
int rm_file(const char *file, const struct rm_option *option) {
  if (option->f == 0) {
    printf("rm '%s' ?", file);
    if (getUserConfirmation() == 0) {
      return 0;
    }
  }

  if (unlink(file) != 0) {
    fprintf(stderr, "unlink file '%s' error: %s\n", file, strerror(errno));
    return errno;
  }

  return 0;
}

/**
 * @brief 删除目录（递归函数）
 *
 * @param file_pre
 * @param option
 * @return int
 */
int rm_dir(const char *file_pre, const struct rm_option *option) {
  struct stat statbuf;
  struct dirent *dirp;
  DIR *dp;

  int err = 0;
  char file[256] = {0};
  char child_file[256] = {0};

  strncpy(file, file_pre, sizeof(file));

  if (lstat(file, &statbuf) < 0) {
    fprintf(stderr, "lstat [%s] error[%s]\n", file, strerror(errno));
    return errno;
  }

  if (S_ISDIR(statbuf.st_mode) == 0) {
    // 不是目录
    return rm_file(file, option);
  }

  // 加上斜杠/
  if (strlen(file) > 0 && file[strlen(file) - 1] != '/') {
    file[strlen(file)] = '/';
    file[strlen(file) + 1] = 0;
  }

  strcpy(child_file, file);
  if ((dp = opendir(file)) == NULL) {
    fprintf(stderr, "opendir [%s] error [%s]\n", file, strerror(errno));
    return errno;
  }

  while ((dirp = readdir(dp)) != NULL) {
    // 顺次读取目录内的内容
    if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
      // 去除本目录和上级目录
      continue;
    }

    memset(child_file, 0, sizeof(child_file));
    strcpy(child_file, file);
    strcat(child_file, dirp->d_name);
    err |= rm_dir(child_file, option);
  }

  closedir(dp);

  if (rmdir(file) != 0) {
    fprintf(stderr, "rmdir [%s] error [%s]\n", file, strerror(errno));
    return errno;
  }

  return err;
}

/**
 * @brief 对外开放的rm接口
 *
 * @param file 文件名
 * @param option 参数选项
 * @return int
 */
int rm(char *file, const struct rm_option *option) {
  assert(file != NULL && file[0] != 0);
  assert(option != NULL);

  struct stat statbuf;
  if (!file_isexist(file)) {
    fprintf(stderr, "file '%s' does not exist\n", file);
    return 1;
  }

  lstat(file, &statbuf);
  if (S_ISDIR(statbuf.st_mode)) {
    // 是目录的话就删除目录
    return rm_dir(file, option);
  } else {
    // 不是目录就是普通文件，直接用unlink即可
    return rm_file(file, option);
  }
}