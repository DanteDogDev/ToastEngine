#include "log.hpp"
namespace Engine::Core {
std::shared_ptr<spdlog::logger> Log::m_engineLogger;
void Log::Init() {
  if (!m_engineLogger) { // Avoid reinitialization
    m_engineLogger->set_level(spdlog::level::debug);
    m_engineLogger = spdlog::stdout_color_mt("ENGINE");
    m_engineLogger->set_level(spdlog::level::trace);
  }
}
void Log::PrintList(const std::vector<const char *> &list) {
  for (const auto &item : list) {
    ENGINE_INFO("\t\"{}\"", item);
  }
}
} // namespace Engine::Core
