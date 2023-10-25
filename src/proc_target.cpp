/* Nate Novak
 * CS5330 Project 2 // Spring 2021
 * File to process a target image and compare to the database
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map> 
#include "../include/distance.h"
#include "../include/csv_util.h"

#define TOP_N 3
#define BINS 16
#define MULTI_TOP_CSV "multi_top.csv"
#define MULTI_BOT_CSV "multi_bottom.csv"
#define TANDC_COL_CSV "color.csv"
#define TANDC_TEX_CSV "texture.csv"
#define CUST_COL_CSV "gallery_color.csv"
#define CUST_TEX_CSV "gallery_texture.csv"
#define CT_AL_COL_CSV "alias_color.csv"
#define CT_AL_TEX_CSV "alias_texture.csv"

using namespace std; 

int main(int argc, char *argv[]) {
  char target_file_name[256]; 
  FeatureSet feature = static_cast<FeatureSet>(0);
  char fv_file_name[256]; 
  char fv_file_name_top[256] = MULTI_TOP_CSV; 
  char fv_file_name_bottom[256] = MULTI_BOT_CSV; 
  char fv_file_name_color[256] = TANDC_COL_CSV; 
  char fv_file_name_texture[256] = TANDC_TEX_CSV; 
  char fv_file_name_cust_color[256] = CUST_COL_CSV; 
  char fv_file_name_cust_texture[256] = CUST_TEX_CSV; 
  char fv_file_name_alias_color[256] = CT_AL_COL_CSV; 
  char fv_file_name_alias_texture[256] = CT_AL_TEX_CSV; 


  if(argc < 4) {
    printf("usage: %s <target image> <feature set> <feature vector file>\n", argv[0]); 
    exit(1); 
  }

  strcpy(target_file_name, argv[1]); 

  feature = static_cast<FeatureSet>( atoi(argv[2]) ); 

  strcpy(fv_file_name, argv[3]); 

  std::map<char *, double> img_dist_pairs;
  printf("Creating histogram for target\n");
  std::vector<int> target_hist;
  std::vector<int> target_top; 
  std::vector<int> target_bottom;
  std::vector<int> target_color; 
  std::vector<int> target_texture;
  
  // Process the target
  switch(feature) {
    case BaseLine: { 
      baseline_processing(target_file_name, BINS, target_hist); 
      break;
    } 

    case Intersection: {
      std_processing(target_file_name, BINS, target_hist); 
      break; 
    }

    case Multiple: {
      multi_hor_processing(target_file_name, BINS, target_top, target_bottom);
      break;
    }

    case ColorTexture: {
      texture_and_color(target_file_name, BINS, target_texture, target_color); 
      break; 
    }

    case Custom: {
      texture_and_color(target_file_name, BINS, target_texture, target_color); 
      break;
    }

    case Alias: {
      alias_processing(target_file_name, BINS, target_hist); 
      break;
    }

    case CTAlias: {
      texture_and_color(target_file_name, BINS, target_texture, target_color, true); 
      break;
    }

    default: 
      break;  
    }
    
    std::vector<char*> image_names; 
    std::vector<std::vector<int>> feature_vectors; 
    std::vector<std::vector<int>> feature_vectors_top; 
    std::vector<std::vector<int>> feature_vectors_bottom; 
    std::vector<std::vector<int>> feature_vector_color; 
    std::vector<std::vector<int>> feature_vector_texture; 

    printf("Reading data from CSV file\n"); 
    int read = veci_read_image_data_csv(fv_file_name, image_names, feature_vectors);

    if(read != 0 && feature != Multiple && feature != ColorTexture && feature != Custom && feature != CTAlias) {
      printf("Error with reading vectors\n");
      return -1;  
    }
    
    int success = -1; 
    if(feature != Multiple && feature != ColorTexture && feature != Custom && feature != CTAlias) {
      success = get_distance(target_hist, image_names, feature_vectors, feature, img_dist_pairs); 
    } else if(feature == Multiple) { 
      int read = veci_read_image_data_csv(fv_file_name_top, image_names, feature_vectors_top); // read top
      std::vector<char *> dummy_img_names; 
      read = veci_read_image_data_csv(fv_file_name_bottom, dummy_img_names, feature_vectors_bottom); // read bottom
      success = get_distance(target_top, target_bottom, image_names, feature_vectors_top, feature_vectors_bottom, img_dist_pairs); // get distance --> this pattern repeats
    } else if(feature == ColorTexture) {
      int read = veci_read_image_data_csv(fv_file_name_color, image_names, feature_vector_color); 
      std::vector<char *> dummy_image_names; 
      read = veci_read_image_data_csv(fv_file_name_texture, dummy_image_names, feature_vector_texture); 
      success = get_distance(target_color, target_texture, image_names, feature_vector_color, feature_vector_texture, img_dist_pairs); 
    } else if (feature == Custom) {
      int read = veci_read_image_data_csv(fv_file_name_cust_color, image_names, feature_vector_color); 
      std::vector<char *> dummy_image_names; 
      read = veci_read_image_data_csv(fv_file_name_cust_texture, dummy_image_names, feature_vector_texture); 
      success = get_distance_w(target_color, target_texture, image_names, feature_vector_color, feature_vector_texture, img_dist_pairs, 8, 7); 
    } else if (feature == CTAlias) {
      int read = veci_read_image_data_csv(fv_file_name_alias_color, image_names, feature_vector_color); 
      std::vector<char *> dummy_image_names;
      read = veci_read_image_data_csv(fv_file_name_alias_texture, dummy_image_names, feature_vector_texture);
      success = get_distance(target_color, target_texture, image_names, feature_vector_color, feature_vector_texture, img_dist_pairs); 
    }
    
    if(success != 0) {
        printf("Error calculating distance metric\n"); 
        return -1; 
    }

    std::vector<char*> top_n_matches;
    get_top_n_matches(img_dist_pairs, TOP_N, top_n_matches, target_file_name); 

    for(int i = 0; i < top_n_matches.size(); i++) {
      printf("image: %s || distance: %.4f\n", top_n_matches[i], img_dist_pairs[top_n_matches[i]]); 
    }

    print_matches(top_n_matches, target_file_name); 

    std::vector<char*> worst; 
    get_bottom_n_matches(img_dist_pairs, 3, worst, target_file_name); 

    print_matches(worst, target_file_name); 

  return 0; 
}

