/*
* This file is part of the VSS-Vision project.
*
* This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
* v. 3.0. If a copy of the GPL was not distributed with this
* file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
*/

#ifndef VISION_WINDOW_H_
#define VISION_WINDOW_H_

#include <thread>
#include <gtkmm.h>
#include <iostream>

#include "DefaultFilesPath.h"
#include <CameraReader.h>
#include <ColorRecognizerTwoColors.h>
#include <ImageFileReader.h>
#include <Domain/ProgramState.h>
#include <Domain/ObjectType.h>
#include <Domain/ColorType.h>
#include <Domain/ColorPosition.h>
#include <Interfaces/IInputReader.h>
#include <Interfaces/ICalibrationRepository.h>
#include <Interfaces/ICalibrationBuilder.h>
#include <Interfaces/IColorRecognizer.h>
#include <Helpers/FrameHelper.h>
#include <Helpers/TimeHelper.h>
#include <Domain/Ball.h>
#include <Domain/Robot.h>
#include <Interfaces/IRobotRecognizer.h>
#include <RobotRecognizerTwoColors.h>
#include <StateSenderAdapter.h>
#include "GImage.h"
#include "IVisionWindow.h"
#include "opencv2/highgui/highgui.hpp"

using namespace std;

class VisionWindow : public IVisionWindow {
public:
    VisionWindow();
    virtual ~VisionWindow();

    int run(int argc, char *argv[]) override;

    bool onKeyboard(GdkEventKey *, Gtk::Window *) override;
    void onButtonPlay(Gtk::ToggleButton *) override;
    void onButtonLoad(Gtk::FileChooserDialog *) override;
    void onButtonOpenLoadDialog(Gtk::FileChooserDialog *) override;

    void onRadioButtonImage(Gtk::RadioButton *) override;
    void onRadioButtonVideo(Gtk::RadioButton *) override;
    void onRadioButtonCamera(Gtk::RadioButton *) override;

    void onComboBoxSelectPath(Gtk::ComboBox *) override;

    void onComboBoxSelectColorTeam(Gtk::ComboBox *) override;
    void onComboBoxSelectColorOpponent(Gtk::ComboBox *) override;
    void onComboBoxSelectColorRobot1(Gtk::ComboBox *) override;
    void onComboBoxSelectColorRobot2(Gtk::ComboBox *) override;
    void onComboBoxSelectColorRobot3(Gtk::ComboBox *) override;
    void onComboBoxSelectColorRobot4(Gtk::ComboBox *) override;
    void onComboBoxSelectColorRobot5(Gtk::ComboBox *) override;
    void onRobotsNewPositions(std::vector<vss::Robot> blueRobots, std::vector<vss::Robot> yellowRobots, vss::Ball ball) override;

private:
    IStateSenderAdapter* stateSender;

    // Threads
    std::thread *threadCameraReader;
    std::thread *threadWindowControl;

    mutable std::mutex mtx;

    // Comunication between threads
    Glib::Dispatcher dispatcher_update_gtkmm_frame;

    // Classes
    IInputReader *inputReader;
    ICalibrationBuilder *calibrationBuilderFromRepository;
    ICalibrationRepository *calibrationRepository;
    IRobotRecognizer *robotRecognizer;

    Calibration calibration;

    TimeHelper timeHelper;
    TimeHelper timerBall;
    TimeHelper timerTeam;
    TimeHelper timerOpponent;
    std::map<ColorType, std::vector<cv::Rect>> cutPosition;

    bool playing;

    // Opencv image
    cv::Mat frame;

    // GTKMM - Calibration Window
    Gtk::Window *window = nullptr;
    GImage *screenImage = nullptr;

    Gtk::RadioButton *radioButtonImage = nullptr;
    Gtk::RadioButton *radioButtonVideo = nullptr;
    Gtk::RadioButton *radioButtonCamera = nullptr;

    Gtk::ComboBox *comboBoxPath = nullptr;
    Gtk::ComboBox *comboBoxColorTeam1 = nullptr;
    Gtk::ComboBox *comboBoxColorTeam2 = nullptr;
    Gtk::ComboBox *comboBoxColorRobot1 = nullptr;
    Gtk::ComboBox *comboBoxColorRobot2 = nullptr;
    Gtk::ComboBox *comboBoxColorRobot3 = nullptr;
    Gtk::ComboBox *comboBoxColorRobot4 = nullptr;
    Gtk::ComboBox *comboBoxColorRobot5 = nullptr;

    Gtk::Label *labelPositionBall = nullptr;

    Gtk::Label *labelPositionRobot1 = nullptr;
    Gtk::Label *labelPositionRobot2 = nullptr;
    Gtk::Label *labelPositionRobot3 = nullptr;
    Gtk::Label *labelPositionRobot4 = nullptr;
    Gtk::Label *labelPositionRobot5 = nullptr;

    Gtk::Label *labelPositionOpponent1 = nullptr;
    Gtk::Label *labelPositionOpponent2 = nullptr;
    Gtk::Label *labelPositionOpponent3 = nullptr;
    Gtk::Label *labelPositionOpponent4 = nullptr;
    Gtk::Label *labelPositionOpponent5 = nullptr;

    Gtk::Button *buttonLoad = nullptr;
    Gtk::ToggleButton *buttonPlay = nullptr;

    // GTKMM - File Chooser Window
    Gtk::FileChooserDialog *fileChooserDialog = nullptr;
    Gtk::Button *buttonOpenLoadDialog = nullptr;

    std::map<ObjectType, ColorType> whoseColor;

    // Control method
    void initializeWidget();
    void setSignals();
    void builderWidget();
    void initializeWhoseColor();
    void windowThreadWrapper();
    void frameThreadWrapper();

    // Update frame
    void processFrame(cv::Mat);
    void updateGtkImage();
    void receiveNewFrame(cv::Mat);
    void updateFpsLabel(int);
    void send(std::vector<vss::Robot>, std::vector<vss::Robot>, vss::Ball);

    ColorPosition getBallPosition(cv::Mat, ColorType);
    ColorPosition getOpponentPosition(cv::Mat, ColorType);
    std::vector<std::vector<ColorPosition>> getTeamPosition(cv::Mat, ColorType);

    std::vector<cv::Rect> ballRectanglesCut;
    std::vector<cv::Rect> teamRectanglesCut;
    std::vector<cv::Rect> opponentRectanglesCut;

    std::vector<std::vector<cv::Rect>> drawRectangleVector;
};
#endif
