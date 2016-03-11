//-----------------------------------------------------------------
#include <QtGui>
//-----------------------------------------------------------------
#include "cool_tsmfb.h"
//-----------------------------------------------------------------
#define minWinWidth  900
#define minWinHeight 500
//-----------------------------------------------------------------
void loadModules(QSplashScreen *psplash)
{
    QTime time;
    time.start();

    for (int i = 0; i < 100;) {
        if (time.elapsed() > 10) {
            time.start();
            ++i;
        }
        psplash->showMessage("Loading modules: "
                             + QString::number(i) + "%",
                             Qt::AlignHCenter | Qt::AlignBottom,
                             Qt::white
                            );
    }
}
//-----------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication    app(argc, argv);

    QSharedMemory global("your_unique_key");
    if (!global.create(1))
        return 1;

    cool_tsmfb       tsmfbWin;
    QSplashScreen   *splash = new QSplashScreen;

    QObject::connect(&tsmfbWin, SIGNAL(quit()), &app, SLOT(quit()));

    splash->setPixmap(QPixmap(":/images/TSMFB.png"));
    splash->show();
    loadModules(splash);

    tsmfbWin.setMinimumSize(minWinWidth, minWinHeight);
    QRect rect = QApplication::desktop()->availableGeometry(&tsmfbWin);
    tsmfbWin.move(rect.center() - tsmfbWin.rect().center());

    tsmfbWin.show();
    splash->finish(&tsmfbWin);
    delete splash;

    return app.exec();
}
//-----------------------------------------------------------------
