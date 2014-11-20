#ifndef CALLTIMINGANALYSIS_H
#define CALLTIMINGANALYSIS_H

#include <QMainWindow>

namespace Ui {
class CallTimingAnalysis;
}

class MainWindow;

class CallTimingAnalysis : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CallTimingAnalysis(MainWindow *aMainWindow);
    ~CallTimingAnalysis();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::CallTimingAnalysis *ui;
    MainWindow *mMainWindow;
};

#endif // CALLTIMINGANALYSIS_H
