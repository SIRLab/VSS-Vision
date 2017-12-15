#include "CameraCalibration.h"

CameraCalibration::CameraCalibration(){
}

bool CameraCalibration::on_keyboard(GdkEventKey* event, Gtk::Window* window){
    if(event->keyval == GDK_KEY_space) {
		window->close();
	} else if(event->keyval == GDK_KEY_Return) {
		window->close();
	} else if(event->keyval == GDK_KEY_Escape) {
		window->close();
	}
	return true;
}

void CameraCalibration::on_button_load(Gtk::FileChooserWidget* file_chooser){
    std::cout << file_chooser->get_filename() << std::endl;
}

void CameraCalibration::on_button_save(Gtk::FileChooserWidget* file_chooser){
    std::cout << file_chooser->get_filename() << std::endl;
}

void CameraCalibration::on_button_window_file(Gtk::Window* window_file){
    window_file->show();
}

void CameraCalibration::on_color_selected(Gtk::ComboBoxText* color_selected){
    std::cout << color_selected->get_active_text() << std::endl;
}

void CameraCalibration::on_color_team_1(Gtk::ComboBoxText* color_team_1){
    std::cout << color_team_1->get_active_text() << std::endl;
}

void CameraCalibration::on_color_team_2(Gtk::ComboBoxText* color_team_2){
    std::cout << color_team_2->get_active_text() << std::endl;
}

void CameraCalibration::on_color_robot_1(Gtk::ComboBoxText* color_robot_1){
    std::cout << color_robot_1->get_active_text() << std::endl;
}

void CameraCalibration::on_color_robot_2(Gtk::ComboBoxText* color_robot_2){
    std::cout << color_robot_2->get_active_text() << std::endl;
}

void CameraCalibration::on_color_robot_3(Gtk::ComboBoxText* color_robot_3){
    std::cout << color_robot_3->get_active_text() << std::endl;
}

void CameraCalibration::on_input_path(Gtk::ComboBoxText* input_path){
    std::cout << input_path->get_active_text() << std::endl;
}

void CameraCalibration::on_scale_h_max(Gtk::Scale* scale_h_max){
    std::cout << scale_h_max->get_value() << std::endl;	
}

void CameraCalibration::on_scale_h_min(Gtk::Scale* scale_h_min){
    std::cout << scale_h_min->get_value() << std::endl;	
}

void CameraCalibration::on_scale_s_max(Gtk::Scale* scale_s_max){
    std::cout << scale_s_max->get_value() << std::endl;	
}

void CameraCalibration::on_scale_s_min(Gtk::Scale* scale_s_min){
    std::cout << scale_s_min->get_value() << std::endl;	
}

void CameraCalibration::on_scale_v_max(Gtk::Scale* scale_v_max){
    std::cout << scale_v_max->get_value() << std::endl;	
}

void CameraCalibration::on_scale_v_min(Gtk::Scale* scale_v_min){
    std::cout << scale_v_min->get_value() << std::endl;	
}

void CameraCalibration::on_scale_rotation(Gtk::Scale* scale_rotation){
    std::cout << scale_rotation->get_value() << std::endl;
}

void CameraCalibration::on_scale_brightness(Gtk::Scale* scale_brightness){
    std::cout << scale_brightness->get_value() << std::endl;
}

void CameraCalibration::on_scale_contrast(Gtk::Scale* scale_contrast){
    std::cout << scale_contrast->get_value() << std::endl;
}

void CameraCalibration::on_scale_saturation(Gtk::Scale* scale_saturation){
    std::cout << scale_saturation->get_value() << std::endl;
}

void CameraCalibration::on_scale_exposure(Gtk::Scale* scale_exposure){
    std::cout << scale_exposure->get_value() << std::endl;
}

void CameraCalibration::on_scale_gain(Gtk::Scale* scale_gain){
    std::cout << scale_gain->get_value() << std::endl;
}

void CameraCalibration::on_radio_button_image(Gtk::RadioButton* radio_button_image){
    if (!radio_button_image->get_active())
        std::cout << "Image: " << radio_button_image->get_active() << std::endl;
}
void CameraCalibration::on_radio_button_video(Gtk::RadioButton* radio_button_video){
    if (!radio_button_video->get_active())
        std::cout << "Video: " << radio_button_video->get_active() << std::endl;
}
void CameraCalibration::on_radio_button_camera(Gtk::RadioButton* radio_button_camera){
    if (!radio_button_camera->get_active())
        std::cout << "Camera: " << radio_button_camera->get_active() << std::endl;
} 