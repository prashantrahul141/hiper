#include "vcpu.h"
#include "kvm.h"
#include "nhlog.h"

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

void vcpu_clean(vcpu *vcpu) {
  DEBUG("kvm_clean_vcpu");
  munmap(vcpu->kvm_run, vcpu->kvm_run_mmap_size);
  close(vcpu->fd);
}

void vcpu_reset(vcpus_t vcpus) {
  DEBUG("kvm_reset_cpus");
  for (size_t i = 0; i < vcpus.count; i++) {
    vcpu *vcpu = vec_at(vcpus, i);
    if (ioctl(vcpu->fd, KVM_GET_SREGS, &vcpu->sregs) < 0) {
      FATAL("failed to get sregs for vcpu = %d: %s", vcpu->id, strerror(errno));

      /* leaks memory? trust linux */
      exit(-1);
    }

    vcpu->sregs.cs.selector = VCPU_START_IP_LOC;
    vcpu->sregs.cs.base = VCPU_START_IP_LOC;
    vcpu->sregs.ss.selector = VCPU_START_IP_LOC;
    vcpu->sregs.ss.base = VCPU_START_IP_LOC;
    vcpu->sregs.ds.selector = VCPU_START_IP_LOC;
    vcpu->sregs.ds.base = VCPU_START_IP_LOC;
    vcpu->sregs.es.selector = VCPU_START_IP_LOC;
    vcpu->sregs.es.base = VCPU_START_IP_LOC;
    vcpu->sregs.fs.selector = VCPU_START_IP_LOC;
    vcpu->sregs.fs.base = VCPU_START_IP_LOC;
    vcpu->sregs.gs.selector = VCPU_START_IP_LOC;

    /* this is to force real mode */
    vcpu->sregs.cr0 &= ~1;                   // clear PE
    vcpu->sregs.cr0 &= ~((uint32_t)1 << 31); // clear PG

    if (ioctl(vcpu->fd, KVM_SET_SREGS, &vcpu->sregs) < 0) {
      FATAL("failed to set sregs for vcpu = %d: %s", vcpu->id, strerror(errno));

      /* leaks memory? trust linux */
      exit(-1);
    }

    /* 0000 0000 0000 0000 0101 0000 1101 1010 1010 0100 1110 1011 1010 1000
     * 0000 0000  */
    vcpu->regs.rflags = 0x0000000000000002ULL;
    vcpu->regs.rip = VCPU_START_IP_LOC;
    vcpu->regs.rsp = 0x8000;
    vcpu->regs.rbp = 0;

    if (ioctl(vcpu->fd, KVM_SET_REGS, &vcpu->regs) < 0) {
      FATAL("failed to set regs for vcpu = %d: %s", vcpu->id, strerror(errno));

      /* trust linux */
      exit(-1);
    }
  }
}

void *vcpu_thread_fn(void *buf) {
  DEBUG("kvm_cpu_thread");
  /* trust */
  kvm *vm = (kvm *)buf;
  int32_t ret = 0;
  vcpu_reset(vm->cpus);
  while (true) {
    INFO("kvm run");

    /* run each cpu */
    for (size_t i = 0; i < vm->cpus.count; i++) {
      vcpu *cpu = vec_at(vm->cpus, i);
      ret = ioctl(cpu->fd, KVM_RUN, 0);

      if (ret < 0) {
        FATAL("Failed to run kvm");
        exit(0); /* trust linux */
      }

      switch (cpu->kvm_run->exit_reason) {
      case KVM_EXIT_UNKNOWN: {
        DEBUG("KVM_EXIT_UNKNOWN");
        break;
      }
      case KVM_EXIT_DEBUG: {
        DEBUG("KVM_EXIT_DEBUG");
        break;
      }
      case KVM_EXIT_IO: {
        DEBUG("KVM_EXIT_IO");
        INFO(
            "outport: %d, data: %d", cpu->kvm_run->io.port,
            *(int *)((uint8_t *)(cpu->kvm_run) + cpu->kvm_run->io.data_offset));
        sleep(1);
        break;
      }
      case KVM_EXIT_MMIO:
        DEBUG("KVM_EXIT_MMIO");
        return 0;
      case KVM_EXIT_INTR:
        DEBUG("KVM_EXIT_INTR");
        return 0;
      case KVM_EXIT_SHUTDOWN:
        printf("KVM_EXIT_SHUTDOWN");
        return 0;
      default:
        printf("KVM PANIC\n");
        return 0;
      }
    }
  }

  return NULL;
}
