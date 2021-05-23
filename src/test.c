#include <stdio.h>
#include <stdlib.h>
typedef union event {
  int val;
  char ch;
} event;

void assignEvent(event *e);
int main(void) {
  for (int i = 0; i < 20; ++i) {
    event e;
    printf("type %d\n", e.val);
    assignEvent(&e);
  }
}

void assignEvent(event *e) {
  if ((double)rand() / (double)RAND_MAX < 0.5)
    e->val = (double)rand() / RAND_MAX * 100;
  else
    printf("unchanged %d\n", e->val);
}
