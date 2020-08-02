#include <cstdio>
#include "pfile.h"
int
main(){
  FD fd=open("test.txt",O_CREAT|O_RDWR,777);
  char hello[]="hello world\n";
  char str[100];
  write(fd,hello,sizeof(hello));
  lseek(fd,0,SEEK_SET);
  read(fd,str,sizeof(hello));
  printf("%s",hello);
  printf("%s",str);
  return 0;
}
