#include <filesystem>
#include <fstream>
std::vector<char> ReadFile(const char *filename) {
  std::string full_path = std::filesystem::absolute(filename).string();
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    ENGINE_ERROR("File did not open: {}", full_path);
  }
  u32 fileSize = (u32)file.tellg();
  if (fileSize <= 0) {
    ENGINE_ERROR("File is empty or invalid: {}. File Size: {}", full_path, fileSize);
  }
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  if (!file.read(buffer.data(), fileSize)) {
    ENGINE_ERROR("Failed to read file: {}", full_path);
  }
  file.close();
  return buffer;
}
