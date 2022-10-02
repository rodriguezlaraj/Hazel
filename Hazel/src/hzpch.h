#pragma once
#include <iostream>
#include <memory> //To enable the use of shared pointers
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Hazel/Log.h"

#ifdef HZ_PLATFORM_WINDOWS
#include <Windows.h>
#endif