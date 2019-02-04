/*
 * This file is part of the VSS-Vision project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#ifndef VSS_VISION_IPATTERNRECOGNIZER_H
#define VSS_VISION_IPATTERNRECOGNIZER_H

#include <Domain/ObjectType.h>
#include <Domain/ColorRange.h>
#include <Domain/ColorPattern.h>
#include <Domain/ColorPosition.h>

class IPatternRecognizer {
public:
    virtual void recognizeMainColorBall(cv::Mat) = 0;
    virtual void recognizeMainColorTeam(cv::Mat) = 0;
    virtual void recognizeMainColorOpponent(cv::Mat) = 0;
    virtual void recognizeSecondColor(cv::Mat) = 0;
    virtual void setRangeVector(std::vector<ColorRange>) = 0;
    virtual void setPatternVector(std::vector<ColorPattern>) = 0;

    virtual ColorPosition getBallMainColorPosition() = 0;
    virtual ColorPosition getTeamMainColorPosition() = 0;
    virtual ColorPosition getOpponentMainColorPosition() = 0;
    virtual std::vector<ColorPosition> getTeamSecondColorPosition() = 0;

    virtual std::vector<cv::RotatedRect> getBallRotatedRect() = 0;
    virtual std::vector<cv::RotatedRect> getTeamRotatedRect() = 0;
    virtual std::vector<cv::RotatedRect> getOpponentRotatedRect() = 0;

    virtual cv::Mat getImage() = 0;
    virtual std::vector<cv::Rect> getRect() = 0;
    virtual std::vector<cv::RotatedRect> getRotatedRect() = 0;

};

#endif // VSS_VISION_IPATTERNRECOGNIZER_H
