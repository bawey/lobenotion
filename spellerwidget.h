#ifndef SPELLERWIDGET_H
#define SPELLERWIDGET_H

#include <QWidget>
#include <QObject>
#include <QString>
#include <QLabel>
#include <QStackedLayout>

class SpellerWidget : public QWidget
{
    Q_OBJECT

private:
    static constexpr int MATRIX_DIM=6;

    static QString styleRegular;
    static QString styleHighlight;

    //0 - nothing, 1,2,3 - rows 0,1,2, -1,-2.-3 columns 0,1,2
    int highlighted;
    QLabel* message;
    QLabel* tiles[MATRIX_DIM*MATRIX_DIM];
    QStackedLayout* stackedLayout;
    void resizeEvent(QResizeEvent* event);

public:
    SpellerWidget();

private slots:
    void displayInstruction(QString message);
    void highlightTile(int number);
    void highlightRow(int number);
    void highlightColumn(int number);
    void unhighlight();
    void spellerMessage(QString str);
    void randomHint();

    /**
     * @brief notification used to synchronize the speller with DAQ
     */
    void eegFrameNotification();
};

#endif // SPELLERWIDGET_H
