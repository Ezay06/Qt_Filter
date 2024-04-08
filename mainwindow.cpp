#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <cctype>
#include <random>
#include <cmath>
using namespace std;

#include <QTimer>
#include <QFileDialog>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Image_Class_1.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //black and white initiallization
    ui->black_and_white_savesuccessful->setVisible(false);

    //save menu initiallization
    ui->newsave_savesuccessuful->setVisible(false);

    //flip initiallization
    ui->flip_savenew->setVisible(false);
    ui->flip_savesame->setVisible(false);
    ui->flip_savenew->setEnabled(false);
    ui->flip_savesame->setEnabled(false);
    ui->flip_savesuccessful->setVisible(false);

    //crop initiallization
    ui->crop_savenew->setEnabled(false);
    ui->crop_savenew->setVisible(false);
    ui->crop_savesame->setEnabled(false);
    ui->crop_savesame->setVisible(false);
    ui->crop_savesuccessful->setVisible(false);

    //resize initiallization
    ui->resize_savenew->setEnabled(false);
    ui->resize_savenew->setVisible(false);
    ui->resize_savesame->setEnabled(false);
    ui->resize_savesame->setVisible(false);
    ui->resize_savedsuccessful->setVisible(false);

    //old tv initiallization
    ui->old_tv_savesuccessful->setVisible(false);

    //purple initiallization
    ui->purple_savesuccessful->setVisible(false);
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


//Filters menu window
//black and white, old tv, and purple filters require no additional user input; so the algorithms are implemented here in the filters menu.
void MainWindow::on_black_and_white_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    Image image(in_image);
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

    Image image(in_image);

    for (int i = 0; i < image.width; i++){
        for (int j = 0; j < image.height; j++){
            int new_r = image(i, j, 0) + 50;
            int new_b = image(i, j, 2) + 50;
            int new_g = image(i, j, 1) - 20;


            if (new_r > 255){
                new_r = 255;
            }
            if (new_b > 255){
                new_b = 255;
            }
            if (new_g < 0){
                new_g = 0;
            }


            image(i, j, 0) = new_r;
            image(i, j, 1) = new_g;
            image(i, j, 2) = new_b;
        }
    }

    out_image = image;
}

void MainWindow::on_old_tv_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);

    Image image(in_image);

    for (int i = 0; i < image.width; i++){
        for (int j = 0; j < image.height; j++){
            for (int c = 0; c < 3; c++){
                int pixel_value = image(i, j, c);
                default_random_engine generator;
                uniform_int_distribution<int> noise(10, 50);
                pixel_value = pixel_value + noise(generator);
                pixel_value = pixel_value - 30;
                pixel_value = pixel_value * 0.8;

                if (j % 2 == 0){
                    pixel_value = pixel_value - 75;
                }
                else{
                    pixel_value = pixel_value + 75;
                }

                if (pixel_value > 255){
                    pixel_value = 255;
                }

                if (pixel_value < 0){
                    pixel_value = 0;
                }

                image(i, j, c) = pixel_value;


            }
        }
    }

    out_image = image;
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
    QTimer::singleShot(3000, this, [this](){  //Timer for 3 seconds, all commands inside the block are executed after 3 seconds.
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
    ui->flip_savenew->setVisible(true);
    ui->flip_savesame->setVisible(true);
    ui->flip_savenew->setEnabled(true);
    ui->flip_savesame->setEnabled(true);
}

void MainWindow::on_flip_savenew_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->flip_savenew->setVisible(false);
    ui->flip_savesame->setVisible(false);
    ui->flip_savenew->setEnabled(false);
    ui->flip_savesame->setEnabled(false);
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



//crop window
void MainWindow::on_crop_crop_clicked()
{
    Image image(in_image);
    bool values_valid;
    int center_x;
    int center_y;
    int new_dimensions_w;
    int new_dimensions_h;

    QRegularExpression rx("[0-9]+");
    QRegularExpressionValidator x_validator(rx, ui->crop_positionx);
    QRegularExpressionValidator y_validator(rx, ui->crop_positiony);
    QRegularExpressionValidator h_validator(rx, ui->crop_height);
    QRegularExpressionValidator w_validator(rx, ui->crop_width);
    ui->crop_positionx->setValidator(&x_validator);  // applies a regular expression pattern to a line edit, in this case it checks if the input is only numbers
    ui->crop_positiony->setValidator(&y_validator);
    ui->crop_height->setValidator(&h_validator);
    ui->crop_width->setValidator(&x_validator);

    if (ui->crop_positionx->text() == ""  || ui->crop_positiony->text() == "" || ui->crop_height->text() == "" || ui->crop_width->text() == ""){
        ui->crop_errormessage->setStyleSheet("color: red");
        ui->crop_errormessage->setText("Please enter all values.");
    }

    //checks if the input matches the regular expression
    else if(!ui->crop_positionx->hasAcceptableInput() || !ui->crop_positiony->hasAcceptableInput() || !ui->crop_height->hasAcceptableInput() || !ui->crop_width->hasAcceptableInput()){
        ui->crop_errormessage->setStyleSheet("color: red");
        ui->crop_errormessage->setText("All values must be numbers.");
    }

    else{
        center_x = ui->crop_positionx->text().toInt();
        center_y = ui->crop_positiony->text().toInt();
        new_dimensions_w = ui->crop_width->text().toInt();
        new_dimensions_h = ui->crop_height->text().toInt();

        if (new_dimensions_w + center_x > image.width || new_dimensions_h + center_y > image.height){
            ui->crop_errormessage->setStyleSheet("color: red");
            ui->crop_errormessage->setText("The dimensions you entered are out of bounds relative to the corner you chose. Please try again.");
        }

        else{
            try{
                image(center_x, center_y, 0);
                Image new_image(new_dimensions_w, new_dimensions_h);
                ui->crop_errormessage->setText("");
                for (int i = 0; i < image.width; i++){
                    for (int j = 0; j < image.height; j++){
                        if ( i >= center_x && i < center_x + new_image.width && j >= center_y && j < center_y + new_image.height){
                            for (int c = 0; c < 3; c++){
                                new_image(i - center_x, j - center_y, c) = image(i, j, c);
                            }
                        }

                    }

                }
                out_image = new_image;
                ui->crop_savenew->setEnabled(true);
                ui->crop_savenew->setVisible(true);
                ui->crop_savesame->setEnabled(true);
                ui->crop_savesame->setVisible(true);
            }
            catch(const out_of_range& e){
                ui->crop_errormessage->setStyleSheet("color: red");
                ui->crop_errormessage->setText("The position you entered is invalid. Please try again.");
            }
        }

    }

}

void MainWindow::on_crop_savesame_clicked()
{
    out_image.saveImage(in_image_path);
    ui->crop_savesuccessful->setVisible(true);
    QTimer::singleShot(3000, this, [this](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->crop_savenew->setEnabled(false);
        ui->crop_savenew->setVisible(false);
        ui->crop_savesame->setEnabled(false);
        ui->crop_savenew->setVisible(false);
        ui->crop_savesuccessful->setVisible(false);
        ui->crop_height->setText("");
        ui->crop_width->setText("");
        ui->crop_positionx->setText("");
        ui->crop_positiony->setText("");
    });
}

void MainWindow::on_crop_savenew_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->crop_savenew->setEnabled(false);
    ui->crop_savenew->setVisible(false);
    ui->crop_savesame->setEnabled(false);
    ui->crop_savesame->setVisible(false);
    ui->crop_savesuccessful->setVisible(false);
    ui->crop_height->setText("");
    ui->crop_width->setText("");
    ui->crop_positionx->setText("");
    ui->crop_positiony->setText("");
}


//Resize window
void MainWindow::on_resize_resize_clicked()
{
    Image image(in_image);
    long new_dimensions_w;
    long new_dimensions_h;
    QRegularExpression rx("[0-9]+");
    QRegularExpressionValidator w_validator(rx, ui->resize_W);
    QRegularExpressionValidator h_validator(rx, ui->resize_H);
    ui->resize_H->setValidator(&h_validator);
    ui->resize_W->setValidator(&w_validator);

    if (ui->resize_H->text() == "" || ui->resize_W->text() == ""){
        ui->resize_errormessage->setStyleSheet("color: red");
        ui->resize_errormessage->setText("Please enter all values");
    }

    else if (!ui->resize_H->hasAcceptableInput() || !ui->resize_W->hasAcceptableInput()){
        ui->resize_errormessage->setStyleSheet("color: red");
        ui->resize_errormessage->setText("All values must be numbers.");
    }

    else{
        ui->resize_errormessage->setText("");
        new_dimensions_w = ui->resize_W->text().toLong();
        new_dimensions_h = ui->resize_H->text().toLong();

        Image new_image(new_dimensions_w, new_dimensions_h);

        double ratio_w = static_cast<double> (image.width) / new_image.width;
        double ratio_h = static_cast<double> (image.height) / new_image.height;

        for (int i = 0; i < new_image.width; i++){
            int neighbor_w = round(i * ratio_w);
            for (int j = 0; j < new_image.height; j++){
                int neighbor_h = round(j * ratio_h);
                for (int c = 0; c < 3; c++){
                    new_image(i,j,c) = image(neighbor_w, neighbor_h, c);
                }
            }
        }

        out_image = new_image;
        ui->resize_savenew->setEnabled(true);
        ui->resize_savenew->setVisible(true);
        ui->resize_savesame->setEnabled(true);
        ui->resize_savesame->setVisible(true);
    }
}

void MainWindow::on_resize_savenew_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->resize_savenew->setEnabled(false);
    ui->resize_savenew->setVisible(false);
    ui->resize_savesame->setEnabled(false);
    ui->resize_savesame->setVisible(false);
    ui->resize_H->setText("");
    ui->resize_W->setText("");
}

void MainWindow::on_resize_savesame_clicked()
{
    out_image.saveImage(in_image_path);
    ui->resize_savedsuccessful->setVisible(true);
    QTimer::singleShot(3000, this, [this](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->resize_savenew->setEnabled(false);
        ui->resize_savenew->setVisible(false);
        ui->resize_savesame->setEnabled(false);
        ui->resize_savesame->setVisible(false);
        ui->resize_savedsuccessful->setVisible(false);
    });
}



//old tv window
void MainWindow::on_old_tv_savenew_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_old_tv_savesame_clicked()
{
    out_image.saveImage(in_image_path);
    ui->old_tv_savesuccessful->setVisible(true);
    QTimer::singleShot(3000, this, [this](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->old_tv_savesuccessful->setVisible(false);
    });
}


//purple window
void MainWindow::on_purple_savenew_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_purple_savesame_clicked()
{
    out_image.saveImage(in_image_path);
    ui->purple_savesuccessful->setVisible(true);
    QTimer::singleShot(3000, this, [this](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->purple_savesuccessful->setVisible(false);
    });
}

