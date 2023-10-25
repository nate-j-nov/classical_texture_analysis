/* Nate Novak
* CS5330 // Spring 2022
* Header file for histogramming library
*/

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>

/* Function to make a 3D histogram for an image
* Parameter src is the source image
* parameter bins is the number of bins to write to.
* returns 0 to indicate success
*/
cv::Mat std_D3_histogram(const cv::Mat &src, const int bins);

/* Function to make a 2D histogram for an image
* Parameter src is the source image
* parameter bins is the number of bins to write to.
* returns 0 to indicate success
*/
cv::Mat std_D2_histogram(const cv::Mat &src, const int bins);

/* Function to make a 1D histogram for an image
* Parameter src is the source image
* parameter bins is the number of bins to write to.
* returns 0 to indicate success
*/
cv::Mat std_D1_histogram(const cv::Mat &src, const int bins);

/* Function to make a 2D histogram for an image using aliasing
* Parameter src is the source image
* parameter bins is the number of bins to write to.
* returns 0 to indicate success
*/
cv::Mat alias_D2_histogram(const cv::Mat &src, const int bins);

/* Function to convert a Mat to a vector
* Parameter src is the source image
* parameter bins is the number of bins to write to.
* returns 0 to indicate success
*/
std::vector<int> hist_to_vector(const cv::Mat src, const int bins, const int dims);
