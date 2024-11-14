/* This program needs 3 arguments! */
/* 1st A file witch is locked or you wanna lock. */
/* 2nd The new name for the file. */
/* 3rd the password! */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>

char buf[4096];
int len = 0;
long int filesize; 
int main(int argc, char *argv[]){
  if(argc != 4){
    printf("This program needs 3 arguments!\n");
    printf("1st A file witch is locked or you wanna unlock.\n");
    printf("2nd The new name for the file.\n");
    printf("3rd the password!\n");
    return 0;
  }
  int fd = open(argv[1], O_RDONLY);
  assert(fd > 0);
  struct stat statbuf;
  stat(argv[1], &statbuf);
  printf("file size : %ld\n", statbuf.st_size);
  filesize = statbuf.st_size;

  int nfd = open(argv[2], O_WRONLY | O_CREAT, 0600);
  assert(nfd > 0);

  char *password = argv[3];
  assert(password != NULL);
  printf("Password = %s\n", password);

  for(int i=0; i < 128; i++){
    if(password[i] != 0){
      len++;
    }else break;
  }
  printf("Password len : %d\n", len);

  while(filesize > 0){
    int x = read(fd, &buf, 4096);
    for(int i=0; i < x; i++){
      buf[i] = buf[i] ^ (i%8);
    }
    write(nfd, &buf, x);
    filesize = filesize - x;
  }
  assert(unlink(argv[1]) == 0);
  return 0;
}
