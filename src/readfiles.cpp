/*
  Bruce A. Maxwell

  EDITED BY NATE NOVAK
  
  S21
  
  Sample code to identify image fils in a directory
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>

/*
  Given a directory on the command line, scans through the directory for image files.

  Prints out the full path name for each file.  This can be used as an argument to fopen or to cv::imread.
 */
int main(int argc, char *argv[]) {
  char dirname[256];
  char buffer[256];
  FILE *fp;
  DIR *dirp;
  struct dirent *dp;
  int i;
  int wtfind = -1; 
  bool wtfbool = false;  

  if( argc < 1) {
    printf("usage: %s <directory path> <positive write to file indicator> \n", argv[0]);
    exit(-1);
  }

  // check for sufficient arguments
  if( argc < 2) {
    printf("usage: %s <directory path> \n", argv[0]);
    exit(-1);
  }

  if(argc == 3){
    wtfind = atoi(argv[2]);
  } 

  if(wtfind > 0) {
    wtfbool = true; 
  }


  
  // get the directory path
  strcpy(dirname, argv[1]);
  printf("Processing directory %s\n", dirname );

  // open the directory
  dirp = opendir( dirname );
  if( dirp == NULL) {
    printf("Cannot open directory %s\n", dirname);
    char curdir[256]; 
    getcwd(curdir, 256); 
    printf("CURR DIR: %s\n", curdir); 

    exit(-1);
  }

  if(wtfbool) {
    fp = fopen("img_dirs.txt", "w");
    if(!fp) {
      printf("File opening failed\n"); 
      exit(-1); 
    }
  }
  
  // loop over all the files in the image file listing
  while( (dp = readdir(dirp)) != NULL ) {

    // check if the file is an image
    if( strstr(dp->d_name, ".jpg") ||
	    strstr(dp->d_name, ".png") ||
	    strstr(dp->d_name, ".ppm") ||
	    strstr(dp->d_name, ".tif") ) {

      printf("processing image file: %s\n", dp->d_name);

      // build the overall filename
      strcpy(buffer, dirname);
      strcat(buffer, "/");
      strcat(buffer, dp->d_name);
      if(wtfbool) {
        strcat(buffer, "\n"); 
        fputs(buffer, fp); 
      } else {
        printf("full path name: %s\n", buffer);
      }
    }
  }

  if(wtfbool){
    fclose(fp);   
  }
  
  printf("Terminating\n");

  return(0);
}


