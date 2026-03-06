#ifndef _OPT_H_
#define _OPT_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t vcpu_count;
  int32_t ram_size;
  uint8_t path[100];
} options_t;

bool parse_args(options_t *options, int32_t argc, char *argv[]);

void show_help();

void dump_options(options_t *options);

#endif // _OPT_H_
