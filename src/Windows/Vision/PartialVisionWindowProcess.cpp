/*
* This file is part of the VSS-Vision project.
*
* This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
* v. 3.0. If a copy of the GPL was not distributed with this
* file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
*/

#include <Windows/Vision/VisionWindow.h>

void VisionWindow::setNewFrame() {
    processFrame();
    screenImage->set_image(frame);
}

void VisionWindow::receiveNewFrame(cv::Mat _frame) {
    frame = _frame;
    signal_set_new_frame.emit();
}

void VisionWindow::processFrame() {
    changeRotation(frame, calibration.rotation);

    if(calibration.shouldCropImage){
        cropImage(frame, calibration.cut[0], calibration.cut[1]);
    }

    map<WhoseName, ColorPosition> positions = getColorPosition();

    robotRecognizer->recognizeRobots(positions);

    signalRobotsNewPositions.emit(robotRecognizer->getRobots(), robotRecognizer->getBall());
}

std::map<WhoseName, ColorPosition> VisionWindow::getColorPosition() {
    map<WhoseName, ColorPosition> whosePosition;

    for (auto colorRange : calibration.colorsRange) {
        WhoseName objectName = game->whoseColor[colorRange.colorType];

        if (objectName != WhoseName::Unknown) {
            colorRecognizer->setColorRange(colorRange);
            colorRecognizer->processImage(frame);

            ColorPosition colorPosition;
            colorPosition.color = colorRange.colorType;
            colorPosition.points = colorRecognizer->getCenters();

            whosePosition[objectName] = colorPosition;

            /* TO DRAW IN IMAGE */
            auto rectangles = colorRecognizer->getRectangles();
            for (unsigned int i = 0; i < rectangles.size(); i++) {
                cv::rectangle(frame, rectangles.at(i), cv::Scalar(255, 255, 255), 1, 1, 0);
            }
        }
    }

    return whosePosition;
}