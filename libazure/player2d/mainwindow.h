#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qaction.h>

#include "playbackmanager.h"
#include "displaymanager.h"
#include "TreeItems.h"

namespace Ui {
class MainWindow;
}

class DrawTargetWidget;
class CallTimingAnalysis;

const uint32_t sBackendCount = int(mozilla::gfx::BackendType::NVPR) + 1;

class BackendSwitch : public QAction
{
  Q_OBJECT
public:
  BackendSwitch(const QString &aName, mozilla::gfx::BackendType aType, 
                bool aSimulation, QObject *aParent)
    : QAction(aName, aParent)
    , mType(aType)
    , mSimulation(aSimulation)
  {
    setCheckable(true);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(selected(bool)));
  }

private slots:
  void selected(bool aChecked);
private:
  mozilla::gfx::BackendType mType;
  bool mSimulation;
};

class MainWindow : public QMainWindow
{
  Q_OBJECT
    
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  DrawTargetWidget *GetDTWidget();

  void DefaultArrangement();
  void UpdateObjects();
  void FilterToObject(mozilla::gfx::ReferencePtr aObject);

  virtual void resizeEvent(QResizeEvent *);

  void SwitchToBackend(mozilla::gfx::BackendType aType);
  void SwitchSimulationBackend(mozilla::gfx::BackendType aType);

  static mozilla::gfx::BackendType mMainBackend;

signals:
  void UpdateViews();
  void EventChange();
  void SwitchingBackend(uint32_t aType);

private slots:
    void on_actionOpen_Recording_triggered();

    void on_treeWidget_itemSelectionChanged();

    void on_objectTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_viewWidget_tabCloseRequested(int index);

    void on_actionExit_triggered();

    void on_actionAnalyze_Redundancy_triggered();

    void UpdateEventColor(int32_t aID);

    void on_actionBack_triggered();

    void on_actionForward_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void ObjectContextMenu(const QPoint &aPoint);

    void FilterByCurrentObject();
    void on_actionAnalyze_Call_Timings_triggered();

    void SetTargetView(mozilla::gfx::ReferencePtr aEvent);
private:
  friend class CallTimingAnalysis;

    static ID3D10Device1 *sDevice;

    Ui::MainWindow *ui;
    PlaybackManager mPBManager;
    DisplayManager mDPManager;
    std::vector<QTreeWidgetItem*> mEventItems;

  std::vector<uint32_t> mEventPlayHistory;
  hash_set<void*> mObjects;
  int32_t mCurrentHistoryPosition;
  bool mAutomatedItemChange;

  BackendSwitch *mBackends[sBackendCount];
  BackendSwitch *mSimulationBackends[sBackendCount];
};

#endif // MAINWINDOW_H
