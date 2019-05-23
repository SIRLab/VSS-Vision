//
// Created by johnathan on 03/02/18.
//

#ifndef VSS_VISION_TYPECOLOR_H
#define VSS_VISION_TYPECOLOR_H
#include <opencv2/core/types.hpp>
#include "iostream"

enum ColorType {
    UnknownType = 0,
    Blue = 1,
    Yellow = 2,
    Orange = 3,
    Green = 4,
    Pink = 5,
    Purple = 6,
    Red = 7,
    Brown = 8
};

std::string toDescription(ColorType);
ColorType toColorType(std::string);
cv::Scalar colorRGB(std::string colorName);

#endif //VSS_VISION_TYPECOLOR_H
