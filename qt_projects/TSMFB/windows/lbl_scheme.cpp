#include "lbl_scheme.h"

lblScheme::lblScheme(QWidget *parent) :
    QLabel(parent)
{
    setShowFlag();
}
//-----------------------------------------------------------------
void lblScheme::closeEvent(QCloseEvent *)
{
    setShowFlag();
    emit closeScheme();
}
//-----------------------------------------------------------------
void lblScheme::resetShowFlag()
{
    flagShowScheme = false;
}
//-----------------------------------------------------------------
void lblScheme::setShowFlag()
{
    flagShowScheme = true;
}
//-----------------------------------------------------------------
bool lblScheme::isShowFlag()
{
    return flagShowScheme;
}
