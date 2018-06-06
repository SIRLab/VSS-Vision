//
// Created by johnathan on 08/03/18.
//

#include <Domain/ColorRange.h>
#include <vector>
#include <cxcore.h>
#include <ColorRecognizer.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <Domain/ColorSpace.h>

void ColorRecognizer::setColorRange(ColorRange colorRange) {
  this->colorRange = colorRange;
}

void ColorRecognizer::processImage(cv::Mat frame) {
  originalFrame = frame.clone();

  binarizesImage();
  recognizesRectangles();
  calculateCenters();
}

void ColorRecognizer::processImageInsideSectors(cv::Mat, std::vector<cv::Point>, int) {

}

std::vector<cv::Rect> ColorRecognizer::getRectangles() {
  return rectangles;
}

std::vector<cv::Point> ColorRecognizer::getCenters() {
  return centers;
}

void ColorRecognizer::binarizesImage() {
  cv::Mat processed;

  cv::inRange(originalFrame,
              cv::Scalar(colorRange.min[V], colorRange.min[S], colorRange.min[H]),
              cv::Scalar(colorRange.max[V], colorRange.max[S], colorRange.max[H]),
              processed);

  cv::medianBlur(processed, processed, 3);

  binaryFrame = processed.clone();
}

void ColorRecognizer::recognizesRectangles() {
  cv::Mat auxImage = binaryFrame.clone();
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(auxImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

  // sort in crescent order the contours vector by found area
  sort(contours.begin(), contours.end(),
      [](const std::vector<cv::Point> c1, const std::vector<cv::Point> c2){
          return cv::contourArea(c1, false) < cv::contourArea(c2, false);
      }
  );

  std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
  std::vector<cv::Rect> boundRect( contours.size() );

  for(unsigned int i = 0; i < contours.size(); i++){
    approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 0, true );
    boundRect[i] = boundingRect( cv::Mat(contours_poly[i]) );
  }

  rectangles = boundRect;
}

void ColorRecognizer::calculateCenters() {
  auto centers = std::vector<cv::Point>();
  auto centimeterPoint = cv::Point();

  for(unsigned int i = 0 ; i < rectangles.size() ; i++){
    auto pixelPoint = getCenter(rectangles.at(i));

    centimeterPoint = {
      (pixelPoint.x * 170) / originalFrame.cols,
      (pixelPoint.y * 130) / originalFrame.rows
    };

    centers.push_back(centimeterPoint);
  }

  this->centers = centers;
}

cv::Point ColorRecognizer::getCenter(cv::Rect rect) {
  cv::Point point = (rect.br() + rect.tl())*0.5;
  return point;
}

ColorRange ColorRecognizer::getColorRange() {
  return colorRange;
}

cv::Mat ColorRecognizer::getOriginalFrame() {
  return originalFrame;
}

cv::Mat ColorRecognizer::getBinaryFrame() {
  return binaryFrame;
}
