#include <string>
#include <vector>
#include <random>
#include <cmath>
using namespace std;

#include <QTimer>
#include <QFileDialog>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QCloseEvent>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Image_Class_1.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //application initialization
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_og->setVisible(false);
    ui->label_curr->setVisible(false);

    //load initiallization
    ui->original_image->setScaledContents(true);
    ui->original_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->current_image->setScaledContents(true);
    ui->current_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    //save menu initiallization
    ui->newsave_savesuccessuful->setVisible(false);
    ui->newsave_newfilename->setEnabled(false);



    //merge initiallization
    ui->merge_image->setScaledContents(true);
    ui->merge_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->merge_common->setEnabled(false);
    ui->merge_larger->setEnabled(false);
    ui->merge_common->setVisible(false);
    ui->merge_larger->setVisible(false);

}

//global variables
Image out_image;
Image curr_image;
Image in_image;
string in_image_path;
QString qin_image_path;
string newimage_path;
Image before_blur;
// I added in_image2 for merge filter. me Ahmad
Image in_image2;
bool first = true;
char choice;
Image before_frame;
Image simple_image;
Image fancy_image;
Image circle_image;





MainWindow::~MainWindow()
{
    delete ui;
}

int square(int x){return x * x ;}

//Close event
void MainWindow::closeEvent(QCloseEvent *event) {
    // Create a QMessageBox
    if (first == false){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Exit", "Would you like to save the image before you exit?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // Allow the application to close
            event->ignore();
            ui->stackedWidget->setCurrentIndex(8);
        }
        else {
            in_image.saveImage(in_image_path);
            event->accept();
        }
    }

    else {
        event->accept();
    }


}

//Filters menu window
//black and white, old tv, and purple filters require no additional user input; so the algorithms are implemented here in the filters menu.
void MainWindow::on_load_new_clicked()
{
    ui->no_image_errormessage->setStyleSheet("");
    ui->no_image_errormessage->setText("");
    if(first == true){
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Select a file", QDir::homePath(), "All Files (*)");
        if (filePath == ""){
            ui->load_errormessage->setStyleSheet("color: red");
            ui->load_errormessage->setText("No file selected.");
        }
        else{

            try{
                Image image(filePath.toStdString());
                in_image = image;
                out_image = in_image;
                curr_image = out_image;
                in_image_path = filePath.toStdString();
                qin_image_path = filePath;
                ui->load_errormessage->setText("");
                ui->stackedWidget->setCurrentIndex(1);
                QPixmap pixmap(qin_image_path);
                ui->label_og->setVisible(true);
                ui->label_curr->setVisible(true);
                ui->original_image->setPixmap(pixmap);
                ui->current_image->setPixmap(pixmap);
                first = false;
            }

            catch(const invalid_argument& e){
                QString qerror_message = QString::fromStdString(e.what());
                ui->load_errormessage->setStyleSheet("color: red");
                ui->load_errormessage->setText(qerror_message);
            }

        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Would you like to save before loading a new image?");

        // Add buttons to the dialog
        msgBox.addButton(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);

        // Set the default button
        msgBox.setDefaultButton(QMessageBox::Yes);

        // Execute the dialog modally and get the result
        int result = msgBox.exec();

        // Handle the user's choice
        if (result == QMessageBox::Yes) {
            // User clicked "Yes"
            ui->stackedWidget->setCurrentIndex(8);
        } else if (result == QMessageBox::No) {
            // User clicked "No"
            in_image.saveImage(in_image_path);
            ui->stackedWidget->setCurrentIndex(1);
            ui->label_og->setVisible(false);
            ui->label_curr->setVisible(false);
            ui->original_image->clear();
            ui->current_image->clear();
            first = true;
            MainWindow::on_load_new_clicked();
        }
    }
}

void MainWindow::on_save_clicked()
{

    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        ui->stackedWidget->setCurrentIndex(8);
    }
}

void MainWindow::on_remove_all_filters_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        out_image = in_image;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_black_and_white_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);
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
            curr_image = out_image;
            out_image.saveImage(in_image_path);
            QPixmap pixmap(qin_image_path);
            ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_flip_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void MainWindow::on_crop_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        ui->stackedWidget->setCurrentIndex(4);
    }
}

void MainWindow::on_resize_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        ui->stackedWidget->setCurrentIndex(5);
    }
}

void MainWindow::on_purple_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);

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
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_old_tv_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);

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
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_greyscale_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);

        for(int i=0;i<image.width;++i){

            for(int j=0;j<image.height;++j){

                //calculate the pixels average

                int x = (image(i,j,0)+image(i,j,1)+image(i,j,2))/3;

                image(i,j,0)=x;
                image(i,j,1)=x;
                image(i,j,2)=x;

            }
        }
        out_image = image;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_infrared_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);

        for(int i=0;i<image.width;i++){
            for(int j=0;j<image.height;j++){

                image(i, j, 1) = 255 - image(i, j, 0);
                image(i, j, 2) = 255 - image(i, j, 0);
                image(i, j, 0) = 255;

            }
        }
        out_image = image;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_detectEdge_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image.width,out_image.height);

        uint32_t x=0;
        for(int i=0;i<out_image.width;++i){
            for(int j=0;j<out_image.height;++j){
                x += out_image(i,j,0) + out_image(i,j,1) + out_image(i,j,2);
            }
        }

        x /= 3 * out_image.width * out_image.height;

        for(int i=0;i<out_image.width;++i){
            for(int j=0;j<out_image.height;++j){

                out_image(i,j,0)=(out_image(i,j,0)>x)?255:0;
                out_image(i,j,1)=(out_image(i,j,1)>x)?255:0;
                out_image(i,j,2)=(out_image(i,j,2)>x)?255:0;
            }
        }


        for(int i=1;i<out_image.width;i+=1){
            for(int j=1;j<out_image.height;j+=1){

                int x=( out_image(i,j,0)!=out_image(i-1,j-1,0) || out_image(i,j,0)!=out_image(i-1,j,0) ||
                         out_image(i,j,0)!=out_image(i,j-1,0)) ? 0 : 255;

                image(i,j,0)=x;
                image(i,j,1)=x;
                image(i,j,2)=x;
            }
        }


        out_image = image;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }

}

void MainWindow::on_light_dark_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        ui->stackedWidget->setCurrentIndex(11);
    }
}

void MainWindow::on_ball_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        //here take the smaller dimention till the image is square
        Image image((out_image.width > out_image.height)?out_image.height:out_image.width,
                    (out_image.width > out_image.height)?out_image.height:out_image.width);

        // assign the half radius
        int r = 0.5 * image.width;

        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                double ball = (sqrt((r * r  - (i - r)*(i - r)  - (j - r)*(j - r) )) / r);
                image(i, j, 0) = out_image(i, j, 0) * ball;
                image(i, j, 1) = out_image(i, j, 1) * ball;
                image(i, j, 2) = out_image(i, j, 2) * ball;

            }
        }
        out_image = image;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_skewing_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        before_blur = out_image;
        ui->stackedWidget->setCurrentIndex(14);
    }
}

void MainWindow::on_dropwater_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);
        float coef = 100.0/((image.width<image.height)?image.width:image.height);
        for(int i=0;i<image.width;++i){
            for(int j=0;j<image.height;++j){
                float c = coef * (sqrt(square(i-image.width/2)+ square(j-image.height/2)));
                float z = .8 *( sin(c)/c + 0.24);
                image(i,j,0) = (image(i,j,0)*z*3 >255)? 255:image(i,j,0)*z*3;
                image(i,j,1) = (image(i,j,0)*z*3 >255)? 255:image(i,j,1)*z*3;
                image(i,j,2) = (image(i,j,0)*z*3 >255)? 255:image(i,j,2)*z*3;

            }
        }

        out_image = image;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_merge_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        ui->stackedWidget->setCurrentIndex(10);
    }
}

void MainWindow::on_invert_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);

        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                image(i, j, 0) = 225-image(i, j, 0);
                image(i, j, 1) = 225-image(i, j, 1);
                image(i, j, 2) = 225-image(i, j, 2);
            }
        }
        out_image = image;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_rotate_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        ui->stackedWidget->setCurrentIndex(16);
    }
}

void MainWindow::on_blur_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        before_blur = out_image;
        ui->stackedWidget->setCurrentIndex(19);
    }
}

void MainWindow::on_frame_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        before_frame = out_image;
        Image image(before_frame);
            for (int i = 0; i < image.width; ++i) {
                for (int j = 0; j < image.height; ++j) {

                    if(image.width-image.width/50 <= i ||
                        image.height-image.height/40 <= j ||
                        image.width/50 >= i ||
                        image.width/40 >= j)
                    {
                        image(i,j,0)=225;
                        image(i,j,1)=0;
                        image(i,j,2)=0;
                    }

                    if(image.width-image.width/80 <= i ||
                        image.height-image.height/80 <= j ||
                        image.width/80 >= i ||
                        image.width/80 >= j)
                    {
                        image(i,j,0)=30;
                        image(i,j,1)=30;
                        image(i,j,2)=30;
                    }
                }
            }
            ui->simple_image->setScaledContents(true);
            ui->simple_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            simple_image = image;
            simple_image.saveImage(in_image_path);
            QPixmap pixmap(qin_image_path);
            ui->simple_image->setPixmap(pixmap);

            image = before_frame;

            for(int i=0;i<image.width;++i){
                for(int j=0;j<image.height;++j){

                    //draw the white square bounded the maximum is 60
                    if(((i>image.width-60 && i<image.width-53 || j>53 && j<60) && i>image.width-60 && j<60) ||
                        ((i>image.width-48 && i<image.width-45 || j>45 && j<48) && i>image.width-48 && j<48) ||(i>image.width-35 && j<35) ||
                        ((i<60 && i>53 || j<image.height-53 && j>image.height-60) && i<60 && j>image.height-60) ||
                        ((i<48 && i>45 || j<image.height-45 && j>image.height-48) && i<48 && j>image.height-48) ||(i<35 && j>image.height-35)||
                        ((i<60 && i>53 || j>53 && j<60) && i<60 && j<60) || ((i<48 && i>45 || j>45 && j<48) && i<48 && j<48) ||(i<35 && j<35)||
                        ((i>image.width-60 && i<image.width-53 || j<image.height-53 && j>image.height-60) && i>image.width-60 && j>image.height-60) ||
                        ((i>image.width-48 && i<image.width-45 || j<image.height-45 && j>image.height-48) && i>image.width-48 && j>image.height-48) ||
                        (i>image.width-35 && j>image.height-35)){
                        image(i,j,0)=225;
                        image(i,j,1)=225;
                        image(i,j,2)=225;
                    }
                    //draw the colored lines
                    if(i<20 || j<20 || i>image.width-20 || j>image.height-20){
                        image(i,j,0)=225;
                        image(i,j,1)=0;
                        image(i,j,2)=0;
                    }
                    //draw the white straight lines
                    if((i>10 &&i<13       ) || (i>16 && i<20) || (j>10 && j<13) || (j>16 && j<20) || (i<image.width-10 && i>image.width-13) || (i<image.width-16 && i>image.width-20) || (j<image.height-10 && j>image.height-13) || (j<image.height-16 && j>image.height-20) ){
                        image(i,j,0)=225;
                        image(i,j,1)=225;
                        image(i,j,2)=225;
                    }
                    //This part draws the eight circles each line draws one circle
                    if((square(j-30)+square(i-image.width+60+10)<150 && square(j-30)+square(i-image.width+60+10)>100) ||
                        /*5*/ (square(i-30)+square(j-image.height+70)<150 && square(i-30)+square(j-image.height+70)>100)||
                        (square(i-70)+square(j-30)>100 && square(i-70)+square(j-30)<150) ||
                        (square(j-70)+square(i-30)>100 && square(j-70)+square(i-30)<150) ||
                        (square(j-70)+square(i-image.width+30)>100 && square(j-70)+square(i-image.width+30)<150) ||
                        /*6*/(square(i-70)+square(j-image.height+30)>100 && square(i-70)+square(j-image.height+30)<150) ||
                        (square(i-image.width+30)+square(j-image.height+70)>100 && square(i-image.width+30)+square(j-image.height+70)<150) ||
                        /*8*/(square(j-image.height+30)+square(i-image.width+70)>100 && (square(j-image.height+30)+square(i-image.width+70)<150))
                        ){
                        image(i,j,0)=225;
                        image(i,j,1)=225;
                        image(i,j,2)=225;
                    }
                }
            }
            ui->fancy_image->setScaledContents(true);
            ui->fancy_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            fancy_image = image;
            fancy_image.saveImage(in_image_path);
            QPixmap pixmap2(qin_image_path);
            ui->fancy_image->setPixmap(pixmap2);

            image = before_frame;

            //coef for coefficient the sin
            float coef = ((image.width > image.height)?image.height:image.width)/80.0;
            float arcoef = 1/coef;
            for(int i=0;i<image.width;++i){
                for(int j=0;j<image.height;++j){
                    if(abs(sin(i*arcoef))*coef>j || abs(sin(j*arcoef))*coef>i || abs(sin(i*arcoef))*coef>image.height-j || abs(sin(j*arcoef))*coef>image.width-i ){
                        image(i,j,0)=225;
                        image(i,j,1)=0;
                        image(i,j,2)=0;
                    }
                }
            }
            ui->circle_image->setScaledContents(true);
            ui->circle_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            circle_image = image;
            circle_image.saveImage(in_image_path);
            QPixmap pixmap3(qin_image_path);
            ui->circle_image->setPixmap(pixmap3);

            ui->stackedWidget->setCurrentIndex(20);
    }
}

void MainWindow::on_wano_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);

        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {

                if(image(i,j,1)<206){
                    image(i, j, 1) = image(i,j,1)+20;
                }
                else{
                    image(i, j, 1) = 225;
                }


            }
        }
        out_image = image;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_oil_painting_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);
        Image image2(out_image);


        vector<int> intensity(20, 0);
        vector<int> avR(20, 0);
        vector<int> avG(20, 0);
        vector<int> avB(20, 0);

        for (int i = 2; i < image.width - 2; i++) {
            for (int j = 2; j < image.height - 2; j++) {
                int curMax = 0;
                int MaxIndex = 0;

                for (int o = -2; o <= 2; o++) {
                    for (int p = -2; p <= 2; p++) {
                        int r = image(i + o, j + p, 0);
                        int g = image(i + o, j + p, 1);
                        int b = image(i + o, j + p, 2);


                        int curintensity = ((r + g + b) / 3) * 20 / 255;


                        intensity[curintensity]++;


                        avR[curintensity] += r;
                        avG[curintensity] += g;
                        avB[curintensity] += b;


                    }
                }


                for (int k = 0; k < 20; k++) {
                    if (intensity[k] > curMax) {
                        curMax = intensity[k];
                        MaxIndex = k;
                    }
                }


                int FR = avR[MaxIndex] / curMax;
                int FG = avG[MaxIndex] / curMax;
                int FB = avB[MaxIndex] / curMax;

                image2(i, j, 0) = FR;
                image2(i, j, 1) = FG;
                image2(i, j, 2) = FB;


                intensity.assign(20, 0);
                avR.assign(20, 0);
                avG.assign(20, 0);
                avB.assign(20, 0);
            }
        }
        out_image = image2;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_pixel_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        before_frame = out_image;
        ui->stackedWidget->setCurrentIndex(22);
    }
}

void MainWindow::on_behind_bars_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        Image image(out_image);
        for(int i=0;i<image.width;++i){
            for(int j=0;j<image.height;++j){
                float z = sin(0.2 * j  * i);
                z = sin(0.1 * i);
                if(z<0) {
                    image(i, j, 0) = -255 * z;
                    image(i, j, 1) = -255 * z;
                    image(i, j, 2) = -255 * z;
                }
            }
        }
        out_image = image;
        curr_image = out_image;
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}

void MainWindow::on_decorate_button_clicked()
{
    if (first == true){
        ui->no_image_errormessage->setStyleSheet("color: red");
        ui->no_image_errormessage->setText("No image was loaded");
    }
    else{
        before_frame = out_image;
        ui->stackedWidget->setCurrentIndex(12);
    }
}




//save window
void MainWindow::on_newsave_browse_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select a folder", QDir::homePath());
    if (folderPath.isEmpty()){
        ui->newsave_errormessage->setStyleSheet("color: red");
        ui->newsave_errormessage->setText("No Folder selected.");
    }
    else{
        newimage_path = folderPath.toStdString() + '\\';
        ui->newsave_newfilename->setEnabled(true);
        ui->newsave_errormessage->setText("");
    }
}

void MainWindow::on_newsave_newfilename_returnPressed()
{
    try{
        QString newfilename = ui->newsave_newfilename->text();
        newimage_path += newfilename.toStdString();
        out_image.saveImage(newimage_path);
        in_image.saveImage(in_image_path);
        ui->newsave_errormessage->setText("");
        newimage_path = "";
        ui->newsave_savesuccessuful->setVisible(true);
        QTimer::singleShot(3000, this, [this](){
            ui->original_image->clear();
            ui->current_image->clear();
            ui->stackedWidget->setCurrentIndex(1);
            ui->newsave_newfilename->setEnabled(false);
            ui->newsave_savesuccessuful->setVisible(false);
            ui->label_og->setVisible(false);
            ui->label_curr->setVisible(false);
            ui->newsave_newfilename->setText("");
            first = true;
        });

    }
    catch(const invalid_argument& e){
        QString qerror_message = QString::fromStdString(e.what());
        ui->newsave_errormessage->setStyleSheet("color: red");
        ui->newsave_errormessage->setText(qerror_message);
        ui->newsave_newfilename->setFocus();
    }
}

void MainWindow::on_newsave_savesame_clicked()
{
    ui->newsave_errormessage->setText("");
    newimage_path = "";
    ui->newsave_savesuccessuful->setVisible(true);
    QTimer::singleShot(3000, this, [this](){
        ui->original_image->clear();
        ui->current_image->clear();
        ui->stackedWidget->setCurrentIndex(1);
        ui->newsave_newfilename->setEnabled(false);
        ui->newsave_savesuccessuful->setVisible(false);
        ui->label_og->setVisible(false);
        ui->label_curr->setVisible(false);
        ui->newsave_newfilename->setText("");
        first = true;
    });
}

void MainWindow::on_save_apply_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


//Flip window
void MainWindow::on_flip_V_clicked()
{
    Image image(out_image);
    Image new_image(image.width, image.height);
    for (int i = 0; i < image.width; i++){
        for (int j = 0; j < image.height; j++){
            for (int c = 0; c < 3; c++){
                new_image(i, image.height - 1 - j, c) = image(i, j, c);

            }
        }
    }

    out_image = new_image;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::on_flip_H_clicked()
{
    Image image(out_image);
    Image new_image(image.width, image.height);
    for (int i = 0; i < image.width; i++){
        for (int j = 0; j < image.height; j++){
            for (int c = 0; c < 3; c++){
                new_image(image.width - 1 - i, j, c) = image(i, j, c);

            }
        }
    }

    out_image = new_image;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
    ui->stackedWidget->setCurrentIndex(1);
}


//crop window
void MainWindow::on_crop_crop_clicked()
{
    Image image(curr_image);
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
                out_image.saveImage(in_image_path);
                QPixmap pixmap(qin_image_path);
                ui->current_image->setPixmap(pixmap);

            }
            catch(const out_of_range& e){
                ui->crop_errormessage->setStyleSheet("color: red");
                ui->crop_errormessage->setText("The position you entered is invalid. Please try again.");
            }
        }

    }
}
void MainWindow::on_crop_apply_clicked()
{
    curr_image = out_image;
    ui->stackedWidget->setCurrentIndex(1);
    ui->crop_positionx->setText("");
    ui->crop_positiony->setText("");
    ui->crop_height->setText("");
    ui->crop_width->setText("");
}


//Resize window
void MainWindow::on_resize_resize_clicked()
{
    Image image(curr_image);
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
        out_image.saveImage(in_image_path);
        QPixmap pixmap(qin_image_path);
        ui->current_image->setPixmap(pixmap);
    }
}
void MainWindow::on_resize_apply_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    curr_image = out_image;
    ui->resize_H->setText("");
    ui->resize_W->setText("");
}



//light and dark window
void MainWindow::on_horiSlider_L_D_valueChanged(int value)
{
    ui->lab_HoriSlider_L_D->setText(QString::number(value));

}
void MainWindow::on_apply_LD_Button_clicked()
{
    float value = ui->lab_HoriSlider_L_D->text().toFloat();
    Image image(curr_image.width,curr_image.height);
    for(int i=0;i<curr_image.width;++i){
        for(int j=0;j<curr_image.height;++j){
            image(i,j,0)=(curr_image(i,j,0) * value/50.0 >255)?255:curr_image(i,j,0) * value/50.0;
            image(i,j,1)=(curr_image(i,j,1) * value/50.0 >255)?255:curr_image(i,j,1) * value/50.0;
            image(i,j,2)=(curr_image(i,j,2) * value/50.0 >255)?255:curr_image(i,j,2) * value/50.0;
        }
    }
    out_image = image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_brightness_apply_clicked()
{
    curr_image = out_image;
    ui->lab_HoriSlider_L_D->setText(QString::number(50));
    ui->horiSlider_L_D->setValue(50);
    ui->stackedWidget->setCurrentIndex(1);
}


//Rotate window
void MainWindow::on_degree90_button_clicked()
{
    Image image(out_image);
    Image image2(image.height,image.width);

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {

            image2(image.height-j-1 ,i ,0) = image(i,j,0);
            image2(image.height-j-1 ,i ,1) = image(i,j,1);
            image2(image.height-j-1 ,i ,2) = image(i,j,2);

        }
    }
    out_image = image2;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_degree180_button_clicked()
{
    Image image(out_image);
    Image image2(image.width,image.height);
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            image2(image.width-i-1 ,image.height-j-1 ,0) = image(i,j,0);
            image2(image.width-i-1 ,image.height-j-1 ,1) = image(i,j,1);
            image2(image.width-i-1 ,image.height-j-1 ,2) = image(i,j,2);
        }
    }
    out_image = image2;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_degree270_button_clicked()
{
    Image image(out_image);
    Image image2(image.height,image.width);

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {



            image2(j ,image.width-i-1 ,0) = image(i,j,0);
            image2(j ,image.width-i-1 ,1) = image(i,j,1);
            image2(j ,image.width-i-1 ,2) = image(i,j,2);

        }
    }
    out_image = image2;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_rotate_apply_clicked()
{
       ui->stackedWidget->setCurrentIndex(1);
}

//Blur window
void MainWindow::on_light_blur_button_clicked()
{
    out_image = before_blur;
    Image image(out_image);
    Image image2(out_image);
    int r=7 , sum=0;
    for (int i = r; i < image2.width-r; i++) {
        for (int j = r; j < image2.height-r; j++) {
            for (int k = 0; k < 3; k++){
                for (int o = -r; o < r; o++){
                    for (int p = -r; p < r; p++){
                        sum += image(i+o,j+p,k);
                    }
                }
                image2(i,j,k)=(sum/pow(2*r,2));
                sum=0;

            }

        }
    }
    out_image = image2;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_medium_blur_button_clicked()
{
    out_image = before_blur;
    Image image(out_image);
    Image image2(out_image);
    int r=10 , sum=0;
    for (int i = r; i < image2.width-r; i++) {
        for (int j = r; j < image2.height-r; j++) {
            for (int k = 0; k < 3; k++){
                for (int o = -r; o < r; o++){
                    for (int p = -r; p < r; p++){
                        sum += image(i+o,j+p,k);
                    }
                }
                image2(i,j,k)=(sum/pow(2*r,2));
                sum=0;

            }

        }
    }
    out_image = image2;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_strong_blur_button_clicked()
{
    out_image = before_blur;
    Image image(out_image);
    Image image2(out_image);
    int r=13 , sum=0;
    for (int i = r; i < image2.width-r; i++) {
        for (int j = r; j < image2.height-r; j++) {
            for (int k = 0; k < 3; k++){
                for (int o = -r; o < r; o++){
                    for (int p = -r; p < r; p++){
                        sum += image(i+o,j+p,k);
                    }
                }
                image2(i,j,k)=(sum/pow(2*r,2));
                sum=0;

            }

        }
    }
    out_image = image2;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_blur_apply_clicked()
{
        ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_remove_blur_button_clicked()
{
    out_image = before_blur;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}

//Frame window
void MainWindow::on_simple_button_clicked()
{
    choice = '1';
    ui->stackedWidget->setCurrentIndex(21);
}
void MainWindow::on_fancy_button_clicked()
{
    choice = '2';
    ui->stackedWidget->setCurrentIndex(21);
}
void MainWindow::on_circle_button_clicked()
{
    choice = '3';
    ui->stackedWidget->setCurrentIndex(21);
}
void MainWindow::on_red_frame_button_clicked()
{
    out_image = before_frame;
    Image image(out_image);
    if(choice == '1'){
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {

                if(image.width-image.width/50 <= i ||
                    image.height-image.height/40 <= j ||
                    image.width/50 >= i ||
                    image.width/40 >= j)
                {
                    image(i,j,0)=225;
                    image(i,j,1)=0;
                    image(i,j,2)=0;
                }

                if(image.width-image.width/80 <= i ||
                    image.height-image.height/80 <= j ||
                    image.width/80 >= i ||
                    image.width/80 >= j)
                {
                    image(i,j,0)=30;
                    image(i,j,1)=30;
                    image(i,j,2)=30;
                }
            }
        }
    }

    if(choice == '2'){
        for(int i=0;i<image.width;++i){
            for(int j=0;j<image.height;++j){

                //draw the white square bounded the maximum is 60
                if(((i>image.width-60 && i<image.width-53 || j>53 && j<60) && i>image.width-60 && j<60) ||
                    ((i>image.width-48 && i<image.width-45 || j>45 && j<48) && i>image.width-48 && j<48) ||(i>image.width-35 && j<35) ||
                    ((i<60 && i>53 || j<image.height-53 && j>image.height-60) && i<60 && j>image.height-60) ||
                    ((i<48 && i>45 || j<image.height-45 && j>image.height-48) && i<48 && j>image.height-48) ||(i<35 && j>image.height-35)||
                    ((i<60 && i>53 || j>53 && j<60) && i<60 && j<60) || ((i<48 && i>45 || j>45 && j<48) && i<48 && j<48) ||(i<35 && j<35)||
                    ((i>image.width-60 && i<image.width-53 || j<image.height-53 && j>image.height-60) && i>image.width-60 && j>image.height-60) ||
                    ((i>image.width-48 && i<image.width-45 || j<image.height-45 && j>image.height-48) && i>image.width-48 && j>image.height-48) ||
                    (i>image.width-35 && j>image.height-35)){
                    image(i,j,0)=225;
                    image(i,j,1)=225;
                    image(i,j,2)=225;
                }
                //draw the colored lines
                if(i<20 || j<20 || i>image.width-20 || j>image.height-20){
                    image(i,j,0)=225;
                    image(i,j,1)=0;
                    image(i,j,2)=0;
                }
                //draw the white straight lines
                if((i>10 &&i<13       ) || (i>16 && i<20) || (j>10 && j<13) || (j>16 && j<20) || (i<image.width-10 && i>image.width-13) || (i<image.width-16 && i>image.width-20) || (j<image.height-10 && j>image.height-13) || (j<image.height-16 && j>image.height-20) ){
                    image(i,j,0)=225;
                    image(i,j,1)=225;
                    image(i,j,2)=225;
                }
                //This part draws the eight circles each line draws one circle
                if((square(j-30)+square(i-image.width+60+10)<150 && square(j-30)+square(i-image.width+60+10)>100) ||
                    /*5*/ (square(i-30)+square(j-image.height+70)<150 && square(i-30)+square(j-image.height+70)>100)||
                    (square(i-70)+square(j-30)>100 && square(i-70)+square(j-30)<150) ||
                    (square(j-70)+square(i-30)>100 && square(j-70)+square(i-30)<150) ||
                    (square(j-70)+square(i-image.width+30)>100 && square(j-70)+square(i-image.width+30)<150) ||
                    /*6*/(square(i-70)+square(j-image.height+30)>100 && square(i-70)+square(j-image.height+30)<150) ||
                    (square(i-image.width+30)+square(j-image.height+70)>100 && square(i-image.width+30)+square(j-image.height+70)<150) ||
                    /*8*/(square(j-image.height+30)+square(i-image.width+70)>100 && (square(j-image.height+30)+square(i-image.width+70)<150))
                    ){
                    image(i,j,0)=225;
                    image(i,j,1)=225;
                    image(i,j,2)=225;
                }
            }
        }
    }

    if(choice == '3'){
        //coef for coefficient the sin
        float coef = ((image.width > image.height)?image.height:image.width)/80.0;
        float arcoef = 1/coef;
        for(int i=0;i<image.width;++i){
            for(int j=0;j<image.height;++j){
                if(abs(sin(i*arcoef))*coef>j || abs(sin(j*arcoef))*coef>i || abs(sin(i*arcoef))*coef>image.height-j || abs(sin(j*arcoef))*coef>image.width-i ){
                    image(i,j,0)=225;
                    image(i,j,1)=0;
                    image(i,j,2)=0;
                }
            }
        }
    }
    out_image = image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);

}
void MainWindow::on_green_frame_button_clicked()
{
    out_image = before_frame;
    Image image(out_image);
    if(choice == '1'){
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {

                if(image.width-image.width/50 <= i ||
                    image.height-image.height/40 <= j ||
                    image.width/50 >= i ||
                    image.width/40 >= j)
                {
                    image(i,j,0)=0;
                    image(i,j,1)=225;
                    image(i,j,2)=0;
                }

                if(image.width-image.width/80 <= i ||
                    image.height-image.height/80 <= j ||
                    image.width/80 >= i ||
                    image.width/80 >= j)
                {
                    image(i,j,0)=30;
                    image(i,j,1)=30;
                    image(i,j,2)=30;
                }
            }
        }
    }
    if(choice == '2'){
        for(int i=0;i<image.width;++i){
            for(int j=0;j<image.height;++j){

                //draw the white square bounded the maximum is 60
                if(((i>image.width-60 && i<image.width-53 || j>53 && j<60) && i>image.width-60 && j<60) ||
                    ((i>image.width-48 && i<image.width-45 || j>45 && j<48) && i>image.width-48 && j<48) ||(i>image.width-35 && j<35) ||
                    ((i<60 && i>53 || j<image.height-53 && j>image.height-60) && i<60 && j>image.height-60) ||
                    ((i<48 && i>45 || j<image.height-45 && j>image.height-48) && i<48 && j>image.height-48) ||(i<35 && j>image.height-35)||
                    ((i<60 && i>53 || j>53 && j<60) && i<60 && j<60) || ((i<48 && i>45 || j>45 && j<48) && i<48 && j<48) ||(i<35 && j<35)||
                    ((i>image.width-60 && i<image.width-53 || j<image.height-53 && j>image.height-60) && i>image.width-60 && j>image.height-60) ||
                    ((i>image.width-48 && i<image.width-45 || j<image.height-45 && j>image.height-48) && i>image.width-48 && j>image.height-48) ||
                    (i>image.width-35 && j>image.height-35)){
                    image(i,j,0)=225;
                    image(i,j,1)=225;
                    image(i,j,2)=225;
                }
                //draw the colored lines
                if(i<20 || j<20 || i>image.width-20 || j>image.height-20){
                    image(i,j,0)=0;
                    image(i,j,1)=225;
                    image(i,j,2)=0;
                }
                //draw the white straight lines
                if((i>10 &&i<13       ) || (i>16 && i<20) || (j>10 && j<13) || (j>16 && j<20) || (i<image.width-10 && i>image.width-13) || (i<image.width-16 && i>image.width-20) || (j<image.height-10 && j>image.height-13) || (j<image.height-16 && j>image.height-20) ){
                    image(i,j,0)=225;
                    image(i,j,1)=225;
                    image(i,j,2)=225;
                }
                //This part draws the eight circles each line draws one circle
                if((square(j-30)+square(i-image.width+60+10)<150 && square(j-30)+square(i-image.width+60+10)>100) ||
                    /*5*/ (square(i-30)+square(j-image.height+70)<150 && square(i-30)+square(j-image.height+70)>100)||
                    (square(i-70)+square(j-30)>100 && square(i-70)+square(j-30)<150) ||
                    (square(j-70)+square(i-30)>100 && square(j-70)+square(i-30)<150) ||
                    (square(j-70)+square(i-image.width+30)>100 && square(j-70)+square(i-image.width+30)<150) ||
                    /*6*/(square(i-70)+square(j-image.height+30)>100 && square(i-70)+square(j-image.height+30)<150) ||
                    (square(i-image.width+30)+square(j-image.height+70)>100 && square(i-image.width+30)+square(j-image.height+70)<150) ||
                    /*8*/(square(j-image.height+30)+square(i-image.width+70)>100 && (square(j-image.height+30)+square(i-image.width+70)<150))
                    ){
                    image(i,j,0)=225;
                    image(i,j,1)=225;
                    image(i,j,2)=225;
                }
            }
        }
    }
    if(choice == '3'){
        //coef for coefficient the sin
        float coef = ((image.width > image.height)?image.height:image.width)/80.0;
        float arcoef = 1/coef;
        for(int i=0;i<image.width;++i){
            for(int j=0;j<image.height;++j){
                if(abs(sin(i*arcoef))*coef>j || abs(sin(j*arcoef))*coef>i || abs(sin(i*arcoef))*coef>image.height-j || abs(sin(j*arcoef))*coef>image.width-i ){
                    image(i,j,0)=0;
                    image(i,j,1)=225;
                    image(i,j,2)=0;
                }
            }
        }
    }
    out_image = image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_blue_frame_button_clicked()
{
    out_image = before_frame;
    Image image(out_image);
    if(choice == '1'){
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {

                if(image.width-image.width/50 <= i ||
                    image.height-image.height/40 <= j ||
                    image.width/50 >= i ||
                    image.width/40 >= j)
                {
                    image(i,j,0)=0;
                    image(i,j,1)=0;
                    image(i,j,2)=225;
                }

                if(image.width-image.width/80 <= i ||
                    image.height-image.height/80 <= j ||
                    image.width/80 >= i ||
                    image.width/80 >= j)
                {
                    image(i,j,0)=30;
                    image(i,j,1)=30;
                    image(i,j,2)=30;
                }
            }
        }
    }
    if(choice == '2'){
        for(int i=0;i<image.width;++i){
            for(int j=0;j<image.height;++j){

                //draw the white square bounded the maximum is 60
                if(((i>image.width-60 && i<image.width-53 || j>53 && j<60) && i>image.width-60 && j<60) ||
                    ((i>image.width-48 && i<image.width-45 || j>45 && j<48) && i>image.width-48 && j<48) ||(i>image.width-35 && j<35) ||
                    ((i<60 && i>53 || j<image.height-53 && j>image.height-60) && i<60 && j>image.height-60) ||
                    ((i<48 && i>45 || j<image.height-45 && j>image.height-48) && i<48 && j>image.height-48) ||(i<35 && j>image.height-35)||
                    ((i<60 && i>53 || j>53 && j<60) && i<60 && j<60) || ((i<48 && i>45 || j>45 && j<48) && i<48 && j<48) ||(i<35 && j<35)||
                    ((i>image.width-60 && i<image.width-53 || j<image.height-53 && j>image.height-60) && i>image.width-60 && j>image.height-60) ||
                    ((i>image.width-48 && i<image.width-45 || j<image.height-45 && j>image.height-48) && i>image.width-48 && j>image.height-48) ||
                    (i>image.width-35 && j>image.height-35)){
                    image(i,j,0)=255;
                    image(i,j,1)=255;
                    image(i,j,2)=255;
                }
                //draw the colored lines
                if(i<20 || j<20 || i>image.width-20 || j>image.height-20){
                    image(i,j,0)=0;
                    image(i,j,1)=0;
                    image(i,j,2)=225;
                }
                //draw the white straight lines
                if((i>10 &&i<13       ) || (i>16 && i<20) || (j>10 && j<13) || (j>16 && j<20) || (i<image.width-10 && i>image.width-13) || (i<image.width-16 && i>image.width-20) || (j<image.height-10 && j>image.height-13) || (j<image.height-16 && j>image.height-20) ){
                    image(i,j,0)=255;
                    image(i,j,1)=255;
                    image(i,j,2)=255;
                }
                //This part draws the eight circles each line draws one circle
                if((square(j-30)+square(i-image.width+60+10)<150 && square(j-30)+square(i-image.width+60+10)>100) ||
                    /*5*/ (square(i-30)+square(j-image.height+70)<150 && square(i-30)+square(j-image.height+70)>100)||
                    (square(i-70)+square(j-30)>100 && square(i-70)+square(j-30)<150) ||
                    (square(j-70)+square(i-30)>100 && square(j-70)+square(i-30)<150) ||
                    (square(j-70)+square(i-image.width+30)>100 && square(j-70)+square(i-image.width+30)<150) ||
                    /*6*/(square(i-70)+square(j-image.height+30)>100 && square(i-70)+square(j-image.height+30)<150) ||
                    (square(i-image.width+30)+square(j-image.height+70)>100 && square(i-image.width+30)+square(j-image.height+70)<150) ||
                    /*8*/(square(j-image.height+30)+square(i-image.width+70)>100 && (square(j-image.height+30)+square(i-image.width+70)<150))
                    ){
                    image(i,j,0)=255;
                    image(i,j,1)=255;
                    image(i,j,2)=255;
                }
            }
        }
    }
    if(choice == '3'){
        //coef for coefficient the sin
        float coef = ((image.width > image.height)?image.height:image.width)/80.0;
        float arcoef = 1/coef;
        for(int i=0;i<image.width;++i){
            for(int j=0;j<image.height;++j){
                if(abs(sin(i*arcoef))*coef>j || abs(sin(j*arcoef))*coef>i || abs(sin(i*arcoef))*coef>image.height-j || abs(sin(j*arcoef))*coef>image.width-i ){
                    image(i,j,0)=0;
                    image(i,j,1)=0;
                    image(i,j,2)=225;
                }
            }
        }
    }
    out_image = image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_back_to_menu_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    curr_image = out_image;
}
void MainWindow::on_back_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
}
void MainWindow::on_remove_frame_button_clicked()
{
    out_image = before_frame;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}


//merge window
void MainWindow::on_merge_browse_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select a file", QDir::homePath(), "All Files (*)");
    if (filePath == ""){
        ui->merge_errormessage->setStyleSheet("color: red");
        ui->merge_errormessage->setText("No file selected.");
    }
    else{

        try{
            Image image(filePath.toStdString());
            in_image2 = image;
            ui->merge_errormessage->setText("");
            QPixmap pixmap(filePath);
            ui->merge_image->setPixmap(pixmap);
            ui->merge_common->setEnabled(true);
            ui->merge_larger->setEnabled(true);
            ui->merge_common->setVisible(true);
            ui->merge_larger->setVisible(true);
        }

        catch(const invalid_argument& e){
            QString qerror_message = QString::fromStdString(e.what());
            ui->merge_errormessage->setStyleSheet("color: red");
            ui->merge_errormessage->setText(qerror_message);
        }

    }
}

void MainWindow::on_merge_larger_clicked()
{
    Image image(out_image);
    Image image2(in_image2);
    float x = (float )image.width/image2.width;
    float y = (float )image.height/image2.height;
    Image image4((image.width > image2.width)?image.width:image2.width,(image.height > image2.height)?image.height:image2.height);
    if(image.width>=image2.width && image.height>=image2.height){
        for(int i=0;i<image4.width;++i){
            for(int j=0;j<image4.height;++j){
                image4(i,j,0) = (image(i,j,0)+image2(i/x,j/y,0))/2;
                image4(i,j,1) = (image(i,j,1)+image2(i/x,j/y,1))/2;
                image4(i,j,2) = (image(i,j,2)+image2(i/x,j/y,2))/2;
            }
        }
    }
    else if(image.width<image2.width && image.height<image2.height){
        for(int i=0;i<image4.width;++i){
            for(int j=0;j<image4.height;++j){
                image4(i,j,0) = (image(i*x,j*y,0)+image2(i,j,0))/2;
                image4(i,j,1) = (image(i*x,j*y,1)+image2(i,j,1))/2;
                image4(i,j,2) = (image(i*x,j*y,2)+image2(i,j,2))/2;
            }
        }
    }
    else if(image.width>image2.width && image.height<image2.height){
        for(int i=0;i<image4.width;++i){
            for(int j=0;j<image4.height;++j){
                image4(i,j,0) = (image(i,j*y,0)+image2(i/x,j,0))/2;
                image4(i,j,1) = (image(i,j*y,1)+image2(i/x,j,1))/2;
                image4(i,j,2) = (image(i,j*y,2)+image2(i/x,j,2))/2;
            }
        }
    }
    else if(image.width<image2.width && image.height>image2.height){
        for(int i=0;i<image4.width;++i){
            for(int j=0;j<image4.height;++j){
                image4(i,j,0) = (image(i*x,j,0)+image2(i,j/y,0))/2;
                image4(i,j,1) = (image(i*x,j,1)+image2(i,j/y,1))/2;
                image4(i,j,2) = (image(i*x,j,2)+image2(i,j/y,2))/2;
            }
        }
    }

    out_image = image4;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
    ui->merge_image->clear();
    ui->merge_common->setEnabled(false);
    ui->merge_larger->setEnabled(false);
    ui->merge_common->setVisible(false);
    ui->merge_larger->setVisible(false);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_merge_common_clicked()
{
    Image image(out_image);
    Image image2(in_image2);
    Image image3((image.width<=image2.width)?image.width:image2.width,(image.height<=image2.height)?image.height:image2.height);
    for(int i=0;i<image3.width;++i){
        for(int j=0;j<image3.height;++j){
            image3(i,j,0)=(image(i,j,0)+image2(i,j,0))/2;
            image3(i,j,1)=(image(i,j,0)+image2(i,j,1))/2;
            image3(i,j,2)=(image(i,j,0)+image2(i,j,2))/2;
        }
    }
    out_image = image3;
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
    ui->merge_image->clear();
    ui->merge_common->setEnabled(false);
    ui->merge_larger->setEnabled(false);
    ui->merge_common->setVisible(false);
    ui->merge_larger->setVisible(false);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_menu_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}




//skew window
void MainWindow::on_skew_skew_clicked()
{
    Image image(curr_image);
    float angle;
    QRegularExpression rx("-?[0-9]+");
    QRegularExpressionValidator angle_validator(rx, ui->skew_angle);
    ui->skew_angle->setValidator(&angle_validator);

    if (ui->skew_angle->text() == ""){
        ui->skew_errormessage->setStyleSheet("color: red");
        ui->skew_errormessage->setText("Please enter all values");
    }

    else if (!ui->skew_angle->hasAcceptableInput()){
        ui->skew_errormessage->setStyleSheet("color: red");
        ui->skew_errormessage->setText("All values must be numbers.");
    }

    else{
        angle = ui->skew_angle->text().toFloat();
        if (abs(angle) >= 90){
            ui->skew_errormessage->setStyleSheet("color: red");
            ui->skew_errormessage->setText("The absolute value of the angle must be less than 90.");
        }

        else{
            ui->skew_errormessage->setText("");
            if (angle > 0){
                float ang=((90 - angle)/180)*3.1416;
                float slop=tan(ang);
                Image image2(image.width + image.height * cos(ang),image.height * sin(ang));
                float n = (float)image.height / image2.height;
                for(int i=0;i<image2.width;++i){
                    for(int j=0;j<image2.height;++j){
                        if((j>image2.height-slop*i) && ((j<image2.height -slop*(i - image.width)))){
                            image2(i,j,0) = image(i-(image2.height-j)/slop,j*n,0);
                            image2(i,j,1) = image(i-(image2.height-j)/slop,j*n,1);
                            image2(i,j,2) = image(i-(image2.height-j)/slop,j*n,2);
                        }
                        else{
                            image2(i,j,0)=225;
                            image2(i,j,1)=225;
                            image2(i,j,2)=225;
                        }
                    }
                }
                out_image = image2;
                out_image.saveImage(in_image_path);
                QPixmap pixmap(qin_image_path);
                ui->current_image->setPixmap(pixmap);
            }

            else if (angle < 0){
                float ang=((90.0-abs(angle))/180)*3.1416;
                float slop=tan(ang);
                Image image2(image.width + image.height * cos(ang),image.height * sin(ang));
                float n = (float)image.height / image2.height;
                for(int i=image2.width-1;i>=0;--i){
                    for(int j=image2.height-1;j>=0;--j){
                        if((j<slop*i) && j>(slop*i-slop*image.width)){
                            image2(i,j,0)=image((i-j/slop),j*n,0);
                            image2(i,j,1)=image((i-j/slop),j*n,1);
                            image2(i,j,2)=image((i-j/slop),j*n,2);
                        }
                    }
                }

                out_image = image2;
                out_image.saveImage(in_image_path);
                QPixmap pixmap(qin_image_path);
                ui->current_image->setPixmap(pixmap);
            }
        }
    }
}

void MainWindow::on_skew_apply_clicked()
{
    curr_image = out_image;
    ui->skew_angle->setText("");
    ui->skew_errormessage->setText("");
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_remove_skew_button_clicked()
{
    out_image = before_blur;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
    ui->skew_angle->setText("");
}



//pixel window
void MainWindow::on_pixel_slider_valueChanged(int SlideValue)
{
    if(SlideValue!=0){
        Image image(before_frame);
        int value = 0;
        value += SlideValue;
        int b = (value/200.0)*((image.width < image.height)? image.height: image.width);
        for(int i=0;i<image.width;i+=b){
            for(int j=0;j<image.height;j+=b){
                int R=0,G=0,B=0,k = 0, l = 0;
                for(;(k<b)&&(i+k<image.width);++k){
                    for(;(l<b) && (l+j<image.height);++l) {
                        R += image(i + k, j + l, 0);
                        G += image(i + k, j + l, 1);
                        B += image(i + k, j + l, 2);
                    }
                }
                R/=  l ;
                G/=  l ;
                B/=  l ;
                for(int k=i;((k<i+b))&&(k<image.width);++k){
                    for(int l=j;((l<j+b))&&(l<image.height);++l){
                        image(k,l,0)=R;
                        image(k,l,1)=G;
                        image(k,l,2)=B;
                    }
                }
            }
        }
        out_image = image;
    }
    else{
        out_image=before_frame;
    }
    curr_image = out_image;
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_pixel_apply_button_clicked()
{
    before_frame = out_image;
    ui->pixel_slider->setValue(0);
    ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_pixel_menu_button_clicked()
{
    out_image = before_frame;
    ui->pixel_slider->setValue(0);
    ui->stackedWidget->setCurrentIndex(1);
}


//decorate window
void MainWindow::on_decorate_slider_valueChanged(int value)
{
    ui->lab_deco_slider->setText(QString::number(value));
}
void MainWindow::on_decorate_apply_clicked()
{
    int value = ui->lab_deco_slider->text().toFloat();
    if(value!=0){
        Image image(curr_image);
        float coef= value;
        coef /= 10;
        for(int i=0;i<image.width;++i){
            for(int j=0;j<image.height;++j){
                float z = sin(coef * j  * i);
                if(z>0) {
                    image(i, j, 0) = 255 * z;
                    image(i, j, 1) = 255 * z;
                    image(i, j, 2) = 255 * z;
                }
            }
        }
        out_image = image;
    }
    else{
        out_image=curr_image;
    }
    out_image.saveImage(in_image_path);
    QPixmap pixmap(qin_image_path);
    ui->current_image->setPixmap(pixmap);
}
void MainWindow::on_decorate_menu_button_clicked()
{
    curr_image = out_image;
    ui->lab_deco_slider->setText(QString::number(0));
    ui->decorate_slider->setValue(0);
    ui->stackedWidget->setCurrentIndex(1);
}









void MainWindow::on_no_image_errormessage_linkActivated(const QString &link)
{

}
































