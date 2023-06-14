#include "include/my_copy.h"

struct cp2mv_option g_option;

/**
 * @brief 由于参数错误退出cp命令和mv命令，附带正确用法的提示
 *
 * @param status
 */
void cp2mv_usage_exit(int status) {
  if (status != EXIT_FAILURE) {
    fprintf(stderr, "Try '%s --help -h' for more infomation\n", program_name);
  } else {
    printf("Usage: %s [OPTION]... SOURCE DEST\n ", program_name);
    printf("%s [OPTION]... SOURCEDIR DESTDIR\n ", program_name);
    printf("DESCRIPTION\n");
    printf("treat all the file as normal files\n");
    printf("OPTION\n");
    printf("-h, -H  --help          Help infomation\n");
    printf("-r, -R, --recursive     Recursive cp files\n");
    printf("-l, -L, --link          Copy linkfile not the actual file\n");
    printf("-f, -F, --fource        Force to overwrite file without prompt\n");
  }

  exit(status);
}

/**
 * @brief 复制文件
 *
 * @param psrc 源文件
 * @param pdst 目的地
 * @param option 参数选项
 * @param needdel
 */
void cp_file(char *psrc, char *pdst, struct cp2mv_option *option, int needdel) {
  char dst[MAX_NAME_LEN];
  size_t name_size;

  printf("cp [%s] [%s]\n", psrc, pdst);

  strncpy(dst, pdst, MAX_NAME_LEN);
  dst[MAX_NAME_LEN - 1] = 0;

  if (file_isdir(psrc)) {
    // 源不能是目录
    fprintf(stderr, "file '%s' is directory\n", psrc);
    return;
  }

  if (file_isdir(pdst)) {
    // 目的地必须是目录
    name_size = strlen(dst);
    if (name_size > 0 && dst[name_size - 1] != '/' &&
        name_size < MAX_NAME_LEN - 3) {
      dst[name_size++] = '/';
      dst[name_size++] = 0;
    }
    strncpy(dst + name_size, psrc, MAX_NAME_LEN - name_size);
  }

  copy(psrc, dst, option->f, option->l, needdel);
}

/**
 * @brief 递归复制
 *
 * @param psrc 源文件夹
 * @param pdst 目的地
 * @param option 参数选项
 * @param need_del
 */
void cp_r(char *psrc, char *pdst, struct cp2mv_option *option, int need_del) {
  size_t name_size;
  struct dirent *dirp;
  DIR *dp;
  char src[256] = {0};
  char dst[256] = {0};

  // 去除字符串最后的 '/0'
  strncpy(src, psrc, sizeof(src) - 1);
  strncpy(dst, pdst, sizeof(dst) - 1);

  // 去除潜在的目录斜杠
  name_size = strlen(src);
  if (name_size > 0 && src[name_size - 1] == '/')
    src[name_size - 1] = 0;

  name_size = strlen(dst);
  if (name_size > 0 && dst[name_size - 1] == '/')
    dst[name_size - 1] = 0;

  if (file_isdir(src)) {
    // 递归复制要求源文件为文件夹
    if ((dp = opendir(src)) == NULL) {
      // 如果源文件夹为空，则不用拷贝
      fprintf(stderr, "opendir '%s' error: %s\n", src, strerror(errno));
      return;
    }

    strcat(src, "/");
    strcat(dst, "/");
    // 将ps指向src的末尾，将pd指向dst的末尾
    char *ps = src + strlen(src);
    char *pd = dst + strlen(dst);

    while ((dirp = readdir(dp)) != NULL) {
      if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
        // 每个文件夹内都包含.和..作为当前目录和上级目录，需要跳过二者
        continue;
      }

      strcpy(ps, dirp->d_name);
      strcpy(pd, dirp->d_name);

      if (file_isdir(src)) {
        if (mkdir(dst, 0) != 0) {
          // 如果创建失败了，执行以下选项
          if (option->f == 0) {
            // -f选项用于在目标文件已存在时确认是否强制执行
            printf("do you want overwrite dir '%s' ?", dst);
            if (!getUserConfirmation()) {
              return;
            }
          }
        }
        cp_r(src, dst, option, need_del);
      } else {
        cp_file(src, dst, option, need_del);
      }
    }
  } else {
    cp_file(src, dst, option, need_del);
  }

  if (need_del) {
    struct rm_option rm_op;
    rm_op.f = 1;
    rm_op.r = 1;
    rm_dir(psrc, &rm_op);
  }
}

/**
 * @brief 对外开放的cp、mv接口
 *
 * @param psrc 源
 * @param pdst 目的地
 * @param option 选项
 * @param needdel 是否删除源
 */
void cp(char *psrc, char *pdst, struct cp2mv_option *option, int needdel) {
  if (option->r) {
    cp_r(psrc, pdst, option, needdel);
  } else {
    cp_file(psrc, pdst, option, needdel);
  }
}

/**
 * @brief 真正执行拷贝文件的函数
 *
 * @param src_file 需要复制的源文件
 * @param dst_file 指向目标目录
 * @param is_force 当目标文件存在时，是否强制覆盖
 * @param deal_link 是否复制链接文件
 * @param needdel 是否需要删除源文件
 * @return int
 */
int copy(const char *src_file, const char *dst_file, int is_force,
         int deal_link, int needdel) {
  assert(src_file != NULL && src_file[0] != 0);
  assert(src_file != NULL && src_file[0] != 0);

  int fdin = -1, fdout = -1;
  void *psrc = NULL, *pdst = NULL;
  struct stat statbuf;

  errno = 0;
  if ((is_force == 0) && file_isexist(dst_file)) {
    // 如果目标文件已存在，并且没有开启-f强制覆盖选项，则要询问用户
    printf("do you want to overwrite file '%s' ?(Y/N)\t", dst_file);
    if (!getUserConfirmation()) {
      // 不覆盖，返回假成功
      return 0;
    }
  }

  if ((fdin = open(src_file, O_RDONLY)) < 0) {
    fprintf(stderr, "open file '%s' error: %s\n", src_file, strerror(errno));
    return errno;
  }

  if (fstat(fdin, &statbuf) < 0) {
    fprintf(stderr, "fstat file '%s' error: %s\n", src_file, strerror(errno));
    close(fdin);
    return errno;
  }

  if ((fdout = open(dst_file, O_RDWR | O_CREAT | O_TRUNC)) < 0) {
    fprintf(stderr, "can't create file '%s' error: %s\n", dst_file,
            strerror(errno));
    close(fdin);
    return errno;
  }

  /* 设置输出文件的大小 */
  if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1) {
    fprintf(stderr, "lseek file '%s' error: %s\n", dst_file, strerror(errno));
    close(fdin);
    close(fdout);
    return errno;
  }

  if (write(fdout, "", 1) != 1) {
    fprintf(stderr, "write file '%s' error: %s\n", dst_file, strerror(errno));
    close(fdin);
    close(fdout);
    return errno;
  }

  /* 映射源文件与目标文件  */
  if ((psrc = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fdin, 0)) ==
      MAP_FAILED) {
    fprintf(stderr, "mmap file '%s' error: %s\n", src_file, strerror(errno));
    close(fdin);
    close(fdout);
    return errno;
  }
  if ((pdst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,
                   fdout, 0)) == MAP_FAILED) {
    fprintf(stderr, "mmap file '%s' error: %s\n", dst_file, strerror(errno));
    close(fdin);
    close(fdout);
    munmap(psrc, statbuf.st_size);
    return errno;
  }

  /* 将源文件映射的区域move到目标映射区域，完成拷贝 */
  memmove(pdst, psrc, statbuf.st_size);

  /* 关闭文件并解除文件映射 */
  close(fdin);
  close(fdout);
  munmap(psrc, statbuf.st_size);
  munmap(pdst, statbuf.st_size);

  if (needdel) {
    // 需要删除源文件
    struct rm_option rm_op;
    rm_op.r = 0;
    rm_op.f = 1;
    return rm_file(src_file, &rm_op);
  }

  return 0;
}