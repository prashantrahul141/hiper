#include "opt.h"
#include "nhlog.h"

#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char program_name[20] = {0};

bool parse_args(options_t *options, int32_t argc, char *argv[]) {
  memcpy(program_name, basename(argv[0]), strlen(basename(argv[0])));

  int opt = -1;
  while ((opt = getopt(argc, argv, "hv:r:p:")) != -1) {
    switch (opt) {
    case 'h': {
      TRACE("help command");
      show_help();
      exit(0);
    }

    case 'v': {
      TRACE("setting vcpu count = %s", optarg);
      options->vcpu_count = atoi(optarg);
      break;
    }

    case 'r': {
      TRACE("setting ram size = %s", optarg);
      options->ram_size = atoi(optarg);
      break;
    }

    default: {
      WARN("unknown option: %c", opt);
      fprintf(stderr, "unknown option : %c\n", opt);
      return false;
    }
    }
  }

  if (optind >= argc) {
    WARN("Missing file argument");
    fprintf(stderr, "Missing file argument\n");
    show_help();
    return false;
  }

  TRACE("setting file path = %s", argv[optind]);
  memcpy(options->path, argv[optind], strlen(argv[optind]));
  return true;
}

void dump_options(options_t *options) {
  TRACE("vcpu_count = %d", options->vcpu_count);
  TRACE("ram_size = %d", options->ram_size);
  TRACE("path = %s", options->path);
}

void show_help() {
  TRACE("printing help message");
  printf("USAGE:\n");
  printf("\t%s [OPTIONS] <FILE>\n", program_name);
  printf("\nOPTIONS:\n");
  printf("\t-v <number>\tVirtual cpu count\n");
  printf("\t-r <number>\tRam size (in bytes)\n");
  printf("\t-h         \tPrint help message\n");
  printf("\n");
}
