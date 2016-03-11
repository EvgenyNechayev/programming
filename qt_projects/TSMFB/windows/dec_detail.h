#ifndef DECDETAIL_H
#define DECDETAIL_H
//-----------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class plot;
//-----------------------------------------------------------------

class decDetail : public QWidget
{
    Q_OBJECT
public:
    explicit decDetail(QWidget *parent = 0);
    void setShowFlag();
    void resetShowFlag();
    bool isShowFlag();
    void showComponent(const QVector<double> &x, const QVector<double> &y, const QString &);
    void setPlotTitle(const QString &);

    plot    *decPlotDetail;
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

#endif // DECDETAIL_H
