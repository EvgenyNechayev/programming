#include "rec_detail.h"
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
recDetail::recDetail(QWidget *parent) :
    QWidget(parent)
{
    recPlotDetail = new plot(this);
//-----------------------------------------------------------------
    zoomer = new Zoomer(QwtPlot::xBottom, QwtPlot::yLeft,
        recPlotDetail->canvas());
//-----------------------------------------------------------------
    panner = new QwtPlotPanner(recPlotDetail->canvas());
    panner->setMouseButton(Qt::MidButton);
//-----------------------------------------------------------------
    picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPlotPicker::NoRubberBand, QwtPicker::AlwaysOn,
        recPlotDetail->canvas());
    picker->setStateMachine(new QwtPickerDragPointMachine());
    picker->setTrackerPen(QColor(Qt::black));
    picker->setTrackerFont(QFont("Times", 12, QFont::Normal, true));
//-----------------------------------------------------------------
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(recPlotDetail);

    setLayout(mainLayout);
    setShowFlag();
}
//-----------------------------------------------------------------
void recDetail::closeEvent(QCloseEvent *)
{
    setShowFlag();
    emit closeComponent();
}
//-----------------------------------------------------------------
void recDetail::resetShowFlag()
{
    flagShowComponent = false;
}
//-----------------------------------------------------------------
void recDetail::setShowFlag()
{
    flagShowComponent = true;
}
//-----------------------------------------------------------------
bool recDetail::isShowFlag()
{
    return flagShowComponent;
}
//-----------------------------------------------------------------
void recDetail::showComponent(const QVector<double> &x, const QVector<double> &y, const QString &str)
{
    QwtText recPlotTitle;
    recPlotTitle.setColor(Qt::black);
    recPlotTitle.setFont(QFont("Times", 16, QFont::Bold, false));
    recPlotTitle.setText(str, QwtText::AutoText);

    recPlotDetail->setTitle(recPlotTitle);
//-----------------------------------------------------------------
    recPlotDetail->setAxisAutoScale(QwtPlot::xBottom, true);
    recPlotDetail->setAxisAutoScale(QwtPlot::yLeft, true);

    recPlotDetail->showData(x, y);
    enableZoomMode(true);
}
//-----------------------------------------------------------------
void recDetail::enableZoomMode(bool on)
{
    panner->setEnabled(on);

    zoomer->setZoomBase();
    zoomer->setEnabled(on);
    zoomer->zoom(0);

    picker->setEnabled(on);
}
//-----------------------------------------------------------------
void recDetail::setPlotTitle(const QString &str)
{
    QwtText recPlotTitle;
    recPlotTitle.setColor(Qt::black);
    recPlotTitle.setFont(QFont("Times", 16, QFont::Bold, false));
    recPlotTitle.setText(str, QwtText::AutoText);

    recPlotDetail->setTitle(recPlotTitle);
}
//-----------------------------------------------------------------
