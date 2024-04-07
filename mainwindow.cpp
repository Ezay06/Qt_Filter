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
#include <QFileDialog>
#include <QDebug>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Image_Class_1.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->black_and_white_savesuccessful->setVisible(false);


    ui->newsave_savesuccessuful->setVisible(false);


    ui->flip_savenew->setVisible(false);
    ui->flip_savesame->setVisible(false);
    ui->flip_label2->setVisible(false);
    ui->flip_savesuccessful->setVisible(false);
}

//global variables
Image out_image;
Image in_image;
string in_image_path;
string newimage_path;

MainWindow::~MainWindow()
{
    delete ui;
}


//Menu window
void MainWindow::on_black_and_white_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    Image image(in_image);
    for (int i = 0; i < image.width; i++){
        for (int j = 0; j < image.height; j++){
            unsigned int avg = 0;
            for (int c = 0; c < 3; c++){
                avg += in_image(i, j, c);
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
}


void MainWindow::on_flip_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_crop_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_resize_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_purple_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_old_tv_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}



//Black and wihte window


void MainWindow::on_black_and_white_savenew_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->black_and_white_savesuccessful->setVisible(false);
}

void MainWindow::on_black_and_white_savesame_clicked()
{
    out_image.saveImage(in_image_path);
    ui->black_and_white_savesuccessful->setVisible(true);
    QTimer::singleShot(3000, this, [this](){
        ui->black_and_white_savesuccessful->setVisible(false);
        ui->stackedWidget->setCurrentIndex(0);
    });

}


//New save window
void MainWindow::on_newsave_browse_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select a folder", QDir::homePath());
    if (folderPath.isEmpty()){
        ui->newsave_errormessage->setStyleSheet("color: red");
        ui->newsave_errormessage->setText("No Folder selected.");
    }
    else{
        newimage_path = folderPath.toStdString() + '\\';
        ui->newsave_newfilename->setReadOnly(false);
        ui->newsave_errormessage->setText("");
    }
}

void MainWindow::on_newsave_newfilename_returnPressed()
{
    try{
        QString newfilename = ui->newsave_newfilename->text();
        newimage_path += newfilename.toStdString();
        out_image.saveImage(newimage_path);
        ui->newsave_errormessage->setText("");
        newimage_path = "";
        ui->newsave_savesuccessuful->setVisible(true);
        QTimer::singleShot(3000, this, [this](){
            ui->stackedWidget->setCurrentIndex(0);
            ui->newsave_newfilename->setReadOnly(true);
            ui->newsave_savesuccessuful->setVisible(false);
            ui->newsave_newfilename->setText("");
        });

    }
    catch(const invalid_argument& e){
        QString qerror_message = QString::fromStdString(e.what());
        ui->newsave_errormessage->setStyleSheet("color: red");
        ui->newsave_errormessage->setText(qerror_message);
        ui->newsave_newfilename->setFocus();
    }
}


//Flip window
void MainWindow::on_flip_V_clicked()
{
    Image image(in_image);
    Image new_image(image.width, image.height);
    for (int i = 0; i < image.width; i++){
        for (int j = 0; j < image.height; j++){
            for (int c = 0; c < 3; c++){
                new_image(i, image.height - 1 - j, c) = image(i, j, c);

            }
        }
    }

    out_image = new_image;
    ui->flip_V->setEnabled(false);
    ui->flip_H->setEnabled(false);
    ui->flip_savenew->setVisible(true);
    ui->flip_savesame->setVisible(true);
    ui->flip_savenew->setEnabled(true);
    ui->flip_savesame->setEnabled(true);
}

void MainWindow::on_flip_H_clicked()
{
    Image image(in_image);
    Image new_image(image.width, image.height);
    for (int i = 0; i < image.width; i++){
        for (int j = 0; j < image.height; j++){
            for (int c = 0; c < 3; c++){
                new_image(image.width - 1 - i, j, c) = image(i, j, c);

            }
        }
    }

    out_image = new_image;
    ui->flip_V->setEnabled(false);
    ui->flip_H->setEnabled(false);
    ui->flip_savenew->setVisible(true);
    ui->flip_savesame->setVisible(true);
    ui->flip_savenew->setEnabled(true);
    ui->flip_savesame->setEnabled(true);
}

void MainWindow::on_flip_savenew_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->flip_H->setVisible(false);
    ui->flip_V->setVisible(false);
    ui->flip_savenew->setVisible(false);
    ui->flip_savesame->setVisible(false);
    ui->flip_savenew->setEnabled(false);
    ui->flip_savesame->setEnabled(false);
    ui->flip_label2->setVisible(false);
}

void MainWindow::on_flip_savesame_clicked()
{
    out_image.saveImage(in_image_path);
    ui->flip_savesuccessful->setVisible(true);
    QTimer::singleShot(3000, this, [this](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->flip_savenew->setVisible(false);
        ui->flip_savesame->setVisible(false);
        ui->flip_savenew->setEnabled(false);
        ui->flip_savesame->setEnabled(false);
        ui->flip_label2->setVisible(false);
        ui->flip_savesuccessful->setVisible(false);
    });
}


void MainWindow::on_load_browse_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select a file", QDir::homePath(), "All Files (*)");
    if (filePath == ""){
        ui->load_errormessage->setStyleSheet("color: red");
        ui->load_errormessage->setText("No file selected.");
    }
    else{

        try{
            Image image(filePath.toStdString());
            in_image = image;
            in_image_path = filePath.toStdString();
            ui->load_errormessage->setText("");
            ui->stackedWidget->setCurrentIndex(1);
        }

        catch(const invalid_argument& e){
            QString qerror_message = QString::fromStdString(e.what());
            ui->load_errormessage->setStyleSheet("color: red");
            ui->load_errormessage->setText(qerror_message);
        }

    }
}


