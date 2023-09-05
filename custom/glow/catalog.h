// CAUTION GENERATED FILE
#pragma once
#include "Frame.h"
namespace glow {
enum CATALOG_INDEX : uint8_t {
BLACK_WHITE_SCAN,
COMPLEMENTARY_SCAN,
DOUBLE_SCAN,
GRADIENT_SCAN,
RAINBOW_DIAGONAL,
RAINBOW_HORIZONTAL,
RAINBOW_VERTICAL,
SPLIT_IN_THREE,
SPLIT_IN_TWO,
SPOTLIGHT,
FRAME_COUNT,
};
extern const char *catalog_names[FRAME_COUNT];
extern Frame catalog[FRAME_COUNT];
extern Frame &from_catalog(CATALOG_INDEX index);
extern const char *catalog_name(CATALOG_INDEX index);
} // namespace glow
