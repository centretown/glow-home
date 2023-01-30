#pragma once

#include "Spinner.h"
#include "HostLight.h"
#include "HostTimer.h"

namespace glow
{
  class HostSpinner : public Spinner<HostLight, HostTimer>
  {
  };
}
