/*
* This file is part of the VSS-Vision project.
*
* This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
* v. 3.0. If a copy of the GPL was not distributed with this
* file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
*/

#include <Windows/Calibration/CalibrationWindow.h>

CalibrationWindow::CalibrationWindow() {
    calibrationBuilder = new CalibrationBuilder();
    calibrationBuilder->shouldInitializeColors(true);
    calibrationBuilder->shouldInitializeCuts(true);

    calibrationBuilderFromRepository = new CalibrationBuilder();
    calibrationRepository = new CalibrationRepository(calibrationBuilderFromRepository);

    calibration = calibrationBuilder->getInstance();

    //inputReader = new CameraReader();
    inputReader = new ImageFileReader();

    colorRecognizer = new ColorRecognizer();

    shouldReadInput = true;
    showBinaryImage = false;
    actualColorRangeIndex = 0;
}

CalibrationWindow::~CalibrationWindow() {
    connection_update_screen.disconnect();
}

int CalibrationWindow::run(int argc, char *argv[]){

    threadWindowControl = new thread(std::bind(&CalibrationWindow::windowThreadWrapper, this));
    threadCameraReader = new thread( std::bind( &CalibrationWindow::cameraThreadWrapper, this ));

    threadWindowControl->join();
    threadCameraReader->detach();

    shouldReadInput = false;
    inputReader->stopReceivement();

    return MENU;
}

void CalibrationWindow::cameraThreadWrapper() {
    inputReader->setSource( inputReader->getAllPossibleSources().at(0) );
    inputReader->initializeReceivement();

    while(shouldReadInput) {
        mutexChangeInput.lock();
            cv::Mat receivedFrame = inputReader->getFrame();
        mutexChangeInput.unlock();
        receiveNewFrame( receivedFrame );
    }
}

void CalibrationWindow::windowThreadWrapper() {
    builderWidget();
    setSignals();
    initializeWidget();

    Gtk::Main::run(*window);
}

void CalibrationWindow::initializeWidget(){

    radioButtonImage->set_active();

    listColors = {"Blue", "Yellow", "Orange", "Green", "Pink", "Purple", "Red", "Brown"};

    int sizeListColors = listColors.size();

    for (int i=0; i<sizeListColors; i++){
        comboBoxColor->append(listColors[i]);
    }

    // define initial folder for file chooser
    fileChooserDialog->set_current_folder(defaultFilesPath + "/data");
    
    window->maximize();
    window->show_all_children();
}

void CalibrationWindow::builderWidget(){

    auto builder = Gtk::Builder::create();

    try {
        builder->add_from_file(defaultFilesPath + "/glade/Calibration.glade");

        builder->get_widget("window", window);

        builder->get_widget_derived("image", screenImage);

        builder->get_widget("button_save_calibration", buttonSave);
        builder->get_widget("button_load_calibration", buttonLoad);
        builder->get_widget("button_save_dialog", buttonOpenSaveDialog);
        builder->get_widget("button_load_dialog", buttonOpenLoadDialog);

        builder->get_widget("entry_chooser", entryChooserDialog);
        builder->get_widget("file_chooser_dialog", fileChooserDialog);

        scaleHSV.resize(6);
        builder->get_widget("hscale_hmax", scaleHSV[H_MAX]);
        builder->get_widget("hscale_hmin", scaleHSV[H_MIN]);
        builder->get_widget("hscale_smax", scaleHSV[S_MAX]);
        builder->get_widget("hscale_smin", scaleHSV[S_MIN]);
        builder->get_widget("hscale_vmax", scaleHSV[V_MAX]);
        builder->get_widget("hscale_vmin", scaleHSV[V_MIN]);

        builder->get_widget("radiobutton_image", radioButtonImage);
        builder->get_widget("radiobutton_video", radioButtonVideo);
        builder->get_widget("radiobutton_camera", radioButtonCamera);

        builder->get_widget("combobox_path", comboBoxPath);
        builder->get_widget("combobox_color", comboBoxColor);

        builder->get_widget("togglebutton_cut_mode", toggleButtonCutMode);
        builder->get_widget("button_restore_cut", buttonRestoreCut);

    } catch(const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;

    } catch(const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;

    } catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
    }
}

void CalibrationWindow::setSignals(){

    connection_update_screen = Glib::signal_timeout().connect(sigc::mem_fun( this, &CalibrationWindow::emitUpdateGtkImage), 16);
    dispatcher_update_gtkmm_frame.connect(sigc::mem_fun( this, &CalibrationWindow::updateGtkImage));

    window->signal_key_press_event().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(this, &ICalibrationWindow::onKeyboard), window) , false);

    buttonOpenSaveDialog->signal_clicked().connect(sigc::bind<Gtk::FileChooserDialog*, Gtk::Entry*>(sigc::mem_fun(this, &ICalibrationWindow::onButtonOpenSaveDialog), fileChooserDialog, entryChooserDialog ));
    buttonOpenLoadDialog->signal_clicked().connect(sigc::bind<Gtk::FileChooserDialog*, Gtk::Entry*>(sigc::mem_fun(this, &ICalibrationWindow::onButtonOpenLoadDialog), fileChooserDialog, entryChooserDialog ));

    buttonSave->signal_clicked().connect(sigc::bind<Gtk::FileChooserDialog*, Gtk::Entry*>(sigc::mem_fun(this, &ICalibrationWindow::onButtonSave), fileChooserDialog, entryChooserDialog ));
    buttonLoad->signal_clicked().connect(sigc::bind<Gtk::FileChooserDialog*, Gtk::Entry*>(sigc::mem_fun(this, &ICalibrationWindow::onButtonLoad), fileChooserDialog, entryChooserDialog));


    scaleHSV[H_MAX]->signal_value_changed().connect(sigc::bind<Gtk::Scale*>(sigc::mem_fun(this, &ICalibrationWindow::onScaleHMAX), scaleHSV[H_MAX]));
    scaleHSV[H_MIN]->signal_value_changed().connect(sigc::bind<Gtk::Scale*>(sigc::mem_fun(this, &ICalibrationWindow::onScaleHMIN), scaleHSV[H_MIN]));
    scaleHSV[S_MAX]->signal_value_changed().connect(sigc::bind<Gtk::Scale*>(sigc::mem_fun(this, &ICalibrationWindow::onScaleSMAX), scaleHSV[S_MAX]));
    scaleHSV[S_MIN]->signal_value_changed().connect(sigc::bind<Gtk::Scale*>(sigc::mem_fun(this, &ICalibrationWindow::onScaleSMIN), scaleHSV[S_MIN]));
    scaleHSV[V_MAX]->signal_value_changed().connect(sigc::bind<Gtk::Scale*>(sigc::mem_fun(this, &ICalibrationWindow::onScaleVMAX), scaleHSV[V_MAX]));
    scaleHSV[V_MIN]->signal_value_changed().connect(sigc::bind<Gtk::Scale*>(sigc::mem_fun(this, &ICalibrationWindow::onScaleVMIN), scaleHSV[V_MIN]));


    radioButtonImage->signal_pressed().connect(sigc::bind<Gtk::RadioButton*>(sigc::mem_fun(this, &ICalibrationWindow::onRadioButtonImage), radioButtonImage));
    radioButtonVideo->signal_pressed().connect(sigc::bind<Gtk::RadioButton*>(sigc::mem_fun(this, &ICalibrationWindow::onRadioButtonVideo), radioButtonVideo));
    radioButtonCamera->signal_pressed().connect(sigc::bind<Gtk::RadioButton*>(sigc::mem_fun(this, &ICalibrationWindow::onRadioButtonCamera), radioButtonCamera));

    toggleButtonCutMode->signal_pressed().connect(sigc::bind<Gtk::ToggleButton*>(sigc::mem_fun(this, &ICalibrationWindow::onToggleButtonCutMode), toggleButtonCutMode));
    buttonRestoreCut->signal_pressed().connect(sigc::mem_fun(this, &ICalibrationWindow::onButtonRestoreCut));

    fileChooserDialog->signal_selection_changed().connect(sigc::bind<Gtk::FileChooserDialog*, Gtk::Entry*>(sigc::mem_fun(this, &ICalibrationWindow::onSignalSelectFileInDialog), fileChooserDialog, entryChooserDialog ));

    comboBoxPath->signal_changed().connect(sigc::bind<Gtk::ComboBoxText*>(sigc::mem_fun(this, &ICalibrationWindow::onComboBoxSelectPath), comboBoxPath));
    comboBoxColor->signal_changed().connect(sigc::bind<Gtk::ComboBoxText*, std::vector<Gtk::Scale*>>(sigc::mem_fun(this, &ICalibrationWindow::onComboBoxSelectColor), comboBoxColor, scaleHSV));
}
