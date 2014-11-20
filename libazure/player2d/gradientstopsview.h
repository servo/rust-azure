#ifndef GRADIENTSTOPSVIEW_H
#define GRADIENTSTOPSVIEW_H

#include <QWidget>
#include "2D.h"
#include "RecordedEvent.h"

namespace Ui {
class GradientStopsView;
}

class GradientStopsView : public QWidget
{
    Q_OBJECT
    
public:
    explicit GradientStopsView(mozilla::gfx::ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator, QWidget *parent = 0);
    ~GradientStopsView();
    
public slots:
    void UpdateView();

protected slots:
    void EventChanged();

signals:
    void SwitchingBackend(uint32_t aType);

private:
    Ui::GradientStopsView *ui;
    mozilla::gfx::Translator *mTranslator;
    mozilla::gfx::ReferencePtr mRefPtr;
};

#endif // GRADIENTSTOPSVIEW_H
