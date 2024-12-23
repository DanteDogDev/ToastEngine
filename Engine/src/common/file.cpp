#include <fstream>
std::vector<char> ReadFile(const char *filename) {
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    ENGINE_ERROR("File did not open: {}", filename);
  }
  u32 fileSize = (u32)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();
  return buffer;
}
