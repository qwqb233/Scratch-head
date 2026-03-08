#ifndef CUSTOMCHARTWIDGET_H
#define CUSTOMCHARTWIDGET_H

#include <QWidget>
#include <QtCharts>

// QT_CHARTS_USE_NAMESPACE

    class CustomChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomChartWidget(QWidget *parent = nullptr);
    ~CustomChartWidget();

    // 公共接口方法
    void setChartTitle(const QString &title);
    void setXAxisTitle(const QString &title);
    void setYAxisTitle(const QString &title);
    void setTheme(QChart::ChartTheme theme);

    // 数据操作
    void addSeries(const QString &name, const QList<QPointF> &points);
    void clearAllSeries();
    void removeSeries(const QString &name);

    // 样式设置
    void setLineColor(const QString &seriesName, const QColor &color);
    void setLineWidth(const QString &seriesName, int width);

    // 显示控制
    void setLegendVisible(bool visible);
    void setAnimationEnabled(bool enabled);
    void setGridLinesVisible(bool visible);

signals:
    void seriesClicked(const QString &seriesName, const QPointF &point);

private slots:
    void onSeriesClicked(const QPointF &point);

private:
    void setupUI();
    void setupConnections();

    QChart *m_chart;
    QChartView *m_chartView;
    QHash<QString, QLineSeries*> m_seriesMap;
};

#endif // CUSTOMCHARTWIDGET_H
