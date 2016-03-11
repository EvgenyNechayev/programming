//-----------------------------------------------------------------
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>
//-----------------------------------------------------------------
#include <qmath.h>
//-----------------------------------------------------------------
#include "plot.h"
//-----------------------------------------------------------------
plot::plot(QWidget *parent):
    QwtPlot(parent)
{
    setAutoReplot(false);
    setCanvasBackground(QColor(Qt::white));
//-----------------------------------------------------------------
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::gray,  0, Qt::DotLine));
    grid->attach(this);
//-----------------------------------------------------------------
    writeAxisTitle();

    setAxisFont(QwtPlot::xBottom , QFont("Times", 10, QFont::Normal, false));
    setAxisFont(QwtPlot::yLeft, QFont("Times", 10, QFont::Normal, false));

    setAxisMaxMajor(QwtPlot::xBottom, 12);
    setAxisMaxMinor(QwtPlot::xBottom, 5);
//-----------------------------------------------------------------
    curve = new QwtPlotCurve;
    curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve->setPen(QPen(Qt::red));
    curve->setYAxis(QwtPlot::yLeft);
    curve->attach(this);

    setAutoReplot(true);
}
//-----------------------------------------------------------------
void plot::showData(const QVector<double> &xData, const QVector<double> &yData)
{
    curve->setSamples(xData, yData);
}
//-----------------------------------------------------------------
void plot::writeAxisTitle()
{
QwtText axisBottomTitle;
axisBottomTitle.setColor(Qt::black);
axisBottomTitle.setFont(QFont("Times", 14, QFont::Normal, false));
axisBottomTitle.setText(tr("Normalized discrete time"), QwtText::AutoText);
setAxisTitle(QwtPlot::xBottom, axisBottomTitle);

QwtText axisLeftTitle;
axisLeftTitle.setColor(Qt::black);
axisLeftTitle.setFont(QFont("Times", 14, QFont::Normal, false));
axisLeftTitle.setText(tr("Amplitude"), QwtText::AutoText);
setAxisTitle(QwtPlot::yLeft, axisLeftTitle);
}
//-----------------------------------------------------------------
