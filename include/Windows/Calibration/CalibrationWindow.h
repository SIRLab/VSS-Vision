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
#include <Interfaces/ICalibrationRepository.h>
#include <Interfaces/ICalibrationBuilder.h>
#include <Interfaces/IImageInputReader.h>
#include <Interfaces/IColorRecognizer.h>
#include <Domain/ProgramState.h>

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
    void onButtonLoad(Gtk::FileChooserDialog*, Gtk::Entry*, std::vector<Gtk::Scale*>) override;

    void onScaleHMAX(Gtk::Scale*) override;
    void onScaleHMIN(Gtk::Scale*) override;
    void onScaleSMAX(Gtk::Scale*) override;
    void onScaleSMIN(Gtk::Scale*) override;
    void onScaleVMAX(Gtk::Scale*) override;
    void onScaleVMIN(Gtk::Scale*) override;

    void onScaleGain(Gtk::Scale*) override;
    void onScaleContrast(Gtk::Scale*) override;
    void onScaleRotation(Gtk::Scale*) override;
    void onScaleExposure(Gtk::Scale*) override;
    void onScaleBrightness(Gtk::Scale*) override;
    void onScaleSaturation(Gtk::Scale*) override;

    void onRadioButtonImage(Gtk::RadioButton*) override;
    void onRadioButtonVideo(Gtk::RadioButton*) override;
    void onRadioButtonCamera(Gtk::RadioButton*) override;

    void onToggleButtonCutMode(Gtk::ToggleButton*) override;

    void onSignalSelectFileInDialog(Gtk::FileChooserDialog*, Gtk::Entry*) override;
	
    void onComboBoxSelectPath(Gtk::ComboBox*) override;
    void onComboBoxSelectColor(Gtk::ComboBox*, std::vector<Gtk::Scale*>) override;

private:

	// Threads
	std::thread *threadCameraReader;
    std::thread *threadWindowControl;

	// Comunication between threads
	Glib::Dispatcher signal_set_new_frame;

	// Classes
	IImageInputReader *inputReader;
	IColorRecognizer *colorRecognizer;
	ICalibrationBuilder *calibrationBuilder;
    ICalibrationBuilder *calibrationBuilderFromRepository;
	ICalibrationRepository *calibrationRepository;

    Calibration calibration;
    ColorRange *actualColorRange;
	unsigned int actualColorRangeIndex;

	// Opencv image
	cv::Mat frame;

	// GTKMM - Calibration Window
		Gtk::Window* window = nullptr;

		GImage* screenImage = nullptr;

		Gtk::RadioButton* radioButtonImage = nullptr;
		Gtk::RadioButton* radioButtonVideo = nullptr;
		Gtk::RadioButton* radioButtonCamera = nullptr;

		Gtk::ComboBox* comboBoxPath = nullptr;
		Gtk::ComboBox* comboBoxColor = nullptr;

		std::vector<Gtk::Scale*> scaleHSV;
		std::vector<Gtk::Scale*> scaleCameraConfig;

		Gtk::Button* buttonSave = nullptr;
		Gtk::Button* buttonLoad = nullptr;
		
		Gtk::ToggleButton* toggleButtonCutMode = nullptr;

	// GTKMM - File Chooser Window
		Gtk::FileChooserDialog* fileChooserDialog = nullptr;

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
	void setNewFrame();
	void processFrame();
	void receiveNewFrame(cv::Mat);
	
	// Events
    void defineActualColorRange(ColorType type);
    void applyRectangleToFrame();
	void getAllAttributsFromCapture(bool signal);
    void applyActualColorRangeToSlidersHSV(ColorType type, std::vector<Gtk::Scale*>);
};
#endif
