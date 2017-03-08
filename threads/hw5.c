#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
  int height;
  int width;
  int maxPixValue;
  int threadNum;
  int pixelsPerThread;
  int start; // so threads know what pixel to start on
  int end; // so threads know what pixel to end on
  float P; // for contrast
}Data;

int ***pixels;
int ***pixels2 = NULL; // for multi threaded roatations

void *rotateRight(void *arg) {
  Data *data = (Data*) arg;
  int height = data->height;
  int width = data->width;
  int start = data->start;
  int end = data->end;
  int maxPixValue = data->maxPixValue;

  height = end / 3 / width; // set the height to the (# of pixels / 3) / width to give a thread the correct height
  int heightStart = start / 3 / width;
  width = end / 3 / height;
  int widthStart = 0;

  // the width/height may be over the limit if the # of pixels cannot be divided by # of threads evenly
  if (width > data->width)
    width = data->width;
  if (height > data->height)
    height = data->height;

  for (int x = heightStart; x < height; x++) {
		for (int y = widthStart; y < width; y++) {
      for (int z = 0; z < 3; z++) {
         pixels2[y][width-x-1][z] = pixels[x][y][z];
         //printf("THREAD: %d processed %d\n", data->threadNum ,pixels2[y][width-x-1][z]); // verify threads working
       }
     }
   }
   return NULL;
}

/*
// rotateLeft will work with 1 thread but not more than 1

void *rotateLeft(void *arg) {
  Data *data = (Data*) arg;
  int height = data->height;
  int width = data->width;
  int start = data->start;
  int end = data->end;
  int maxPixValue = data->maxPixValue;

  height = end / 3 / width; // set the height to the (# of pixels / 3) / width to give a thread the correct height
  int heightStart = start / 3 / width;
  width = end / 3 / height;
  int widthStart = 0;

  // the width/height may be over the limit if the # of pixels cannot be divided by # of threads evenly
  if (width > data->width)
    width = data->width;
  if (height > data->height)
    height = data->height;

  for (int x = heightStart; x < height; x++) {
		for (int y = widthStart; y < width; y++) {
      for (int z = 0; z < 3; z++) {
          printf("THREAD: %d processed %d\n", data->threadNum ,pixels2[height-y-1][x][z]); // verify threads working
         pixels2[height-y-1][x][z] = pixels[x][y][z];
       }
     }
   }
   return NULL;
}*/

void *red(void *arg) {
  Data *data = (Data*) arg;
  int height = data->height;
  int width = data->width;
  int start = data->start;
  int end = data->end;

  height = end / 3 / width;
  int heightStart = start / 3 / width;
  width = end / 3 / height;
  int widthStart = 0;

  // the width/height may be over the limit if the # of pixels cannot be divided by # of threads evenly
  if (width > data->width)
    width = data->width;
  if (height > data->height)
    height = data->height;

  for (int i = heightStart; i < height; i++) {
    for (int j = widthStart; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        if (k != 0) {
          pixels[i][j][k] = 0;
        }
        //printf("THREAD: %d processed %d\n", data->threadNum ,pixels[i][j][k]); // verify threads working
      }
    }
  }
  return NULL;
}

void *green(void *arg) {
  Data *data = (Data*) arg;
  int height = data->height;
  int width = data->width;

  int start = data->start;
  int end = data->end;
  //printf("thread # %d\nstart: %d\nend: %d\n", data->threadNum, start, end);

  height = end / 3 / width;
  int heightStart = start / 3 / width;
  width = end / 3 / height;
  int widthStart = 0; //start / 3 / height; // 0

  // the width/height may be over the limit if the # of pixels cannot be divided by # of threads evenly
  if (width > data->width)
    width = data->width;
  if (height > data->height)
    height = data->height;
  //printf("thread # %d\nheight: %d\nheightStart: %d\nwidth: %d\nwidthStart: %d\n", data->threadNum, height, heightStart, width, widthStart);

  for (int i = heightStart; i < height; i++) {
    for (int j = widthStart; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        if (k != 1) {
          pixels[i][j][k] = 0;
        }
        //printf("THREAD: %d processed %d\n", data->threadNum ,pixels[i][j][k]); // verify threads working
      }
    }
  }
  return NULL;
}

void *blue(void *arg) {
  Data *data = (Data*) arg;
  int height = data->height;
  int width = data->width;
  int start = data->start;
  int end = data->end;

  height = end / 3 / width;
  int heightStart = start / 3 / width;
  width = end / 3 / height;
  int widthStart = 0;

  // the width/height may be over the limit if the # of pixels cannot be divided by # of threads evenly
  if (width > data->width)
    width = data->width;
  if (height > data->height)
    height = data->height;

  for (int i = heightStart; i < height; i++) {
    for (int j = widthStart; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        if (k != 2) {
          pixels[i][j][k] = 0;
        }
        //printf("THREAD: %d processed %d\n", data->threadNum ,pixels[i][j][k]); // verify threads working
      }
    }
  }
  return NULL;
}

void *invert(void *arg) {
  Data *data = (Data*) arg;
  int height = data->height;
  int width = data->width;
  int start = data->start;
  int end = data->end;
  int maxPixValue = data->maxPixValue;

  height = end / 3 / width;
  int heightStart = start / 3 / width;
  width = end / 3 / height;
  int widthStart = 0;

  // the width/height may be over the limit if the # of pixels cannot be divided by # of threads evenly
  if (width > data->width)
    width = data->width;
  if (height > data->height)
    height = data->height;

  for (int i = heightStart; i < height; i++) {
    for (int j = widthStart; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        pixels[i][j][k] = maxPixValue - pixels[i][j][k];
      }
    }
  }
  return NULL;
}

void *contrast(void *arg) {
  Data *data = (Data*) arg;
  int height = data->height;
  int width = data->width;
  int start = data->start;
  int end = data->end;
  float P = data->P;
  int maxPixValue = data->maxPixValue;

  height = end / 3 / width;
  int heightStart = start / 3 / width;
  width = end / 3 / height;
  int widthStart = 0;

  // the width/height may be over the limit if the # of pixels cannot be divided by # of threads evenly
  if (width > data->width)
    width = data->width;
  if (height > data->height)
    height = data->height;

  for (int i = heightStart; i < height; i++) {
    for (int j = widthStart; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        if (pixels[i][j][k] <= (maxPixValue	/	2	))
          pixels[i][j][k]	-=	(maxPixValue * P);
        else
          pixels[i][j][k]	+= (maxPixValue * P);
        if (pixels[i][j][k] < 0) {
          pixels[i][j][k] = 0;
        }
        if (pixels[i][j][k] > 255) {
          pixels[i][j][k] = 255;
        }
      }
    }
  }
  return NULL;
}

int main(int argc, char **argv)
{
  char *line;
  size_t n = 0;
  int x = 0;
  int y = 0;
  int z = 0;
  // get the height and width to allocate our pixels array
  int height, width, maxPixValue, counter = 0;
  char tmp1 [256], tmp2 [256];
  int secondHalfBool = 0;
  // get the first 3 lines
  for (int i = 0; i < 3; i++) {
    getline(&line, &n, stdin);
      if (i == 1) {
        for (int j = 0; line[j] != '\0'; j++) { // iterate through our line

          if (line[j] == ' ') {
            tmp1[counter+1] = '\n';
            width = atoi(tmp1);
            counter = 0;
            secondHalfBool = 1;
          }
          if (secondHalfBool) {
            tmp2[counter] = line[j];
            counter++;
          }
          else {
            //printf("%c ",line[j]);
            tmp1[counter] = line[j];
            counter++;
          }
        }
      }
      if (i == 2) {
        maxPixValue = atoi(line);
      }
  }
  tmp2[counter + 1] = '\n';
  height = atoi(tmp2);
  //printf("height:%d\n",height);
  //printf("width:%d\n",width);
  //printf("maxPixValue:%d\n",maxPixValue);
  printf("P3\n");
  printf("%d %d\n",height,width);
  printf("%d\n",maxPixValue);

  // allocate our pixels array
  pixels = (int ***) malloc(height * sizeof(int **));
  for (int i = 0; i < height; i++) {
      pixels[i] = (int **) malloc(width * sizeof(int *));
      for (int j = 0; j < width; j++)
          pixels[i][j] = (int *) malloc(3 * sizeof(int));
  }

  int number;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < 3; k++ ) {
        if (fscanf(stdin, "%d", &number) != -1)
          pixels[i][j][k] = number;
      }
    }
  }

  if (argc < 3 || argc > 4) {
    fprintf(stderr, "usage: ./hw5 num_threads option [arg]\n");
    exit(-1);
  }

  int numberOfThreads = atoi(argv[1]);
  pthread_t threads[numberOfThreads];

  int width2 = height;
  int height2 = width;

  if (strcmp(argv[2],"-R") == 0) {
    // malloc pixels2
    pixels2 = (int ***) malloc(height2 * sizeof(int **));
    for (int i = 0; i < height2; i++) {
        pixels2[i] = (int **) malloc(width2 * sizeof(int *));
        for (int j = 0; j < width2; j++)
            pixels2[i][j] = (int *) malloc(3 * sizeof(int));
    }
    for (int i = 0; i < numberOfThreads; i++) {

      Data *data = malloc(sizeof(Data));
      data->height = height;
      data->width = width;
      data->maxPixValue = maxPixValue;
      data->pixelsPerThread = height * width * 3 / numberOfThreads;
      data->threadNum = i;
      data->start = data->pixelsPerThread * i;
      data->end = data->start + data->pixelsPerThread;
      int err = pthread_create( &threads[i], NULL, rotateRight, (void *)data); // error check
      if (err != 0)
        exit(-1);
    }
  }
  if (strcmp(argv[2],"-L") == 0) {
    // malloc pixels2
    pixels2 = (int ***) malloc(height2 * sizeof(int **));
    for (int i = 0; i < height2; i++) {
        pixels2[i] = (int **) malloc(width2 * sizeof(int *));
        for (int j = 0; j < width2; j++)
            pixels2[i][j] = (int *) malloc(3 * sizeof(int));
    }
    // rotating left failed with more than 1 thread failed so, I just call right 3 times
    for (int i = 0; i < 3; i++) {
      for (int i = 0; i < numberOfThreads; i++) {
        Data *data = malloc(sizeof(Data));
        data->height = height;
        data->width = width;
        data->maxPixValue = maxPixValue;
        data->pixelsPerThread = height * width * 3 / numberOfThreads;
        data->threadNum = i;
        data->start = data->pixelsPerThread * i;
        data->end = data->start + data->pixelsPerThread;
        int err = pthread_create( &threads[i], NULL, rotateRight, (void *)data); // error check
        if (err != 0)
          exit(-1);
      }
      // join the threads back together
      for (int i = 0; i < numberOfThreads; i++) {
        int err = pthread_join(threads[i], NULL); // error check
        if (err != 0)
          exit(-1);
      }
      // if we did a rotation we need to put pixels2 into pixels
      if (pixels2 != NULL) {
        for (int i = 0; i < height2; i++) {
          for (int j = 0; j < width2; j++) {
            for (int k = 0; k < 3; k++) {
              pixels[i][j][k] = pixels2[i][j][k];
           }
         }
       }
      }
    }
  }
  if (strcmp(argv[2],"-green") == 0) {
    for (int i = 0; i < numberOfThreads; i++) {
      //fprintf(stdout, "New thread: %d\n", i);
      Data *data = malloc(sizeof(Data));
      data->height = height;
      data->width = width;
      data->maxPixValue = maxPixValue;
      data->pixelsPerThread = height * width * 3 / numberOfThreads;
      //printf("pixels per thread %d\n", height * width * 3 / numberOfThreads);
      data->threadNum = i;
      data->start = data->pixelsPerThread * i;
      data->end = data->start + data->pixelsPerThread;
      int err = pthread_create( &threads[i], NULL, green, (void *)data); // error check
      if (err != 0)
        exit(-1);
    }
  }
  if (strcmp(argv[2],"-blue") == 0) {
    for (int i = 0; i < numberOfThreads; i++) {
      Data *data = malloc(sizeof(Data));
      data->height = height;
      data->width = width;
      data->maxPixValue = maxPixValue;
      data->pixelsPerThread = height * width * 3 / numberOfThreads;
      data->threadNum = i;
      data->start = data->pixelsPerThread * i;
      data->end = data->start + data->pixelsPerThread;
      int err = pthread_create( &threads[i], NULL, blue, (void *)data); // error check
      if (err != 0)
        exit(-1);
    }
  }
  if (strcmp(argv[2],"-red") == 0) {
    for (int i = 0; i < numberOfThreads; i++) {
      Data *data = malloc(sizeof(Data));
      data->height = height;
      data->width = width;
      data->maxPixValue = maxPixValue;
      data->pixelsPerThread = height * width * 3 / numberOfThreads;
      data->threadNum = i;
      data->start = data->pixelsPerThread * i;
      data->end = data->start + data->pixelsPerThread;
      int err = pthread_create( &threads[i], NULL, red, (void *)data); // error check
      if (err != 0)
        exit(-1);
    }
  }
  if (strcmp(argv[2],"-I") == 0) {
    for (int i = 0; i < numberOfThreads; i++) {
      Data *data = malloc(sizeof(Data));
      data->height = height;
      data->width = width;
      data->maxPixValue = maxPixValue;
      data->pixelsPerThread = height * width * 3 / numberOfThreads;
      data->threadNum = i;
      data->start = data->pixelsPerThread * i;
      data->end = data->start + data->pixelsPerThread;
      int err = pthread_create( &threads[i], NULL, invert, (void *)data); // error check
      if (err != 0)
        exit(-1);
    }
  }
  if (strcmp(argv[2],"-C") == 0) {
    for (int i = 0; i < numberOfThreads; i++) {
      Data *data = malloc(sizeof(Data));
      data->height = height;
      data->width = width;
      data->maxPixValue = maxPixValue;
      data->pixelsPerThread = height * width * 3 / numberOfThreads;
      data->threadNum = i;
      data->start = data->pixelsPerThread * i;
      data->end = data->start + data->pixelsPerThread;
      data->P = atof(argv[3]);
      int err = pthread_create( &threads[i], NULL, contrast, (void *)data); // error check
      if (err != 0)
        exit(-1);
    }
  }

  // join the threads back together
  for (int i = 0; i < numberOfThreads; i++) {
    pthread_join(threads[i], NULL); // error check
  }

  // if we did a rotation we need to put pixels2 into pixels
  if (pixels2 != NULL) {
    for (int i = 0; i < height2; i++) {
      for (int j = 0; j < width2; j++) {
        for (int k = 0; k < 3; k++) {
          pixels[i][j][k] = pixels2[i][j][k];
       }
     }
   }
 }

  //(512 x 512 x 3) % nThreads

  // once all of the pixel manipulations are done, print
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        printf("%d\n",pixels[i][j][k]);
      }
    }
  }
  //free(Data);
  free(pixels);
  //free(pixels2); // this causes problems (idk why???)
}
