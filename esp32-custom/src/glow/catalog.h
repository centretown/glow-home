// CAUTION GENERATED FILE
#pragma once
#include "Frame.h"
namespace glow {
enum LIBRARY_INDEX : uint8_t {
DOUBLE_SCANNER,
SALMON_STRAWBERRY,
SKY_BLUE_SCAN,
SPLIT_IN_THREE,
SPLIT_IN_TWO,
SPOTLIGHT,
STRAWBERRY,
FRAME_COUNT,
};
extern const char *catalog_names[FRAME_COUNT];
extern Frame catalog[FRAME_COUNT];
extern Frame &from_catalog(LIBRARY_INDEX index);
extern const char *catalog_name(LIBRARY_INDEX index);
} // namespace glow
