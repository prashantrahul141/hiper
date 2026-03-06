#ifndef _COMMON_H_
#define _COMMON_H_

#include "kvm.h"

void load_binary_file(kvm *vm, const uint8_t filepath[static 100]);

#endif // _COMMON_H_
