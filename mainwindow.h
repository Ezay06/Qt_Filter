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

    void on_black_and_white_savenew_clicked();

    void on_black_and_white_savesame_clicked();

    void on_newsave_browse_clicked();

    void on_newsave_newfilename_returnPressed();

    void on_flip_V_clicked();

    void on_flip_H_clicked();

    void on_flip_savenew_clicked();

    void on_flip_savesame_clicked();

    void on_load_browse_clicked();

    void on_crop_crop_clicked();

    void on_crop_savesame_clicked();

    void on_crop_savenew_clicked();

    void on_resize_resize_clicked();

    void on_resize_savenew_clicked();

    void on_resize_savesame_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
