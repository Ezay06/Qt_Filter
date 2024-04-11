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

    void on_load_browse_clicked();

    void on_crop_crop_clicked();

    void on_resize_resize_clicked();

    void on_greyscale_button_clicked();

    void on_btnSaveNewImage_clicked();

    void on_btnSaveSamemage_clicked();

    void on_dark_button_clicked();

    void on_btnDLSaveNimage_clicked();

    void on_btnDLSaveSimage_clicked();

    void on_light_button_clicked();

    void on_save_clicked();

    void on_crop_positionx_cursorPositionChanged(int arg1, int arg2);

    void on_crop_positiony_cursorPositionChanged(int arg1, int arg2);

    void on_crop_width_cursorPositionChanged(int arg1, int arg2);

    void on_crop_height_cursorPositionChanged(int arg1, int arg2);

    void on_resize_W_cursorPositionChanged(int arg1, int arg2);

    void on_resize_H_cursorPositionChanged(int arg1, int arg2);

    void on_newsave_savesame_clicked();

    void on_load_new_clicked();

    void on_infrared_button_clicked();

    void on_detectEdge_button_clicked();


protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
