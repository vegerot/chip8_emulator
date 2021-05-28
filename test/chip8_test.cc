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

TEST(Chip8_Test, GetWrappedIndex) {
  int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int size = sizeof arr / sizeof arr[0];
  // trivial
  EXPECT_EQ(get_wrapped_index(8, size), 8);
  EXPECT_EQ(arr[get_wrapped_index(8, size)], 8);

  // offset > length
  EXPECT_EQ(get_wrapped_index(18, size), 8);
  EXPECT_EQ(get_wrapped_index(80, size), 0);
  EXPECT_EQ(get_wrapped_index(88, size), 8);

  EXPECT_EQ(arr[get_wrapped_index(18, size)], 8);
  EXPECT_EQ(arr[get_wrapped_index(80, size)], 0);
  EXPECT_EQ(arr[get_wrapped_index(88, size)], 8);
  // offset < length
  // EXPECT_EQEXPECT_EQ(get_wrapped_index(-1, 10), 9);
  // EXPECT_EQ(get_wrapped_index(-(size - 1), 10), 1);

  EXPECT_EQ(arr[get_wrapped_index(-1, 10)], 9);
  EXPECT_EQ(arr[get_wrapped_index(-(9), 10)], 1);
  EXPECT_EQ(arr[get_wrapped_index(-16, 10)], 4);
  EXPECT_EQ(arr[get_wrapped_index(-15, 10)], 5);
  EXPECT_EQ(arr[get_wrapped_index(-1600, 10)], 0);
  EXPECT_EQ(arr[get_wrapped_index(-1609, 10)], 1);

  EXPECT_EQ(get_wrapped_index(-9, 1), 0);
  EXPECT_EQ(get_wrapped_index(-9, 2), 1);
}
