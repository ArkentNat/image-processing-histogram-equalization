#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <iostream>
#include <QFileDialog>

#define L 256

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->display1->setScaledContents(true);
    ui->display2->setScaledContents(true);
    ui->display3->setScaledContents(true);
    ui->display4->setScaledContents(true);

    ui->display1->setFixedSize( QLWidth, QLHeight );
    ui->display2->setFixedSize( QLWidth, QLHeight );
    ui->display3->setFixedSize( QLWidth, QLHeight );
    ui->display4->setFixedSize( QLWidth, QLHeight );

    //cap.open(1);

    // Call update every 100ms
//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(333);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    if( !cap.isOpened() ) return;

    cap >> capture;
    cvtColor(capture, capture, COLOR_BGR2GRAY);

    auto equalized = Histogram::equalizeHistogram(capture);

    double* inputHistogram = Histogram::calculateHistogram(capture);
    double* inputPdf = Histogram::calculatePdf(inputHistogram, capture.rows * capture.cols);

    double* outputHistogram = Histogram::calculateHistogram(equalized);
    double* outputPdf = Histogram::calculatePdf(outputHistogram, equalized.rows * equalized.cols);

    QImage img1 = MatToQImage(capture);
    QImage img2 = MatToQImage(equalized);
    QImage img3 = MatToQImage(dip::drawHistogram(inputPdf, L));
    QImage img4 = MatToQImage(dip::drawHistogram(outputPdf, L));


    ui->display1->setPixmap(QPixmap::fromImage(img1));
    ui->display2->setPixmap(QPixmap::fromImage(img2));
    ui->display3->setPixmap(QPixmap::fromImage(img3));
    ui->display4->setPixmap(QPixmap::fromImage(img4));
}

void MainWindow::on_btn_image_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("images (*.png, *jpg)"));

    if(QString::compare(filename, QString()) != 0){
        QImage img1, img2, img3, img4;
        bool valid = img1.load(filename);

        if(valid){
            img1 = img1.scaledToWidth(ui->display1->width(), Qt::SmoothTransformation);

            capture = imread(filename.toStdString());
            cvtColor(capture, capture, COLOR_BGR2GRAY);

            auto equalized = Histogram::equalizeHistogram(capture);

            double* inputHistogram = Histogram::calculateHistogram(capture);
            double* inputPdf = Histogram::calculatePdf(inputHistogram, capture.rows * capture.cols);

            double* outputHistogram = Histogram::calculateHistogram(equalized);
            double* outputPdf = Histogram::calculatePdf(outputHistogram, equalized.rows * equalized.cols);

            QImage img1 = MatToQImage(capture);
            QImage img2 = MatToQImage(equalized);
            QImage img3 = MatToQImage(dip::drawHistogram(inputPdf, L));
            QImage img4 = MatToQImage(dip::drawHistogram(outputPdf, L));


            ui->display1->setPixmap(QPixmap::fromImage(img1));
            ui->display2->setPixmap(QPixmap::fromImage(img2));
            ui->display3->setPixmap(QPixmap::fromImage(img3));
            ui->display4->setPixmap(QPixmap::fromImage(img4));
        } else {
            // Error Handling
        }
    }
}

//#include <iostream>
//#include "histogram.h"
//#include "utility.h"

//using namespace cv;

//int main(int argc, char** argv) {
//        const String keys =
//    "{help h usage ?    || The program does histogram equalization on the image.}"
//    "{input             | histogram-equalization.png | input image}"
//    ;

//    CommandLineParser cmdParser(argc , argv, keys);

//    if (cmdParser.has("help"))
//    {
//        cmdParser.printMessage();
//        return 0;
//    }

//    Mat input;

//    input = imread("C:\\Users\\user\\OneDrive\\Gambar\\Saved pictures\\hehe_cat.jpg");

//    if ( !input.data )
//    {
//        printf("No input data \n");
//        return -1;
//    }

//    cvtColor(input , input , COLOR_BGR2GRAY);

//    //Performs histogram equalization
//    Mat histogramEqualizedImage = Histogram::equalizeHistogram(input);

//    imshow("input" , input);
//    imshow("Histogram equalized output", histogramEqualizedImage);

//    waitKey(0);

//    return 0;
//}
