//-----------------------------------------------------------------
#include <QtGui>
#include <QVector>
//-----------------------------------------------------------------
#include "tsmfb.h"
#include "plot.h"
#include "fir_filter.h"
#include "fir_coef.h"
#include "fir_const.h"
//-----------------------------------------------------------------
#include <qwt_text.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_div.h>
//-----------------------------------------------------------------
#define minWinWidth  400
#define minWinHeight 600
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
tsmfb::tsmfb(QWidget *parent) :
    QWidget(parent)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForTr(codec);
//-----------------------------------------------------------------
    t.resize(NUM_SAMP_512);
    x.resize(NUM_SAMP_512);
    t.fill(0, NUM_SAMP_512);
    x.fill(0, NUM_SAMP_512);
//-----------------------------------------------------------------
    pMenuFile = new QMenu("&File");

    actionExit = new QAction(tr("&Exit"), this);
    actionExit->setShortcut(tr("Ctrl+Q"));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(quit()));
    pMenuFile->addAction(actionExit);
    menuBar.addMenu(pMenuFile);
    menuBar.show();
//-----------------------------------------------------------------
    decWin = new decDetail;
    recWin = new recDetail;

    decWin->setWindowTitle(tr("Detailed decomposition of signal"));
    recWin->setWindowTitle(tr("Detailed reconstruction of signal"));

    decWin->setMinimumSize(minWinWidth, minWinHeight);
    recWin->setMinimumSize(minWinWidth, minWinHeight);

    decWin->setGeometry(100, 100, minWinWidth, minWinHeight);
    recWin->setGeometry(100, 100, minWinWidth, minWinHeight);
//    decWin->setWindowModality(
//-----------------------------------------------------------------
    decGroupBox = new QGroupBox(tr("Decomposition of signal"));
    recGroupBox = new QGroupBox(tr("Reconstruction of signal"));

    decGroupBox->setFont(QFont("Times", 16, QFont::Normal, true));
    recGroupBox->setFont(QFont("Times", 16, QFont::Normal, true));
//-----------------------------------------------------------------
    decPlot = new plot (this);
    recPlot = new plot (this);

    QwtText decPlotTitle;
    decPlotTitle.setColor(Qt::black);
    decPlotTitle.setFont(QFont("Times", 16, QFont::Bold, false));
    decPlotTitle.setText(tr("Test signal"), QwtText::AutoText);

    QwtText recPlotTitle;
    recPlotTitle.setColor(Qt::black);
    recPlotTitle.setFont(QFont("Times", 16, QFont::Bold, false));
    recPlotTitle.setText(tr("Reconstructed signal"), QwtText::AutoText);

    decPlot->setTitle(decPlotTitle);
    recPlot->setTitle(recPlotTitle);

    decPlot->setAxisScale(QwtPlot::xBottom, 0, NUM_SAMP_512);
    recPlot->setAxisScale(QwtPlot::xBottom, 0, NUM_SAMP_512);
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
    calcTestInSignal();
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
    calcDecInSignal();

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

    setLayout(mainLayout);
}
//-----------------------------------------------------------------
void tsmfb::enableZoomMode(bool on)
{
    panner[0]->setEnabled(on);
    panner[1]->setEnabled(on);

    zoomer[0]->setZoomBase();
    zoomer[0]->setEnabled(on);
    zoomer[0]->zoom(0);

    zoomer[1]->setZoomBase();
    zoomer[1]->setEnabled(on);
    zoomer[1]->zoom(0);

    picker[0]->setEnabled(on);
    picker[1]->setEnabled(on);
}
//-----------------------------------------------------------------
void tsmfb::slotDecButClicked()
{
    decWin->hide();
    decWin->show();
}
//-----------------------------------------------------------------
void tsmfb::slotRecButClicked()
{
    recWin->hide();
    recWin->show();
}
//-----------------------------------------------------------------
void tsmfb::calcTestInSignal()
{
    for(int i = 0; i < NUM_SAMP_512; ++i)
    {
        t[i] = i;

//        if(i >= 21 && i <= 50)
//            x[i] = 1;
//        else
//            x[i] = 0;

//        if(i >= 0 && i <= 23)
//            x[i] = 1.013456345;
//        else if(i >= 24 && i <= 30)
//            x[i] = 3.56542452;
//        else if(i >= 31 && i <= 37)
//            x[i] = 2.1111454334563;
//        else
//            x[i] = 0;

        x[i] = 2 * sin(5 * 2 * M_PI * i / NUM_SAMP_512) + \
                0.5 * cos(50 * 2 * M_PI * i / NUM_SAMP_512);

//        x[i] = 2 * sin(3 * 2 * M_PI * i / NUM_SAMP_512);
    }
}
//-----------------------------------------------------------------
QVector<double> tsmfb::downSampData(const QVector<double> &d_i, int intFactor)
{
    QVector<double> d_o(d_i.size() / intFactor);

    for(int i = 0, j = 0; i < d_i.size(); i += intFactor, ++j)
        d_o[j] = d_i[i];

    return d_o;
}
//-----------------------------------------------------------------
QVector<double> tsmfb::upSampData(const QVector<double> &d_i, int intFactor)
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
QVector<double> tsmfb::shiftData(const QVector<double> &d_i, int shiftCount)
{
    QVector<double> d_o(d_i.size());

    for(int i = 0; i < d_i.size(); ++i)
        (i < shiftCount) ? d_o[i] = 0 : d_o[i] = d_i[i - shiftCount];

    return d_o;
}
//-----------------------------------------------------------------
QVector<double> tsmfb::sumData(const QVector<double> &d_i1, const QVector<double> &d_i2)
{
    QVector<double> d_o(d_i1.size());

    for(int i = 0; i < d_i1.size(); ++i)
        d_o[i] = d_i1[i] + d_i2[i];

    return d_o;
}
//-----------------------------------------------------------------
void tsmfb::calcDecInSignal()
{
    initData();
    // Декомпозиция сигнала (Анализ)
    // Уровень 1
    decLPF_512->setData(x);
    decLPF_512->setCoef(decLPF);
    decLPF_512->filtData();             // Фильтрация ФНЧ
    x0 = decLPF_512->getFiltData();
    decHPF_512->setData(x);
    decHPF_512->setCoef(decHPF);
    decHPF_512->filtData();             // Фильтрация ФВЧ
    x1 = decHPF_512->getFiltData();
    v0 = downSampData(x0, 2);           // Децимация, компонент v_0[n]
    v1 = downSampData(x1, 2);           // Децимация, компонент v_1[n]
    // Уровень 2
    x2 = v0; // Выход ФНЧ 1-ого уровня ко входу QMF 2-ого уровня
    decLPF_256->setData(x2);
    decLPF_256->setCoef(decLPF);
    decLPF_256->filtData();             // Фильтрация ФНЧ
    x02 = decLPF_256->getFiltData();
    decHPF_256->setData(x2);
    decHPF_256->setCoef(decHPF);
    decHPF_256->filtData();             // Фильтрация ФВЧ
    x12 = decHPF_256->getFiltData();
    v02 = downSampData(x02, 2);         // Децимация
    v12 = downSampData(x12, 2);         // Децимация
    v2 = v12;                           // Компонент v_2[n]
    // Уровень 3
    x3 = v02; // Выход ФНЧ 2-ого уровня ко входу QMF 3-ого уровня
    decLPF_128->setData(x3);
    decLPF_128->setCoef(decLPF);
    decLPF_128->filtData();             // Фильтрация ФНЧ
    x03 = decLPF_128->getFiltData();
    decHPF_128->setData(x3);
    decHPF_128->setCoef(decHPF);
    decHPF_128->filtData();             // Фильтрация ФВЧ
    x13 = decHPF_128->getFiltData();
    v03 = downSampData(x03, 2);         // Децимация
    v13 = downSampData(x13, 2);         // Децимация
    v3 = v13;                           // Компонент v_3[n]
    // Уровень 4
    x4 = v03; // Выход ФНЧ 3-ого уровня ко входу QMF 4-ого уровня
    decLPF_64->setData(x4);
    decLPF_64->setCoef(decLPF);
    decLPF_64->filtData();             // Фильтрация ФНЧ
    x04 = decLPF_64->getFiltData();
    decHPF_64->setData(x4);
    decHPF_64->setCoef(decHPF);
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
    recLPF_64->setCoef(recLPF);
    recLPF_64->filtData();              // Фильтрация ФНЧ
    recHPF_64->setData(w14);
    recHPF_64->setCoef(recHPF);
    recHPF_64->filtData();              // Фильтрация ФВЧ
    y3 = sumData(recLPF_64->getFiltData(), recHPF_64->getFiltData()); // сигнал y_3[n]
    // Уровень 3
    w13sh = shiftData(v3, (LEN_FILTER_18 - 1));// + 2);
    w13 = upSampData(w13sh, 2);         // Интерполяция
    yu3 = upSampData(y3, 2);            // Интерполяция
    recLPF_128->setData(yu3);
    recLPF_128->setCoef(recLPF);
    recLPF_128->filtData();             // Фильтрация ФНЧ
    recHPF_128->setData(w13);
    recHPF_128->setCoef(recHPF);
    recHPF_128->filtData();             // Фильтрация ФВЧ
    y2 = sumData(recLPF_128->getFiltData(), recHPF_128->getFiltData()); // сигнал y_2[n]
    // Уровень 2
    w12sh = shiftData(v2, 3 * (LEN_FILTER_18 - 1));// + 2 + 4);
    w12 = upSampData(w12sh, 2);         // Интерполяция
    yu2 = upSampData(y2, 2);            // Интерполяция
    recLPF_256->setData(yu2);
    recLPF_256->setCoef(recLPF);
    recLPF_256->filtData();             // Фильтрация ФНЧ
    recHPF_256->setData(w12);
    recHPF_256->setCoef(recHPF);
    recHPF_256->filtData();             // Фильтрация ФВЧ
    y1 = sumData(recLPF_256->getFiltData(), recHPF_256->getFiltData()); // сигнал y_1[n]
    // Уровень 1
    w11sh = shiftData(v1, 7 * (LEN_FILTER_18 - 1));// + 2 + 4 + 8);
    w11 = upSampData(w11sh, 2);         // Интерполяция
    yu1 = upSampData(y1, 2);            // Интерполяция
    recLPF_512->setData(yu1);
    recLPF_512->setCoef(recLPF);
    recLPF_512->filtData();             // Фильтрация ФНЧ
    recHPF_512->setData(w11);
    recHPF_512->setCoef(recHPF);
    recHPF_512->filtData();             // Фильтрация ФВЧ
    y = sumData(recLPF_512->getFiltData(), recHPF_512->getFiltData()); // сигнал y[n]

    decPlot->showData(t, x);
    recPlot->showData(t, y);

    enableZoomMode(true);
}
//-----------------------------------------------------------------
void tsmfb::resizeData()
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
void tsmfb::initData()
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
