#ifndef DATATYPE_H
#define DATATYPE_H

#include <stdint.h>
#include <stdlib.h>

// 整数
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
#ifdef HAS_INT64
typedef uint64_t u64;
typedef int64_t s64;
typedef volatile u64 vu64;
typedef volatile s64 vs64;
#endif
// 浮点数
typedef float f32;
typedef double f64;
typedef volatile f32 vf32;
typedef volatile f64 vf64;
// size
typedef size_t usize;
typedef volatile size_t vsize;


#endif // !DATATYPE_H
