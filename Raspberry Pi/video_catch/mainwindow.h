#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QLineSeries>

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

    void rect_dected(cv::Mat frame);

private slots:
    void onCapture();

    void on_tabWidget_currentChanged(int index);

    void on_MinSlider_sliderMoved(int position);

    void on_MaxSlider_sliderMoved(int position);

    void on_MaxSlider_2_sliderMoved(int position);

    void on_MinSlider_2_sliderMoved(int position);

    void on_kalmanXButton_toggled(bool checked);

    void on_kalmanYButton_toggled(bool checked);

    void on_pidXButton_toggled(bool checked);

    void on_pidYButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QTimer     *timer;
    cv::VideoCapture cap;      // OpenCV 摄像头对象
    QImage originImg;
    QImage binImg;
    QImage edgeImg;
    QImage resultImg;

    // for draw line
    int time_counter;
    QLineSeries* kalman_x_series;
    bool show_kalman_x;
    QLineSeries* kalman_y_series;
    bool show_kalman_y;
    QLineSeries* pid_out_x_series;
    bool show_pid_x;
    QLineSeries* pid_out_y_series;
    bool show_pid_y;

    int nowTab;
    int max_thresh;
    int min_thresh;

    void update_list(int kalman_x, int kalman_y, int pid_out_x, int pid_out_y);
    void check_show();
};
#endif // MAINWINDOW_H
