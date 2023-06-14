/**
 * @file my_dir.c
 * @brief windows dir命令-l、-i、-t选项
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/my_dir.h"

extern bool flag[4];

int main(int argc, char **argv) {
  char c;
  char s[256];
  while ((c = getopt(argc, argv, "lit")) != -1) // 获取命令行输入的选项参数
  {
    switch (c) {
    case 'l':
      flag[0] = 1;
      break;
    case 'i':
      flag[1] = 1;
      break;
    case 't':
      flag[2] = 1;
      break;
    default:
      printf("参数错误\n");
      exit(1);
    }
  }
  if (argc == optind) {
    // optind记录下一参数的位置，该句表示剩余参数为0，即没有输入filename，默认目录参数为当前目录
    dirs(".", flag);
  } else {
    int i;
    struct stat var;
    for (i = optind; i < argc; i++) {
      // 对输入的目录依次执行ls命令

      // 通过文件名argv[i]获取文件信息，并保存在var所指的结构体stat中
      int rstat = stat(argv[i], &var);
      if (rstat == -1) {
        memset(s, 0, 256);
        strcpy(s, "stat ");
        strcat(s, argv[i]);
        perror(s);
        exit(1);
      }

      if (S_ISDIR(var.st_mode)) {
        // 判断是否是目录
        dirs(argv[i], flag);
      } else {
        // 不是则按文件处理
        if (flag[1] == 1) {
          // 文件-i输出
          printf("%ld\t", var.st_ino);
        }
        if (flag[0] == 1) {
          // 文件-l输出
          showlist(argv[i], &var);
        }
        printf("%s\n", argv[i]);
      }
      printf("-----------------------------------------------------------\n");
    }
  }
  return 0;
}
