//
// Created by johnathan on 08/03/18.
//

#ifndef VSS_VISION_COLORRECOGNIZER_H
#define VSS_VISION_COLORRECOGNIZER_H

#include <Interfaces/IColorRecognizer.h>
#include <Domain/Constants.h>

class ColorRecognizer : public IColorRecognizer{
public:
    void setColorRange(ColorRange) override;
    void processImage(cv::Mat) override;
    void processImageInsideSectors(cv::Mat, std::vector<cv::Rect>, int, int) override;
    std::vector<cv::Rect> getRectangles() override;
    std::vector<cv::Point2f> getCenters() override;
    ColorType getColor() override;

    void binarizesImage(cv::Mat);
    std::vector<cv::Rect> recognizesRectangles(unsigned int = 1);
    void calculateCenters();
    cv::Point2f getCenter(cv::Rect);
    ColorRange getColorRange();
    cv::Mat getOriginalFrame();
    cv::Mat getBinaryFrame();

private:
    ColorRange colorRange;
    cv::Mat originalFrame;
    cv::Mat binaryFrame;
    std::vector<cv::Rect> rectangles;
    std::vector<cv::Point2f> centers;
};
#endif //VSS_VISION_COLORRECOGNIZER_H
