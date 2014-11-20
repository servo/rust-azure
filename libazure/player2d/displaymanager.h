#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "2D.h"

class DisplayManager
{
public:
    DisplayManager();

private:
    mozilla::RefPtr<mozilla::gfx::SourceSurface> mCurrentSourceSurface;
};

#endif // DISPLAYMANAGER_H
