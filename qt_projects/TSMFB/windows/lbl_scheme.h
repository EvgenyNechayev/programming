#ifndef LBLSCHEME_H
#define LBLSCHEME_H

#include <QWidget>
#include <QLabel>
#include <QCloseEvent>

class lblScheme : public QLabel
{
    Q_OBJECT
public:
    explicit lblScheme(QWidget *parent = 0);
    void setShowFlag();
    void resetShowFlag();
    bool isShowFlag();

signals:
    void closeScheme();

public slots:

protected:
    virtual void closeEvent(QCloseEvent *);

private:
    bool flagShowScheme;
};

#endif // LBLSCHEME_H
