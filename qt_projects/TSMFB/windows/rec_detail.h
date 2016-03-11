#ifndef RECDETAIL_H
#define RECDETAIL_H
//-----------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class plot;
//-----------------------------------------------------------------

class recDetail : public QWidget
{
    Q_OBJECT
public:
    explicit recDetail(QWidget *parent = 0);
    void setShowFlag();
    void resetShowFlag();
    bool isShowFlag();
    void showComponent(const QVector<double> &x, const QVector<double> &y, const QString &);
    void setPlotTitle(const QString &);

    plot    *recPlotDetail;

signals:
    void closeComponent();

public slots:

protected:
    virtual void closeEvent(QCloseEvent *);

private:
    void enableZoomMode(bool on);

    bool flagShowComponent;

    QwtPlotZoomer   *zoomer;
    QwtPlotPanner   *panner;
    QwtPlotPicker   *picker;
};

#endif // RECDETAIL_H
