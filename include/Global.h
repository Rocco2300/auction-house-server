#pragma once

#include "Session.h"

#include <memory>
#include <vector>

namespace global
{
inline std::vector<std::weak_ptr<Session>> sessions;
}