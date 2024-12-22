#pragma once

void LogList(const char **list, u32 size);
void LogList(const std::vector<const char *> &list);

void LogVersion(u32 version);
void LogQueues(const std::vector<vk::QueueFamilyProperties> &queueFamilies);
void LogList(std::vector<vk::ExtensionProperties> &extensions);
void LogList(std::vector<vk::LayerProperties> &layers);
void LogList(const std::vector<vk::PresentModeKHR> &modes);
void LogList(const std::vector<vk::SurfaceFormatKHR> &formats);
void LogList(const vk::SurfaceCapabilitiesKHR &capabilities);
