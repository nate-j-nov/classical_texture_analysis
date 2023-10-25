/* Nate Novak
* CS5330 Project 2 // Spring 2022
* Program to process all images in the DB
*/ 

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>
#include "../include/process.h"

#define BUFSIZE 256
#define BINS 16

using namespace std;

int main(int argc, char *argv[]) {
  char dirname[256];
  char buffer[256];
  DIR *dirp;
  struct dirent *dp;
  int i;

  FeatureSet feature = static_cast<FeatureSet>(0); 

  if( argc < 3) {
    printf("usage: %s <directory path> <feature>\n", argv[0]);
    exit(-1);
  }
  
  // get the directory path
  strcpy(dirname, argv[1]);
  printf("Processing directory %s\n", dirname );
  
  int feati = atoi(argv[2]); 
  feature = static_cast<FeatureSet>(feati);
  char feats[BUFSIZE];    

  switch(feature) {
    case BaseLine: { 
      strcpy(feats, "9X9 Sum-Of-Squares"); 
      break;
    }

    case Intersection: {
      strcpy(feats, "Intersction"); 
      break;
    }

    case Multiple: {
      strcpy(feats, "Multiple Histograms"); 
      break; 
    }

    case ColorTexture: {
      strcpy(feats, "Texture and Color"); 
      break;
    }

    case Custom: {
      strcpy(feats, "Identifying Art Gallery"); 
    }

    case Alias: {
      strcpy(feats, "Aliasing"); 
      break;
    }

    case CTAlias: {
      strcpy(feats, "Texture and Colr w/ Aliasing"); 
      break;
    }

    default: 
      strcpy(feats, "\0");
      break;
  }

  printf("Processing images using feature vector: %s\n", feats); 

  // open the directory
  dirp = opendir( dirname );
  if( dirp == NULL) {
    printf("Cannot open directory %s\n", dirname);
    char curdir[256]; 
    getcwd(curdir, 256); 
    printf("CURR DIR: %s\n", curdir); 

    exit(-1);
  }
  
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
      int success = -1; 
      switch(feature) {
        case BaseLine: 
          success = baseline_processing(buffer, BINS); 
          break;

        case Intersection:
          success = std_processing(buffer, BINS, feature); 
          break;

        case Multiple: 
          success = multi_hor_processing(buffer, BINS); 
          break;

        case ColorTexture: 
          success = texture_and_color(buffer, BINS, feature);
          break; 
        
        case Custom: 
          success = texture_and_color(buffer, BINS, feature); 
          break;

        case Alias:
          success = alias_processing(buffer, BINS); 
          break;

        case CTAlias:
          success = texture_and_color(buffer, BINS, feature); 
          break; 

        default:
          success = -1;  
          break;
      }

      if(success != 0) {
        printf("Error Processing images\n"); 
        exit(-1); 
      }
    }
  }
  
  printf("Terminating\n");

  return(0);
}