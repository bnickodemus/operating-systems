#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <utime.h>
#include <stdbool.h>
#include <fcntl.h>

// FileData contains file information
typedef struct {
  char permissions[10];
  long long size;
  unsigned long t_time;
  char fileName[256];
} FileData;

int main(int argc, char **argv)
{
  // check cmd line args
  if (argc < 2) {
    fprintf(stderr, "usage: ./my_tar option [files . . .]\n                option = -c | -x\n");
    exit(-1);
  }

  // make sure the second argument is -c or -x
  if (strcmp(argv[1], "-c") == 0) {
    if (argc < 3) {
      fprintf(stderr, "usage: ./my_tar -c file.ext \n");
      exit(-1);
    }

    FILE *myfp = fopen("tarFile.mytar", "wb");

    // write the number of files
    char c = (char)((argc - 2) + '0');
    fwrite(&c, sizeof(c), 1, myfp);

    // for each file
    for (int i = 2; i < argc; i++) {

      struct stat sb;
      if (lstat(argv[i], &sb) == -1) {
        fprintf(stderr, "error: stat failed\n");
        exit(-1);
      }

      // get the permissions, size, time, filename

      FileData filedata;

      filedata.permissions[0] = S_ISREG(sb.st_mode) ? '-' : S_ISDIR(sb.st_mode) ? 'd' : S_ISLNK(sb.st_mode) ? 'l' : '?';
      filedata.permissions[1] = ( S_IRUSR & sb.st_mode ) ? 'r' : '-';
      filedata.permissions[2] = ( S_IWUSR & sb.st_mode ) ? 'w' : '-';
      filedata.permissions[3] = ( S_IXUSR & sb.st_mode ) ? 'x' : '-';
      filedata.permissions[4] = ( S_IRGRP & sb.st_mode ) ? 'r' : '-';
      filedata.permissions[5] = ( S_IWGRP & sb.st_mode ) ? 'w' : '-';
      filedata.permissions[6] = ( S_IXGRP & sb.st_mode ) ? 'x' : '-';
      filedata.permissions[7] = ( S_IROTH & sb.st_mode ) ? 'r' : '-';
      filedata.permissions[8] = ( S_IWOTH & sb.st_mode ) ? 'w' : '-';
      filedata.permissions[9] = ( S_IXOTH & sb.st_mode ) ? 'x' : '-';
      filedata.size = sb.st_size;
      filedata.t_time = sb.st_mtime;

      for (int idx = 0; idx < strlen(argv[i]); idx++) {
        filedata.fileName[idx] = argv[i][idx];
      }

      /*printf("permissions: %s\n", filedata.permissions);
      printf("size: %d\n", filedata.size);
      printf("time: %lu\n", filedata.t_time);
      printf("filename: %s\n", filedata.fileName);*/

      // make sure that we have a normal file
      if (filedata.permissions[0] != '-') {
        fprintf(stderr, "error file: not a normal file\n");
        exit(-1);
      }

      // make sure that the user can read
      if (!(sb.st_mode & S_IRUSR)) {
        fprintf(stderr, "error: current user does not have read permissions\n");
        exit(-1);
      }

      // open the passed file for reading bytes
      FILE *fp = fopen(argv[i], "rb");

      // malloc a buffer with the size of the file
      unsigned char *buffer = (int*)malloc(sizeof(unsigned char) * filedata.size);
      size_t bytes_read = 0;

      // store the bytes read into bytes_read buffer
      bytes_read = fread(buffer, sizeof(unsigned char), filedata.size, fp);
      //printf("bytes_read: %zu\n", bytes_read);

      fclose(fp);

      fwrite(&filedata, sizeof(filedata), 1, myfp);
      fwrite(buffer, filedata.size, 1, myfp);

      free(buffer);
    }
    fclose(myfp);
  }
  else if (strcmp(argv[1], "-x") == 0) {

    if (argc != 2) {
      fprintf(stderr, "usage: ./my_tar -x\n");
      exit(-1);
    }

    // open tar for reading bytes
    FILE *fp = fopen("tarFile.mytar", "rb");

    // get the number of files
    int numFiles;
    char c;
    fread(&c, sizeof(c), 1, fp);
    numFiles = c - '0';
    //printf("int: %d\n",numFiles);

    // for each file
    for (int i = 0; i < numFiles; i++) {

      FileData filedata1;
      fread(&filedata1, sizeof(filedata1), 1, fp);
      //printf("size: %lld", filedata1.size);

      // malloc a buffer with the size of the file
      unsigned char *buffer = (int*)malloc(sizeof(unsigned char) * filedata1.size);
      size_t bytes_read = 0;

      // store the bytes read into bytes_read buffer
      bytes_read = fread(buffer, sizeof(unsigned char), filedata1.size, fp);
      //printf("bytes_read: %zu\n", bytes_read);


      // write bytes to the new file
      FILE *myfp = fopen(filedata1.fileName, "wb");
      fwrite(buffer, sizeof(unsigned char), filedata1.size, myfp);
      fclose(myfp);
      free(buffer);

      char *s = "./";
      char *s1 = filedata1.fileName;
      char path[strlen(s) + strlen(s1) + 1];
      strcpy(path, s);
      strcat(path, s1); // path now contains "./ filedata"

      struct stat sb;
      if (lstat(filedata1.fileName, &sb) == -1) { // validate lstat succeeded
        fprintf(stderr, "error: stat failed\n");
        exit(-1);
      }

      // get the permissions from the filedata
      int bitMsk = 0;
      if (filedata1.permissions[1] == 'r')
        bitMsk += S_IRUSR;
      if (filedata1.permissions[2] == 'w')
        bitMsk += S_IWUSR;
      if (filedata1.permissions[3] == 'x')
        bitMsk += S_IXUSR;
      if (filedata1.permissions[4] == 'r')
        bitMsk += S_IRGRP;
      if (filedata1.permissions[5] == 'w')
        bitMsk += S_IWGRP;
      if (filedata1.permissions[6] == 'x')
        bitMsk += S_IXGRP;
      if (filedata1.permissions[7] == 'r')
        bitMsk += S_IROTH;
      if (filedata1.permissions[8] == 'w')
        bitMsk += S_IWGRP;
      if (filedata1.permissions[9] == 'x')
        bitMsk += S_IXGRP;
      int result = chmod(path, bitMsk);
      if (result == -1) {
        fprintf(stderr, "error: chmod failed\n");
        exit(-1);
      }

      // set the modification time to the correct time
      struct utimbuf tb;
      tb.modtime = (time_t)filedata1.t_time;
      int res = utime(path, &tb);
      if (res == -1) {
        fprintf(stderr, "error: utime failed\n");
        exit(-1);
      }
    }
    fclose(fp);
  }
  else {
    fprintf(stderr, "error: option = -c | -x\n");
    exit(-1);
  }
}
