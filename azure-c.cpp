#include "azure-c.h"
#include "mozilla/gfx/2D.h"

#include <assert.h>

using namespace mozilla;

extern "C"
void AzureCSanityCheck() {
    assert(sizeof(Float) == sizeof(gfx::Float));
}
