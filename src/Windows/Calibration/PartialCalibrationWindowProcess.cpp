/*
* This file is part of the VSS-Vision project.
*
* This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
* v. 3.0. If a copy of the GPL was not distributed with this
* file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
*/

#include <Repositories/CalibrationRepository.h>
#include <CameraReader.h>
#include <Windows/Calibration/CalibrationWindow.h>

void CalibrationWindow::receiveNewFrame(cv::Mat _frame){
    frame = _frame.clone();
    dispatcher_update_gtkmm_frame.emit();
}

void CalibrationWindow::updateGtkImage(){
    cv::Mat processedFrame = processFrame(frame.clone());
    screenImage->set_image(processedFrame, showBinaryImage);
    updateFpsLabel( timeHelper.framesPerSecond() );
}

cv::Mat CalibrationWindow::processFrame(cv::Mat _frame) {
    changeRotation(_frame, calibration.rotation);

    if(calibration.shouldCropImage) {
        cropImage(_frame, calibration.cut[0], calibration.cut[1]);
    }

    colorRecognizer->processImage(_frame);

    if(showBinaryImage){
        return colorRecognizer->getBinaryFrame();
    }

    drawRectangle(_frame, colorRecognizer->getRectangles());

    return _frame;
}
