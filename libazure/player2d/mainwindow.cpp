#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QFileDialog"
#include "qmessagebox.h"
#include <fstream>

#ifdef WIN32
#include <d3d10_1.h>
#include <d3d11.h>
#endif

#include "RecordedEvent.h"
#include "PathRecording.h"

#include "drawtargetwidget.h"
#include "qmdisubwindow.h"
#include "redundancyanalysis.h"
#include "calltiminganalysis.h"

using namespace std;
using namespace mozilla;
using namespace mozilla::gfx;

ID3D10Device1 *MainWindow::sDevice = NULL;
BackendType MainWindow::mMainBackend = BackendType::NONE;

QString
NameForBackend(uint32_t aType)
{
  switch (aType) {
  case BackendType::NONE:
    return "None";
  case BackendType::CAIRO:
    return "Cairo";
  case BackendType::DIRECT2D:
    return "Direct2D";
  case BackendType::DIRECT2D1_1:
    return "Direct2D 1.1";
  case BackendType::COREGRAPHICS:
    return "Quartz";
  case BackendType::COREGRAPHICS_ACCELERATED:
    return "Accelerated Quartz";
  case BackendType::SKIA:
    return "Skia";
  case BackendType::NVPR:
    return "NVidia Path Rendering";
  default:
    return "Unknown";
  }
}

void
BackendSwitch::selected(bool aChecked)
{
  if (aChecked) {
    if (mSimulation) {
      static_cast<MainWindow*>(parentWidget())->SwitchSimulationBackend(mType);
    } else {
      static_cast<MainWindow*>(parentWidget())->SwitchToBackend(mType);
    }
  }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mAutomatedItemChange(false)
{
  ui->setupUi(this);

  for (int i = 0; i < sBackendCount; i++) {
    mBackends[i] = new BackendSwitch(NameForBackend(i), BackendType(i), false, this);
    mSimulationBackends[i] = new BackendSwitch(NameForBackend(i), BackendType(i), true, this);
  }

#ifdef WIN32
  if (!sDevice) {
    ::D3D10CreateDevice1(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL,
                         D3D10_CREATE_DEVICE_BGRA_SUPPORT, D3D10_FEATURE_LEVEL_10_0,
                         D3D10_1_SDK_VERSION, &sDevice);

    if (!sDevice) {
      // EEEP!
    }
    Factory::SetDirect3D10Device(sDevice);

    RefPtr<ID3D11Device> device;
    D3D_FEATURE_LEVEL featureLevels[] = {
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_3
    };


    HRESULT hr = ::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL,
                                     D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                                     featureLevels, sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL),
                                     D3D11_SDK_VERSION, byRef(device), nullptr, nullptr);

    Factory::SetDirect3D11Device(device);
  }
  ui->menuBackend->addAction(mBackends[int(BackendType::DIRECT2D)]);
  ui->menuSimulationBackend->addAction(mSimulationBackends[int(BackendType::DIRECT2D)]);
  ui->menuBackend->addAction(mBackends[int(BackendType::DIRECT2D1_1)]);
  ui->menuSimulationBackend->addAction(mSimulationBackends[int(BackendType::DIRECT2D1_1)]);
#elif __APPLE__
  //ui->menuBackend->addAction(mBackends[BackendType::COREGRAPHICS]);
  //RefPtr<DrawTarget> refDT = Factory::CreateDrawTarget(BackendType::COREGRAPHICS, IntSize(1, 1), FORMAT_B8G8R8A8);
  //mPBManager.SetBaseDT(refDT);
  // TODO Add a way to select backends
#endif
#ifdef USE_CAIRO
  ui->menuBackend->addAction(mBackends[int(BackendType::CAIRO)]);
  ui->menuSimulationBackend->addAction(mSimulationBackends[int(BackendType::CAIRO)]);
#endif
#ifdef USE_SKIA
  ui->menuBackend->addAction(mBackends[int(BackendType::SKIA)]);
  ui->menuSimulationBackend->addAction(mSimulationBackends[int(BackendType::SKIA)]);
#endif
#ifdef USE_NVPR
  ui->menuBackend->addAction(mBackends[int(BackendType::NVPR)]);
  ui->menuSimulationBackend->addAction(mSimulationBackends[int(BackendType::NVPR)]);
#endif

  ui->menuBackend->actions()[0]->toggle();
  ui->menuSimulationBackend->actions()[0]->toggle();

  QWidget *targetWidget = new QWidget();
  ui->viewWidget->addTab(targetWidget, "Event Target");
  targetWidget->setLayout(new QHBoxLayout(targetWidget));
  targetWidget->layout()->setContentsMargins(0, 0, 0, 0);

  connect(&mPBManager, SIGNAL(EventDisablingUpdated(int32_t)), this, SLOT(UpdateEventColor(int32_t)));
  ui->objectTree->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->objectTree, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ObjectContextMenu(const QPoint &)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

DrawTargetWidget *
MainWindow::GetDTWidget()
{
  return NULL;
}

void
MainWindow::DefaultArrangement()
{
  QList<QMdiSubWindow*> list = ui->mdiArea->subWindowList();
  list[1]->move(QPoint(0, 150));
  list[1]->resize(QSize(350, ui->mdiArea->size().height() - 150));
  list[2]->move(QPoint(0, 0));
  list[2]->resize(QSize(ui->mdiArea->size().width(), 150));
  list[0]->move(350, 150);
  list[0]->resize(ui->mdiArea->size().width() - 350, ui->mdiArea->size().height() - 250);
  list[3]->move(350, ui->mdiArea->size().height() - 100);
  list[3]->resize(ui->mdiArea->size().width() - 350, 100);
}

void
MainWindow::UpdateObjects()
{
   QStringList list;
   list.push_back("");
   list.push_back("DrawTarget");
   list.push_back("");

   ui->objectTree->clear();

   {
     PlaybackManager::DTMap::iterator iter = mPBManager.mDrawTargets.begin();

     for (;mPBManager.mDrawTargets.end() != iter; iter++) {
       list[0] = QString::fromStdString(StringFromPtr(iter->first));
       stringstream stream;
       stream << iter->second->GetSize().width << " x " << iter->second->GetSize().height;
       list[2] = QString::fromStdString(stream.str());
       new DrawTargetItem(list, ui->objectTree, iter->first, &mPBManager);
     }
   }

   {
     PlaybackManager::PathMap::iterator iter = mPBManager.mPaths.begin();

     list[1] = "Path";
     for (;mPBManager.mPaths.end() != iter; iter++) {
       list[0] = QString::fromStdString(StringFromPtr(iter->first));
       list[2] = "";
       new PathItem(list, ui->objectTree, iter->first, &mPBManager);
     }
   }

   {
     PlaybackManager::SourceSurfaceMap::iterator iter = mPBManager.mSourceSurfaces.begin();

     list[1] = "SourceSurface";
     for (;mPBManager.mSourceSurfaces.end() != iter; iter++) {
       list[0] = QString::fromStdString(StringFromPtr(iter->first));
       stringstream stream;
       stream << iter->second->GetSize().width << " x " << iter->second->GetSize().height;
       list[2] = QString::fromStdString(stream.str());
       new SourceSurfaceItem(list, ui->objectTree, iter->first, &mPBManager);
     }
   }

   {
     PlaybackManager::GradientStopsMap::iterator iter = mPBManager.mGradientStops.begin();

     list[1] = "GradientStops";
     list[2] = "";
     for (;mPBManager.mGradientStops.end() != iter; iter++) {
       list[0] = QString::fromStdString(StringFromPtr(iter->first));
       new GradientStopsItem(list, ui->objectTree, iter->first, &mPBManager);
     }
   }
}

void
MainWindow::FilterToObject(ReferencePtr aObject)
{
  if (!aObject) {
    foreach(QTreeWidgetItem *item, mEventItems) {
      item->setHidden(false);
    }
    return;
  }

  foreach(QTreeWidgetItem *item, mEventItems) {
    int64_t idx = static_cast<EventItem*>(item)->mID;

    if (mPBManager.mRecordedEvents[idx]->GetObjectRef() != aObject) {
      item->setHidden(true);
    } else {
      item->setHidden(false);
    }
  }
}

void
MainWindow::resizeEvent(QResizeEvent *)
{
  DefaultArrangement();
}

void
MainWindow::SwitchToBackend(BackendType aType)
{
  for (int i = 0; i < sBackendCount; i++) {
    if (mBackends[i]) {
      if (i != int(aType)) {
        mBackends[i]->setChecked(false);
      }
    }
  }

  mPBManager.PlaybackToEvent(0);

  mMainBackend = aType;
  
  SwitchingBackend(int(aType));
  
  QApplication::processEvents();

  QTreeWidgetItem *item = ui->treeWidget->currentItem();
  if (item) {
    int64_t idx = static_cast<EventItem*>(item)->mID;

    mPBManager.PlaybackToEvent(idx + 1);
  }
}

void
MainWindow::SwitchSimulationBackend(BackendType aType)
{
  for (int i = 0; i < sBackendCount; i++) {
    if (mSimulationBackends[i]) {
      if (i != int(aType)) {
        mSimulationBackends[i]->setChecked(false);
      }
    }
  }

  mPBManager.PlaybackToEvent(0);
  RefPtr<DrawTarget> refDT = Factory::CreateDrawTarget(aType, IntSize(1, 1), SurfaceFormat::B8G8R8A8);
  mPBManager.SetBaseDT(refDT);
  
  QTreeWidgetItem *item = ui->treeWidget->currentItem();
  if (item) {
    int64_t idx = static_cast<EventItem*>(item)->mID;

    mPBManager.PlaybackToEvent(idx + 1);
  }
}

void MainWindow::on_actionOpen_Recording_triggered()
{
  mEventItems.clear();
  ui->treeWidget->clear();

  QString fileName = QFileDialog::getOpenFileName(this, "Open File Recording", QString(), "*.aer");

  ifstream inputFile;

  ui->comboBox->clear();
  ui->comboBox->addItem("All");
  
  inputFile.open(fileName.toStdString().c_str(), istream::in | istream::binary);

  inputFile.seekg(0, ios::end);
  int length = inputFile.tellg();
  inputFile.seekg(0, ios::beg);
  int64_t i = 0;
  ui->treeWidget->setColumnWidth(0, 50);
  ui->treeWidget->setColumnWidth(2, 150);

  QList<ReferencePtr> objects;

  uint32_t magicInt;
  ReadElement(inputFile, magicInt);
  if (magicInt != 0xc001feed) {
    QMessageBox::critical(this, "Error", "File is not a valid recording");
    return;
  }

  uint16_t majorRevision;
  uint16_t minorRevision;
  ReadElement(inputFile, majorRevision);
  ReadElement(inputFile, minorRevision);

  if (majorRevision != kMajorRevision) {
    QMessageBox::critical(this, "Error", "Recording was made with a different major revision");
    return;
  }

  if (minorRevision > kMinorRevision) {
    QMessageBox::critical(this, "Error", "Recording was made with a later minor revision");
    return;
  }

  while (inputFile.tellg() < length) {

    if (inputFile.tellg() < 0) {
      QMessageBox::critical(this, "Error", "Stream error: File could not be parsed");
      return;
    }

    int32_t type;
    ReadElement(inputFile, type);

    RecordedEvent *newEvent = RecordedEvent::LoadEventFromStream(inputFile, (RecordedEvent::EventType)type);

    QStringList list;
    list.push_back(QString::number(i));
    list.push_back(QString::fromStdString(StringFromPtr(newEvent->GetObjectRef())));
    list.push_back(QString::fromStdString(newEvent->GetName()));
    EventItem *item =
      new EventItem(list, ui->treeWidget, i++);
    item->setTextColor(0, QColor(180, 180, 180, 255));
    mPBManager.AddEvent(newEvent);
    mEventItems.push_back(item);

    if (mObjects.find(newEvent->GetObjectRef()) == mObjects.end()) {
      objects.push_back(newEvent->GetObjectRef());
    }
    mObjects.insert(newEvent->GetObjectRef());
  }

  qSort(objects);

  foreach(ReferencePtr ptr, objects) {
    QVariant itemData = QVariant(qulonglong(ptr.mLongPtr));
    QString str = "0x" + QString::fromStdString(StringFromPtr(ptr));
    ui->comboBox->addItem(str, itemData);
  }
  DefaultArrangement();
}

void MainWindow::SetTargetView(ReferencePtr aEvent)
{
  ObjectItem *objItem = nullptr;

  QStringList list;
  if (mPBManager.mDrawTargets.count(aEvent)) {
    objItem = new DrawTargetItem(list, ui->objectTree, aEvent, &mPBManager);
  } else if (mPBManager.mPaths.count(aEvent)) {
    objItem = new PathItem(list, ui->objectTree, aEvent, &mPBManager);
  } else if (mPBManager.mSourceSurfaces.count(aEvent)) {
    objItem = new SourceSurfaceItem(list, ui->objectTree, aEvent, &mPBManager);
  } else if (mPBManager.mGradientStops.count(aEvent)) {
    objItem = new GradientStopsItem(list, ui->objectTree, aEvent, &mPBManager);
  }

  QWidget* targetWidget = ui->viewWidget->widget(0);

  QWidget* currentTarget = targetWidget->findChild<QWidget*>();

  if (currentTarget) {
    uint64_t tabObjItem = currentTarget->property("objectref").value<uint64_t>();
    if (aEvent.mLongPtr == tabObjItem) {
      return;
    }

    targetWidget->layout()->removeWidget(currentTarget);

    delete currentTarget;
  }

  if (!objItem) {
    return;
  }

  QWidget* newTarget = objItem->CreateViewWidget();

  if (!newTarget) {
    return;
  }

  newTarget->setProperty("objectref", qVariantFromValue<uint64_t>(aEvent.mLongPtr));
  newTarget->setParent(targetWidget);
  targetWidget->layout()->addWidget(newTarget);

  QObject::connect(this, SIGNAL(UpdateViews()), newTarget, SLOT(UpdateView()));
  QObject::connect(this, SIGNAL(EventChange()), newTarget, SLOT(EventChanged()));
  QObject::connect(this, SIGNAL(SwitchingBackend(uint32_t)), newTarget, SIGNAL(SwitchingBackend(uint32_t)));
}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
  QTreeWidgetItem *item = ui->treeWidget->currentItem();
  int64_t idx = static_cast<EventItem*>(item)->mID;

  mPBManager.PlaybackToEvent(idx + 1);

  if (!mAutomatedItemChange) {
    mEventPlayHistory.push_back(idx);
    mCurrentHistoryPosition = mEventPlayHistory.size() - 1;
  }

  stringstream stream;
  mPBManager.mRecordedEvents[idx]->OutputSimpleEventInfo(stream);
  ui->textEventInfo->setText(QString::fromStdString(stream.str()));

  UpdateViews();
  UpdateObjects();
  EventChange();

  SetTargetView(mPBManager.mRecordedEvents[idx]->GetObjectRef());

  activateWindow();
}

void MainWindow::on_objectTree_itemDoubleClicked(QTreeWidgetItem *item, int)
{
  ObjectItem *objItem = static_cast<ObjectItem*>(item);

  for (int i = 0; i < ui->viewWidget->count(); i++) {
    QWidget *tab = ui->viewWidget->widget(i);
    void *tabObjItem = tab->property("objectref").value<void*>();
    if (objItem->GetObjectRef() == tabObjItem) {
      ui->viewWidget->setCurrentIndex(i);
      return;
    }
  }

  QWidget *newTab = objItem->CreateViewWidget();
  if (!newTab) {
    return;
  }
  newTab->setProperty("objectref", qVariantFromValue<void*>(objItem->GetObjectRef()));
  ui->viewWidget->addTab(newTab, objItem->GetTitle());
  ui->viewWidget->setCurrentIndex(ui->viewWidget->count() - 1);

  QObject::connect(this, SIGNAL(UpdateViews()), newTab, SLOT(UpdateView()));
  QObject::connect(this, SIGNAL(EventChange()), newTab, SLOT(EventChanged()));
  QObject::connect(this, SIGNAL(SwitchingBackend(uint32_t)), newTab, SIGNAL(SwitchingBackend(uint32_t)));
}

void MainWindow::on_viewWidget_tabCloseRequested(int index)
{
  QWidget *widget = ui->viewWidget->widget(index);
  ui->viewWidget->removeTab(index);
  delete widget;
}

void MainWindow::on_actionExit_triggered()
{
  QApplication::closeAllWindows();
}

void MainWindow::on_actionAnalyze_Redundancy_triggered()
{
  QWidget *widget = new RedundancyAnalysis(&mPBManager, this);
  widget->show();
}

void
MainWindow::UpdateEventColor(int32_t aID)
{
  if (aID == -1) {
    for (uint32_t i = 0; i < mEventItems.size(); i++) {
      if (mPBManager.IsEventDisabled(i)) {
        mEventItems[i]->setTextColor(2, QColor(255, 0, 0));
      } else {
        mEventItems[i]->setTextColor(2, QColor(0, 0, 0));
      }
    }
    return;
  }
  if (mPBManager.IsEventDisabled(aID)) {
    mEventItems[aID]->setTextColor(2, QColor(255, 0, 0));
  } else {
    mEventItems[aID]->setTextColor(2, QColor(0, 0, 0));
  }
}

void MainWindow::on_actionBack_triggered()
{
  if (--mCurrentHistoryPosition < 0) {
    mCurrentHistoryPosition = 0;
  }

  mAutomatedItemChange = true;
  ui->treeWidget->setCurrentItem(mEventItems[mEventPlayHistory[mCurrentHistoryPosition]]);
  mAutomatedItemChange = false;
}

void MainWindow::on_actionForward_triggered()
{
  if (++mCurrentHistoryPosition >= mEventPlayHistory.size()) {
    mCurrentHistoryPosition = mEventPlayHistory.size() - 1;
  }

  mAutomatedItemChange = true;
  ui->treeWidget->setCurrentItem(mEventItems[mEventPlayHistory[mCurrentHistoryPosition]]);
  mAutomatedItemChange = false;
}

void MainWindow::on_lineEdit_textChanged(const QString &)
{

}

void MainWindow::on_pushButton_clicked()
{
  bool ok;

  uint32_t eventid = ui->lineEdit->text().toUInt(&ok);

  if (!ok) {
    return;
  }

  if (eventid < 0 || eventid >= mEventItems.size()) {
    return;
  }

  ui->treeWidget->setCurrentItem(mEventItems[eventid]);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
  if (index == 0) {
    FilterToObject(nullptr);
    return;
  }

  QVariant itemData = ui->comboBox->itemData(index);

  ReferencePtr obj = (void*)(itemData.toULongLong());

  FilterToObject(obj);
}

void
MainWindow::ObjectContextMenu(const QPoint &aPoint)
{
  QMenu *menu = new QMenu;
  menu->addAction(tr("Filter by object"), this, SLOT(FilterByCurrentObject()));
  menu->exec(ui->objectTree->mapToGlobal(aPoint));
}

void
MainWindow::FilterByCurrentObject()
{
  ObjectItem *objItem =
    static_cast<ObjectItem*>(ui->objectTree->currentItem());

  if (!objItem) {
    return;
  }

  FilterToObject(objItem->GetObjectRef());
  for (int i = 1; i < ui->comboBox->count(); i++) {
    ReferencePtr obj = (void*)(ui->comboBox->itemData(i).toULongLong());

    if (obj == objItem->GetObjectRef()) {
      ui->comboBox->setCurrentIndex(i);
      break;
    }
  }
}

void MainWindow::on_actionAnalyze_Call_Timings_triggered()
{
  QWidget *widget = new CallTimingAnalysis(this);
  widget->show();
}
