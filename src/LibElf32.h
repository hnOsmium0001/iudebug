#pragma once

#include "BaseDefs.h"
#include "LibElfCommon.h"

#include <stddef.h>

IU_EXTERN_C_BEGIN

// TODO(hnosm): do we event want to support ELF32?
// Section header (ELF32 figure 1-8)
typedef struct {
    // TODO
} Iu_Efl32SectionHeader;

IU_EXTERN_C_END
