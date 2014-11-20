#ifndef REDUNDANCYANALYSIS_H
#define REDUNDANCYANALYSIS_H

#include <QMainWindow>

#include "playbackmanager.h"

namespace Ui {
  class RedundancyAnalysis;
}

class RedundancyAnalysis : public QMainWindow
{
  Q_OBJECT
    
public:
  explicit RedundancyAnalysis(PlaybackManager *aPBManager, QWidget *parent = 0);
  ~RedundancyAnalysis();
    
private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
  PlaybackManager *mPBManager;
  Ui::RedundancyAnalysis *ui;
};

#endif // REDUNDANCYANALYSIS_H
