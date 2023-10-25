/* Nate Novak
* CS5330 Project 2 // Spring 2022
* Header file for process.cpp
*/

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include "histogram.h"
#include "csv_util.h"
#include "filter.h"

#define BL_CSV "baseline.csv"
#define INTER_CSV "intersection.csv"
#define MULTI_TOP_CSV "multi_top.csv"
#define MULTI_BOT_CSV "multi_bottom.csv"
#define TANDC_COL_CSV "color.csv"
#define TANDC_TEX_CSV "texture.csv"
#define CUST_COL_CSV "gallery_color.csv"
#define CUST_TEX_CSV "gallery_texture.csv"
#define ALIAS_CSV "alias.csv"
#define CT_AL_COL_CSV "alias_color.csv"
#define CT_AL_TEX_CSV "alias_texture.csv"

enum FeatureSet { None, BaseLine, Intersection, Multiple, ColorTexture, Custom, Alias, CTAlias };

enum TorC { Nope, Color, Texture };

/* *OVERLOAD* Function to conduct the 9X9 baseline matching
*  It writes to the proper image csv file
*  Parameter file_name is the name of source image to conduct the hitograming on
*  bins is the number of bins
*  Returns 0 when successful
*/
int baseline_processing(char *file_name, const int bins);

/* Function to conduct the baseline matching algorithm for an image
*  Does *NOT* write to the file
*  Parameter file_name is the name of source image to conduct the hitograming on
*  bins is the number of bins
*  dvect is the vector to wrtie to
*  Returns 0 when successful
*/
int baseline_processing(char *image_file_name, const int bins, std::vector<int> &dvect);

/* Function to create a standard histogram for an image.
*  It does *NOT* write to csv file
*  Parameter file_name is the name of source image to conduct the histograming on
*  Bins is the number of bins
*  dvect is the vector to write the histogram to
*  Returns 0 when successful
*/
int std_processing(char *image_file_name, const int bins, std::vector<int> &dvect);

/* *OVERLOAD* Function to create a standard histogram for an image.
*  It writes to the proper image csv file
*  Parameter file_name is the name of source image to conduct the hitograming on
*  feature indicates which feature is being used. Each has different behavior
*  torc is indicating if it's a texture or color histogram, and defaulted to neither
*  Returns 0 when successful
*/
int std_processing(char *image_file_name, const int bins, FeatureSet feature, TorC torc = Nope);

/* Function to create a histagrams from the top and bottom of an image.
*  Does not write to file
*  image_file name is the name of the image
*  bins is number of bins
*  dvect_top is the vector to write to for one section of the image
*  dvect_bottom is the vector to write to for the other section of the image
*  Returns 0 when successful
*/
int multi_hor_processing(char *image_file_name, const int bins, std::vector<int> &dvect_top, std::vector<int> &dvect_bottom);

/* *OVERLOAD* Function to create a histograms from the top and bottom of an image.
*  It writes to the proper image csv files multi_top and multi_bottom
*  Parameter file_name is the name of source image to conduct the histograming on
*  bins is the number of bins
*  Returns 0 when successful
*/
int multi_hor_processing(char *image_file_name, const int bins);

/* Function to create histograms using a color and texture gradient
*  Does *NOT* write to file
*  Parameter file_name is the name of source image to conduct the hitograming on
*  parameter dvect_texture is texture histogram
*  parameter dvect_color is color histogram
*  alias is a bool that indicates if aliasing is being used
*  Returns 0 when successful
*/
int texture_and_color(char *image_file_name, const int bins, std::vector<int> &dvect_texture, std::vector<int> &dvect_color, bool alias = 0);

/*  Function to create histograms using a color and texture gradient
*  Writes to proper image csv files texture.csv / color.csv
*  Parameter file_name is the name of source image to conduct the histograming on
*  bins is the number of bins
*  feature is the feature being conducted. each have different behavior
*  Returns 0 when successful
*/
int texture_and_color(char *image_file_name, const int bins, FeatureSet feature = ColorTexture);

/* *OVERLOAD* Function to create a standard histogram for an image.
*  Does not write to file
*  Parameter file_name is the name of source image to conduct the hitograming on
*  bins is the number of bins
*  dvect is the vector to write the histogram to
*  Returns 0 when successful
*/
int alias_processing(char *image_file_name, const int bins, std::vector<int> &dvect);

/* *OVERLOAD* Function to create a standard histogram for an image.
*  It writes to the proper image csv file
*  Parameter file_name is the name of source image to conduct the hitograming on
*  bins is the number of bins
*  Returns 0 when successful
*/
int alias_processing(char *image_file_name, const int bins);
