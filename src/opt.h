#ifndef _OPT_H_
#define _OPT_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum : uint8_t {
  EXEC_MODE_REAL = 0,
  EXEC_MODE_PROTECTED,
  // EXEC_MODE_PAGED32,
  EXEC_MODE_LONG,
  EXEC_MODE_UNKNOWN = 255,
} ExecMode;

typedef struct {
  uint8_t vcpu_count;
  int32_t ram_size;
  uint8_t path[100];
  ExecMode exec_mode;
} options_t;

static ExecMode get_mode_from_str(char *m);

bool parse_args(options_t *options, int32_t argc, char *argv[]);

void show_help();

void dump_options(options_t *options);

#endif // _OPT_H_
