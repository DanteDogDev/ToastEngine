#pragma once

void LogQueues(const std::vector<vk::QueueFamilyProperties> &queueFamilies);
void LogList(const char **list, u32 size);
// FINAL LOGGERS
void LogVersion(u32 version);
void LogList(std::vector<vk::ExtensionProperties> &extensions);
void LogList(std::vector<vk::LayerProperties> &layers);
void LogList(const std::vector<const char *> &list);
