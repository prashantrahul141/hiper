#include "common.h"
#include "kvm.h"
#include "nhlog.h"
#include "opt.h"
#include "vcpu.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  nhlog_set_level(NHLOG_DEBUG);

  /* parse command line arguments */
  options_t options = {
      .vcpu_count = 1, .ram_size = DEFAULT_RAM_SIZE, .path = {0}};

  if (!parse_args(&options, argc, argv)) {
    return EXIT_FAILURE;
  }

  /* debug print options */
  dump_options(&options);

  kvm vm;
  kvm_init(&vm, &options);

  load_binary_file(&vm, options.path);

  kvm_add_vcpu(&vm, 0, vcpu_thread_fn);

  kvm_run(&vm);

  kvm_destroy(&vm);

  return EXIT_SUCCESS;
}
