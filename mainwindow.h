#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_black_and_white_button_clicked();

    void on_flip_button_clicked();

    void on_crop_button_clicked();

    void on_resize_button_clicked();

    void on_purple_button_clicked();

    void on_old_tv_button_clicked();

    void on_newsave_browse_clicked();

    void on_newsave_newfilename_returnPressed();

    void on_flip_V_clicked();

    void on_flip_H_clicked();

    void on_crop_crop_clicked();

    void on_resize_resize_clicked();

    void on_greyscale_button_clicked();

    void on_light_dark_button_clicked();

    void on_save_clicked();

    void on_newsave_savesame_clicked();

    void on_load_new_clicked();

    void on_infrared_button_clicked();

    void on_detectEdge_button_clicked();

    void on_ball_button_clicked();

    void on_horiSlider_L_D_valueChanged(int value);

    void on_skewing_button_clicked();

    void on_dropwater_button_clicked();

    void on_merge_button_clicked();

    void on_apply_LD_Button_clicked();

    void on_invert_button_clicked();

    void on_rotate_button_clicked();

    void on_degree90_button_clicked();

    void on_degree180_button_clicked();

    void on_degree270_button_clicked();

    void on_light_blur_button_clicked();

    void on_medium_blur_button_clicked();

    void on_strong_blur_button_clicked();

    void on_blur_button_clicked();

    void on_wano_button_clicked();

    void on_oil_painting_button_clicked();

    void on_brightness_apply_clicked();

    void on_crop_apply_clicked();

    void on_resize_apply_clicked();

    void on_frame_button_clicked();

    void on_simple_button_clicked();

    void on_fancy_button_clicked();

    void on_red_frame_button_clicked();

    void on_green_frame_button_clicked();

    void on_blue_frame_button_clicked();

    void on_back_to_menu_button_clicked();

    void on_remove_all_filters_button_clicked();

    void on_merge_browse_clicked();

    void on_merge_larger_clicked();

    void on_merge_common_clicked();

    void on_skew_skew_clicked();

    void on_skew_apply_clicked();

    void on_save_apply_clicked();

    void on_rotate_apply_clicked();

    void on_blur_apply_clicked();

    void on_remove_frame_button_clicked();

    void on_back_button_clicked();

    void on_remove_blur_button_clicked();

    void on_remove_skew_button_clicked();

    void on_no_image_errormessage_linkActivated(const QString &link);

    void on_circle_button_clicked();

    void on_menu_button_clicked();

    void on_pixel_button_clicked();

    void on_pixel_slider_valueChanged(int value);

    void on_pixel_apply_button_clicked();

    void on_pixel_menu_button_clicked();

    void on_behind_bars_button_clicked();

    void on_decorate_button_clicked();



    void on_decorate_apply_clicked();

    void on_decorate_menu_button_clicked();

    void on_decorate_slider_valueChanged(int value);


protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
