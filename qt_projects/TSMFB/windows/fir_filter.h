#ifndef FIR_FILTER_H_
#define FIR_FILTER_H_
//-----------------------------------------------------------------
#include <QVector>
//-----------------------------------------------------------------
#include "fir_const.h"
//-----------------------------------------------------------------
class fir_filter
{

public:
    fir_filter(int lenSamp, int lenFilt);
    ~fir_filter();

    void setData(QVector<double> d_i);
    void setCoef(double *c);
    QVector<double> getFiltData();
    void filtData();

private:
    int lenSample;
    int lenFilter;

    QVector<double> sample;

    QVector<double> bufSampI;
    QVector<double> bufSampO;
    QVector<double> coefFilt;

    double convolution(double input);
    void initData();
};
//-----------------------------------------------------------------
#endif /*FIR_FILTER_H_*/
