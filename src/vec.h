/*
 * https://gist.github.com/prashantrahul141/85270ce3c9b2889c5ed2d54025902bdb
 *
 * Usage Example:
 *   // first create a new vector with vec_t giving the type of vector.
 *   vec_t(int) vector;
 *
 *   // The vector has 3 fields:
 *   // count - the number of elements currently present in the vector
 *   // capacity - total capacity of the vector
 *   // data - pointer to the actual vector data
 *
 *   // initialize it using vec_init()
 *   vec_init(vector);
 *
 *   // Elements can be pushed at the end of the vector
 *   // using vec_push, this will automatically resize
 *   // the vector if needed.
 *   vec_push(int, vector, 23);
 *
 *   // vec_at can be used to retrieve element at the given index
 *   printf("Element at position 0 = %d\n", vec_at(vector, 0));
 *
 *   // vec_at can be also used to assign value at a certain index
 *   // in the vector.
 *   // WARN: using vec_at to assign values doesn't increment count value.
 *   // WARN: there is nothing stopping you from writing at an index
 *   // out of bounds
 *   vec_at(vector, 0) = 1;
 *   vec_at(vector, 2) = 1;
 *
 *   // vec_resize can be used to manually resize vectors.
 *   // WARN: this doesn't increment the count value, only the capacity.
 *   vec_resize(int, vector, 10);
 *
 *   // iteration can be be done easily like so:
 *   for (size_t i = 0; i < vector.count; i++) {
 *     printf("Element at %zu = %d\n", i, vec_at(vector, i));
 *   }
 *
 *   // vectors can be copied from from to another
 *   vec_copy(int, dest_vec, src_vec);
 *
 *   // finally destroy the vector using vec_destroy
 *   vec_destroy(array);
 *
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VEC_H_
#define _VEC_H_

#include <stdlib.h>
#include <string.h>

#define vec_t(type)                                                            \
  struct {                                                                     \
    size_t count, capacity;                                                    \
    type *data;                                                                \
  }

#define vec_init(vec) ((vec).count = 0, (vec).capacity = 0, (vec).data = 0)
#define vec_at(v, i) ((v).data[(i)])
#define vec_push(type, vec, value)                                             \
  do {                                                                         \
    if ((vec).count == (vec).capacity) {                                       \
      (vec).capacity = (vec).capacity ? (vec).capacity << 1 : 2;               \
      (vec).data = (type *)realloc((vec).data, sizeof(type) * (vec).capacity); \
    }                                                                          \
    (vec).data[(vec).count++] = (value);                                       \
  } while (0)

#define vec_pop(vec) ((vec).data[--(vec).count])
#define vec_size(vec) ((vec).count)
#define vec_max(vec) ((vec).capacity)
#define vec_resize(type, vec, size)                                            \
  ((vec).capacity = (size),                                                    \
   (vec).data = (type *)realloc((vec).data, sizeof(type) * (vec).capacity))

#define vec_copy(type, to_vec, from_vec)                                       \
  do {                                                                         \
    if ((to_vec).capacity < (from_vec).count) {                                \
      vec_resize(type, to_vec, (from_vec).count);                              \
    }                                                                          \
    (to_vec).count = (from_vec).count;                                         \
    memcpy((to_vec).data, (from_vec).data, sizeof(type) * (from_vec).count);   \
  } while (0)

#define vec_destroy(vec) ((vec).capacity = (vec).count = 0, free((vec).data))

#endif // _VEC_H_

#ifdef __cplusplus
}
#endif
