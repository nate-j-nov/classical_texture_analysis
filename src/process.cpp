/* Nate Novak
* CS5330 // Spring 2022
* File that conducts the analysis for each step of this project
*/

#include "../include/process.h"

/* *OVERLOAD* Function to conduct the 9X9 baseline matching
*  It writes to the proper image csv file 
*  Parameter file_name is the name of source image to conduct the hitograming on
*  bins is the number of bins
*  Returns 0 when successful
*/
int baseline_processing(char *image_file_name, const int bins) {
  // Convert this histogram to a vector
  std::vector<int> image_data; 
  int success = baseline_processing(image_file_name, bins, image_data); 
  
  if(success != 0) {
    printf("Error creating feature vector for %s\n", image_file_name); 
  }

  // Write this vector to the file.
  success = veci_append_image_data_csv(BL_CSV, image_file_name, image_data, 0);
  return success; 
} 

/* Function to conduct the baseline matching algorithm for an image
*  Does *NOT* write to the file 
*  Parameter file_name is the name of source image to conduct the hitograming on
*  bins is the number of bins
*  dvect is the vector to wrtie to
*  Returns 0 when successful
*/
int baseline_processing(char *image_file_name, const int bins, std::vector<int> &dvect) {
  // First, read in the image file 
  cv::Mat src = cv::imread(image_file_name, cv::IMREAD_COLOR);
  
  // Then get the middle 9X9 square
  int xcenter = src.cols / 2; 
  int ymiddle = src.rows / 2; 
  int xstart = xcenter - 4;
  int ystart = ymiddle - 4; 

  cv::Mat temp = src(cv::Rect(xstart, ystart, 9, 9));  
  cv::Mat newsrc; 
  temp.copyTo(newsrc);
  
  dvect.clear(); 
  // Create a histogram of the 9X9 square
  for(int i = 0; i < newsrc.rows; i++) {
    for(int j = 0; j < newsrc.cols; j++) {
      for(int z = 0; z < 3; z++) {
        dvect.push_back(newsrc.at<cv::Vec3b>(i, j)[z]); 
      }   
    }
  }

  return 0; 
} 

/* Function to create a standard histogram for an image.  
*  It does *NOT* write to csv file 
*  Parameter file_name is the name of source image to conduct the histograming on
*  Bins is the number of bins
*  dvect is the vector to write the histogram to
*  Returns 0 when successful
*/
int std_processing(char *image_file_name, const int bins, std::vector<int> &dvect) {
  cv::Mat src = imread(image_file_name, cv::IMREAD_COLOR); // read image

  cv::Mat hist = std_D3_histogram(src, bins); // make histogram

  std::vector<int> dummy = hist_to_vector(hist, bins, 3); // convert to vector<int>

  dvect.clear(); 
  dvect.insert(dvect.end(), dummy.begin(), dummy.end()); // add to destination vector 
  return 0; 
}

/* *OVERLOAD* Function to create a standard histogram for an image.  
*  It writes to the proper image csv file 
*  Parameter file_name is the name of source image to conduct the hitograming on
*  feature indicates which feature is being used. Each has different behavior
*  torc is indicating if it's a texture or color histogram, and defaulted to neither
*  Returns 0 when successful
*/
int std_processing(char *image_file_name, const int bins, FeatureSet feature, TorC torc) {
  std::vector<int> image_data; 

  int success = std_processing(image_file_name, bins, image_data); 

  if(success != 0) {
    printf("Error feature vector for %s\n", image_file_name); 
  }

  char fv_file_name[256]; 
  switch(feature) {
    case Intersection:
      strcpy(fv_file_name, INTER_CSV); 
      break; 
    
    case ColorTexture: 
    {
      switch(torc) {
        case Color: 
          strcpy(fv_file_name, TANDC_COL_CSV); 
          break;
        
        case Texture: 
          strcpy(fv_file_name, TANDC_COL_CSV); 
          break; 
      }
      break;
    }
      
    
    default: 
      printf("Cannot do standard processing and write to file associate with %d", feature); 
      return -1; 
  }

  success = veci_append_image_data_csv(INTER_CSV, image_file_name, image_data, 0); // write to csv

  return success; 
}

/* Function to create a histagrams from the top and bottom of an image.  
*  Does not write to file
*  image_file name is the name of the image
*  bins is number of bins
*  dvect_top is the vector to write to for one section of the image
*  dvect_bottom is the vector to write to for the other section of the image
*  Returns 0 when successful
*/
int multi_hor_processing(char *image_file_name, const int bins, std::vector<int> &dvect_top, std::vector<int> &dvect_bottom) {
  cv::Mat src = imread(image_file_name, cv::IMREAD_COLOR); // read image

  int mid = src.rows / 2; 

  cv::Mat top = src(cv::Rect(0, 0, src.cols, mid));
  
  cv::Mat bottom = src(cv::Rect(0, mid, src.cols, mid)); // may need to adjust

  // make hists
  cv::Mat hist_top = std_D3_histogram(top, bins); 
  cv::Mat hist_bottom = std_D3_histogram(bottom, bins); 

  // convert to to vectors
  std::vector<int> vec_top = hist_to_vector(hist_top, bins, 3); 
  std::vector<int> vec_bottom = hist_to_vector(hist_bottom, bins, 3); 

  // Insert into destination vectors
  dvect_top.clear(); 
  dvect_top.insert(dvect_top.end(), vec_top.begin(), vec_top.end()); 

  dvect_bottom.clear(); 
  dvect_bottom.insert(dvect_bottom.end(), vec_bottom.begin(), vec_bottom.end()); 

  return 0; 

}

/* *OVERLOAD* Function to create a histograms from the top and bottom of an image.
*  It writes to the proper image csv files multi_top and multi_bottom 
*  Parameter file_name is the name of source image to conduct the histograming on
*  bins is the number of bins
*  Returns 0 when successful
*/
int multi_hor_processing(char *image_file_name, const int bins) {
  std::vector<int> image_data_top; 
  std::vector<int> image_data_bottom; 

  int success = multi_hor_processing(image_file_name, bins, image_data_top, image_data_bottom); 

  if(success != 0) {
    printf("Error feature vector for %s\n", image_file_name); 
  }

  success = veci_append_image_data_csv(MULTI_TOP_CSV, image_file_name, image_data_top, 0);

  if(success != 0) {
    printf("error writing data for %s to %s", image_file_name, MULTI_TOP_CSV); 
  }

  success = veci_append_image_data_csv(MULTI_BOT_CSV, image_file_name, image_data_bottom, 0); 

  return success; 
}

/* Function to create histograms using a color and texture gradient
*  Does *NOT* write to file
*  Parameter file_name is the name of source image to conduct the hitograming on
*  parameter dvect_texture is texture histogram 
*  parameter dvect_color is color histogram
*  alias is a bool that indicates if aliasing is being used
*  Returns 0 when successful
*/
int texture_and_color(char *image_file_name, const int bins, std::vector<int> &dvect_texture, std::vector<int> &dvect_color, bool alias) {
  std::vector<int> color_hist;
  int success = -1; 
  if(alias) {
    success = alias_processing(image_file_name, bins, color_hist);
  } else {
    success = std_processing(image_file_name, bins, color_hist); 
  }

  // Read in source
  cv::Mat src = imread(image_file_name, IMREAD_COLOR); 
  // create a vector magnitude image of src. 
  cv::Mat xsob; 
  cv::Mat ysob; 
  src.copyTo(xsob); 
  src.copyTo(ysob); 
  
  sobelX3x3(src, xsob); 
  sobelY3x3(src, ysob); 
  
  cv::Mat newsrc; 
  src.copyTo(newsrc); 
  magnitude_nate(xsob, ysob, newsrc); 

  cv::Mat texture_mat; 
  std::vector<int> texture_hist;
  if(alias) {
    texture_mat = alias_D2_histogram(newsrc, bins); 
    texture_hist = hist_to_vector(texture_mat, bins, 2);
    
  } else {
    texture_mat = std_D3_histogram(newsrc, bins);
    texture_hist = hist_to_vector(texture_mat, bins, 3);
  }
  
  dvect_color.clear(); 
  dvect_color.insert(dvect_color.end(), color_hist.begin(), color_hist.end()); 

  dvect_texture.clear(); 
  dvect_texture.insert(dvect_texture.end(), texture_hist.begin(), texture_hist.end()); 

  return success; 
}

/* *OVERLOAD* Function to create histograms using a color and texture gradient
*  Writes to proper image csv files texture.csv / color.csv
*  Parameter file_name is the name of source image to conduct the histograming on
*  bins is the number of bins
*  feature is the feature being conducted. each have different behavior
*  Returns 0 when successful
*/
int texture_and_color(char *image_file_name, const int bins, FeatureSet feature) {
  std::vector<int> image_data_color; 
  std::vector<int> image_data_texture; 
  int success = -1; 

  if(feature != CTAlias){
    success = texture_and_color(image_file_name, bins, image_data_texture, image_data_color); 
  } else {
    success = texture_and_color(image_file_name, bins, image_data_texture, image_data_color, true); 
  }
  
  if(success != 0) {
    printf("Error feature vector for %s\n", image_file_name); 
  }

  if(feature == ColorTexture) {
    success = veci_append_image_data_csv(TANDC_COL_CSV, image_file_name, image_data_color, 0);
    success = veci_append_image_data_csv(TANDC_TEX_CSV, image_file_name, image_data_texture, 0);
  } else if(feature == Custom) {
    success = veci_append_image_data_csv(CUST_COL_CSV, image_file_name, image_data_color, 0);
    success = veci_append_image_data_csv(CUST_TEX_CSV, image_file_name, image_data_texture, 0);
  } else if(feature == CTAlias){
    success = veci_append_image_data_csv(CT_AL_COL_CSV, image_file_name, image_data_color, 0); 
    success = veci_append_image_data_csv(CT_AL_TEX_CSV, image_file_name, image_data_texture, 0); 
  } else  {
    printf("Not a valid feature input\n"); 
    return -1; 
  }

  if(success != 0)  {
    printf("Error writing data to file\n"); 
  }

  return success; 
}

/* *OVERLOAD* Function to create a standard histogram for an image.  
*  Does not write to file 
*  Parameter file_name is the name of source image to conduct the hitograming on
*  bins is the number of bins
*  dvect is the vector to write the histogram to
*  Returns 0 when successful
*/
int alias_processing(char *image_file_name, const int bins, std::vector<int> &dvect) {
  cv::Mat src = imread(image_file_name, cv::IMREAD_COLOR); // read image

  cv::Mat hist = alias_D2_histogram(src, bins); // make histogram

  std::vector<int> dummy = hist_to_vector(hist, bins, 2); // convert to vector<int>

  dvect.clear(); 
  dvect.insert(dvect.end(), dummy.begin(), dummy.end()); // add to destination vector

  return 0; 
}

/* *OVERLOAD* Function to create a standard histogram for an image.  
*  It writes to the proper image csv file 
*  Parameter file_name is the name of source image to conduct the hitograming on
*  bins is the number of bins
*  Returns 0 when successful
*/
int alias_processing(char *image_file_name, const int bins) {
  std::vector<int> image_data; 

  int success = alias_processing(image_file_name, bins, image_data); 

  if(success != 0) {
    printf("Error feature vector for %s\n", image_file_name); 
  }

  success = veci_append_image_data_csv(ALIAS_CSV, image_file_name, image_data, 0); // write to csv

  return success; 
}