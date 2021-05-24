#ifndef CONFIG_H
#define CONFIG_H

namespace config {
constexpr char const *title = "Chip8 Window";
constexpr int memory_size = 4096;
constexpr auto width = 64;
constexpr auto height = 32;
constexpr auto window_scale = 0xF;
constexpr char gp_register_count = 0x10;
} // namespace config

#endif
