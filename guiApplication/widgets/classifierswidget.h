#ifndef CLASSIFIERSWIDGET_H
#define CLASSIFIERSWIDGET_H

#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QList>
#include <QCheckBox>

class ClassifiersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClassifiersWidget(QWidget *parent = 0);

private:
    QStackedLayout* stackedLayout = new QStackedLayout(this);
    QVBoxLayout* boxLayout = new QVBoxLayout(this);
    QWidget* errorPane = new QLabel("Classifiers folder not found");
    QWidget* dataPane = NULL;
    QList<QCheckBox*>* boxes = new QList<QCheckBox*>();

    void manageCheckboxConnections(bool connect);

signals:
    void signalClassifierEnabled(QString classifier, bool enabled);
public slots:
    /**
     * @brief slotUpdateClassifiers
     * should be called when octave scripts are reloaded
     */
    void slotUpdateContent();
    void slotConfigurationChanged(QString parameter);
    void slotClassifierEnabled(bool enabled);
};

#endif // CLASSIFIERSWIDGET_H
