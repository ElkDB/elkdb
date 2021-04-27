
#include <cstdlib>

char* duplicate_segment(const char* token, int token_length) {
  char* dup = (char*)malloc(token_length + 1);
  if (!dup) { /* handle memory allocation error */ }
  memcpy(dup, token, token_length);
  dup[token_length] = 0;
  return dup;
}
