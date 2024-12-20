#pragma once
#include "defines.h"
#include "src/core/log.hpp"
#include <vulkanEngine.hpp>

void LogList(const std::vector<const char *> &list);
void LogList(const char **list, u32 size);
void LogExtensions(std::vector<vk::ExtensionProperties> &extensions);
void LogLayers(std::vector<vk::LayerProperties> &layers);
void ReportVersionNumber(u32 version);
