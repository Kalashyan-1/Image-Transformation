#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_zoom {1.0}
{
    ui->setupUi(this);
    ui->horizontalSlider->setRange(0, 4);
    ui->horizontalSlider->setValue(0);
    ui->ZoomSlider->setRange(50, 150);
    ui->ZoomSlider->setValue(100);
    ui->label->setAlignment(Qt::AlignCenter);
    connect(ui->Load_Button, SIGNAL(released()), this, SLOT(LoadImg()));
}

void MainWindow::LoadImg()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::homePath(), tr("Images (*.png *.xpm *.jpg *.bmp)"));
    if (fileName.isEmpty()) {
        qDebug() << "Can not Load Image";
        return;
    }
    m_img = rotated_img = scaled_mat = cv::imread(fileName.toStdString());
    QPixmap image(fileName);
    ui->label->setPixmap(image.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::RotateImg);
    connect(ui->ZoomSlider, &QSlider::valueChanged, this, &MainWindow::ZoomImg);
    connect(ui->Save_Button, SIGNAL(released()), this, SLOT(SaveImg()));
}

void MainWindow::SaveImg()
{
    QPixmap pixmap = ui->label->pixmap();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), QDir::homePath(), tr("Images (*.png *.xpm *.jpg *.bmp)"));
    if (fileName.isEmpty()) {
        qDebug() << "Can not Load Image";
        return;
    }
    pixmap.save(fileName);
}

void MainWindow::RotateImg()
{
    cv::rotate(scaled_mat, rotated_img, cv::ROTATE_90_CLOCKWISE);
    cv::rotate(m_img, m_img, cv::ROTATE_90_CLOCKWISE);
    scaled_mat = rotated_img;
    QImage qImage = Mat_QImage(rotated_img);
    QPixmap pix(QPixmap::fromImage(qImage));
    ui->label->setPixmap(pix);
}

void MainWindow::ZoomImg(int zoom)
{
    m_zoom = zoom / 100.0;
    cv::resize(m_img, scaled_mat, cv::Size(),m_zoom, m_zoom);
    QImage qImage = Mat_QImage(scaled_mat);
    QPixmap pix(QPixmap::fromImage(qImage));
    ui->label->setPixmap(pix);
}

QImage MainWindow::Mat_QImage(const cv::Mat& mat)
{
    if(mat.type() == CV_8UC1)
    {
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++) {
            colorTable.push_back(qRgb(i, i, i));
        }
        const uchar *qImageBuffer = (const uchar*)mat.data;
        QImage img(qImageBuffer, mat.cols,mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
     }
     if(mat.type() == CV_8UC3)
     {
        const uchar *qImageBuffer = (const uchar*)mat.data;
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
     }else{
        return QImage();
     }
}

MainWindow::~MainWindow()
{
    delete ui;
}

