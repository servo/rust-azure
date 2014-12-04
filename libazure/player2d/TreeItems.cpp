#include "TreeItems.h"
#include "drawtargetview.h"
#include "sourcesurfaceview.h"
#include "gradientstopsview.h"

#include <sstream>

using namespace mozilla::gfx;
using namespace std;

QWidget*
DrawTargetItem::CreateViewWidget()
{
  return new DrawTargetView(mRefPtr, mTranslator);
}

QString
DrawTargetItem::GetTitle() const
{
  stringstream stream;
  stream << "DrawTarget (" << mRefPtr << ")";

  return QString::fromStdString(stream.str());
}

QWidget*
SourceSurfaceItem::CreateViewWidget()
{
  return new SourceSurfaceView(mRefPtr, mTranslator);
}

QString
SourceSurfaceItem::GetTitle() const
{
  stringstream stream;
  stream << "SourceSurface (" << mRefPtr << ")";

  return QString::fromStdString(stream.str());
}

QWidget*
GradientStopsItem::CreateViewWidget()
{
  return new GradientStopsView(mRefPtr, mTranslator);
}

QString
GradientStopsItem::GetTitle() const
{
  stringstream stream;
  stream << "GradientStops (" << mRefPtr << ")";

  return QString::fromStdString(stream.str());
}
