#include "TransformHelper.h"
#include <iostream>
#include <fstream>
#include <functional>

namespace nsK2EngineLow
{
	bool LoadScene(const std::string& filepath, const std::function<bool(const nlohmann::json&)>& callback);
}