/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2026-01-10 15:00:30
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \video_catch\mainwindow.cpp
 * @Description:
 */
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <QLineSeries>
#include <QTimer>

#include <QtCharts>

qint64 start_time = 0;

QRandomGenerator gen1(12345);
QRandomGenerator gen2(12346);
QRandomGenerator gen3(12347);
QRandomGenerator gen4(12348);


enum pic_state
{
    origin,
    bin,
    edge,
    result
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , nowTab(0)
    , max_thresh(200)
    , min_thresh(100)
    , time_counter(0)
{
    ui->setupUi(this);

    // 打开默认摄像头（编号 0）
    cap.open(0);
    if (!cap.isOpened()) {
        qDebug("Cannot open camera!");
        return;
    }

    // 创建定时器，30 ms 触发一次 ≈ 33 FPS
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onCapture);
    timer->start(16);

    // 居中显示
    ui->piclable->setAlignment(Qt::AlignCenter);

    ui->MaxValueLabel->setText(QString::number(this->max_thresh));
    ui->MinValueLabel->setText(QString::number(this->min_thresh));


    // 模拟数据
    // QLineSeries* series = new QLineSeries();
    // ui->targetCurrentView->chart()->addSeries(series);
    // ui->targetCurrentView->chart()->createDefaultAxes();


    // data init
    kalman_x_series = new QLineSeries();
    kalman_y_series = new QLineSeries();
    pid_out_x_series = new QLineSeries();
    pid_out_y_series = new QLineSeries();


    // add to graph
    ui->targetCurrentView->chart()->addSeries(kalman_x_series);
    ui->targetCurrentView->chart()->addSeries(kalman_y_series);
    ui->targetCurrentView->chart()->addSeries(pid_out_x_series);
    ui->targetCurrentView->chart()->addSeries(pid_out_y_series);


    // create axes
    ui->targetCurrentView->chart()->createDefaultAxes();

    start_time = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();

    QValueAxis* axisX = qobject_cast<QValueAxis*>(ui->targetCurrentView->chart()->axes(Qt::Horizontal).first());
    axisX->setRange(0, 3000);

}


MainWindow::~MainWindow()
{
    timer->stop();
    cap.release();
    delete ui;
}

void rect_dected(cv::Mat frame)
{
    // cv::Mat tmp;
    // cv::findContours(erode_frame, tmp, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    // contours_frame = tmp.size[0] == 2 ? tmp[0] : tmp[1];
    // cv::approxPolyDP(erode_frame, approx_frame, 0.02 * cv::arcLength())
}

void MainWindow::update_list(int kalman_x, int kalman_y, int pid_out_x, int pid_out_y)
{
    QValueAxis* axisX = qobject_cast<QValueAxis*>(ui->targetCurrentView->chart()->axes(Qt::Horizontal).first());
    QValueAxis* axisY = qobject_cast<QValueAxis*>(ui->targetCurrentView->chart()->axes(Qt::Vertical).first());
    static int max_x = kalman_x;
    static int min_x = pid_out_x;

    qint64 now_time = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - start_time;

    this->kalman_x_series->append(now_time, kalman_x);
    this->kalman_y_series->append(now_time, kalman_y);
    this->pid_out_x_series->append(now_time, pid_out_x);
    this->pid_out_y_series->append(now_time, pid_out_y);

    if(kalman_x > max_x || pid_out_x > max_x)
        max_x = MAX(kalman_x, pid_out_x);

    if(kalman_x < min_x || pid_out_x < min_x)
        min_x = MIN(kalman_x, pid_out_x);

    if (axisY) axisY->setRange(min_x, max_x);
    if(axisX && now_time > 3000) axisX->setRange(now_time - 3000, now_time);

    if(this->kalman_x_series->count() > 1000)
    {
        // TODO 崩溃
        this->kalman_x_series->remove(0);
        this->kalman_y_series->remove(0);
        this->pid_out_x_series->remove(0);
        this->pid_out_y_series->remove(0);
    }
    std::cout << "start_time:" << start_time << "\tnow_time" << QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - start_time
              << "\tdata_counter:" << this->kalman_x_series->count() << std::endl;
    this->time_counter++;
}

void MainWindow::check_show()
{
    QValueAxis* axisX = qobject_cast<QValueAxis*>(ui->targetCurrentView->chart()->axes(Qt::Horizontal).first());
    QValueAxis* axisY = qobject_cast<QValueAxis*>(ui->targetCurrentView->chart()->axes(Qt::Vertical).first());
    // 显示不正常
    ui->targetCurrentView->chart()->removeSeries(this->kalman_x_series);
    ui->targetCurrentView->chart()->removeSeries(this->kalman_y_series);
    ui->targetCurrentView->chart()->removeSeries(this->pid_out_x_series);
    ui->targetCurrentView->chart()->removeSeries(this->pid_out_y_series);
    ui->targetCurrentView->chart()->update();

    if(this->show_kalman_x)
        ui->targetCurrentView->chart()->addSeries(this->kalman_x_series);
    if(this->show_kalman_y)
        ui->targetCurrentView->chart()->addSeries(this->kalman_y_series);
    if(this->show_pid_x)
        ui->targetCurrentView->chart()->addSeries(this->pid_out_x_series);
    if(this->show_pid_y)
        ui->targetCurrentView->chart()->addSeries(this->pid_out_y_series);

    kalman_x_series->attachAxis(axisX);
    kalman_x_series->attachAxis(axisY);
    kalman_y_series->attachAxis(axisX);
    kalman_y_series->attachAxis(axisY);
    pid_out_x_series->attachAxis(axisX);
    pid_out_x_series->attachAxis(axisY);
    pid_out_y_series->attachAxis(axisX);
    pid_out_y_series->attachAxis(axisY);

    ui->targetCurrentView->chart()->update();
}

// TODO 分离图像处理和显示线程
void MainWindow::onCapture()
{
    cv::Mat frame;
    cv::Mat gray_frame;
    cv::Mat max_bin_frame;
    cv::Mat min_bin_frame;
    cv::Mat bin_frame;
    cv::Mat edge_frame;
    cv::Mat dilate_frame;
    cv::Mat erode_frame;
    cv::Mat contours_frame;
    cv::Mat approx_frame;
    QImage showImg;

    cap >> frame;
    if (frame.empty()) return;


    this->update_list(gen1.bounded(10, 100), gen2.bounded(50, 80), gen3.bounded(0, 80), gen4.bounded(30, 50));

    // Mat → QImage 灰度
    cv::cvtColor(frame, gray_frame, cv::COLOR_RGB2GRAY);
    QImage img(gray_frame.data,
               gray_frame.cols,
               gray_frame.rows,
               gray_frame.step,
               QImage::Format_Grayscale8);
    this->originImg = img;

    if (this->nowTab == bin)
    {
        cv::threshold(gray_frame, max_bin_frame, this->max_thresh, 255, cv::THRESH_BINARY);
        cv::threshold(gray_frame, min_bin_frame, this->min_thresh, 255, cv::THRESH_BINARY_INV);
        cv::bitwise_or(max_bin_frame, min_bin_frame, bin_frame);
        QImage img_bin(bin_frame.data,
                       bin_frame.cols,
                       bin_frame.rows,
                       bin_frame.step,
                       QImage::Format_Grayscale8);
        this->binImg = img_bin;
    }

    if (this->nowTab == edge || this->nowTab == result)
    {
        cv::Canny(frame, edge_frame, this->max_thresh, this->min_thresh);
        cv::Mat kernel_dilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));
        cv::Mat kernel_erode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));
        cv::dilate(edge_frame, dilate_frame, kernel_dilate);
        cv::erode(dilate_frame, erode_frame, kernel_erode);
        QImage img_edge(erode_frame.data,
                        erode_frame.cols,
                        erode_frame.rows,
                        erode_frame.step,
                        QImage::Format_Grayscale8);
        this->edgeImg = img_edge;
    }

    if(this->nowTab == result)
    {
        cv::Mat tmp;
        // 查找轮廓
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(erode_frame, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (size_t i = 0;i < contours.size(); i++)
        {
            approx_frame = frame;
            double area = cv::contourArea(contours[i]);
            if (area < 100) continue;

            std::vector<cv::Point> approx;
            cv::approxPolyDP(contours[i], approx, 0.02 * cv::arcLength(contours[i], true), true);
            cv::Rect rect = cv::boundingRect(approx);
            int cx = rect.x + rect.width / 2;
            int cy = rect.y + rect.height / 2;

            double aspect_ratio = (double)rect.width / rect.height;

            if (approx.size() == 4 && aspect_ratio > 0.6 && aspect_ratio < 0.9)
            {
                cv::drawContours(approx_frame, contours, i, cv::Scalar(0, 255, 0), 2);
                cv::circle(approx_frame, cv::Point(cx, cy), 5, cv::Scalar(255, 0, 0), -1);
                cv::rectangle(approx_frame, rect, cv::Scalar(0, 0, 255), 2);
            }

            QImage img_approx(approx_frame.data,
                              approx_frame.cols,
                              approx_frame.rows,
                              approx_frame.step,
                              QImage::Format_Grayscale8);
            this->resultImg = img_approx;
        }

        // contours_frame = tmp.size[0] == 2 ? tmp[0] : tmp[1];
        // cv::approxPolyDP(erode_frame, approx_frame, 0.02 * cv::arcLength());
    }

    // TODO 添加图片出错的提示图片

    switch (this->nowTab)
    {
    case origin:
        showImg = this->originImg;
        break;
    case bin:
        showImg = this->binImg;
        break;
    case edge:
        showImg = this->edgeImg;
        break;
    case result:
        showImg = this->resultImg;
        break;
    }

    ui->piclable->setPixmap(
        QPixmap::fromImage(showImg).scaled(ui->piclable->size(),
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation));
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    // 判断当前页，改变需要显示的图像
    this->nowTab = index;
}


void MainWindow::on_MinSlider_sliderMoved(int position)
{
    this->min_thresh = position;
    ui->MinValueLabel->setText(QString::number(position));
}


void MainWindow::on_MaxSlider_sliderMoved(int position)
{
    this->max_thresh = position;
    ui->MaxValueLabel->setText(QString::number(position));
}


void MainWindow::on_MaxSlider_2_sliderMoved(int position)
{
    this->max_thresh = position;
    ui->MaxValueLabel_3->setText(QString::number(position));
}



void MainWindow::on_MinSlider_2_sliderMoved(int position)
{
    this->min_thresh = position;
    ui->MinValueLabel_3->setText(QString::number(position));
}


void MainWindow::on_kalmanXButton_toggled(bool checked)
{
    this->show_kalman_x = checked;
    check_show();
}



void MainWindow::on_kalmanYButton_toggled(bool checked)
{
    this->show_kalman_y = checked;
    check_show();
}


void MainWindow::on_pidXButton_toggled(bool checked)
{
    this->show_pid_x = checked;
    check_show();
}


void MainWindow::on_pidYButton_toggled(bool checked)
{
    this->show_pid_y = checked;
    check_show();
}

