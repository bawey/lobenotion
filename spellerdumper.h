#ifndef SPELLERDUMPER_H
#define SPELLERDUMPER_H
#include <QObject>
#include <QFile>
#include <QTextStream>

class SpellerDumper : public QObject
{
    Q_OBJECT

private:
    QFile file;
    QTextStream out;
    int nowRow;
    int nowColumn;

public:
    SpellerDumper();

public slots:
    void spellerHint(int row, int column);
    void spellerHighlight(int i);
};

#endif // SPELLERDUMPER_H
