#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void LoadImg();
    void SaveImg();
    void RotateImg();
    void ZoomImg(int zoom);
    QImage Mat_QImage(const cv::Mat& mat);

private:
    Ui::MainWindow *ui;
    cv::Mat m_img;  
    cv::Mat scaled_mat;
    cv::Mat rotated_img;
    double m_zoom;
};
#endif // MAINWINDOW_H
