/*
* Nate Novak
* CS5330 Project 2 // Spring 2021
* Function library for creating histograms
*/ 

#include "../include/histogram.h"

using namespace std; 

/* Function to make a 3D histogram for an image
* Parameter src is the source image
* parameter bins is the number of bins to write to. 
* returns 0 to indicate success
*/
cv::Mat std_D3_histogram(const cv::Mat &src, const int bins) {
  
  // dimensions
  int dim3[3] = { bins, bins, bins }; 

  // ALWAYS BE STORED AS RGB BECAUSE I'M NORMAL
  cv::Mat hist = cv::Mat::zeros(3, dim3, CV_32S);
  
  const uchar divisor = 256 / bins; 

  for(int r = 0; r < src.rows; r++) {
    for(int c = 0; c < src.cols; c++) {
        cv::Vec3b colors = src.at<cv::Vec3b>(r, c); 
        uchar bix = colors[0] / divisor; 
        uchar gix = colors[1] / divisor; 
        uchar rix = colors[2] / divisor;
        hist.at<int>(rix, gix, bix)++; 
      }
    }
  return hist; 
}

/* Function to make a 2D histogram for an image
* Parameter src is the source image
* parameter bins is the number of bins to write to. 
* returns 0 to indicate success
*/
cv::Mat std_D2_histogram(const cv::Mat &src, const int bins) {
  
  int dim2[2] = { bins, bins };  

  cv::Mat hist = cv::Mat::zeros(2, dim2, CV_32S);
  
  const uchar divisor = 256 / bins; 

  for(int r = 0; r < src.rows; r++) {
    for(int c = 0; c < src.cols; c++) {
        cv::Vec3b colors = src.at<cv::Vec3b>(r, c); 
        uchar blue = colors[0]; 
        uchar green = colors[1]; 
        uchar red = colors[2]; 
        float r = ((float) red) / ((float) (red + green + blue + 1.0)); 
        float g = ((float) green) / ((float) (red + green + blue + 1.0)); 
        int rix = (int) (r * bins); 
        int gix = (int) (g * bins); 

        hist.at<int>(rix, gix)++; 
      }
    }
  return hist; 
}

/* Function to make a 1D histogram for an image
* Parameter src is the source image
* parameter bins is the number of bins to write to. 
* returns 0 to indicate success
*/
cv::Mat std_D1_histogram(const cv::Mat &src, const int bins) {
  cv::Mat newsrc; 
  cv::cvtColor(src, newsrc, cv::COLOR_BGR2GRAY);
  int dim1[1] = { bins };

  cv::Mat hist = cv::Mat::zeros(1, dim1, CV_32S);

  const uchar divisor = 256 / bins; 

  for(int r = 0; r < src.rows; r++) {
    for(int c = 0; c < src.cols; c++) {
      uchar gray_index = newsrc.at<uchar>(r, c) / divisor;
      hist.at<int>(gray_index)++; 
    }
  }

  return hist; 
}

/* Function to make a 2D histogram for an image using aliasing
* Parameter src is the source image
* parameter bins is the number of bins to write to. 
* returns 0 to indicate success
*/
cv::Mat alias_D2_histogram(const cv::Mat &src, const int bins) {
  int dim2[2] = { bins, bins };  

  cv::Mat hist = cv::Mat::zeros(2, dim2, CV_32S);
  
  const uchar divisor = 256 / bins; 

  for(int r = 0; r < src.rows; r++) {
    for(int c = 0; c < src.cols; c++) {
        cv::Vec3b colors = src.at<cv::Vec3b>(r, c); 
        uchar blue = colors[0]; 
        uchar green = colors[1]; 
        uchar red = colors[2]; 
        float r = ((float) red) / ((float) (red + green + blue + 1.0)); 
        float g = ((float) green) / ((float) (red + green + blue + 1.0)); 
        int rix = (int) (r * bins); 
        int gix = (int) (g * bins); 

        bool is_top_row = rix == 0; 
        bool is_bottom_row = rix == hist.rows - 1; 
        bool is_far_left = gix == 0; 
        bool is_far_right = gix == hist.cols - 1; 

        if(is_top_row && is_far_left) { // corners
          hist.at<int>(rix, gix) += 4; 
          hist.at<int>(rix, gix + 1) += 2; 
          hist.at<int>(rix + 1, gix) += 2; 
          hist.at<int>(rix + 1, gix - 1) += 2;   
        } else if(is_top_row && is_far_right) {
          hist.at<int>(rix, gix - 1) += 2; 
          hist.at<int>(rix, gix) += 4; 
          hist.at<int>(rix + 1, gix - 1) += 1; 
          hist.at<int>(rix + 1, gix) += 2; 
        } else if(is_bottom_row && is_far_left) {
          hist.at<int>(rix - 1, gix) += 2; 
          hist.at<int>(rix - 1, gix + 1) += 1;
          hist.at<int>(rix, gix) += 4; 
          hist.at<int>(rix, gix + 1) += 2;  
        } else if(is_bottom_row && is_far_right) {
          hist.at<int>(rix - 1, gix - 1) += 1; 
          hist.at<int>(rix - 1, gix) += 2; 
          hist.at<int>(rix, gix - 1) += 2; 
          hist.at<int>(rix, gix) += 4; 
        } else if(is_top_row && !(is_far_left || is_far_right)) { // top/bottom edges
          // middle
          hist.at<int>(rix, gix - 1) += 2; 
          hist.at<int>(rix, gix) += 4;
          hist.at<int>(rix, gix + 1) += 2; 
          // bottom
          hist.at<int>(rix + 1, gix - 1) += 1; 
          hist.at<int>(rix + 1, gix) += 2; 
          hist.at<int>(rix + 1, gix + 1) += 1; 
        } else if(is_bottom_row && !(is_far_left || is_far_right)) {
          // top
          hist.at<int>(rix - 1, gix - 1) += 1; 
          hist.at<int>(rix - 1, gix) += 2; 
          hist.at<int>(rix - 1, gix + 1) += 1; 
          // middle
          hist.at<int>(rix, gix - 1) += 2; 
          hist.at<int>(rix, gix) += 4; 
          hist.at<int>(rix, gix + 1) += 2; 
        } else if(is_far_left && !(is_top_row || is_bottom_row)) { // left / right edges
          // top row
          hist.at<int>(rix - 1, gix) += 2; 
          hist.at<int>(rix - 1, gix + 1) +=1; 
          // middle row
          hist.at<int>(rix, gix) += 4; 
          hist.at<int>(rix, gix + 1) += 2; 
          //bottom
          hist.at<int>(rix + 1, gix) += 2; 
          hist.at<int>(rix + 1, gix + 1) += 1; 
        } else if(is_far_right && !(is_top_row || is_bottom_row)) {
          // top row
          hist.at<int>(rix - 1, gix - 1) +=1; 
          hist.at<int>(rix - 1, gix) += 2; 
          // middle row
          hist.at<int>(rix, gix - 1) += 2; 
          hist.at<int>(rix, gix) += 4; 
          // bottom row
          hist.at<int>(rix - 1, gix -1) += 1; 
          hist.at<int>(rix - 1, gix) += 2; 
        } else { // any other cell
          //top
          hist.at<int>(rix - 1, gix - 1) += 1; 
          hist.at<int>(rix - 1, gix) += 2; 
          hist.at<int>(rix - 1, gix + 1) += 1; 
          //middle
          hist.at<int>(rix, gix - 1) += 2; 
          hist.at<int>(rix, gix) += 4; 
          hist.at<int>(rix, gix + 1) += 2; 
          //bot
          hist.at<int>(rix + 1, gix - 1) += 1; 
          hist.at<int>(rix + 1, gix) += 2; 
          hist.at<int>(rix + 1, gix + 1) += 1; 
        }
      }
    }
  return hist; 
}

/* Function to convert a Mat to a vector
* Parameter src is the source image
* parameter bins is the number of bins to write to. 
* returns 0 to indicate success
*/
std::vector<int> hist_to_vector(const cv::Mat src, const int bins, const int dims) 
{
  std::vector<int> dst; 
  switch(dims) {
    case 1: {
      for(int i = 0; i < bins; i++) {
        dst.push_back(src.at<int>(i)); 
      }
      break;
    }

    case 2: {
      for(int r = 0; r < bins; r++) {
        for(int c = 0; c < bins; c++) {
          dst.push_back(src.at<int>(r, c)); 
        }
      }
      break; 
    }

    case 3: {
      for(int r = 0; r < bins; r++) {
        for(int c = 0; c < bins; c++) {
          for(int z = 0; z < bins; z++) {
            dst.push_back(src.at<int>(r, c, z)); 
          }
        }
      }
      break; 
    }

    default: 
      break;
  }

  return dst; 
}