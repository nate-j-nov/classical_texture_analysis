/* Nate Novak
 * CS5330 Project 2 // Spring 2022
 * Functions that calculate distance metrics between two histograms
 */ 

#include "../include/distance.h"

/* Wrapper function to calculate the distance metrics between two histograms
 * image_names is vector of image names
 * feature_vectors are the feature vectors for the images
 * feature is a flag because different features have different distance metrics
 * img_dist pairs is the map that holds the image/distance parings
 * return 0 on success
 */
int get_distance(const std::vector<int> target, std::vector<char *> image_names, std::vector<std::vector<int>> feature_vectors, FeatureSet feature, std::map<char *, double> &img_dist_pairs) {
  for(int i = 0; i < image_names.size(); i++) {
    char* curr_image = image_names[i]; // get current image nam
    std::vector<int> current_fv = feature_vectors[i]; // get current feature vector in DB 
    double distance = 0.0; 
    int success = -1; 
    switch(feature) {
      case BaseLine: { 
        success = sum_squared_difference(distance, target, current_fv); 
        break;
      }

      case Intersection: {
        success = normalized_intersection(distance, target, current_fv); 
        break;
      }

      case Alias: {
        success = normalized_intersection(distance, target, current_fv); 
        break;
      }

      default:  
        break; 
    }
    if(success != 0) {
      printf("Error calculating Distance Metric for %s\n", curr_image); 
      continue; 
    }

    img_dist_pairs[curr_image] = distance;
  }

  return 0; 
}

/* *OVERLOAD* Wrapper function to calculate the distance metrics for multiple histograms. 
 * image_names is vector of image names
 * This works for any multi-histogram pardigm. However, target_top and feature_vectors_top must correspond to the same "section" of the image.
 * It could also be referring to color and texture histograms
 * target_top is one histogram of the image (top or color)
 * target_bottom is the other histogram of the image (bottom / texture)
 * image_names is the names of the images
 * feature_vectors_top is list of feature vectors for the top/color histogram
 * feature_vectors_bot is list of feature vectors for the bottom/texture histogram
 * img_dist pairs is the map that holds the image/distance parings
 * returns 0 on success
 */
int get_distance(const std::vector<int> target_top, std::vector<int> target_bottom, std::vector<char *> image_names, std::vector<std::vector<int>> feature_vectors_top, std::vector<std::vector<int>> feature_vectors_bot, std::map<char *, double> &img_dist_pairs) {
  for(int i = 0; i < image_names.size(); i++) {
    char* curr_image = image_names[i]; // get current image name
    std::vector<int> current_fv_top = feature_vectors_top[i];
    std::vector<int> current_fv_bottom = feature_vectors_bot[i]; 

    // get current feature vector in DB 
    double dist_top = 0.0; 
    double dist_bottom = 0.0; 

    int success = normalized_intersection(dist_top, target_top, current_fv_top); 
    if(success != 0) {
      printf("Error calculating Distance Metric for top of %s\n", curr_image); 
      continue; 
    }

    success = normalized_intersection(dist_bottom, target_bottom, current_fv_bottom); 
    if(success != 0) {
      printf("Error calculating Distance Metric for bottom of %s\n", curr_image); 
      continue; 
    }

    double dist_total = ( dist_top + dist_bottom ) / 2;

    img_dist_pairs[curr_image] = dist_total;
  }

  return 0; 
}

/* Wrapper function to calculate the distance metrics for multiple histograms with weights 
 * target_top is one histogram of the image (top or color)
 * target_bottom is the other histogram of the image (bottom / texture)
 * image_names is the names of the images
 * feature_vectors_top is list of feature vectors for the top/color histogram
 * feature_vectors_bot is list of feature vectors for the bottom/texture histogram
 * img_dist pairs is the map that holds the image/distance parings
 * w_top is weight applied to the average of the top/color histogram
 * w_bottom is weight applied to average of the bottom/texture histogram
 * returns 0 on success
 */
int get_distance_w(const std::vector<int> target_top, std::vector<int> target_bottom, std::vector<char *> image_names, std::vector<std::vector<int>> feature_vectors_top, std::vector<std::vector<int>> feature_vectors_bot, std::map<char *, double> &img_dist_pairs, int w_top, int w_bottom) {
  float total_weight = w_top + w_bottom; 
  for(int i = 0; i < image_names.size(); i++) {
    char* curr_image = image_names[i]; // get current image name

    std::vector<int> current_fv_top = feature_vectors_top[i];
    std::vector<int> current_fv_bottom = feature_vectors_bot[i]; 

     // get current feature vector in DB 
    double dist_top = 0.0; 
    double dist_bottom = 0.0; 

    int success = normalized_intersection(dist_top, target_top, current_fv_top); 
    if(success != 0) {
      printf("Error calculating Distance Metric for top of %s\n", curr_image); 
      continue; 
    }

    success = normalized_intersection(dist_bottom, target_bottom, current_fv_bottom); 
    if(success != 0) {
      printf("Error calculating Distance Metric for bottom of %s\n", curr_image); 
      continue; 
    }

    double dist_total = ( w_top * dist_top + w_bottom * dist_bottom ) / (w_top + w_bottom);

    img_dist_pairs[curr_image] = dist_total;
  }

  return 0; 
}

/* Function to get the top N images.
 * Parameter distances is the map that contains the distances for each images
 * N is number of images we want to be returned
 * top_images is list of image names to be populated
 * target_fn is the name of the target image
 */
int get_top_n_matches(std::map<char*, double> distances, const int N, std::vector<char*> &top_imgs, char* target_fn) {
  distances.erase(target_fn); 
  printf("Finding top %d matches\n", N); 
  for(int i = 0; i < N + 1; i++) {
    char* min_file_name; 
    double current_min = std::numeric_limits<double>::max(); 
    for(auto const& item : distances) {
       if(isless(item.second, current_min)) {
         min_file_name = item.first;
         current_min = item.second; 
       }
    }
    if(i != 0) {
      top_imgs.push_back(min_file_name);
    }
    
    distances.erase(min_file_name); 
  }

  return 0; 
}

/* Function to get the bottom N images.
 * Parameter distances is the map that contains the distances for each images
 * N is number of images we want to be returned
 * top_images is list of image names to be populated
 * target_fn is the name of the target image
 */
int get_bottom_n_matches(std::map<char*, double> distances, const int N, std::vector<char*> &bottom_imgs, char* target_fn) {
  distances.erase(target_fn); 
  printf("Finding bottom %d matches\n", N); 
  for(int i = 0; i < N + 1; i++) {
    char* max_file_name; 
    double current_max = std::numeric_limits<double>::min(); 
    for(auto const& item : distances) {
       if(isgreater(item.second, current_max)) {
         max_file_name = item.first;
         current_max = item.second; 
       }
    }
    if(i != 0) {
      bottom_imgs.push_back(max_file_name);
    }
    
    distances.erase(max_file_name); 
  }

  return 0; 
}


/* Function to print the top n matches 
 * top_n_matches is a vector of file names that contain the top N matches
 * target is name of target image
 */
int print_matches(const std::vector<char*> top_n_matches, char* target) {
  printf("Target %s\n", target); 
  for(int i = 0; i < top_n_matches.size(); i++) {
    printf(" %s", top_n_matches[i]); 
  }
  printf("\n"); 
  return 0; 
}

/* Function to to calculate the summed square dinstance between two histograms
 * parameter distance is a double to be written to
 * parameter target is the target image's feature vector
 * parameter comparer is the image we are comparing it to in the database
 * returns 0 unless there's an error. 
 */
int sum_squared_difference(double &distance, const std::vector<int> target, const std::vector<int> comparer) {
  if(target.size() != comparer.size()) {
    printf("error: Target and comparer feature vectors are not the same length\n");
    printf("\t try checking that the number of bins are correct\n");  
    return -1; 
  }

  for(int i = 0; i < target.size(); i++) {
    distance += (target[i] - comparer[i]) * (target[i] - comparer[i]); 
  }

  distance /= target.size(); 

  return 0; 
}

/* Function to to calculate the normalized intersection distance metric
 * parameter distance is a double to be written to
 * parameter target is the target image's feature vector
 * parameter comparer is the image we are comparing it to in the database
 * returns 0 unless there's an error. 
 */
int normalized_intersection(double &distance, const std::vector<int> target, const std::vector<int> comparer) { 
  if(target.size() != comparer.size()) {
    printf("error: Target and comparer feature vectors are not the same length\n");
    printf("\t try checking that the number of bins are correct\n");  
    return -1; 
  }

  int target_t = 0; 
  for(int k = 0; k < target.size(); k++) {
    target_t += target[k]; 
  }

  int comparer_t = 0; 
  for(int k = 0; k < comparer.size(); k++) {
    comparer_t += comparer[k]; 
  }

  for(int s = 0; s < target.size(); s++) {
    double target_n =  ( (double) target[s] / (double) target_t );
    double comparer_n =  ( (double) comparer[s] / (double) comparer_t ); 

    if(isless(target_n, comparer_n)) {
      distance += target_n;  
    } else {  
      distance += comparer_n;
    }
  }

  distance = 1 - distance; 

  return 0; 
}

