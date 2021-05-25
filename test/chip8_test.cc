#include "chip8.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>

/*
TEST(Chip8_TEST, init) {
  struct chip8 chip8;
  chip8_init(&chip8);
  @@@
}
*/

TEST(Chip8_TEST, Memory) {
  struct chip8 chip8;
  chip8_init(&chip8);

  {
    ASSERT_EQ(chip8_memory_set(&chip8.memory, 0x69, 'D'), 0);

    ASSERT_EQ(chip8_memory_get(&chip8.memory, 0x69), 'D');
  }

  {
    ASSERT_EQ(chip8_memory_set(&chip8.memory, 0x69999, 'D'), -1);
    ASSERT_EQ(chip8_memory_set(&chip8.memory, -1, 'D'), -1);
  }
}

TEST(Chip8_TEST, Stack) {
  {
    struct chip8 chip8;
    chip8_init(&chip8);

    ASSERT_EQ(chip8.registers.sp, 0);

    chip8_stack_push(&chip8, 0xff);
    chip8_stack_push(&chip8, 0xaa);

    ASSERT_EQ(chip8_stack_pop(&chip8), 0xaa);
    ASSERT_EQ(chip8_stack_pop(&chip8), 0xff);
  }
  {
    struct chip8 chip8;
    chip8_init(&chip8);

    for (int i = 0; i < config::stack_depth; ++i) {
      chip8_stack_push(&chip8, i);
    }

    // TODO: test the next line
    // Maybe `push` should return `-1` instead of assert
    // ASSERT_EQ(chip8_stack_push(&chip8, 69), some error);
  }
}
