#pragma once

#include <any>
#include <map>
#include <string>

std::map<std::string, std::any> GetConfig(int argc, char** argv);
