//-----------------------------------------------------------------
#include "fir_filter.h"
//-----------------------------------------------------------------

fir_filter::fir_filter(int lenSamp, int lenFilt)
{
    lenSample = lenSamp;
    lenFilter = lenFilt;

    bufSampI.resize(lenSamp);
    bufSampO.resize(lenSamp);
    coefFilt.resize(lenFilt);
    sample.resize(lenFilt);

    initData();
}
//-----------------------------------------------------------------
fir_filter::~fir_filter()
{

}
//-----------------------------------------------------------------
void fir_filter::initData()
{
    bufSampI.fill(0, lenSample);
    bufSampO.fill(0, lenSample);
    coefFilt.fill(0, lenFilter);
    sample.fill(0, lenFilter);
}
//-----------------------------------------------------------------
void fir_filter::setData(QVector<double> d_i)
{
    initData();
    bufSampI = d_i;
}
//-----------------------------------------------------------------
void fir_filter::setCoef(double *c)
{
    for(int i = 0; i < coefFilt.size(); ++i, ++c)
        coefFilt[i] = *c;
}
//-----------------------------------------------------------------
QVector<double> fir_filter::getFiltData()
{
    return bufSampO;
}
//-----------------------------------------------------------------
void fir_filter::filtData()
{
    for(int i = 0; i < bufSampI.size(); ++i)
        bufSampO[i] = convolution(bufSampI[i]);
}
//-----------------------------------------------------------------
double fir_filter::convolution(double input)
{
    double acc = 0;
    double output = 0;
 
    sample[0] = input;
    
    for(int i = 0; i < LEN_FILTER_18; ++i)
        acc += (coefFilt[i] * sample[i]);
 
    output = acc;

    for(int i = LEN_FILTER_18 - 1; i > 0; --i)
        sample[i] = sample[i - 1];

    return output;
}
//-----------------------------------------------------------------
