#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <array>
#include <sstream>
#include <cctype>
#include <random>
#include <cmath>
#include <chrono>
#include <thread>
using namespace std;

#include <QTimer>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Image_Class_1.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->black_and_white_savenew->setVisible(false);
    ui->black_and_white_savesame->setVisible(false);
    ui->black_and_white_error_message->setVisible(false);
    ui->black_and_white_error_message2->setVisible(false);
    ui->black_and_white_label2->setVisible(false);
    ui->black_and_white_newfilename->setVisible(false);
    ui->black_and_white_savesuccessful->setVisible(false);
}

Image out_image;

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_black_and_white_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_flip_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_crop_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_resize_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_purple_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_old_tv_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_black_and_white_filename_returnPressed()
{
    QString qfilename = ui->black_and_white_filename->text();
    string filename = qfilename.toStdString();
    try{
        Image image(filename);
        ui->black_and_white_error_message->setText("");
        ui->black_and_white_error_message->setVisible(false);
        ui->black_and_white_filename->setReadOnly(true);
        for (int i = 0; i < image.width; i++){
            for (int j = 0; j < image.height; j++){

                unsigned int avg = 0;
                for (int c = 0; c < 3; c++){
                    avg += image(i, j, c);
                }

                avg /= 3;

                if (avg <= 127){
                    image(i, j, 0) = 0;
                    image(i, j, 1) = 0;
                    image(i, j, 2) = 0;
                }

                else{
                    image(i, j, 0) = 255;
                    image(i, j, 1) = 255;
                    image(i, j, 2) = 255;
                }


            }
        }
        out_image = image;
        ui->black_and_white_savenew->setVisible(true);
        ui->black_and_white_savenew->setEnabled(true);
        ui->black_and_white_savesame->setVisible(true);
        ui->black_and_white_savesame->setEnabled(true);
    }
    catch (const invalid_argument& e) {
        QString qerror_message = QString::fromStdString(e.what());
        ui->black_and_white_error_message->setVisible(true);
        ui->black_and_white_error_message->setStyleSheet("color: red");
        ui->black_and_white_error_message->setText(qerror_message);
        ui->black_and_white_filename->setFocus();
    }
}


void MainWindow::on_black_and_white_savenew_clicked()
{
    ui->black_and_white_error_message2->setVisible(true);
    ui->black_and_white_label2->setVisible(true);
    ui->black_and_white_newfilename->setVisible(true);
    ui->black_and_white_error_message2->setEnabled(true);
    ui->black_and_white_label2->setEnabled(true);
    ui->black_and_white_newfilename->setEnabled(true);
}


void MainWindow::on_black_and_white_savesame_clicked()
{
    QString qfilename = ui->black_and_white_filename->text();
    string filename = qfilename.toStdString();
    out_image.saveImage(filename);
    ui->black_and_white_savesuccessful->setVisible(true);
    QTimer::singleShot(3000, this, [this](){
        ui->black_and_white_filename->setText("");
        ui->black_and_white_newfilename->setText("");
        ui->black_and_white_savenew->setVisible(false);
        ui->black_and_white_savesame->setVisible(false);
        ui->black_and_white_error_message->setVisible(false);
        ui->black_and_white_error_message2->setVisible(false);
        ui->black_and_white_label2->setVisible(false);
        ui->black_and_white_newfilename->setVisible(false);
        ui->black_and_white_savesuccessful->setVisible(false);
        ui->black_and_white_newfilename->setReadOnly(false);
        ui->black_and_white_filename->setReadOnly(false);
        ui->black_and_white_savesuccessful->setVisible(false);
        ui->stackedWidget->setCurrentIndex(0);
    });

}


void MainWindow::on_black_and_white_newfilename_returnPressed()
{
    try{
        QString qnew_filename = ui->black_and_white_newfilename->text();
        string new_filename = qnew_filename.toStdString();
        out_image.saveImage(new_filename);
        ui->black_and_white_error_message2->setText("");
        ui->black_and_white_error_message2->setVisible(false);
        ui->black_and_white_newfilename->setReadOnly(true);
        ui->black_and_white_savesuccessful->setVisible(true);
        this_thread::sleep_for(chrono::seconds(3));
        ui->black_and_white_filename->setText("");
        ui->black_and_white_newfilename->setText("");
        ui->black_and_white_savenew->setVisible(false);
        ui->black_and_white_savesame->setVisible(false);
        ui->black_and_white_error_message->setVisible(false);
        ui->black_and_white_error_message2->setVisible(false);
        ui->black_and_white_label2->setVisible(false);
        ui->black_and_white_newfilename->setVisible(false);
        ui->black_and_white_savesuccessful->setVisible(false);
        ui->black_and_white_newfilename->setReadOnly(false);
        ui->black_and_white_filename->setReadOnly(false);
        ui->black_and_white_savesuccessful->setVisible(false);
        ui->stackedWidget->setCurrentIndex(0);

    }
    catch (const invalid_argument& e) {
        QString qerror_message = QString::fromStdString(e.what());
        ui->black_and_white_error_message2->setVisible(true);
        ui->black_and_white_error_message2->setStyleSheet("color: red");
        ui->black_and_white_error_message2->setText(qerror_message);
        ui->black_and_white_newfilename->setFocus();
    }
}

