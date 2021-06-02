#include "chip8.hpp"
#include "fixtures.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>

/*
TEST(Chip8_TEST_hw, init) {
  struct chip8 chip8;
  chip8_init(&chip8);
  @@@
}
*/

TEST(Chip8_TEST_hw, Memory) {
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

TEST(Chip8_TEST_hw, Stack) {
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

TEST(Chip8_TEST_hw, GetWrappedIndex) {
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

TEST(Chip8_TEST_is, JP) {

  struct chip8 chip8;
  chip8_init(&chip8);

  chip8_exec(&chip8, 0x1ff2);

  EXPECT_EQ(chip8.registers.pc, 0xff2);
}

TEST(Chip8_TEST_is, SEByte) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8_exec(&chip8, 0x3022);

    EXPECT_EQ(chip8.registers.pc, 0x0);
  }

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0x00] = 0x22;

    chip8_exec(&chip8, 0x3022);

    EXPECT_EQ(chip8.registers.pc, 0x2);
  }
}

TEST(Chip8_TEST_is, SNE) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.V[0x00] = 0x23;
    chip8_exec(&chip8, 0x4022);

    EXPECT_EQ(chip8.registers.pc, 0x2);
  }

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0x00] = 0x22;

    chip8_exec(&chip8, 0x4022);

    EXPECT_EQ(chip8.registers.pc, 0x0);
  }
}

TEST(Chip8_TEST_is, SE) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0x00] = 0x22;
    chip8.registers.V[0x04] = 0x22;

    chip8_exec(&chip8, 0x5040);

    EXPECT_EQ(chip8.registers.pc, 0x2);
  }

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0x00] = 0x22;
    chip8.registers.V[0x04] = 0x69;

    chip8_exec(&chip8, 0x5040);

    EXPECT_EQ(chip8.registers.pc, 0x0);
  }
}

TEST(Chip8_TEST_is, LD1) {

  struct chip8 chip8;
  chip8_init(&chip8);

  chip8.registers.V[0xc] = 0x00;

  chip8_exec(&chip8, 0x6c69);

  EXPECT_EQ(chip8.registers.V[0xc], 0x69);
}

TEST(Chip8_TEST_is, ADD) {

  struct chip8 chip8;
  chip8_init(&chip8);

  chip8.registers.V[0xd] = 0x22;

  chip8_exec(&chip8, 0x7d22);

  EXPECT_EQ(chip8.registers.V[0xd], 0x44);
}

TEST(Chip8_TEST_is, LD2) {

  struct chip8 chip8;
  chip8_init(&chip8);

  chip8.registers.V[0xa] = 0x0;
  chip8.registers.V[0xb] = 0x42;

  chip8_exec(&chip8, 0x8ab0);

  EXPECT_EQ(chip8.registers.V[0xa], 0x42);
}

TEST(Chip8_TEST_is, OR) {

  struct chip8 chip8;
  chip8_init(&chip8);

  chip8.registers.V[0xa] = 0x22;
  chip8.registers.V[0xb] = 0x42;

  chip8_exec(&chip8, 0x8ab1);

  EXPECT_EQ(chip8.registers.V[0xa], 0x42 | 0x22);
}

TEST(Chip8_TEST_is, AND) {

  struct chip8 chip8;
  chip8_init(&chip8);

  chip8.registers.V[0xa] = 0x22;
  chip8.registers.V[0xb] = 0x42;

  chip8_exec(&chip8, 0x8ab2);

  EXPECT_EQ(chip8.registers.V[0xa], 0x42 & 0x22);
}

TEST(Chip8_TEST_is, XOR) {

  struct chip8 chip8;
  chip8_init(&chip8);

  chip8.registers.V[0xa] = 0x22;
  chip8.registers.V[0xb] = 0x42;

  chip8_exec(&chip8, 0x8ab3);

  EXPECT_EQ(chip8.registers.V[0xa], 0x42 ^ 0x22);
}

TEST(Chip8_TEST_is, ADD2) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0x22;
    chip8.registers.V[0xb] = 0x42;

    chip8_exec(&chip8, 0x8ab4);

    EXPECT_EQ(chip8.registers.V[0xa], 0x22 + 0x42);
  }
  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0x22;
    chip8.registers.V[0xb] = 0xF0;

    chip8_exec(&chip8, 0x8ab4);

    EXPECT_EQ(chip8.registers.V[0xa], 0x12);
    EXPECT_EQ(chip8.registers.V[0xF], 0x1);
  }
}

TEST(Chip8_TEST_is, SUB) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0x22;
    chip8.registers.V[0xb] = 0x42;

    chip8_exec(&chip8, 0x8ab5);

    // ?? EXPECT_EQ(chip8.registers.V[0xa], 0x22 - 0x42);
    EXPECT_EQ(chip8.registers.V[0x0F], 0x0);
  }
  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0xF0;
    chip8.registers.V[0xb] = 0x20;

    chip8_exec(&chip8, 0x8ab5);

    EXPECT_EQ(chip8.registers.V[0xa], 0xF0 - 0x20);
    EXPECT_EQ(chip8.registers.V[0xF], 0x1);
  }
}

TEST(Chip8_TEST_is, SHR) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0x22;

    chip8_exec(&chip8, 0x8ab6);

    EXPECT_EQ(chip8.registers.V[0xa], 0x11);
    EXPECT_EQ(chip8.registers.V[0x0F], 0x0);
  }
  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0x11;

    chip8_exec(&chip8, 0x8ab6);

    EXPECT_EQ(chip8.registers.V[0xa], 8);
    EXPECT_EQ(chip8.registers.V[0xF], 0x1);
  }
}

TEST(Chip8_TEST_is, SUBN) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0x22;
    chip8.registers.V[0xb] = 0x42;

    chip8_exec(&chip8, 0x8ab7);

    EXPECT_EQ(chip8.registers.V[0xa], 0x42 - 0x22);
    EXPECT_EQ(chip8.registers.V[0x0F], 0x1);
  }
  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0xF0;
    chip8.registers.V[0xb] = 0x20;

    chip8_exec(&chip8, 0x8ab7);

    // ?? EXPECT_EQ(chip8.registers.V[0xa], 0xF0 - 0x20);
    EXPECT_EQ(chip8.registers.V[0xF], 0x0);
  }
}

TEST(Chip8_TEST_is, SHL) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0x7f;

    chip8_exec(&chip8, 0x8abE);

    EXPECT_EQ(chip8.registers.V[0xa], 0x7f << 1);
    EXPECT_EQ(chip8.registers.V[0x0F], 0x0);
  }
  {
    struct chip8 chip8;
    chip8_init(&chip8);

    chip8.registers.V[0xa] = 0xFF;

    chip8_exec(&chip8, 0x8abE);

    EXPECT_EQ(chip8.registers.V[0xa], 0xfe);
    EXPECT_EQ(chip8.registers.V[0xF], 0x1);
  }
}

TEST(Chip8_TEST_is, SNE2) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.V[0x00] = 0x23;
    chip8.registers.V[0x01] = 0x23;
    chip8_exec(&chip8, 0x9010);

    EXPECT_EQ(chip8.registers.pc, 0x0);
  }

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.V[0x00] = 0x23;
    chip8.registers.V[0x01] = 0x24;
    chip8_exec(&chip8, 0x9010);

    EXPECT_EQ(chip8.registers.pc, 0x2);
  }
}

TEST(Chip8_TEST_is, LD3) {

  struct chip8 chip8;
  chip8_init(&chip8);
  chip8.registers.I = 0;
  chip8_exec(&chip8, 0xA123);

  EXPECT_EQ(chip8.registers.I, 0x123);
}

TEST(Chip8_TEST_is, JP2) {

  struct chip8 chip8;
  chip8_init(&chip8);
  chip8.registers.V[0] = 0x69;
  chip8.registers.pc = 0;
  chip8_exec(&chip8, 0xB420);

  EXPECT_EQ(chip8.registers.pc, 0x69 + 0x420);
}

TEST(Chip8_TEST_is, RND) {

  struct chip8 chip8;
  chip8_init(&chip8);
  chip8.registers.V[0x0] = 0x69;
  chip8_exec(&chip8, 0xC021);

  // ?? EXPECT_EQ(chip8.registers.V[0], // ?? 0x69 + 0x420);
}

TEST(Chip8_TEST_is, DRW) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    for (size_t row = 0; row < config::height; ++row) {
      for (size_t col = 0; col < config::width; ++col) {
        EXPECT_EQ(chip8.screen.pixels[row][col],
                  chip8_fixtures::blankScreen[row][col]);
      }
    }
  }

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.I = 0;
    chip8.registers.V[0] = 3;
    chip8.registers.V[1] = 4;

    chip8_exec(&chip8, 0xD015);

    for (size_t row = 0; row < config::height; ++row) {
      for (size_t col = 0; col < config::width; ++col) {
        EXPECT_EQ(chip8.screen.pixels[row][col],
                  chip8_fixtures::zero[row][col]);
      }
    }
    EXPECT_EQ(chip8.registers.V[0xF], 0);
  }

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.I = 0;
    chip8.registers.V[0] = config::height - 2;
    chip8.registers.V[1] = config::width - 2;

    chip8_exec(&chip8, 0xD015);

    for (size_t row = 0; row < config::height; ++row) {
      for (size_t col = 0; col < config::width; ++col) {
        EXPECT_EQ(chip8.screen.pixels[row][col],
                  chip8_fixtures::wrap[row][col]);
      }
    }
    EXPECT_EQ(chip8.registers.V[0xF], 0);
  }
  /*
  printf("{\n");
  for (size_t row = 0; row < config::height; ++row) {
    printf("{ ");
    for (size_t col = 0; col < config::width; ++col) {
      printf("%i, ", chip8.screen.pixels[row][col]);
    }
    printf("},\n");
  }
  printf("}\n");
  */
}

TEST(Chip8_TEST_is, SKP) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.pc = 0;
    chip8.registers.V[0] = 2;
    chip8_keyboard_up(&chip8.keyboard, 2);
    chip8_exec(&chip8, 0xE09E);

    EXPECT_EQ(chip8.registers.pc, 0);
  }

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.pc = 0;
    chip8.registers.V[0] = 2;
    chip8_keyboard_down(&chip8.keyboard, 2);
    chip8_exec(&chip8, 0xE09E);

    EXPECT_EQ(chip8.registers.pc, 2);
  }
}

TEST(Chip8_TEST_is, SKNP) {

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.pc = 0;
    chip8.registers.V[0] = 0;
    chip8_exec(&chip8, 0xE0A1);

    EXPECT_EQ(chip8.registers.pc, 2);
  }

  {
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.pc = 0;
    chip8.registers.V[0] = 2;
    chip8_keyboard_down(&chip8.keyboard, 2);
    chip8_exec(&chip8, 0xE0A1);

    EXPECT_EQ(chip8.registers.pc, 0);
  }
}
