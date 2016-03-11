//-----------------------------------------------------------------
#ifndef TSMFB_H
#define TSMFB_H
//-----------------------------------------------------------------
#include <QWidget>
#include <QVector>
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QTextCodec>
//-----------------------------------------------------------------
#include "decdetail.h"
#include "recdetail.h"
//-----------------------------------------------------------------
class QGroupBox;
//class QMenu;
//class QAction;
//-----------------------------------------------------------------
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class plot;
class fir_filter;
//-----------------------------------------------------------------

class tsmfb : public QWidget
{
    Q_OBJECT
public:
    explicit tsmfb(QWidget *parent = 0);

signals:

public slots:
    void slotDecButClicked();
    void slotRecButClicked();

private:

    void enableZoomMode(bool on);
    void calcTestInSignal();
    void calcDecInSignal();
    void resizeData();
    void initData();
    QVector<double> downSampData(const QVector<double> &d_i, int intFactor);
    QVector<double> upSampData(const QVector<double> &d_i, int intFactor);
    QVector<double> shiftData(const QVector<double> &d_i, int shiftCount);
    QVector<double> sumData(const QVector<double> &d_i1, const QVector<double> &d_i2);

    QMenuBar         menuBar;
    QMenu           *pMenuFile;
    QAction         *actionExit;

    QGroupBox       *decGroupBox;
    QGroupBox       *recGroupBox;

    plot            *decPlot;
    plot            *recPlot;

    decDetail       *decWin;
    recDetail       *recWin;

    QwtPlotZoomer   *zoomer[2];
    QwtPlotPanner   *panner[2];
    QwtPlotPicker   *picker[2];

    fir_filter      *decLPF_512;
    fir_filter      *decHPF_512;
    fir_filter      *decLPF_256;
    fir_filter      *decHPF_256;
    fir_filter      *decLPF_128;
    fir_filter      *decHPF_128;
    fir_filter      *decLPF_64;
    fir_filter      *decHPF_64;

    fir_filter      *recLPF_64;
    fir_filter      *recHPF_64;
    fir_filter      *recLPF_128;
    fir_filter      *recHPF_128;
    fir_filter      *recLPF_256;
    fir_filter      *recHPF_256;
    fir_filter      *recLPF_512;
    fir_filter      *recHPF_512;

    QVector<double>  t;
    QVector<double>  x;
    // Для анализа
    // Уровень 1
    QVector<double>  x0;
    QVector<double>  x1;
    QVector<double>  v0;
    QVector<double>  v1;
    // Уровень 2
    QVector<double>  x2;
    QVector<double>  x02;
    QVector<double>  x12;
    QVector<double>  v02;
    QVector<double>  v12;
    QVector<double>  v2;
    // Уровень 3
    QVector<double>  x3;
    QVector<double>  x03;
    QVector<double>  x13;
    QVector<double>  v03;
    QVector<double>  v13;
    QVector<double>  v3;
    // Уровень 4
    QVector<double>  x4;
    QVector<double>  x04;
    QVector<double>  x14;
    QVector<double>  v04;
    QVector<double>  v14;
    QVector<double>  v4;
    QVector<double>  v5;
    // Для синтеза
    // Уровень 4
    QVector<double>  w04;
    QVector<double>  w14;
    QVector<double>  y3;
    // Уровень 3
    QVector<double>  w13sh;
    QVector<double>  w13;
    QVector<double>  yu3;
    QVector<double>  y2;
    // Уровень 2
    QVector<double>  w12sh;
    QVector<double>  w12;
    QVector<double>  yu2;
    QVector<double>  y1;
    // Уровень 1
    QVector<double>  w11sh;
    QVector<double>  w11;
    QVector<double>  yu1;
    QVector<double>  y;

};
//-----------------------------------------------------------------
#endif // TSMFB_H
