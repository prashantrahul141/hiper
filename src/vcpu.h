#ifndef _VCPU_H_
#define _VCPU_H_

#include "vec.h"

#include <linux/kvm.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>

/* vcpu */
typedef struct {
  int32_t id;
  int32_t fd;
  pthread_t thread;
  struct kvm_run *kvm_run;
  int32_t kvm_run_mmap_size;
  struct kvm_regs regs;
  struct kvm_sregs sregs;
  void *(*vcpu_thread_func)(void *);
} vcpu_t;

typedef vec_t(vcpu_t *) vcpus_t;

void vcpu_clean(vcpu_t *vcpu);

void vcpu_reset(vcpu_t *vcpu);

void *vcpu_thread_fn(void *buf);

#endif // _VCPU_H_
