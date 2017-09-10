#pragma once
#include <vector>
#include "MonitorInfo.hpp"

namespace FastWindowSwitcherLib
{
  namespace MonitorFunctions
  {
    std::vector<MonitorInfo> GetMonitorInfos();
  };
};