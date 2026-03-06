#include "common.h"
#include "kvm.h"
#include "nhlog.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void load_binary_file(kvm *vm, const uint8_t filepath[static 100]) {
  DEBUG("load_binary_file: %s", filepath);
  int32_t fd = open((char *)filepath, O_RDONLY);
  if (fd < 0) {
    FATAL("Opening binary file: %s", strerror(errno));
    exit(-1);
  }

  uint8_t *p = (uint8_t *)vm->ram_start;
  while (true) {
    int32_t ret = read(fd, p, 4096);
    if (ret <= 0) {
      break;
    }
    TRACE("read_size = %d", ret);
    p += ret;
  }
}
