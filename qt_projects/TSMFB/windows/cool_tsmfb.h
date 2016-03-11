#ifndef COOL_TSMFB_H
#define COOL_TSMFB_H
//-----------------------------------------------------------------
#include <QMainWindow>
#include <QVector>
#include <QWidget>
//-----------------------------------------------------------------
#include "dec_detail.h"
#include "rec_detail.h"
#include "lbl_scheme.h"
//-----------------------------------------------------------------
class QGroupBox;
class QLabel;
class QTranslator;
class QActionGroup;
//-----------------------------------------------------------------
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class plot;
class fir_filter;
//-----------------------------------------------------------------

class cool_tsmfb : public QMainWindow
{
    Q_OBJECT
public:
    explicit cool_tsmfb(QWidget *parent = 0);
    ~cool_tsmfb();

signals:
    void quit();

public slots:
    void createActions();
    void createMenus();
    void calcTest1();
    void calcTest2();
    void calcTest3();
    void calcTest4();
    void calcTest5();
    void calcTest6();
    void chooseFIR();
    void chooseDaubechies();
    void calcDecRecInSignal();
//-----------------------------------------------------------------
    void showComponentX0();
    void showComponentX1();
    void showComponentV0();
    void showComponentV1();
    void showComponentX02();
    void showComponentX12();
    void showComponentV02();
    void showComponentV2();
    void showComponentX03();
    void showComponentX13();
    void showComponentV03();
    void showComponentV3();
    void showComponentX04();
    void showComponentX14();
    void showComponentV4();
    void showComponentV5();
//-----------------------------------------------------------------
    void showComponentW04();
    void showComponentW14();
    void showComponentY3();
    void showComponentYu3();
    void showComponentW13sh();
    void showComponentW13();
    void showComponentY2();
    void showComponentYu2();
    void showComponentW12sh();
    void showComponentW12();
    void showComponentY1();
    void showComponentYu1();
    void showComponentW11sh();
    void showComponentW11();
//-----------------------------------------------------------------
    void noCheckComponentX0();
    void noCheckComponentX1();
    void noCheckComponentV0();
    void noCheckComponentV1();
    void noCheckComponentX02();
    void noCheckComponentX12();
    void noCheckComponentV02();
    void noCheckComponentV2();
    void noCheckComponentX03();
    void noCheckComponentX13();
    void noCheckComponentV03();
    void noCheckComponentV3();
    void noCheckComponentX04();
    void noCheckComponentX14();
    void noCheckComponentV4();
    void noCheckComponentV5();
//-----------------------------------------------------------------
    void noCheckComponentW04();
    void noCheckComponentW14();
    void noCheckComponentY3();
    void noCheckComponentYu3();
    void noCheckComponentW13sh();
    void noCheckComponentW13();
    void noCheckComponentY2();
    void noCheckComponentYu2();
    void noCheckComponentW12sh();
    void noCheckComponentW12();
    void noCheckComponentY1();
    void noCheckComponentYu1();
    void noCheckComponentW11sh();
    void noCheckComponentW11();
//-----------------------------------------------------------------
    void showScheme();
    void noCheckScheme();
    void switchLanguage(QAction *);
    void showAbout();
    void exit();

protected:
    virtual void closeEvent(QCloseEvent *);

private:
    void enableZoomModeDec(bool on);
    void enableZoomModeRec(bool on);
    void resizeData();
    void initData();
    void checkTest(int n);
    void checkFilter(int n);
    void setEnAnalSint(bool on);
    void showDecComponent(int n);
    void showRecComponent(int n);
    void retranslate();
    void writePlotTitle();
    void closeAllComponents();
//-----------------------------------------------------------------
    QVector<double> downSampData(const QVector<double> &d_i, int intFactor);
    QVector<double> upSampData(const QVector<double> &d_i, int intFactor);
    QVector<double> shiftData(const QVector<double> &d_i, int shiftCount);
    QVector<double> sumData(const QVector<double> &d_i1, const QVector<double> &d_i2);

    bool             kindFilter;
    QTranslator     *pTranslator;
//-----------------------------------------------------------------
    QMenu           *pMenuActions;
    QMenu           *pMenuChooseTest;
    QMenu           *pMenuKindFilter;
    QMenu           *pMenuAnalysis;
    QMenu           *pMenuSynthesis;
    QMenu           *pMenuScheme;
    QMenu           *pMenuTranslator;
    QMenu           *pMenuHelp;
//-----------------------------------------------------------------
    QAction         *actionTest1;
    QAction         *actionTest2;
    QAction         *actionTest3;
    QAction         *actionTest4;
    QAction         *actionTest5;
    QAction         *actionTest6;
    QAction         *actionFIR;
    QAction         *actionDaubechies;
    QAction         *actionCalculate;
    QAction         *actionExit;
//-----------------------------------------------------------------
    QAction         *actionX0;
    QAction         *actionX1;
    QAction         *actionV0;
    QAction         *actionV1;
    QAction         *actionX02;
    QAction         *actionX12;
    QAction         *actionV02;
    QAction         *actionV2;
    QAction         *actionX03;
    QAction         *actionX13;
    QAction         *actionV03;
    QAction         *actionV3;
    QAction         *actionX04;
    QAction         *actionX14;
    QAction         *actionV4;
    QAction         *actionV5;
//-----------------------------------------------------------------
    QAction         *actionW04;
    QAction         *actionW14;
    QAction         *actionY3;
    QAction         *actionYu3;
    QAction         *actionW13sh;
    QAction         *actionW13;
    QAction         *actionY2;
    QAction         *actionYu2;
    QAction         *actionW12sh;
    QAction         *actionW12;
    QAction         *actionY1;
    QAction         *actionYu1;
    QAction         *actionW11sh;
    QAction         *actionW11;
//-----------------------------------------------------------------
    QAction         *actionShowScheme;
//-----------------------------------------------------------------
    QActionGroup    *actionLangGroup;
    QAction         *actionRus;
    QAction         *actionEn;
//-----------------------------------------------------------------
    QAction         *actionAbout;
//-----------------------------------------------------------------
    QWidget         *mainWidget;
//-----------------------------------------------------------------
    decDetail       *decDetails[16];
    recDetail       *recDetails[15];
//-----------------------------------------------------------------
    QGroupBox       *decGroupBox;
    QGroupBox       *recGroupBox;
//-----------------------------------------------------------------
    lblScheme       *lblSch;
//-----------------------------------------------------------------
    plot            *decPlot;
    plot            *recPlot;
//-----------------------------------------------------------------
    QwtPlotZoomer   *zoomer[2];
    QwtPlotPanner   *panner[2];
    QwtPlotPicker   *picker[2];
//-----------------------------------------------------------------
    fir_filter      *decLPF_512;
    fir_filter      *decHPF_512;
    fir_filter      *decLPF_256;
    fir_filter      *decHPF_256;
    fir_filter      *decLPF_128;
    fir_filter      *decHPF_128;
    fir_filter      *decLPF_64;
    fir_filter      *decHPF_64;
//-----------------------------------------------------------------
    fir_filter      *recLPF_64;
    fir_filter      *recHPF_64;
    fir_filter      *recLPF_128;
    fir_filter      *recHPF_128;
    fir_filter      *recLPF_256;
    fir_filter      *recHPF_256;
    fir_filter      *recLPF_512;
    fir_filter      *recHPF_512;
//-----------------------------------------------------------------
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
#endif // COOL_TSMFB_H
