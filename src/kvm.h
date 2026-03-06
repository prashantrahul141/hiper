#ifndef _KVM_H_
#define _KVM_H_

#include "opt.h"
#include "vcpu.h"

#include <linux/kvm.h>
#include <stddef.h>
#include <stdint.h>

#define KVM_DEVICE "/dev/kvm"
#define DEFAULT_RAM_SIZE (512 * 1024 * 1024) /* 512MB */
#define VCPU_START_IP_LOC                                                      \
  (0x00) /* vcpu start location, see linker script for guest */

/* vm */
typedef struct {
  int32_t dev_fd;
  int32_t vm_fd;
  size_t ram_size;
  intptr_t ram_start;
  int32_t kvm_api_version;
  struct kvm_userspace_memory_region mem;
  vcpus_t cpus;
} kvm;

void kvm_init(kvm *vm, const options_t *options);
void kvm_add_vcpu(kvm *vm, uint8_t vcpu_id, void *(*thread_fn)(void *));
void kvm_run(kvm *vm);
void kvm_destroy(kvm *vm);

#endif // _KVM_H_
