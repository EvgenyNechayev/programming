#include "dec_detail.h"
//-----------------------------------------------------------------
#include "plot.h"
//-----------------------------------------------------------------
#include <QHBoxLayout>
//-----------------------------------------------------------------
#include <qwt_text.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_picker_machine.h>
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
decDetail::decDetail(QWidget *parent) :
    QWidget(parent)
{   
    decPlotDetail = new plot(this);
//-----------------------------------------------------------------
    zoomer = new Zoomer(QwtPlot::xBottom, QwtPlot::yLeft,
        decPlotDetail->canvas());
//-----------------------------------------------------------------
    panner = new QwtPlotPanner(decPlotDetail->canvas());
    panner->setMouseButton(Qt::MidButton);
//-----------------------------------------------------------------
    picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPlotPicker::NoRubberBand, QwtPicker::AlwaysOn,
        decPlotDetail->canvas());
    picker->setStateMachine(new QwtPickerDragPointMachine());
    picker->setTrackerPen(QColor(Qt::black));
    picker->setTrackerFont(QFont("Times", 12, QFont::Normal, true));
//-----------------------------------------------------------------
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(decPlotDetail);

    setLayout(mainLayout);
    setShowFlag();
}
//-----------------------------------------------------------------
void decDetail::closeEvent(QCloseEvent *)
{
    setShowFlag();
    emit closeComponent();
}
//-----------------------------------------------------------------
void decDetail::resetShowFlag()
{
    flagShowComponent = false;
}
//-----------------------------------------------------------------
void decDetail::setShowFlag()
{
    flagShowComponent = true;
}
//-----------------------------------------------------------------
bool decDetail::isShowFlag()
{
    return flagShowComponent;
}
//-----------------------------------------------------------------
void decDetail::showComponent(const QVector<double> &x, const QVector<double> &y, const QString &str)
{
    QwtText decPlotTitle;
    decPlotTitle.setColor(Qt::black);
    decPlotTitle.setFont(QFont("Times", 16, QFont::Bold, false));
    decPlotTitle.setText(str, QwtText::AutoText);

    decPlotDetail->setTitle(decPlotTitle);
//-----------------------------------------------------------------
    decPlotDetail->setAxisAutoScale(QwtPlot::xBottom, true);
    decPlotDetail->setAxisAutoScale(QwtPlot::yLeft, true);

    decPlotDetail->showData(x, y);
    enableZoomMode(true);
}
//-----------------------------------------------------------------
void decDetail::enableZoomMode(bool on)
{
    panner->setEnabled(on);

    zoomer->setZoomBase();
    zoomer->setEnabled(on);
    zoomer->zoom(0);

    picker->setEnabled(on);
}
//-----------------------------------------------------------------
void decDetail::setPlotTitle(const QString &str)
{
    QwtText decPlotTitle;
    decPlotTitle.setColor(Qt::black);
    decPlotTitle.setFont(QFont("Times", 16, QFont::Bold, false));
    decPlotTitle.setText(str, QwtText::AutoText);

    decPlotDetail->setTitle(decPlotTitle);
}
//-----------------------------------------------------------------
