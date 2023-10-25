/* Nate Novak
 * CS5330 Projec 2 // Spring 2022
 * Header file for distance.cpp
 */

#include <stdio.h>
#include <vector>
#include "process.h"
#include <math.h>

/* Wrapper function to calculate the distance metrics between two histograms
 * image_names is vector of image names
 * feature_vectors are the feature vectors for the images
 * feature is a flag because different features have different distance metrics
 * img_dist pairs is the map that holds the image/distance parings\
 * return 0 on success
 */
int get_distance(const std::vector<int> target, std::vector<char *> image_names, std::vector<std::vector<int>> feature_vectors, FeatureSet feature, std::map<char *, double> &img_dist_pairs);

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
int get_distance(const std::vector<int> target_top, std::vector<int> target_bottom, std::vector<char *> image_names, std::vector<std::vector<int>> feature_vectors_top, std::vector<std::vector<int>> feature_vectors_bot, std::map<char *, double> &img_dist_pairs);

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
int get_distance_w(const std::vector<int> target_top, std::vector<int> target_bottom, std::vector<char *> image_names, std::vector<std::vector<int>> feature_vectors_top, std::vector<std::vector<int>> feature_vectors_bot, std::map<char *, double> &img_dist_pairs, int w_top = 1, int w_bottom = 1);

/* Function to get the top N images.
 * Parameter distances is the map that contains the distances for each images
 * N is number of images we want to be returned
 * top_images is list of image names to be populated
 * target_fn is the name of the target image
 */
int get_top_n_matches(std::map<char*, double> distances, const int N, std::vector<char*> &top_imgs, char* target_fn);

/* Function to get the bottom N images.
 * Parameter distances is the map that contains the distances for each images
 * N is number of images we want to be returned
 * top_images is list of image names to be populated
 * target_fn is the name of the target image
 */
int get_bottom_n_matches(std::map<char*, double> distances, const int N, std::vector<char*> &bottom_imgs, char* target_fn);

/* Function to print the top n matches
 * top_n_matches is a vector of file names that contain the top N matches
 * target is name of target image
 */
int print_matches(const std::vector<char*> top_n_matches, char* target);

/* Function to to calculate the summed square dinstance between two histograms
 * parameter distance is a double to be written to
 * parameter target is the target image's feature vector
 * parameter comparer is the image we are comparing it to in the database
 * returns 0 unless there's an error.
 */
int sum_squared_difference(double &distance, const std::vector<int> target, const std::vector<int> comparer);

/* Function to to calculate the normalized intersection distance metric
 * parameter distance is a double to be written to
 * parameter target is the target image's feature vector
 * parameter comparer is the image we are comparing it to in the database
 * returns 0 unless there's an error.
 */
int normalized_intersection(double &distance, const std::vector<int> target, const std::vector<int> comparer);
