#pragma once

#include "Spinner.h"
#include "TestLight.h"
#include "TestTimer.h"

namespace glow
{
  class TestSpinner : public Spinner<TestLight, TestTimer>
  {
  };
}
