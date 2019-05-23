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
#include <Domain/ColorSpace.h>

void CalibrationWindow::applyActualColorRangeToSlidersHSV(ColorType type, std::vector<Gtk::Scale*> scale) {

    for(unsigned int i = 0 ; i < calibration.colorsRange.size() ; i++) {
        if(calibration.colorsRange.at(i).colorType == type) actualColorRangeIndex = i;
    }

    scale[H_MAX]->set_value(calibration.colorsRange.at(actualColorRangeIndex).max[H]);
    scale[S_MAX]->set_value(calibration.colorsRange.at(actualColorRangeIndex).max[S]);
    scale[V_MAX]->set_value(calibration.colorsRange.at(actualColorRangeIndex).max[V]);

    scale[H_MIN]->set_value(calibration.colorsRange.at(actualColorRangeIndex).min[H]);
    scale[S_MIN]->set_value(calibration.colorsRange.at(actualColorRangeIndex).min[S]);
    scale[V_MIN]->set_value(calibration.colorsRange.at(actualColorRangeIndex).min[V]);
}

void CalibrationWindow::updateLabel(int i){
    string title = "CALIBRATION (" + to_string(i) + ")";
    window->set_title(title);
}

bool CalibrationWindow::onKeyboard(GdkEventKey* event, Gtk::Window* window){
    if(event->keyval == GDK_KEY_space) {
        // window->hide();
    } else if(event->keyval == GDK_KEY_Return) {
        window->hide();
    } else if(event->keyval == GDK_KEY_Escape) {
        window->hide();
    } else if(event->keyval == GDK_KEY_F1) {
        showBinaryImage = !showBinaryImage;
    }
    return true;
}

void CalibrationWindow::onButtonSave(Gtk::FileChooserDialog* fileChooser, Gtk::Entry* entry){
    string filename = entry->get_text();
    if (not filename.empty()){
        std::stringstream aux;
        aux << fileChooser->get_current_folder() << "/" << filename;
        calibrationRepository->create(aux.str(), calibration);
        fileChooser->hide();
    }
}

void CalibrationWindow::onButtonLoad(Gtk::FileChooserDialog* fileChooser, Gtk::Entry* entry) {
    if (entry->get_text_length() > 0) {
        calibration = calibrationRepository->read(fileChooser->get_filename());
        fileChooser->hide();
    }

    mutexFrame.lock();
    cv::Mat image = frame;
    mutexFrame.unlock();

    perspectiveMatrix = getPerspectiveMatrix(image, calibration.cut);
}

void CalibrationWindow::onToggleButtonCutMode(Gtk::ToggleButton* toggleButton){
    screenImage->setCutMode(!toggleButton->get_active());

    if (toggleButton->get_active()) {
        calibration.cut.resize(4);
        calibration.cut[0] = vss::Point(screenImage->getPoints()[0].x, screenImage->getPoints()[0].y);
        calibration.cut[1] = vss::Point(screenImage->getPoints()[1].x, screenImage->getPoints()[1].y);
        calibration.cut[2] = vss::Point(screenImage->getPoints()[2].x, screenImage->getPoints()[2].y);
        calibration.cut[3] = vss::Point(screenImage->getPoints()[3].x, screenImage->getPoints()[3].y);

        mutexFrame.lock();
            cv::Mat image = frame;
        mutexFrame.unlock();

        perspectiveMatrix = getPerspectiveMatrix(image, calibration.cut);
    }
}

void CalibrationWindow::onButtonRestoreCut() {
    calibration.cut[0] = vss::Point(0,0);
    calibration.cut[1] = vss::Point(0,0);
    calibration.cut[2] = vss::Point(0,0);
    calibration.cut[3] = vss::Point(0,0);

    perspectiveMatrix = cv::Mat();
}

void CalibrationWindow::onSignalSelectFileInDialog(Gtk::FileChooserDialog* fileChooser, Gtk::Entry* entry){
    std::string str = fileChooser->get_filename();
    std::size_t sub_str = str.find_last_of("/\\");
    entry->set_text(str.substr(sub_str+1));
}

void CalibrationWindow::onComboBoxSelectPath(Gtk::ComboBoxText* inputPath){
    // std::cout << inputPath->get_active_row_number() << std::endl;
}

void CalibrationWindow::onComboBoxSelectColor(Gtk::ComboBoxText* combobox, std::vector<Gtk::Scale*> scale){
    int row = combobox->get_active_row_number();
    auto actualColorToCalibrate = toColorType(listColors[row]);
    applyActualColorRangeToSlidersHSV(actualColorToCalibrate, scale);
}

void CalibrationWindow::onButtonOpenSaveDialog(Gtk::FileChooserDialog* fileChooser, Gtk::Entry* entry){
    entry->set_sensitive(true);
    buttonLoad->set_visible(false);
    buttonSave->set_visible(true);
    fileChooser->show();
}

void CalibrationWindow::onButtonOpenLoadDialog(Gtk::FileChooserDialog* fileChooser, Gtk::Entry* entry){
    entry->set_sensitive(false);
    buttonLoad->set_visible(true);
    buttonSave->set_visible(false);
    fileChooser->show();
}

void CalibrationWindow::onRadioButtonImage(Gtk::RadioButton* radioButton){
    if(inputReader) {
        mutexChangeInput.lock();
        inputReader->stopReceivement();
        inputReader = new ImageFileReader();
        inputReader->setSource(inputReader->getAllPossibleSources().at(0));
        inputReader->initializeReceivement();
        mutexChangeInput.unlock();
    }
}

void CalibrationWindow::onRadioButtonCamera(Gtk::RadioButton* radioButton){
    if(inputReader) {
        mutexChangeInput.lock();
        inputReader->stopReceivement();
        inputReader = new CameraReader();
        inputReader->setSource(inputReader->getAllPossibleSources().at(0));
        inputReader->initializeReceivement();
        mutexChangeInput.unlock();
    }
}

void CalibrationWindow::onRadioButtonVideo(Gtk::RadioButton* radioButton){
    //  if (!radioButton->get_active())
    //    std::cout << "Video: " << radioButton->get_active() << std::endl;
}

void CalibrationWindow::onScaleHMAX(Gtk::Scale* scale){
    calibration.colorsRange.at(actualColorRangeIndex).max[H] = static_cast<float>(scale->get_value());
    colorRecognizer->setColorRange(calibration.colorsRange.at(actualColorRangeIndex));
}

void CalibrationWindow::onScaleHMIN(Gtk::Scale* scale){
    calibration.colorsRange.at(actualColorRangeIndex).min[H] = static_cast<float>(scale->get_value());
    colorRecognizer->setColorRange(calibration.colorsRange.at(actualColorRangeIndex));
}

void CalibrationWindow::onScaleSMAX(Gtk::Scale* scale){
    calibration.colorsRange.at(actualColorRangeIndex).max[S] = static_cast<float>(scale->get_value());
    colorRecognizer->setColorRange(calibration.colorsRange.at(actualColorRangeIndex));
}

void CalibrationWindow::onScaleSMIN(Gtk::Scale* scale){
    calibration.colorsRange.at(actualColorRangeIndex).min[S] = static_cast<float>(scale->get_value());
    colorRecognizer->setColorRange(calibration.colorsRange.at(actualColorRangeIndex));
}

void CalibrationWindow::onScaleVMAX(Gtk::Scale* scale){
    calibration.colorsRange.at(actualColorRangeIndex).max[V] = static_cast<float>(scale->get_value());
    colorRecognizer->setColorRange(calibration.colorsRange.at(actualColorRangeIndex));
}

void CalibrationWindow::onScaleVMIN(Gtk::Scale* scale){
    calibration.colorsRange.at(actualColorRangeIndex).min[V] = static_cast<float>(scale->get_value());
    colorRecognizer->setColorRange(calibration.colorsRange.at(actualColorRangeIndex));
}