#ifndef SPELLERWIDGET_H
#define SPELLERWIDGET_H

#include <QWidget>
#include <QObject>
#include <QString>
#include <QLabel>
#include <QStackedLayout>
#include <spellercontroller.h>

class SpellerWidget : public QWidget
{
    Q_OBJECT

private:
    static constexpr int MATRIX_DIM=6;

    //0 - nothing, 1,2,3 - rows 0,1,2, -1,-2.-3 columns 0,1,2
    short highlighted;
    QLabel* message;
    QLabel* tiles[MATRIX_DIM*MATRIX_DIM];
    QStackedLayout* stackedLayout;

    void resizeEvent(QResizeEvent* event);

    static QString getCellRegularStyle();
    static QString getCellHighlightedStyle();
    static QString getCellIndicatedStyle();

    void highlightOnOff(short code, bool on);


public:
    SpellerWidget();


public slots:

    void highlight(short code);
    void highlightTile(short row, short column);
    void unhighlight();
    void showMessage(QString str);

};

#endif // SPELLERWIDGET_H
