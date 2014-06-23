#ifndef SPELLERWIDGET_H
#define SPELLERWIDGET_H

#include <QWidget>
#include <QObject>
#include <QString>
#include <QLabel>

class SpellerWidget : public QWidget
{
    Q_OBJECT

private:
    static constexpr int MATRIX_DIM=6;

    static QString styleRegular;
    static QString styleHighlight;

    //0 - nothing, 1,2,3 - rows 0,1,2, -1,-2.-3 columns 0,1,2
    int highlighted;
    QLabel* tiles[MATRIX_DIM*MATRIX_DIM];


    void resizeEvent(QResizeEvent* event);

public:
    SpellerWidget();


private slots:
    void highlightRow(int number);
    void highlightColumn(int number);
    void unhighlight();
};

#endif // SPELLERWIDGET_H
