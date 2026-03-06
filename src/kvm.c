#include "kvm.h"
#include "nhlog.h"
#include "opt.h"
#include "vcpu.h"
#include "vec.h"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

void kvm_init(kvm *vm, const options_t *options) {
  DEBUG("vm_init");
  memset(vm, 0, sizeof(kvm));

  vm->dev_fd = open(KVM_DEVICE, O_RDWR);
  if (vm->dev_fd < 0) {
    FATAL("failed to open %s:%s", KVM_DEVICE, strerror(errno));
    exit(-1);
  }

  vm->kvm_api_version = ioctl(vm->dev_fd, KVM_GET_API_VERSION, 0);
  INFO("KVM_VERSION = %d", vm->kvm_api_version);

  vm->vm_fd = ioctl(vm->dev_fd, KVM_CREATE_VM, 0);
  if (vm->vm_fd < 0) {
    FATAL("failed to kvm: %s", strerror(errno));
    exit(-1);
  }

  vm->ram_size = options->ram_size;
  INFO("RAM_SIZE = %d", options->ram_size);

  vm->ram_start =
      (intptr_t)mmap(NULL, DEFAULT_RAM_SIZE, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
  if (MAP_FAILED == (void *)vm->ram_start) {
    FATAL("failed to mmap: %s", strerror(errno));
    exit(-1);
  }

  vm->mem.slot = 0;
  vm->mem.guest_phys_addr = 0;
  vm->mem.memory_size = vm->ram_size;
  vm->mem.userspace_addr = vm->ram_start;

  int32_t ret = ioctl(vm->vm_fd, KVM_SET_USER_MEMORY_REGION, &vm->mem);
  if (ret < 0) {
    FATAL("failed setting userspace memory: %s", strerror(errno));
    exit(-1);
  }
}

void kvm_add_vcpu(kvm *vm, uint8_t vcpu_id, void *(*thread_fn)(void *)) {
  DEBUG("kvm_vcpu_init");
  vcpu_t *vcp = (vcpu_t *)malloc(sizeof(vcpu_t));
  if (NULL == vcp) {
    FATAL("Failed allocating memory for vcpu: %s", strerror(errno));
    exit(ENOMEM);
  }

  vcp->id = vcpu_id;
  vcp->fd = ioctl(vm->vm_fd, KVM_CREATE_VCPU, vcp->id);
  if (vcp->fd < 0) {
    FATAL("Failed to create vcpu: %s", strerror(errno));
    exit(-1);
  }

  INFO("Created cpu with id %d", vcp->id);

  vcp->kvm_run_mmap_size = ioctl(vm->dev_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
  if (vcp->kvm_run_mmap_size < 0) {
    FATAL("failed to get vcpu mmap size: %s", strerror(errno));
    exit(-1);
  }

  vcp->kvm_run = mmap(NULL, vcp->kvm_run_mmap_size, PROT_READ | PROT_WRITE,
                      MAP_SHARED, vcp->fd, 0);
  if (vcp->kvm_run == MAP_FAILED) {
    FATAL("failed to mmap: %s", strerror(errno));
    exit(-1);
  }

  vcp->vcpu_thread_func = thread_fn;
  vec_push(vcpu_t *, vm->cpus, vcp);
  return;
}

void kvm_run(kvm *vm) {
  for (size_t i = 0; i < vm->cpus.count; i++) {
    vcpu_t *cpu = vec_at(vm->cpus, i);
    if (0 != pthread_create(&cpu->thread, (const pthread_attr_t *)NULL,
                            cpu->vcpu_thread_func, vm)) {
      perror("Failed to create kvm thread");
      exit(1);
    }
  }

  for (size_t i = 0; i < vm->cpus.count; i++) {
    vcpu_t *cpu = vec_at(vm->cpus, i);
    pthread_join(cpu->thread, NULL);
  }
}

void kvm_destroy(kvm *vm) {
  DEBUG("vm_destroy");
  close(vm->dev_fd);
  close(vm->vm_fd);
  munmap((void *)vm->ram_start, vm->ram_start);
  for (size_t i = 0; i < vm->cpus.count; i++) {
    free(vec_at(vm->cpus, i));
  }
  vec_destroy(vm->cpus);
}
