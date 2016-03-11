//-----------------------------------------------------------------
#ifndef PLOT_H
#define PLOT_H
//-----------------------------------------------------------------
#include <qwt_plot.h>
//-----------------------------------------------------------------
class QwtPlotCurve;
//-----------------------------------------------------------------

class plot: public QwtPlot
{
    Q_OBJECT
public:
    plot(QWidget *parent);

    void showData(const QVector<double> &xData, const QVector<double> &yData);
    void writeAxisTitle();

signals:

public slots:


private:
    QwtPlotCurve    *curve;
};
//-----------------------------------------------------------------
#endif // PLOT_H
