/*
* This file is part of the VSS-Vision project.
*
* This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
* v. 3.0. If a copy of the GPL was not distributed with this
* file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
*/

#ifndef GPROGRAM_H_
#define GPROGRAM_H_

#include "GImage.h"
#include "ICalibrationWindow.h"
#include "opencv2/highgui/highgui.hpp"
#include <gtkmm.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <Interfaces/ICalibrationRepository.h>
#include <Interfaces/ICalibrationBuilder.h>
#include <Interfaces/IInputReader.h>
#include <Interfaces/IColorRecognizer.h>
#include <Domain/ProgramState.h>
#include <Domain/ColorSpace.h>
#include <Helpers/FrameHelper.h>
#include <Helpers/DefaultFilesPath.h>
#include <Helpers/TimeHelper.h>
#include <Repositories/CalibrationRepository.h>
#include <Builders/CalibrationBuilder.h>
#include <ImageFileReader.h>
#include <CameraReader.h>
#include <ColorRecognizer.h>

using namespace std;

class CalibrationWindow : public ICalibrationWindow {
public:
    CalibrationWindow();
    virtual ~CalibrationWindow();

    int run(int argc, char *argv[]) override;

    bool onKeyboard(GdkEventKey*, Gtk::Window*) override;

    void onButtonOpenSaveDialog(Gtk::FileChooserDialog*, Gtk::Entry*) override;
    void onButtonOpenLoadDialog(Gtk::FileChooserDialog*, Gtk::Entry*) override;

    void onButtonSave(Gtk::FileChooserDialog*, Gtk::Entry*) override;
    void onButtonLoad(Gtk::FileChooserDialog*, Gtk::Entry*) override;

    void onScaleHMAX(Gtk::Scale*) override;
    void onScaleHMIN(Gtk::Scale*) override;
    void onScaleSMAX(Gtk::Scale*) override;
    void onScaleSMIN(Gtk::Scale*) override;
    void onScaleVMAX(Gtk::Scale*) override;
    void onScaleVMIN(Gtk::Scale*) override;

    void onRadioButtonImage(Gtk::RadioButton*) override;
    void onRadioButtonVideo(Gtk::RadioButton*) override;
    void onRadioButtonCamera(Gtk::RadioButton*) override;

    void onToggleButtonCutMode(Gtk::ToggleButton*) override;
    void onButtonRestoreCut() override;

    void onSignalSelectFileInDialog(Gtk::FileChooserDialog*, Gtk::Entry*) override;

    void onComboBoxSelectPath(Gtk::ComboBoxText*) override;
    void onComboBoxSelectColor(Gtk::ComboBoxText*, std::vector<Gtk::Scale*>) override;

private:

    // Threads
    std::thread *threadCameraReader;
    std::thread *threadWindowControl;

    mutable std::mutex mutexCalibration;
    mutable std::mutex mutexChangeInput;
    mutable std::mutex mutexFrame;
    mutable std::mutex mutexFPS;

    // Comunication between threads
    Glib::Dispatcher dispatcher_update_gtkmm_frame;
    sigc::connection connection_update_screen;

    // Classes
    IInputReader *inputReader;
    IColorRecognizer *colorRecognizer;
    ICalibrationBuilder *calibrationBuilder;
    ICalibrationBuilder *calibrationBuilderFromRepository;
    ICalibrationRepository *calibrationRepository;

    Calibration calibration;

    bool showBinaryImage;
    bool shouldReadInput;
    unsigned int actualColorRangeIndex;

    TimeHelper timeHelper;

    // Opencv image
    cv::Mat frame;
    cv::Mat perspectiveMatrix;

    // GTKMM - Calibration Window
    Gtk::Window* window = nullptr;

    GImage* screenImage = nullptr;

    Gtk::RadioButton* radioButtonImage = nullptr;
    Gtk::RadioButton* radioButtonVideo = nullptr;
    Gtk::RadioButton* radioButtonCamera = nullptr;

    Gtk::ComboBoxText* comboBoxPath = nullptr;
    Gtk::ComboBoxText* comboBoxColor = nullptr;

    std::vector<string> listColors;

    std::vector<Gtk::Scale*> scaleHSV;

    Gtk::Button* buttonSave = nullptr;
    Gtk::Button* buttonRestoreCut = nullptr;

    // GTKMM - File Chooser Window
    Gtk::FileChooserDialog* fileChooserDialog = nullptr;

    Gtk::Button* buttonLoad = nullptr;
    Gtk::ToggleButton* toggleButtonCutMode = nullptr;

    Gtk::Entry* entryChooserDialog = nullptr;

    Gtk::Button* buttonOpenSaveDialog = nullptr;
    Gtk::Button* buttonOpenLoadDialog = nullptr;


    // Control method
    void initializeWidget();
    void setSignals();
    void builderWidget();

    void windowThreadWrapper();
    void cameraThreadWrapper();

    // Update frame
    void processFrame(cv::Mat);
    void receiveNewFrame(cv::Mat);
    void updateGtkImage();
    bool emitUpdateGtkImage();

    // Events
    void applyActualColorRangeToSlidersHSV(ColorType type, std::vector<Gtk::Scale*>);

    void updateLabel(int);
};
#endif
