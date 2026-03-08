#include "customchartwidget.h"
#include <QVBoxLayout>
#include <QDebug>

CustomChartWidget::CustomChartWidget(QWidget *parent)
    : QWidget(parent)
{
    // 初始化图表
    m_chart = new QChart();
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    m_chart->setTheme(QChart::ChartThemeLight);

    // 创建图表视图
    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    layout->setContentsMargins(0, 0, 0, 0);

    setupConnections();
}

CustomChartWidget::~CustomChartWidget()
{
    // 清理内存
    clearAllSeries();
    delete m_chartView;
    delete m_chart;
}

void CustomChartWidget::setupConnections()
{
    // 这里可以添加信号槽连接
}

void CustomChartWidget::setChartTitle(const QString &title)
{
    m_chart->setTitle(title);
    m_chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
}

void CustomChartWidget::setXAxisTitle(const QString &title)
{
    if (m_chart->axes(Qt::Horizontal).isEmpty()) {
        QValueAxis *axisX = new QValueAxis;
        axisX->setTitleText(title);
        axisX->setTitleFont(QFont("Arial", 10));
        m_chart->addAxis(axisX, Qt::AlignBottom);
    } else {
        QAbstractAxis *axis = m_chart->axes(Qt::Horizontal).first();
        if (QValueAxis *valueAxis = qobject_cast<QValueAxis*>(axis)) {
            valueAxis->setTitleText(title);
        }
    }
}

void CustomChartWidget::setYAxisTitle(const QString &title)
{
    if (m_chart->axes(Qt::Vertical).isEmpty()) {
        QValueAxis *axisY = new QValueAxis;
        axisY->setTitleText(title);
        axisY->setTitleFont(QFont("Arial", 10));
        m_chart->addAxis(axisY, Qt::AlignLeft);
    } else {
        QAbstractAxis *axis = m_chart->axes(Qt::Vertical).first();
        if (QValueAxis *valueAxis = qobject_cast<QValueAxis*>(axis)) {
            valueAxis->setTitleText(title);
        }
    }
}

void CustomChartWidget::setTheme(QChart::ChartTheme theme)
{
    m_chart->setTheme(theme);
}

void CustomChartWidget::addSeries(const QString &name, const QList<QPointF> &points)
{
    // 如果已存在同名系列，先移除
    if (m_seriesMap.contains(name)) {
        removeSeries(name);
    }

    // 创建新系列
    QLineSeries *series = new QLineSeries();
    series->setName(name);

    // 添加数据点
    for (const QPointF &point : points) {
        series->append(point);
    }

    // 添加到图表
    m_chart->addSeries(series);

    // 连接点击信号
    connect(series, &QLineSeries::clicked, this, &CustomChartWidget::onSeriesClicked);

    // 添加到映射表
    m_seriesMap[name] = series;

    // 关联坐标轴
    if (!m_chart->axes(Qt::Horizontal).isEmpty()) {
        series->attachAxis(m_chart->axes(Qt::Horizontal).first());
    }
    if (!m_chart->axes(Qt::Vertical).isEmpty()) {
        series->attachAxis(m_chart->axes(Qt::Vertical).first());
    }

    // 自动调整坐标轴范围
    m_chart->createDefaultAxes();
}

void CustomChartWidget::clearAllSeries()
{
    for (auto it = m_seriesMap.begin(); it != m_seriesMap.end(); ++it) {
        m_chart->removeSeries(it.value());
        delete it.value();
    }
    m_seriesMap.clear();
}

void CustomChartWidget::removeSeries(const QString &name)
{
    if (m_seriesMap.contains(name)) {
        QLineSeries *series = m_seriesMap.take(name);
        m_chart->removeSeries(series);
        delete series;
    }
}

void CustomChartWidget::setLineColor(const QString &seriesName, const QColor &color)
{
    if (m_seriesMap.contains(seriesName)) {
        QPen pen = m_seriesMap[seriesName]->pen();
        pen.setColor(color);
        pen.setWidth(pen.width());
        m_seriesMap[seriesName]->setPen(pen);
    }
}

void CustomChartWidget::setLineWidth(const QString &seriesName, int width)
{
    if (m_seriesMap.contains(seriesName)) {
        QPen pen = m_seriesMap[seriesName]->pen();
        pen.setWidth(width);
        m_seriesMap[seriesName]->setPen(pen);
    }
}

void CustomChartWidget::setLegendVisible(bool visible)
{
    m_chart->legend()->setVisible(visible);
    if (visible) {
        m_chart->legend()->setAlignment(Qt::AlignBottom);
    }
}

void CustomChartWidget::setAnimationEnabled(bool enabled)
{
    if (enabled) {
        m_chart->setAnimationOptions(QChart::SeriesAnimations);
    } else {
        m_chart->setAnimationOptions(QChart::NoAnimation);
    }
}

void CustomChartWidget::setGridLinesVisible(bool visible)
{
    if (!m_chart->axes(Qt::Horizontal).isEmpty()) {
        if (QValueAxis *axis = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Horizontal).first())) {
            axis->setGridLineVisible(visible);
        }
    }
    if (!m_chart->axes(Qt::Vertical).isEmpty()) {
        if (QValueAxis *axis = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first())) {
            axis->setGridLineVisible(visible);
        }
    }
}

void CustomChartWidget::onSeriesClicked(const QPointF &point)
{
    QLineSeries *series = qobject_cast<QLineSeries*>(sender());
    if (series) {
        emit seriesClicked(series->name(), point);
    }
}
