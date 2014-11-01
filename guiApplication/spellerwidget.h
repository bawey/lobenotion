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

    //0 - nothing, 1,2,3 - rows 0,1,2, -1,-2.-3 columns 0,1,2


    int highlighted;
    QLabel* message;
    QLabel* tiles[MATRIX_DIM*MATRIX_DIM];
    QStackedLayout* stackedLayout;
    void resizeEvent(QResizeEvent* event);

    void highlightRow(int number);
    void highlightColumn(int number);

    int frameCount = 0;
    bool nextHighlightColumn = false;

    static constexpr int exposureFrames=60;
    static constexpr int intervalFrames=100;

    static constexpr int hintFrames=250;
    static constexpr int epochPeriods=42;

    static constexpr int epochFrames=hintFrames+intervalFrames+epochPeriods*(exposureFrames+intervalFrames);

    static QString getCellRegularStyle();
    static QString getCellHighlightedStyle();


public:
    SpellerWidget();

signals:


    void hint(int row, int col);
    /**
     * @param i -7 through -1 for columns 6 through 0, 1 through 7 for rows 0 through 6
     */
    void highlight(int i, long time);
    void spellerLatency(unsigned short d);

public slots:
    void trainingResetFramesCount();

private slots:
    void displayInstruction(QString message);
    void highlightTile(int row, int column);
    void unhighlight();
    void spellerMessage(QString str);
    void randomHint();

    /**
     * @brief notification used to synchronize the speller with DAQ
     */
    void eegFrameNotification();

};

#endif // SPELLERWIDGET_H
