#ifndef CONFIG_H
#define CONFIG_H

#include <_types/_uint16_t.h>
namespace config {
constexpr char const *title = "Chip8 Window";
constexpr int memory_size = 0x1000;
constexpr auto width = 0x40;
constexpr auto height = 0x20;
constexpr auto window_scale = 0x10;

constexpr int gp_register_count = 0x10;
constexpr int stack_depth = 0x10;
constexpr int total_keys = 0x10;

constexpr uint16_t character_set_load_address = 0x00;
} // namespace config

#endif
