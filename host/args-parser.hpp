#pragma once

#include <any>
#include <map>
#include <string>

std::map<std::string, std::any> getConfig(int argc, char** argv);
