//-----------------------------------------------------------------
#include <QtGui>
#include <QVector>
//-----------------------------------------------------------------
#include "cool_tsmfb.h"
#include "plot.h"
#include "fir_filter.h"
#include "fir_coef.h"
#include "fir_const.h"
#include "ui_dialog_about.h"
//-----------------------------------------------------------------
#include <qwt_text.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_picker_machine.h>
//-----------------------------------------------------------------
#define minWinWidth  500
#define minWinHeight 300
//-----------------------------------------------------------------
class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas):
        QwtPlotZoomer(xAxis, yAxis, canvas)
    {
        setRubberBand(QwtPicker::RectRubberBand);
        setTrackerMode(QwtPicker::AlwaysOff);
        setRubberBandPen(QPen(Qt::black, 1, Qt::SolidLine));

        setMousePattern(QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlModifier);
        setMousePattern(QwtEventPattern::MouseSelect3,
            Qt::RightButton);
    }
};
//-----------------------------------------------------------------
cool_tsmfb::cool_tsmfb(QWidget *parent) :
    QMainWindow(parent)
{
    pTranslator = new QTranslator();
    pTranslator->load(":/translator/cool_tsmfb_ru.qm", ".");
    qApp->installTranslator(pTranslator);

    setFont(QFont("Times", 12, QFont::Normal, false));
//-----------------------------------------------------------------
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);
//-----------------------------------------------------------------
    lblSch = new lblScheme;
//-----------------------------------------------------------------
    t.resize(NUM_SAMP_512);
    x.resize(NUM_SAMP_512);
    t.fill(0, NUM_SAMP_512);
    x.fill(0, NUM_SAMP_512);
//-----------------------------------------------------------------
    for(int i = 0; i < 16; ++i)
    {
        decDetails[i] = new decDetail();
        if(i <= 14)
            recDetails[i] = new recDetail();
    }
//-----------------------------------------------------------------
    kindFilter = true;
    createActions();
    createMenus();
//-----------------------------------------------------------------
    decGroupBox = new QGroupBox(tr("Decomposition of signal"));
    recGroupBox = new QGroupBox(tr("Reconstruction of signal"));

    decGroupBox->setFont(QFont("Times", 16, QFont::Bold, false));
    recGroupBox->setFont(QFont("Times", 16, QFont::Bold, false));
//-----------------------------------------------------------------
    decPlot = new plot(this);
    recPlot = new plot(this);

    writePlotTitle();
//-----------------------------------------------------------------
    zoomer[0] = new Zoomer(QwtPlot::xBottom, QwtPlot::yLeft,
        decPlot->canvas());

    zoomer[1] = new Zoomer(QwtPlot::xBottom, QwtPlot::yLeft,
        recPlot->canvas());
//-----------------------------------------------------------------
    panner[0] = new QwtPlotPanner(decPlot->canvas());
    panner[0]->setMouseButton(Qt::MidButton);

    panner[1] = new QwtPlotPanner(recPlot->canvas());
    panner[1]->setMouseButton(Qt::MidButton);
//-----------------------------------------------------------------
    picker[0] = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPlotPicker::NoRubberBand, QwtPicker::AlwaysOn,
        decPlot->canvas());
    picker[0]->setStateMachine(new QwtPickerDragPointMachine());
    picker[0]->setTrackerPen(QColor(Qt::black));
    picker[0]->setTrackerFont(QFont("Times", 12, QFont::Normal, true));

    picker[1] = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPlotPicker::NoRubberBand, QwtPicker::AlwaysOn,
        recPlot->canvas());
    picker[1]->setStateMachine(new QwtPickerDragPointMachine());
    picker[1]->setTrackerPen(QColor(Qt::black));
    picker[1]->setTrackerFont(QFont("Times", 12, QFont::Normal, true));
//-----------------------------------------------------------------
    resizeData();
    initData();
//-----------------------------------------------------------------
    for(int i = 0; i < NUM_SAMP_512; ++i)
        t[i] = i;
    x.fill(0, NUM_SAMP_512);
    decPlot->showData(t, x);
    enableZoomModeDec(true);

    y.fill(0, NUM_SAMP_512);
    recPlot->showData(t, y);
    enableZoomModeRec(true);
//-----------------------------------------------------------------
    decLPF_512 = new fir_filter(NUM_SAMP_512, LEN_FILTER_18);
    decHPF_512 = new fir_filter(NUM_SAMP_512, LEN_FILTER_18);
    decLPF_256 = new fir_filter(NUM_SAMP_256, LEN_FILTER_18);
    decHPF_256 = new fir_filter(NUM_SAMP_256, LEN_FILTER_18);
    decLPF_128 = new fir_filter(NUM_SAMP_128, LEN_FILTER_18);
    decHPF_128 = new fir_filter(NUM_SAMP_128, LEN_FILTER_18);
    decLPF_64 = new fir_filter(NUM_SAMP_64, LEN_FILTER_18);
    decHPF_64 = new fir_filter(NUM_SAMP_64, LEN_FILTER_18);

    recLPF_64 = new fir_filter(NUM_SAMP_64, LEN_FILTER_18);
    recHPF_64 = new fir_filter(NUM_SAMP_64, LEN_FILTER_18);
    recLPF_128 = new fir_filter(NUM_SAMP_128, LEN_FILTER_18);
    recHPF_128 = new fir_filter(NUM_SAMP_128, LEN_FILTER_18);
    recLPF_256 = new fir_filter(NUM_SAMP_256, LEN_FILTER_18);
    recHPF_256 = new fir_filter(NUM_SAMP_256, LEN_FILTER_18);
    recLPF_512 = new fir_filter(NUM_SAMP_512, LEN_FILTER_18);
    recHPF_512 = new fir_filter(NUM_SAMP_512, LEN_FILTER_18);
//-----------------------------------------------------------------
    QVBoxLayout *decLayout = new QVBoxLayout;
    decLayout->addWidget(decPlot);
    decGroupBox->setLayout(decLayout);

    QVBoxLayout *recLayout = new QVBoxLayout;
    recLayout->addWidget(recPlot);
    recGroupBox->setLayout(recLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(decGroupBox);
    mainLayout->addWidget(recGroupBox);

    mainWidget->setLayout(mainLayout);
}
//-----------------------------------------------------------------
cool_tsmfb::~cool_tsmfb()
{
}
//-----------------------------------------------------------------
void cool_tsmfb::enableZoomModeDec(bool on)
{
    panner[0]->setEnabled(on);

    zoomer[0]->setZoomBase();
    zoomer[0]->setEnabled(on);
    zoomer[0]->zoom(0);

    picker[0]->setEnabled(on);
}
//-----------------------------------------------------------------
void cool_tsmfb::enableZoomModeRec(bool on)
{
    panner[1]->setEnabled(on);

    zoomer[1]->setZoomBase();
    zoomer[1]->setEnabled(on);
    zoomer[1]->zoom(0);

    picker[1]->setEnabled(on);
}
//-----------------------------------------------------------------
void cool_tsmfb::calcTest1()
{
    initData();
    closeAllComponents();
    for(int i = 0; i < NUM_SAMP_512; ++i)
    {
        if(i >= 21 && i <= 50)
            x[i] = 1;
        else
            x[i] = 0;
    }
    decPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    recPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    decPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    recPlot->setAxisAutoScale(QwtPlot::yLeft, true);

    decPlot->showData(t, x); // сигнал x[n]
    enableZoomModeDec(true);

    y.fill(0, NUM_SAMP_512); // сигнал y[n]
    recPlot->showData(t, y);
    enableZoomModeRec(true);

    checkTest(1);
    actionCalculate->setEnabled(true);
    setEnAnalSint(false);
}
//-----------------------------------------------------------------
void cool_tsmfb::calcTest2()
{
    initData();
    closeAllComponents();
    for(int i = 0; i < NUM_SAMP_512; ++i)
    {
        if(i >= 5 && i <= 23)
            x[i] = 1.013456345;
        else if(i >= 24 && i <= 30)
            x[i] = 3.56542452;
        else if(i >= 31 && i <= 37)
            x[i] = 2.1111454334563;
        else if(i >= 150 && i <= 180)
            x[i] = 2.1111454334563;
        else
            x[i] = 0;
    }
    decPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    recPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    decPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    recPlot->setAxisAutoScale(QwtPlot::yLeft, true);

    decPlot->showData(t, x); // сигнал x[n]
    enableZoomModeDec(true);

    y.fill(0, NUM_SAMP_512); // сигнал y[n]
    recPlot->showData(t, y);
    enableZoomModeRec(true);

    checkTest(2);
    actionCalculate->setEnabled(true);
    setEnAnalSint(false);
}
//-----------------------------------------------------------------
void cool_tsmfb::calcTest3()
{
    initData();
    closeAllComponents();
    for(int i = 0; i < NUM_SAMP_512; ++i)
    {
        x[i] = 2 * sin(3 * 2 * M_PI * i / NUM_SAMP_512);
    }
    decPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    recPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    decPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    recPlot->setAxisAutoScale(QwtPlot::yLeft, true);

    decPlot->showData(t, x); // сигнал x[n]
    enableZoomModeDec(true);

    y.fill(0, NUM_SAMP_512); // сигнал y[n]
    recPlot->showData(t, y);
    enableZoomModeRec(true);

    checkTest(3);
    actionCalculate->setEnabled(true);
    setEnAnalSint(false);
}
//-----------------------------------------------------------------
void cool_tsmfb::calcTest4()
{
    initData();
    closeAllComponents();
    for(int i = 0; i < NUM_SAMP_512; ++i)
    {
        x[i] = 2 * sin(5 * 2 * M_PI * i / NUM_SAMP_512) + \
                0.2 * cos(100 * 2 * M_PI * i / NUM_SAMP_512);
    }
    decPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    recPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    decPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    recPlot->setAxisAutoScale(QwtPlot::yLeft, true);

    decPlot->showData(t, x); // сигнал x[n]
    enableZoomModeDec(true);

    y.fill(0, NUM_SAMP_512); // сигнал y[n]
    recPlot->showData(t, y);
    enableZoomModeRec(true);

    checkTest(4);
    actionCalculate->setEnabled(true);
    setEnAnalSint(false);
}
//-----------------------------------------------------------------
void cool_tsmfb::calcTest5()
{
    initData();
    closeAllComponents();
    for(int i = 0; i < NUM_SAMP_512; ++i)
    {
        if(i <= 150)
            x[i] = 2 * sin(10 * 2 * M_PI * i / NUM_SAMP_512);
        else if(i > 150 && i <= 300)
            x[i] = 2 * cos(5 * 2 * M_PI * i / NUM_SAMP_512);
        else if(i > 300 && i <= 450)
            x[i] = 2 * sin((3 * 2 * M_PI * i / NUM_SAMP_512) + M_PI);
        else
            x[i] = 2 * sin(10 * 2 * M_PI * i / NUM_SAMP_512);
    }
    decPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    recPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    decPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    recPlot->setAxisAutoScale(QwtPlot::yLeft, true);

    decPlot->showData(t, x); // сигнал x[n]
    enableZoomModeDec(true);

    y.fill(0, NUM_SAMP_512); // сигнал y[n]
    recPlot->showData(t, y);
    enableZoomModeRec(true);

    checkTest(5);
    actionCalculate->setEnabled(true);
    setEnAnalSint(false);
}
//-----------------------------------------------------------------
void cool_tsmfb::calcTest6()
{
    initData();
    closeAllComponents();
    for(int i = 0; i < NUM_SAMP_512; ++i)
    {
        x[i] = 2 * sin((double) i / NUM_SAMP_32 * 2 * M_PI * i / NUM_SAMP_512);
    }
    decPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    recPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    decPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    recPlot->setAxisAutoScale(QwtPlot::yLeft, true);

    decPlot->showData(t, x); // сигнал x[n]
    enableZoomModeDec(true);

    y.fill(0, NUM_SAMP_512); // сигнал y[n]
    recPlot->showData(t, y);
    enableZoomModeRec(true);

    checkTest(6);
    actionCalculate->setEnabled(true);
    setEnAnalSint(false);
}
//-----------------------------------------------------------------
void cool_tsmfb::chooseFIR()
{
    initData();
    closeAllComponents();

    decPlot->showData(t, x); // сигнал x[n]
    enableZoomModeDec(true);

    y.fill(0, NUM_SAMP_512); // сигнал y[n]
    recPlot->showData(t, y);
    enableZoomModeRec(true);

    kindFilter = 0;
    checkFilter(1);
}
//-----------------------------------------------------------------
void cool_tsmfb::chooseDaubechies()
{
    initData();
    closeAllComponents();

    decPlot->showData(t, x); // сигнал x[n]
    enableZoomModeDec(true);

    y.fill(0, NUM_SAMP_512); // сигнал y[n]
    recPlot->showData(t, y);
    enableZoomModeRec(true);

    kindFilter = 1;
    checkFilter(2);
}
//-----------------------------------------------------------------
QVector<double> cool_tsmfb::downSampData(const QVector<double> &d_i, int intFactor)
{
    QVector<double> d_o(d_i.size() / intFactor);

    for(int i = 0, j = 0; i < d_i.size(); i += intFactor, ++j)
        d_o[j] = d_i[i];

    return d_o;
}
//-----------------------------------------------------------------
QVector<double> cool_tsmfb::upSampData(const QVector<double> &d_i, int intFactor)
{
    int j = 0;
    int tempFactor = intFactor;
    QVector<double> d_o(d_i.size() * intFactor);

    for(int i = 0; i < d_i.size(); ++i)
    {
        d_o[j++] = d_i[i];
        while(tempFactor-- > 1)
            d_o[j++] = 0;
        tempFactor = intFactor;
    }

    return d_o;
}
//-----------------------------------------------------------------
QVector<double> cool_tsmfb::shiftData(const QVector<double> &d_i, int shiftCount)
{
    QVector<double> d_o(d_i.size());

    for(int i = 0; i < d_i.size(); ++i)
        (i < shiftCount) ? d_o[i] = 0 : d_o[i] = d_i[i - shiftCount];

    return d_o;
}
//-----------------------------------------------------------------
QVector<double> cool_tsmfb::sumData(const QVector<double> &d_i1, const QVector<double> &d_i2)
{
    QVector<double> d_o(d_i1.size());

    for(int i = 0; i < d_i1.size(); ++i)
        d_o[i] = d_i1[i] + d_i2[i];

    return d_o;
}
//-----------------------------------------------------------------
void cool_tsmfb::calcDecRecInSignal()
{
    initData();
    closeAllComponents();
    // Декомпозиция сигнала (Анализ)
    // Уровень 1
    decLPF_512->setData(x);
    (kindFilter) ? decLPF_512->setCoef(decLPF_D) : decLPF_512->setCoef(decLPF_F);
    decLPF_512->filtData();             // Фильтрация ФНЧ
    x0 = decLPF_512->getFiltData();
    decHPF_512->setData(x);
    (kindFilter) ? decHPF_512->setCoef(decHPF_D) : decHPF_512->setCoef(decHPF_F);
    decHPF_512->filtData();             // Фильтрация ФВЧ
    x1 = decHPF_512->getFiltData();
    v0 = downSampData(x0, 2);           // Децимация, компонент v_0[n]
    v1 = downSampData(x1, 2);           // Децимация, компонент v_1[n]
    // Уровень 2
    x2 = v0; // Выход ФНЧ 1-ого уровня ко входу QMF 2-ого уровня
    decLPF_256->setData(x2);
    (kindFilter) ? decLPF_256->setCoef(decLPF_D) : decLPF_256->setCoef(decLPF_F);
    decLPF_256->filtData();             // Фильтрация ФНЧ
    x02 = decLPF_256->getFiltData();
    decHPF_256->setData(x2);
    (kindFilter) ? decHPF_256->setCoef(decHPF_D) : decHPF_256->setCoef(decHPF_F);
    decHPF_256->filtData();             // Фильтрация ФВЧ
    x12 = decHPF_256->getFiltData();
    v02 = downSampData(x02, 2);         // Децимация
    v12 = downSampData(x12, 2);         // Децимация
    v2 = v12;                           // Компонент v_2[n]
    // Уровень 3
    x3 = v02; // Выход ФНЧ 2-ого уровня ко входу QMF 3-ого уровня
    decLPF_128->setData(x3);
    (kindFilter) ? decLPF_128->setCoef(decLPF_D) : decLPF_128->setCoef(decLPF_F);
    decLPF_128->filtData();             // Фильтрация ФНЧ
    x03 = decLPF_128->getFiltData();
    decHPF_128->setData(x3);
    (kindFilter) ? decHPF_128->setCoef(decHPF_D) : decHPF_128->setCoef(decHPF_F);
    decHPF_128->filtData();             // Фильтрация ФВЧ
    x13 = decHPF_128->getFiltData();
    v03 = downSampData(x03, 2);         // Децимация
    v13 = downSampData(x13, 2);         // Децимация
    v3 = v13;                           // Компонент v_3[n]
    // Уровень 4
    x4 = v03; // Выход ФНЧ 3-ого уровня ко входу QMF 4-ого уровня
    decLPF_64->setData(x4);
    (kindFilter) ? decLPF_64->setCoef(decLPF_D) : decLPF_64->setCoef(decLPF_F);
    decLPF_64->filtData();             // Фильтрация ФНЧ
    x04 = decLPF_64->getFiltData();
    decHPF_64->setData(x4);
    (kindFilter) ? decHPF_64->setCoef(decHPF_D) : decHPF_64->setCoef(decHPF_F);
    decHPF_64->filtData();             // Фильтрация ФВЧ
    x14 = decHPF_64->getFiltData();
    v04 = downSampData(x04, 2);         // Децимация
    v14 = downSampData(x14, 2);         // Децимация
    v4 = v14;                           // Компонент v_4[n]
    v5 = v04;                           // Компонент v_5[n]
    // Реконструкция сигнала (Синтез)
    // Уровень 4
    w04 = upSampData(v5, 2);            // Интерполяция
    w14 = upSampData(v4, 2);            // Интерполяция
    recLPF_64->setData(w04);
    (kindFilter) ? recLPF_64->setCoef(recLPF_D) : recLPF_64->setCoef(recLPF_F);
    recLPF_64->filtData();              // Фильтрация ФНЧ
    recHPF_64->setData(w14);
    (kindFilter) ? recHPF_64->setCoef(recHPF_D) : recHPF_64->setCoef(recHPF_F);
    recHPF_64->filtData();              // Фильтрация ФВЧ
    y3 = sumData(recLPF_64->getFiltData(), recHPF_64->getFiltData()); // сигнал y_3[n]
    // Уровень 3
    w13sh = shiftData(v3, (LEN_FILTER_18 - 1));
    w13 = upSampData(w13sh, 2);         // Интерполяция
    yu3 = upSampData(y3, 2);            // Интерполяция
    recLPF_128->setData(yu3);
    (kindFilter) ? recLPF_128->setCoef(recLPF_D) : recLPF_128->setCoef(recLPF_F);
    recLPF_128->filtData();             // Фильтрация ФНЧ
    recHPF_128->setData(w13);
    (kindFilter) ? recHPF_128->setCoef(recHPF_D) : recHPF_128->setCoef(recHPF_F);
    recHPF_128->filtData();             // Фильтрация ФВЧ
    y2 = sumData(recLPF_128->getFiltData(), recHPF_128->getFiltData()); // сигнал y_2[n]
    // Уровень 2
    w12sh = shiftData(v2, 3 * (LEN_FILTER_18 - 1));
    w12 = upSampData(w12sh, 2);         // Интерполяция
    yu2 = upSampData(y2, 2);            // Интерполяция
    recLPF_256->setData(yu2);
    (kindFilter) ? recLPF_256->setCoef(recLPF_D) : recLPF_256->setCoef(recLPF_F);
    recLPF_256->filtData();             // Фильтрация ФНЧ
    recHPF_256->setData(w12);
    (kindFilter) ? recHPF_256->setCoef(recHPF_D) : recHPF_256->setCoef(recHPF_F);
    recHPF_256->filtData();             // Фильтрация ФВЧ
    y1 = sumData(recLPF_256->getFiltData(), recHPF_256->getFiltData()); // сигнал y_1[n]
    // Уровень 1
    w11sh = shiftData(v1, 7 * (LEN_FILTER_18 - 1));
    w11 = upSampData(w11sh, 2);         // Интерполяция
    yu1 = upSampData(y1, 2);            // Интерполяция
    recLPF_512->setData(yu1);
    (kindFilter) ? recLPF_512->setCoef(recLPF_D) : recLPF_512->setCoef(recLPF_F);
    recLPF_512->filtData();             // Фильтрация ФНЧ
    recHPF_512->setData(w11);
    (kindFilter) ? recHPF_512->setCoef(recHPF_D) : recHPF_512->setCoef(recHPF_F);
    recHPF_512->filtData();             // Фильтрация ФВЧ
    y = sumData(recLPF_512->getFiltData(), recHPF_512->getFiltData()); // сигнал y[n]

    decPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    recPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    decPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    recPlot->setAxisAutoScale(QwtPlot::yLeft, true);

    recPlot->showData(t, y);
    enableZoomModeRec(true);

    setEnAnalSint(true);
}
//-----------------------------------------------------------------
void cool_tsmfb::setEnAnalSint(bool on)
{
    actionX0->setEnabled(on);
    actionX1->setEnabled(on);
    actionV0->setEnabled(on);
    actionV1->setEnabled(on);
    actionX02->setEnabled(on);
    actionX12->setEnabled(on);
    actionV02->setEnabled(on);
    actionV2->setEnabled(on);
    actionX03->setEnabled(on);
    actionX13->setEnabled(on);
    actionV03->setEnabled(on);
    actionV3->setEnabled(on);
    actionX04->setEnabled(on);
    actionX14->setEnabled(on);
    actionV4->setEnabled(on);
    actionV5->setEnabled(on);

    actionW04->setEnabled(on);
    actionW14->setEnabled(on);
    actionY3->setEnabled(on);
    actionYu3->setEnabled(on);
    actionW13sh->setEnabled(on);
    actionW13->setEnabled(on);
    actionY2->setEnabled(on);
    actionYu2->setEnabled(on);
    actionW12sh->setEnabled(on);
    actionW12->setEnabled(on);
    actionY1->setEnabled(on);
    actionYu1->setEnabled(on);
    actionW11sh->setEnabled(on);
    actionW11->setEnabled(on);
}
//-----------------------------------------------------------------
void cool_tsmfb::resizeData()
{
    // Для анализа
    // Уровень 1
    x0.resize(NUM_SAMP_512);
    x1.resize(NUM_SAMP_512);
    v0.resize(NUM_SAMP_256);
    v1.resize(NUM_SAMP_256);
    // Уровень 2
    x2.resize(NUM_SAMP_256);
    x02.resize(NUM_SAMP_256);
    x12.resize(NUM_SAMP_256);
    v02.resize(NUM_SAMP_128);
    v12.resize(NUM_SAMP_128);
    v2.resize(NUM_SAMP_128);
    // Уровень 3
    x3.resize(NUM_SAMP_128);
    x03.resize(NUM_SAMP_128);
    x13.resize(NUM_SAMP_128);
    v03.resize(NUM_SAMP_64);
    v13.resize(NUM_SAMP_64);
    v3.resize(NUM_SAMP_64);
    // Уровень 4
    x4.resize(NUM_SAMP_64);
    x04.resize(NUM_SAMP_64);
    x14.resize(NUM_SAMP_64);
    v04.resize(NUM_SAMP_32);
    v14.resize(NUM_SAMP_32);
    v4.resize(NUM_SAMP_32);
    v5.resize(NUM_SAMP_32);
    // Для синтеза
    // Уровень 4
    w04.resize(NUM_SAMP_64);
    w14.resize(NUM_SAMP_64);
    y3.resize(NUM_SAMP_64);
    // Уровень 3
    w13sh.resize(NUM_SAMP_64);
    w13.resize(NUM_SAMP_128);
    yu3.resize(NUM_SAMP_128);
    y2.resize(NUM_SAMP_128);
    // Уровень 2
    w13sh.resize(NUM_SAMP_128);
    w12.resize(NUM_SAMP_256);
    yu2.resize(NUM_SAMP_256);
    y1.resize(NUM_SAMP_256);
    // Уровень 1
    w13sh.resize(NUM_SAMP_256);
    w11.resize(NUM_SAMP_512);
    yu1.resize(NUM_SAMP_512);
    y.resize(NUM_SAMP_512);
}
//-----------------------------------------------------------------
void cool_tsmfb::initData()
{
    // Для анализа
    // Уровень 1
    x0.fill(0, NUM_SAMP_512);
    x1.fill(0, NUM_SAMP_512);
    v0.fill(0, NUM_SAMP_256);
    v1.fill(0, NUM_SAMP_256);
    // Уровень 2
    x2.fill(0, NUM_SAMP_256);
    x02.fill(0, NUM_SAMP_256);
    x12.fill(0, NUM_SAMP_256);
    v02.fill(0, NUM_SAMP_128);
    v12.fill(0, NUM_SAMP_128);
    v2.fill(0, NUM_SAMP_128);
    // Уровень 3
    x3.fill(0, NUM_SAMP_128);
    x03.fill(0, NUM_SAMP_128);
    x13.fill(0, NUM_SAMP_128);
    v03.fill(0, NUM_SAMP_64);
    v13.fill(0, NUM_SAMP_64);
    v3.fill(0, NUM_SAMP_64);
    // Уровень 4
    x4.fill(0, NUM_SAMP_64);
    x04.fill(0, NUM_SAMP_64);
    x14.fill(0, NUM_SAMP_64);
    v04.fill(0, NUM_SAMP_32);
    v14.fill(0, NUM_SAMP_32);
    v4.fill(0, NUM_SAMP_32);
    v5.fill(0, NUM_SAMP_32);
    // Для синтеза
    // Уровень 4
    w04.fill(0, NUM_SAMP_64);
    w14.fill(0, NUM_SAMP_64);
    y3.fill(0, NUM_SAMP_64);
    // Уровень 3
    w13sh.fill(0, NUM_SAMP_64);
    w13.fill(0, NUM_SAMP_128);
    yu3.fill(0, NUM_SAMP_128);
    y2.fill(0, NUM_SAMP_128);
    // Уровень 2
    w13sh.fill(0, NUM_SAMP_128);
    w12.fill(0, NUM_SAMP_256);
    yu2.fill(0, NUM_SAMP_256);
    y1.fill(0, NUM_SAMP_256);
    // Уровень 1
    w13sh.fill(0, NUM_SAMP_256);
    w11.fill(0, NUM_SAMP_512);
    yu1.fill(0, NUM_SAMP_512);
    y.fill(0, NUM_SAMP_512);
}
//-----------------------------------------------------------------
void cool_tsmfb::createActions()
{
    actionTest1 = new QAction(tr("Test&1"), this);
    actionTest1->setShortcut(tr("Ctrl+1"));
    actionTest1->setCheckable(true);
    connect(actionTest1, SIGNAL(triggered()), this, SLOT(calcTest1()));
    actionTest2 = new QAction(tr("Test&2"), this);
    actionTest2->setShortcut(tr("Ctrl+2"));
    actionTest2->setCheckable(true);
    connect(actionTest2, SIGNAL(triggered()), this, SLOT(calcTest2()));
    actionTest3 = new QAction(tr("Test&3"), this);
    actionTest3->setShortcut(tr("Ctrl+3"));
    actionTest3->setCheckable(true);
    connect(actionTest3, SIGNAL(triggered()), this, SLOT(calcTest3()));
    actionTest4 = new QAction(tr("Test&4"), this);
    actionTest4->setShortcut(tr("Ctrl+4"));
    actionTest4->setCheckable(true);
    connect(actionTest4, SIGNAL(triggered()), this, SLOT(calcTest4()));
    actionTest5 = new QAction(tr("Test&5"), this);
    actionTest5->setShortcut(tr("Ctrl+5"));
    actionTest5->setCheckable(true);
    connect(actionTest5, SIGNAL(triggered()), this, SLOT(calcTest5()));
    actionTest6 = new QAction(tr("Test&6"), this);
    actionTest6->setShortcut(tr("Ctrl+6"));
    actionTest6->setCheckable(true);
    connect(actionTest6, SIGNAL(triggered()), this, SLOT(calcTest6()));
    actionFIR = new QAction(tr("&Minimum-phase halfband FIR filter"), this);
    actionFIR->setCheckable(true);
    connect(actionFIR, SIGNAL(triggered()), this, SLOT(chooseFIR()));
    actionDaubechies = new QAction(tr("&Daubechies db9 filter"), this);
    actionDaubechies->setCheckable(true);
    connect(actionDaubechies, SIGNAL(triggered()), this, SLOT(chooseDaubechies()));
    (kindFilter) ? actionDaubechies->setChecked(true) : actionFIR->setChecked(true);
    actionCalculate = new QAction(tr("&Calculate"), this);
    actionCalculate->setShortcut(tr("Ctrl+R"));
    actionCalculate->setIcon(QIcon(":/images/calculator_16_16.png"));
    actionCalculate->setEnabled(false);
    connect(actionCalculate, SIGNAL(triggered()), this, SLOT(calcDecRecInSignal()));

    actionExit = new QAction(tr("&Exit"), this);
    actionExit->setShortcut(tr("Ctrl+Q"));
    actionExit->setIcon(QIcon(":/images/exit_16_16.png"));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    actionX0 = new QAction(tr("Component x0(n)"), this);
    actionX0->setCheckable(true);
    connect(actionX0, SIGNAL(triggered()), this, SLOT(showComponentX0()));
    connect(decDetails[0], SIGNAL(closeComponent()), this, SLOT(noCheckComponentX0()));
    actionX1 = new QAction(tr("Component x1(n)"), this);
    actionX1->setCheckable(true);
    connect(actionX1, SIGNAL(triggered()), this, SLOT(showComponentX1()));
    connect(decDetails[1], SIGNAL(closeComponent()), this, SLOT(noCheckComponentX1()));
    actionV0 = new QAction(tr("Component v0(n)"), this);
    actionV0->setCheckable(true);
    connect(actionV0, SIGNAL(triggered()), this, SLOT(showComponentV0()));
    connect(decDetails[2], SIGNAL(closeComponent()), this, SLOT(noCheckComponentV0()));
    actionV1 = new QAction(tr("Component v1(n)"), this);
    actionV1->setCheckable(true);
    connect(actionV1, SIGNAL(triggered()), this, SLOT(showComponentV1()));
    connect(decDetails[3], SIGNAL(closeComponent()), this, SLOT(noCheckComponentV1()));
    actionX02 = new QAction(tr("Component x02(n)"), this);
    actionX02->setCheckable(true);
    connect(actionX02, SIGNAL(triggered()), this, SLOT(showComponentX02()));
    connect(decDetails[4], SIGNAL(closeComponent()), this, SLOT(noCheckComponentX02()));
    actionX12 = new QAction(tr("Component x12(n)"), this);
    actionX12->setCheckable(true);
    connect(actionX12, SIGNAL(triggered()), this, SLOT(showComponentX12()));
    connect(decDetails[5], SIGNAL(closeComponent()), this, SLOT(noCheckComponentX12()));
    actionV02 = new QAction(tr("Component v02(n)"), this);
    actionV02->setCheckable(true);
    connect(actionV02, SIGNAL(triggered()), this, SLOT(showComponentV02()));
    connect(decDetails[6], SIGNAL(closeComponent()), this, SLOT(noCheckComponentV02()));
    actionV2 = new QAction(tr("Component v2(n)"), this);
    actionV2->setCheckable(true);
    actionV2->setEnabled(false);
    connect(actionV2, SIGNAL(triggered()), this, SLOT(showComponentV2()));
    connect(decDetails[7], SIGNAL(closeComponent()), this, SLOT(noCheckComponentV2()));
    actionX03 = new QAction(tr("Component x03(n)"), this);
    actionX03->setCheckable(true);
    connect(actionX03, SIGNAL(triggered()), this, SLOT(showComponentX03()));
    connect(decDetails[8], SIGNAL(closeComponent()), this, SLOT(noCheckComponentX03()));
    actionX13 = new QAction(tr("Component x13(n)"), this);
    actionX13->setCheckable(true);
    connect(actionX13, SIGNAL(triggered()), this, SLOT(showComponentX13()));
    connect(decDetails[9], SIGNAL(closeComponent()), this, SLOT(noCheckComponentX13()));
    actionV03 = new QAction(tr("Component v03(n)"), this);
    actionV03->setCheckable(true);
    connect(actionV03, SIGNAL(triggered()), this, SLOT(showComponentV03()));
    connect(decDetails[10], SIGNAL(closeComponent()), this, SLOT(noCheckComponentV03()));
    actionV3 = new QAction(tr("Component v3(n)"), this);
    actionV3->setCheckable(true);
    connect(actionV3, SIGNAL(triggered()), this, SLOT(showComponentV3()));
    connect(decDetails[11], SIGNAL(closeComponent()), this, SLOT(noCheckComponentV3()));
    actionX04 = new QAction(tr("Component x04(n)"), this);
    actionX04->setCheckable(true);
    connect(actionX04, SIGNAL(triggered()), this, SLOT(showComponentX04()));
    connect(decDetails[12], SIGNAL(closeComponent()), this, SLOT(noCheckComponentX04()));
    actionX14 = new QAction(tr("Component x14(n)"), this);
    actionX14->setCheckable(true);
    connect(actionX14, SIGNAL(triggered()), this, SLOT(showComponentX14()));
    connect(decDetails[13], SIGNAL(closeComponent()), this, SLOT(noCheckComponentX14()));
    actionV4 = new QAction(tr("Component v4(n)"), this);
    actionV4->setCheckable(true);
    connect(actionV4, SIGNAL(triggered()), this, SLOT(showComponentV4()));
    connect(decDetails[14], SIGNAL(closeComponent()), this, SLOT(noCheckComponentV4()));
    actionV5 = new QAction(tr("Component v5(n)"), this);
    actionV5->setCheckable(true);
    connect(actionV5, SIGNAL(triggered()), this, SLOT(showComponentV5()));
    connect(decDetails[15], SIGNAL(closeComponent()), this, SLOT(noCheckComponentV5()));

    actionW04 = new QAction(tr("Component w04(n)"), this);
    actionW04->setCheckable(true);
    connect(actionW04, SIGNAL(triggered()), this, SLOT(showComponentW04()));
    connect(recDetails[0], SIGNAL(closeComponent()), this, SLOT(noCheckComponentW04()));
    actionW14 = new QAction(tr("Component w14(n)"), this);
    actionW14->setCheckable(true);
    connect(actionW14, SIGNAL(triggered()), this, SLOT(showComponentW14()));
    connect(recDetails[1], SIGNAL(closeComponent()), this, SLOT(noCheckComponentW14()));
    actionY3 = new QAction(tr("Component y3(n)"), this);
    actionY3->setCheckable(true);
    connect(actionY3, SIGNAL(triggered()), this, SLOT(showComponentY3()));
    connect(recDetails[2], SIGNAL(closeComponent()), this, SLOT(noCheckComponentY3()));
    actionYu3 = new QAction(tr("Component yu3(n)"), this);
    actionYu3->setCheckable(true);
    connect(actionYu3, SIGNAL(triggered()), this, SLOT(showComponentYu3()));
    connect(recDetails[3], SIGNAL(closeComponent()), this, SLOT(noCheckComponentYu3()));
    actionW13sh = new QAction(tr("Component w13sh(n)"), this);
    actionW13sh->setCheckable(true);
    connect(actionW13sh, SIGNAL(triggered()), this, SLOT(showComponentW13sh()));
    connect(recDetails[4], SIGNAL(closeComponent()), this, SLOT(noCheckComponentW13sh()));
    actionW13 = new QAction(tr("Component w13(n)"), this);
    actionW13->setCheckable(true);
    connect(actionW13, SIGNAL(triggered()), this, SLOT(showComponentW13()));
    connect(recDetails[5], SIGNAL(closeComponent()), this, SLOT(noCheckComponentW13()));
    actionY2 = new QAction(tr("Component y2(n)"), this);
    actionY2->setCheckable(true);
    connect(actionY2, SIGNAL(triggered()), this, SLOT(showComponentY2()));
    connect(recDetails[6], SIGNAL(closeComponent()), this, SLOT(noCheckComponentY2()));
    actionYu2 = new QAction(tr("Component yu2(n)"), this);
    actionYu2->setCheckable(true);
    connect(actionYu2, SIGNAL(triggered()), this, SLOT(showComponentYu2()));
    connect(recDetails[7], SIGNAL(closeComponent()), this, SLOT(noCheckComponentYu2()));
    actionW12sh = new QAction(tr("Component w12sh(n)"), this);
    actionW12sh->setCheckable(true);
    connect(actionW12sh, SIGNAL(triggered()), this, SLOT(showComponentW12sh()));
    connect(recDetails[8], SIGNAL(closeComponent()), this, SLOT(noCheckComponentW12sh()));
    actionW12 = new QAction(tr("Component w12(n)"), this);
    actionW12->setCheckable(true);
    connect(actionW12, SIGNAL(triggered()), this, SLOT(showComponentW12()));
    connect(recDetails[9], SIGNAL(closeComponent()), this, SLOT(noCheckComponentW12()));
    actionY1 = new QAction(tr("Component y1(n)"), this);
    actionY1->setCheckable(true);
    connect(actionY1, SIGNAL(triggered()), this, SLOT(showComponentY1()));
    connect(recDetails[10], SIGNAL(closeComponent()), this, SLOT(noCheckComponentY1()));
    actionYu1 = new QAction(tr("Component yu1(n)"), this);
    actionYu1->setCheckable(true);
    connect(actionYu1, SIGNAL(triggered()), this, SLOT(showComponentYu1()));
    connect(recDetails[11], SIGNAL(closeComponent()), this, SLOT(noCheckComponentYu1()));
    actionW11sh = new QAction(tr("Component w11sh(n)"), this);
    actionW11sh->setCheckable(true);
    connect(actionW11sh, SIGNAL(triggered()), this, SLOT(showComponentW11sh()));
    connect(recDetails[12], SIGNAL(closeComponent()), this, SLOT(noCheckComponentW11sh()));
    actionW11 = new QAction(tr("Component w11(n)"), this);
    actionW11->setCheckable(true);
    connect(actionW11, SIGNAL(triggered()), this, SLOT(showComponentW11()));
    connect(recDetails[13], SIGNAL(closeComponent()), this, SLOT(noCheckComponentW11()));
    setEnAnalSint(false);

    actionShowScheme = new QAction(tr("&Show scheme"), this);
    actionShowScheme->setShortcut(tr("Ctrl+V"));
    actionShowScheme->setCheckable(true);
    connect(actionShowScheme, SIGNAL(triggered()), this, SLOT(showScheme()));
    connect(lblSch, SIGNAL(closeScheme()), this, SLOT(noCheckScheme()));

    actionRus = new QAction(tr("Russian"), this);
    actionRus->setIcon(QIcon(":/images/russian_federation.png"));
    actionRus->setFont(QFont("Times", 12, QFont::Bold, false));
    actionEn = new QAction(tr("American"), this);
    actionEn->setIcon(QIcon(":/images/united_states.png"));
    actionEn->setFont(QFont("Times", 10, QFont::Normal, false));

    actionLangGroup = new QActionGroup(this);
    actionLangGroup->addAction(actionRus);
    actionLangGroup->addAction(actionEn);
    connect(actionLangGroup, SIGNAL(triggered(QAction *)),
        this, SLOT(switchLanguage(QAction *)));

    actionAbout = new QAction(tr("A&bout TSMBF..."), this);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
}
//-----------------------------------------------------------------
void cool_tsmfb::createMenus()
{
    pMenuActions = menuBar()->addMenu(tr("&Actions"));
    pMenuChooseTest = pMenuActions->addMenu(tr("Test signal"));
    pMenuChooseTest->setTearOffEnabled(true);
    pMenuChooseTest->addAction(actionTest1);
    pMenuChooseTest->addAction(actionTest2);
    pMenuChooseTest->addAction(actionTest3);
    pMenuChooseTest->addAction(actionTest4);
    pMenuChooseTest->addAction(actionTest5);
    pMenuChooseTest->addAction(actionTest6);
    pMenuKindFilter = pMenuActions->addMenu(tr("Filter bank"));
    pMenuKindFilter->addAction(actionFIR);
    pMenuKindFilter->addAction(actionDaubechies);
    pMenuActions->addAction(actionCalculate);
    pMenuActions->addSeparator();
    pMenuActions->addAction(actionExit);

    pMenuAnalysis = menuBar()->addMenu(tr("A&nalysis"));
    pMenuAnalysis->setTearOffEnabled(true);
    pMenuAnalysis->addAction(actionX0);
    pMenuAnalysis->addAction(actionX1);
    pMenuAnalysis->addAction(actionV0);
    pMenuAnalysis->addAction(actionV1);
    pMenuAnalysis->addAction(actionX02);
    pMenuAnalysis->addAction(actionX12);
    pMenuAnalysis->addAction(actionV02);
    pMenuAnalysis->addAction(actionV2);
    pMenuAnalysis->addAction(actionX03);
    pMenuAnalysis->addAction(actionX13);
    pMenuAnalysis->addAction(actionV03);
    pMenuAnalysis->addAction(actionV3);
    pMenuAnalysis->addAction(actionX04);
    pMenuAnalysis->addAction(actionX14);
    pMenuAnalysis->addAction(actionV4);
    pMenuAnalysis->addAction(actionV5);

    pMenuSynthesis = menuBar()->addMenu(tr("&Synthesis"));
    pMenuSynthesis->setTearOffEnabled(true);
    pMenuSynthesis->addAction(actionW04);
    pMenuSynthesis->addAction(actionW14);
    pMenuSynthesis->addAction(actionY3);
    pMenuSynthesis->addAction(actionYu3);
    pMenuSynthesis->addAction(actionW13sh);
    pMenuSynthesis->addAction(actionW13);
    pMenuSynthesis->addAction(actionY2);
    pMenuSynthesis->addAction(actionYu2);
    pMenuSynthesis->addAction(actionW12sh);
    pMenuSynthesis->addAction(actionW12);
    pMenuSynthesis->addAction(actionY1);
    pMenuSynthesis->addAction(actionYu1);
    pMenuSynthesis->addAction(actionW11sh);
    pMenuSynthesis->addAction(actionW11);

    pMenuScheme = menuBar()->addMenu(tr("S&cheme"));
    pMenuScheme->addAction(actionShowScheme);

    pMenuTranslator = menuBar()->addMenu(tr("Language"));
    pMenuTranslator->addAction(actionRus);
    pMenuTranslator->addAction(actionEn);

    pMenuHelp = menuBar()->addMenu(tr("&Help"));
    pMenuHelp->addAction(actionAbout);
}
//-----------------------------------------------------------------
void cool_tsmfb::showDecComponent(int n)
{
    if(decDetails[n]->isShowFlag())
    {
        decDetails[n]->setMinimumSize(minWinWidth, minWinHeight);

        QRect rect = QApplication::desktop()->availableGeometry(decDetails[n]);
        decDetails[n]->move(rect.center() - decDetails[n]->rect().center());

        decDetails[n]->resetShowFlag();
        switch(n)
        {
        case 0:
            decDetails[n]->showComponent(t, x0, QString(tr("Component x0(n)")));
            break;
        case 1:
            decDetails[n]->showComponent(t, x1, QString(tr("Component x1(n)")));
            break;
        case 2:
            decDetails[n]->showComponent(t, v0, QString(tr("Component v0(n)")));
            break;
        case 3:
            decDetails[n]->showComponent(t, v1, QString(tr("Component v1(n)")));
            break;
        case 4:
            decDetails[n]->showComponent(t, x02, QString(tr("Component x02(n)")));
            break;
        case 5:
            decDetails[n]->showComponent(t, x12, QString(tr("Component x12(n)")));
            break;
        case 6:
            decDetails[n]->showComponent(t, v02, QString(tr("Component v02(n)")));
            break;
        case 7:
            decDetails[n]->showComponent(t, v2, QString(tr("Component v2(n)")));
            break;
        case 8:
            decDetails[n]->showComponent(t, x03, QString(tr("Component x03(n)")));
            break;
        case 9:
            decDetails[n]->showComponent(t, x13, QString(tr("Component x13(n)")));
            break;
        case 10:
            decDetails[n]->showComponent(t, v03, QString(tr("Component v03(n)")));
            break;
        case 11:
            decDetails[n]->showComponent(t, v3, QString(tr("Component v3(n)")));
            break;
        case 12:
            decDetails[n]->showComponent(t, x04, QString(tr("Component x04(n)")));
            break;
        case 13:
            decDetails[n]->showComponent(t, x14, QString(tr("Component x14(n)")));
            break;
        case 14:
            decDetails[n]->showComponent(t, v4, QString(tr("Component v4(n)")));
            break;
        case 15:
            decDetails[n]->showComponent(t, v5, QString(tr("Component v5(n)")));
            break;
        default:
            break;
        }
        decDetails[n]->show();
    }
    else
    {
        decDetails[n]->close();
    }
}
//-----------------------------------------------------------------
void cool_tsmfb::showRecComponent(int n)
{
    if(recDetails[n]->isShowFlag())
    {
        recDetails[n]->setMinimumSize(minWinWidth, minWinHeight);

        QRect rect = QApplication::desktop()->availableGeometry(recDetails[n]);
        recDetails[n]->move(rect.center() - recDetails[n]->rect().center());

        recDetails[n]->resetShowFlag();
        switch(n)
        {
        case 0:
            recDetails[n]->showComponent(t, w04, QString(tr("Component w04(n)")));
            break;
        case 1:
            recDetails[n]->showComponent(t, w14, QString(tr("Component w14(n)")));
            break;
        case 2:
            recDetails[n]->showComponent(t, y3, QString(tr("Component y3(n)")));
            break;
        case 3:
            recDetails[n]->showComponent(t, yu3, QString(tr("Component yu3(n)")));
            break;
        case 4:
            recDetails[n]->showComponent(t, w13sh, QString(tr("Component w13sh(n)")));
            break;
        case 5:
            recDetails[n]->showComponent(t, w13, QString(tr("Component w13(n)")));
            break;
        case 6:
            recDetails[n]->showComponent(t, y2, QString(tr("Component y2(n)")));
            break;
        case 7:
            recDetails[n]->showComponent(t, yu2, QString(tr("Component yu2(n)")));
            break;
        case 8:
            recDetails[n]->showComponent(t, w12sh, QString(tr("Component w12sh(n)")));
            break;
        case 9:
            recDetails[n]->showComponent(t, w12, QString(tr("Component w12(n)")));
            break;
        case 10:
            recDetails[n]->showComponent(t, y1, QString(tr("Component y1(n)")));
            break;
        case 11:
            recDetails[n]->showComponent(t, yu1, QString(tr("Component yu1(n)")));
            break;
        case 12:
            recDetails[n]->showComponent(t, w11sh, QString(tr("Component w11sh(n)")));
            break;
        case 13:
            recDetails[n]->showComponent(t, w11, QString(tr("Component w11(n)")));
            break;
        default:
            break;
        }
        recDetails[n]->show();
    }
    else
    {
        recDetails[n]->close();
    }
}
//-----------------------------------------------------------------
void cool_tsmfb::showComponentX0()
{
    showDecComponent(0);
}
void cool_tsmfb::showComponentX1()
{
    showDecComponent(1);
}
void cool_tsmfb::showComponentV0()
{
    showDecComponent(2);
}
void cool_tsmfb::showComponentV1()
{
    showDecComponent(3);
}
void cool_tsmfb::showComponentX02()
{
    showDecComponent(4);
}
void cool_tsmfb::showComponentX12()
{
    showDecComponent(5);
}
void cool_tsmfb::showComponentV02()
{
    showDecComponent(6);
}
void cool_tsmfb::showComponentV2()
{
    showDecComponent(7);
}
void cool_tsmfb::showComponentX03()
{
    showDecComponent(8);
}
void cool_tsmfb::showComponentX13()
{
    showDecComponent(9);
}
void cool_tsmfb::showComponentV03()
{
    showDecComponent(10);
}
void cool_tsmfb::showComponentV3()
{
    showDecComponent(11);
}
void cool_tsmfb::showComponentX04()
{
    showDecComponent(12);
}
void cool_tsmfb::showComponentX14()
{
    showDecComponent(13);
}
void cool_tsmfb::showComponentV4()
{
    showDecComponent(14);
}
void cool_tsmfb::showComponentV5()
{
    showDecComponent(15);
}
void cool_tsmfb::showComponentW04()
{
    showRecComponent(0);
}
void cool_tsmfb::showComponentW14()
{
    showRecComponent(1);
}
void cool_tsmfb::showComponentY3()
{
    showRecComponent(2);
}
void cool_tsmfb::showComponentYu3()
{
    showRecComponent(3);
}
void cool_tsmfb::showComponentW13sh()
{
    showRecComponent(4);
}
void cool_tsmfb::showComponentW13()
{
    showRecComponent(5);
}
void cool_tsmfb::showComponentY2()
{
    showRecComponent(6);
}
void cool_tsmfb::showComponentYu2()
{
    showRecComponent(7);
}
void cool_tsmfb::showComponentW12sh()
{
    showRecComponent(8);
}
void cool_tsmfb::showComponentW12()
{
    showRecComponent(9);
}
void cool_tsmfb::showComponentY1()
{
    showRecComponent(10);
}
void cool_tsmfb::showComponentYu1()
{
    showRecComponent(11);
}
void cool_tsmfb::showComponentW11sh()
{
    showRecComponent(12);
}
void cool_tsmfb::showComponentW11()
{
    showRecComponent(13);
}
//-----------------------------------------------------------------
void cool_tsmfb::noCheckComponentX0()
{
    actionX0->setChecked(false);
}
void cool_tsmfb::noCheckComponentX1()
{
    actionX1->setChecked(false);
}
void cool_tsmfb::noCheckComponentV0()
{
    actionV0->setChecked(false);
}
void cool_tsmfb::noCheckComponentV1()
{
    actionV1->setChecked(false);
}
void cool_tsmfb::noCheckComponentX02()
{
    actionX02->setChecked(false);
}
void cool_tsmfb::noCheckComponentX12()
{
    actionX12->setChecked(false);
}
void cool_tsmfb::noCheckComponentV02()
{
    actionV02->setChecked(false);
}
void cool_tsmfb::noCheckComponentV2()
{
    actionV2->setChecked(false);
}
void cool_tsmfb::noCheckComponentX03()
{
    actionX03->setChecked(false);
}
void cool_tsmfb::noCheckComponentX13()
{
    actionX13->setChecked(false);
}
void cool_tsmfb::noCheckComponentV03()
{
    actionV03->setChecked(false);
}
void cool_tsmfb::noCheckComponentV3()
{
    actionV3->setChecked(false);
}
void cool_tsmfb::noCheckComponentX04()
{
    actionX04->setChecked(false);
}
void cool_tsmfb::noCheckComponentX14()
{
    actionX14->setChecked(false);
}
void cool_tsmfb::noCheckComponentV4()
{
    actionV4->setChecked(false);
}
void cool_tsmfb::noCheckComponentV5()
{
    actionV5->setChecked(false);
}
void cool_tsmfb::noCheckComponentW04()
{
    actionW04->setChecked(false);
}
void cool_tsmfb::noCheckComponentW14()
{
    actionW14->setChecked(false);
}
void cool_tsmfb::noCheckComponentY3()
{
    actionY3->setChecked(false);
}
void cool_tsmfb::noCheckComponentYu3()
{
    actionYu3->setChecked(false);
}
void cool_tsmfb::noCheckComponentW13sh()
{
    actionW13sh->setChecked(false);
}
void cool_tsmfb::noCheckComponentW13()
{
    actionW13->setChecked(false);
}
void cool_tsmfb::noCheckComponentY2()
{
    actionY2->setChecked(false);
}
void cool_tsmfb::noCheckComponentYu2()
{
    actionYu2->setChecked(false);
}
void cool_tsmfb::noCheckComponentW12sh()
{
    actionW12sh->setChecked(false);
}
void cool_tsmfb::noCheckComponentW12()
{
    actionW12->setChecked(false);
}
void cool_tsmfb::noCheckComponentY1()
{
    actionY1->setChecked(false);
}
void cool_tsmfb::noCheckComponentYu1()
{
    actionYu1->setChecked(false);
}
void cool_tsmfb::noCheckComponentW11sh()
{
    actionW11sh->setChecked(false);
}
void cool_tsmfb::noCheckComponentW11()
{
    actionW11->setChecked(false);
}
//-----------------------------------------------------------------
void cool_tsmfb::showScheme()
{
    QPixmap pixScheme(":/images/Scheme.png");
    if(lblSch->isShowFlag())
    {
        lblSch->setFixedSize(pixScheme.size());
        lblSch->setPixmap(pixScheme);

        QRect rect = QApplication::desktop()->availableGeometry(lblSch);
        lblSch->move(rect.center() - lblSch->rect().center());

        lblSch->resetShowFlag();
        lblSch->show();
    }
    else
    {
        lblSch->close();
    }
}
//-----------------------------------------------------------------
void cool_tsmfb::noCheckScheme()
{
    actionShowScheme->setChecked(false);
}
//-----------------------------------------------------------------
void cool_tsmfb::exit()
{
    emit quit();
}
//-----------------------------------------------------------------
void cool_tsmfb::closeEvent(QCloseEvent *)
{
    emit quit();
}
//-----------------------------------------------------------------
void cool_tsmfb::checkTest(int n)
{
    actionTest1->setChecked(false);
    actionTest2->setChecked(false);
    actionTest3->setChecked(false);
    actionTest4->setChecked(false);
    actionTest5->setChecked(false);
    actionTest6->setChecked(false);

    switch(n)
    {
    case 1: actionTest1->setChecked(true);
        break;
    case 2: actionTest2->setChecked(true);
        break;
    case 3: actionTest3->setChecked(true);
        break;
    case 4: actionTest4->setChecked(true);
        break;
    case 5: actionTest5->setChecked(true);
        break;
    case 6: actionTest6->setChecked(true);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------
void cool_tsmfb::checkFilter(int n)
{
    actionFIR->setChecked(false);
    actionDaubechies->setChecked(false);

    switch(n)
    {
    case 1: actionFIR->setChecked(true);
        break;
    case 2: actionDaubechies->setChecked(true);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------
void cool_tsmfb::showAbout()
{
    Ui::dialogAbout ui;
    QDialog *dialog = new QDialog;
    dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    dialog->setModal(true);
    ui.setupUi(dialog);
    dialog->show();
}
//-----------------------------------------------------------------
void cool_tsmfb::switchLanguage(QAction *lang)
{
    if(lang->text() == QString(tr("Russian")))
    {
        pTranslator->load(":/translator/cool_tsmfb_ru.qm", ".");
        qApp->installTranslator(pTranslator);
        actionRus->setFont(QFont("Times", 12, QFont::Bold, false));
        actionEn->setFont(QFont("Times", 10, QFont::Normal, false));
    }
    else
    {
        qApp->removeTranslator(pTranslator);
        actionEn->setFont(QFont("Times", 12, QFont::Bold, false));
        actionRus->setFont(QFont("Times", 10, QFont::Normal, false));
    }
    retranslate();
}
//-----------------------------------------------------------------
void cool_tsmfb::writePlotTitle()
{
    QwtText decPlotTitle;
    decPlotTitle.setColor(Qt::black);
    decPlotTitle.setFont(QFont("Times", 16, QFont::Normal, false));
    decPlotTitle.setText(tr("Test signal x(n)"), QwtText::AutoText);

    QwtText recPlotTitle;
    recPlotTitle.setColor(Qt::black);
    recPlotTitle.setFont(QFont("Times", 14, QFont::Normal, false));
    recPlotTitle.setText(tr("Reconstructed signal y(n)"), QwtText::AutoText);

    decPlot->setTitle(decPlotTitle);
    recPlot->setTitle(recPlotTitle);
}
//-----------------------------------------------------------------
void cool_tsmfb::retranslate()
{
    for(int i = 0; i < 16; ++i)
    {
        decDetails[i]->decPlotDetail->writeAxisTitle();
        if(i <= 14)
            recDetails[i]->recPlotDetail->writeAxisTitle();
    }

    writePlotTitle();
    decPlot->writeAxisTitle();
    recPlot->writeAxisTitle();

    decGroupBox->setTitle(tr("Decomposition of signal"));
    recGroupBox->setTitle(tr("Reconstruction of signal"));

    actionTest1->setText(tr("Test&1"));
    actionTest1->setShortcut(tr("Ctrl+1"));
    actionTest2->setText(tr("Test&2"));
    actionTest2->setShortcut(tr("Ctrl+2"));
    actionTest3->setText(tr("Test&3"));
    actionTest3->setShortcut(tr("Ctrl+3"));
    actionTest4->setText(tr("Test&4"));
    actionTest4->setShortcut(tr("Ctrl+4"));
    actionTest5->setText(tr("Test&5"));
    actionTest5->setShortcut(tr("Ctrl+5"));
    actionTest6->setText(tr("Test&6"));
    actionTest6->setShortcut(tr("Ctrl+6"));
    actionFIR->setText(tr("&Minimum-phase halfband FIR filter"));
    actionDaubechies->setText(tr("&Daubechies db9 filter"));
    actionCalculate->setText(tr("&Calculate"));
    actionCalculate->setShortcut(tr("Ctrl+R"));

    actionExit->setText(tr("&Exit"));
    actionExit->setShortcut(tr("Ctrl+Q"));

    actionX0->setText(tr("Component x0(n)"));
    actionX1->setText(tr("Component x1(n)"));
    actionV0->setText(tr("Component v0(n)"));
    actionV1->setText(tr("Component v1(n)"));
    actionX02->setText(tr("Component x02(n)"));
    actionX12->setText(tr("Component x12(n)"));
    actionV02->setText(tr("Component v02(n)"));
    actionV2->setText(tr("Component v2(n)"));
    actionX03->setText(tr("Component x03(n)"));
    actionX13->setText(tr("Component x13(n)"));
    actionV03->setText(tr("Component v03(n)"));
    actionV3->setText(tr("Component v3(n)"));
    actionX04->setText(tr("Component x04(n)"));
    actionX14->setText(tr("Component x14(n)"));
    actionV4->setText(tr("Component v4(n)"));
    actionV5->setText(tr("Component v5(n)"));

    actionW04->setText(tr("Component w04(n)"));
    actionW14->setText(tr("Component w14(n)"));
    actionY3->setText(tr("Component y3(n)"));
    actionYu3->setText(tr("Component yu3(n)"));
    actionW13sh->setText(tr("Component w13sh(n)"));
    actionW13->setText(tr("Component w13(n)"));
    actionY2->setText(tr("Component y2(n)"));
    actionYu2->setText(tr("Component yu2(n)"));
    actionW12sh->setText(tr("Component w12sh(n)"));
    actionW12->setText(tr("Component w12(n)"));
    actionY1->setText(tr("Component y1(n)"));
    actionYu1->setText(tr("Component yu1(n)"));
    actionW11sh->setText(tr("Component w11sh(n)"));
    actionW11->setText(tr("Component w11(n)"));

    actionShowScheme->setText(tr("&Show scheme"));

    actionRus->setText(tr("Russian"));
    actionEn->setText(tr("American"));

    actionAbout->setText(tr("A&bout TSMBF"));

    pMenuActions->setTitle(tr("&Actions"));
    pMenuChooseTest->setTitle(tr("Test signal"));
    pMenuKindFilter->setTitle(tr("Filter bank"));
    pMenuAnalysis->setTitle(tr("A&nalysis"));
    pMenuSynthesis->setTitle(tr("&Synthesis"));
    pMenuScheme->setTitle(tr("S&cheme"));
    pMenuTranslator->setTitle(tr("Language"));
    pMenuHelp->setTitle(tr("&Help"));

    decDetails[0]->setPlotTitle(QString(tr("Component x0(n)")));
    decDetails[1]->setPlotTitle(QString(tr("Component x1(n)")));
    decDetails[2]->setPlotTitle(QString(tr("Component v0(n)")));
    decDetails[3]->setPlotTitle(QString(tr("Component v1(n)")));
    decDetails[4]->setPlotTitle(QString(tr("Component x02(n)")));
    decDetails[5]->setPlotTitle(QString(tr("Component x12(n)")));
    decDetails[6]->setPlotTitle(QString(tr("Component v02(n)")));
    decDetails[7]->setPlotTitle(QString(tr("Component v2(n)")));
    decDetails[8]->setPlotTitle(QString(tr("Component x03(n)")));
    decDetails[9]->setPlotTitle(QString(tr("Component x13(n)")));
    decDetails[10]->setPlotTitle(QString(tr("Component v03(n)")));
    decDetails[11]->setPlotTitle(QString(tr("Component v3(n)")));
    decDetails[12]->setPlotTitle(QString(tr("Component x04(n)")));
    decDetails[13]->setPlotTitle(QString(tr("Component x14(n)")));
    decDetails[14]->setPlotTitle(QString(tr("Component v4(n)")));
    decDetails[15]->setPlotTitle(QString(tr("Component v5(n)")));

    recDetails[0]->setPlotTitle(QString(tr("Component w04(n)")));
    recDetails[1]->setPlotTitle(QString(tr("Component w14(n)")));
    recDetails[2]->setPlotTitle(QString(tr("Component y3(n)")));
    recDetails[3]->setPlotTitle(QString(tr("Component yu3(n)")));
    recDetails[4]->setPlotTitle(QString(tr("Component w13sh(n)")));
    recDetails[5]->setPlotTitle(QString(tr("Component w13(n)")));
    recDetails[6]->setPlotTitle(QString(tr("Component y2(n)")));
    recDetails[7]->setPlotTitle(QString(tr("Component yu2(n)")));
    recDetails[8]->setPlotTitle(QString(tr("Component w12sh(n)")));
    recDetails[9]->setPlotTitle(QString(tr("Component w12(n)")));
    recDetails[10]->setPlotTitle(QString(tr("Component y1(n)")));
    recDetails[11]->setPlotTitle(QString(tr("Component yu1(n)")));
    recDetails[12]->setPlotTitle(QString(tr("Component w11sh(n)")));
    recDetails[13]->setPlotTitle(QString(tr("Component w11(n)")));
}
//-----------------------------------------------------------------
void cool_tsmfb::closeAllComponents()
{
    for(int i = 0; i < 16; ++i)
    {
        if(!decDetails[i]->isShowFlag())
            decDetails[i]->close();
        if(i <= 14)
            if(!recDetails[i]->isShowFlag())
                recDetails[i]->close();
    }
}
//-----------------------------------------------------------------
