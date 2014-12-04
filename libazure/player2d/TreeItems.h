#ifndef TREEITEMS_H
#define TREEITEMS_H

#include "qtreewidget.h"
#include "RecordedEvent.h"

class EventItem : public QTreeWidgetItem
{
public:
  EventItem(const QStringList &aStrings, QTreeWidget *aView, int64_t aID)
    : QTreeWidgetItem(aView, aStrings)
    , mID(aID)
  {}

  int64_t mID;
  double mTiming;
};

class ObjectItem : public QTreeWidgetItem
{
protected:
  ObjectItem(QTreeWidget *aView, const QStringList &aStrings,
             mozilla::gfx::ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator)
    : QTreeWidgetItem(aView, aStrings)
    , mRefPtr(aRefPtr)
    , mTranslator(aTranslator)
  {}

public:
  virtual QWidget *CreateViewWidget() = 0;

  virtual QString GetTitle() const { return ""; }
  mozilla::gfx::ReferencePtr GetObjectRef() { return mRefPtr; }

protected:
  mozilla::gfx::ReferencePtr mRefPtr;
  mozilla::gfx::Translator *mTranslator;
};

class DrawTargetItem : public ObjectItem
{
public:
  DrawTargetItem(const QStringList &aStrings, QTreeWidget *aView,
                 mozilla::gfx::ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator)
    : ObjectItem(aView, aStrings, aRefPtr, aTranslator)
  {
  }

  virtual QWidget *CreateViewWidget();
  virtual QString GetTitle() const;
};

class GradientStopsItem : public ObjectItem
{
public:
  GradientStopsItem(const QStringList &aStrings, QTreeWidget *aView,
                 mozilla::gfx::ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator)
    : ObjectItem(aView, aStrings, aRefPtr, aTranslator)
  {
  }

  virtual QWidget *CreateViewWidget();
  virtual QString GetTitle() const;
};

class PathItem : public ObjectItem
{
public:
  PathItem(const QStringList &aStrings, QTreeWidget *aView,
           mozilla::gfx::ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator)
    : ObjectItem(aView, aStrings, aRefPtr, aTranslator)
  {
  }

  virtual QWidget *CreateViewWidget() { return NULL; }
};

class SourceSurfaceItem : public ObjectItem
{
public:
  SourceSurfaceItem(const QStringList &aStrings, QTreeWidget *aView,
                    mozilla::gfx::ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator)
    : ObjectItem(aView, aStrings, aRefPtr, aTranslator)
  {
  }

  virtual QWidget *CreateViewWidget();
  virtual QString GetTitle() const;
};

#endif // TREEITEMS_H
