#include "astc.h"

#include <softfloat.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

// Enums copied from GL/GL.h
#define GL_RED 0x1903
#define GL_RG 0x8227
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A

#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_SHORT 0x1403
#define GL_HALF_FLOAT 0x140B
#define GL_FLOAT 0x1406

enum scanline_copy_method {
  R8_TO_RGBA8,
  RG8_TO_RGBA8,
  RGB8_TO_RGBA8,
  RGBA8_TO_RGBA8,
  BGR8_TO_RGBA8,
  BGRA8_TO_RGBA8,
  L8_TO_RGBA8,
  LA8_TO_RGBA8,

  RGBX8_TO_RGBA8,
  BGRX8_TO_RGBA8,

  R16_TO_RGBA16F,
  RG16_TO_RGBA16F,
  RGB16_TO_RGBA16F,
  RGBA16_TO_RGBA16F,
  BGR16_TO_RGBA16F,
  BGRA16_TO_RGBA16F,
  L16_TO_RGBA16F,
  LA16_TO_RGBA16F,

  R16F_TO_RGBA16F,
  RG16F_TO_RGBA16F,
  RGB16F_TO_RGBA16F,
  RGBA16F_TO_RGBA16F,
  BGR16F_TO_RGBA16F,
  BGRA16F_TO_RGBA16F,
  L16F_TO_RGBA16F,
  LA16F_TO_RGBA16F,

  R32F_TO_RGBA16F,
  RG32F_TO_RGBA16F,
  RGB32F_TO_RGBA16F,
  RGBA32F_TO_RGBA16F,
  BGR32F_TO_RGBA16F,
  BGRA32F_TO_RGBA16F,
  L32F_TO_RGBA16F,
  LA32F_TO_RGBA16F
};

int GetNumComponentsFromGlFormat(uint32_t gl_format) {
  int components;
  switch (gl_format) {
    case GL_RED:
      components = 1;
      break;
    case GL_RG:
      components = 2;
      break;
    case GL_RGB:
      components = 3;
      break;
    case GL_RGBA:
      components = 4;
      break;
    case GL_BGR:
      components = 3;
      break;
    case GL_BGRA:
      components = 4;
      break;
    case GL_LUMINANCE:
      components = 1;
      break;
    case GL_LUMINANCE_ALPHA:
      components = 2;
      break;
    default:
      assert(false);
      return 0;
  };
  return components;
}

void GetBitnessFromGlType(uint32_t gl_type, int& bitness,
                          int& bytes_per_component) {
  switch (gl_type) {
    case GL_UNSIGNED_BYTE: {
      bitness = 8;
      bytes_per_component = 1;
    }
    case GL_UNSIGNED_SHORT: {
      bitness = 16;
      bytes_per_component = 2;
      break;
    }
    case GL_HALF_FLOAT: {
      bitness = 16;
      bytes_per_component = 2;
      break;
    }
    case GL_FLOAT: {
      bitness = 16;
      bytes_per_component = 4;
      break;
    }
    default:
      assert(false);
  }
}

scanline_copy_method GetScanlineCopyMethod(uint32_t gl_type,
                                           uint32_t gl_format) {
  switch (gl_type) {
    case GL_UNSIGNED_BYTE: {
      switch (gl_format) {
        case GL_RED:
          return R8_TO_RGBA8;
          break;
        case GL_RG:
          return RG8_TO_RGBA8;
          break;
        case GL_RGB:
          return RGB8_TO_RGBA8;
          break;
        case GL_RGBA:
          return RGBA8_TO_RGBA8;
          break;
        case GL_BGR:
          return BGR8_TO_RGBA8;
          break;
        case GL_BGRA:
          return BGRA8_TO_RGBA8;
          break;
        case GL_LUMINANCE:
          return L8_TO_RGBA8;
          break;
        case GL_LUMINANCE_ALPHA:
          return LA8_TO_RGBA8;
          break;
      }
      break;
    }
    case GL_UNSIGNED_SHORT: {
      switch (gl_format) {
        case GL_RED:
          return R16_TO_RGBA16F;
          break;
        case GL_RG:
          return RG16_TO_RGBA16F;
          break;
        case GL_RGB:
          return RGB16_TO_RGBA16F;
          break;
        case GL_RGBA:
          return RGBA16_TO_RGBA16F;
          break;
        case GL_BGR:
          return BGR16_TO_RGBA16F;
          break;
        case GL_BGRA:
          return BGRA16_TO_RGBA16F;
          break;
        case GL_LUMINANCE:
          return L16_TO_RGBA16F;
          break;
        case GL_LUMINANCE_ALPHA:
          return LA16_TO_RGBA16F;
          break;
      }
      break;
    }
    case GL_HALF_FLOAT: {
      switch (gl_format) {
        case GL_RED:
          return R16F_TO_RGBA16F;
          break;
        case GL_RG:
          return RG16F_TO_RGBA16F;
          break;
        case GL_RGB:
          return RGB16F_TO_RGBA16F;
          break;
        case GL_RGBA:
          return RGBA16F_TO_RGBA16F;
          break;
        case GL_BGR:
          return BGR16F_TO_RGBA16F;
          break;
        case GL_BGRA:
          return BGRA16F_TO_RGBA16F;
          break;
        case GL_LUMINANCE:
          return L16F_TO_RGBA16F;
          break;
        case GL_LUMINANCE_ALPHA:
          return LA16F_TO_RGBA16F;
          break;
      }
      break;
    }
    case GL_FLOAT: {
      switch (gl_format) {
        case GL_RED:
          return R32F_TO_RGBA16F;
          break;
        case GL_RG:
          return RG32F_TO_RGBA16F;
          break;
        case GL_RGB:
          return RGB32F_TO_RGBA16F;
          break;
        case GL_RGBA:
          return RGBA32F_TO_RGBA16F;
          break;
        case GL_BGR:
          return BGR32F_TO_RGBA16F;
          break;
        case GL_BGRA:
          return BGRA32F_TO_RGBA16F;
          break;
        case GL_LUMINANCE:
          return L32F_TO_RGBA16F;
          break;
        case GL_LUMINANCE_ALPHA:
          return LA32F_TO_RGBA16F;
          break;
      }
      break;
    }
    default:
      assert(false);
  }
  return R8_TO_RGBA8;
}

// scanline copying function: this function expands data to RGBA, either U8 or
// FP16.

static void copy_scanline(void* dst, const void* src, int pixels, int method) {
#define id(x) (x)
#define u16_sf16(x) float_to_sf16(x*(1.0f / 65535.0f), SF_NEARESTEVEN)
#define f32_sf16(x) sf32_to_sf16(x, SF_NEARESTEVEN)

#define COPY_R(dsttype, srctype, convfunc, oneval) \
  do {                                             \
    srctype* s = (srctype*)src;                    \
    dsttype* d = (dsttype*)dst;                    \
    for (i = 0; i < pixels; i++) {                 \
      d[4 * i] = convfunc(s[i]);                   \
      d[4 * i + 1] = 0;                            \
      d[4 * i + 2] = 0;                            \
      d[4 * i + 3] = oneval;                       \
    }                                              \
  } while (0);                                     \
  break;

#define COPY_RG(dsttype, srctype, convfunc, oneval) \
  do {                                              \
    srctype* s = (srctype*)src;                     \
    dsttype* d = (dsttype*)dst;                     \
    for (i = 0; i < pixels; i++) {                  \
      d[4 * i] = convfunc(s[2 * i]);                \
      d[4 * i + 1] = convfunc(s[2 * i + 1]);        \
      d[4 * i + 2] = 0;                             \
      d[4 * i + 3] = oneval;                        \
    }                                               \
  } while (0);                                      \
  break;

#define COPY_RGB(dsttype, srctype, convfunc, oneval) \
  do {                                               \
    srctype* s = (srctype*)src;                      \
    dsttype* d = (dsttype*)dst;                      \
    for (i = 0; i < pixels; i++) {                   \
      d[4 * i] = convfunc(s[3 * i]);                 \
      d[4 * i + 1] = convfunc(s[3 * i + 1]);         \
      d[4 * i + 2] = convfunc(s[3 * i + 2]);         \
      d[4 * i + 3] = oneval;                         \
    }                                                \
  } while (0);                                       \
  break;

#define COPY_BGR(dsttype, srctype, convfunc, oneval) \
  do {                                               \
    srctype* s = (srctype*)src;                      \
    dsttype* d = (dsttype*)dst;                      \
    for (i = 0; i < pixels; i++) {                   \
      d[4 * i] = convfunc(s[3 * i + 2]);             \
      d[4 * i + 1] = convfunc(s[3 * i + 1]);         \
      d[4 * i + 2] = convfunc(s[3 * i]);             \
      d[4 * i + 3] = oneval;                         \
    }                                                \
  } while (0);                                       \
  break;

#define COPY_RGBX(dsttype, srctype, convfunc, oneval) \
  do {                                                \
    srctype* s = (srctype*)src;                       \
    dsttype* d = (dsttype*)dst;                       \
    for (i = 0; i < pixels; i++) {                    \
      d[4 * i] = convfunc(s[4 * i]);                  \
      d[4 * i + 1] = convfunc(s[4 * i + 1]);          \
      d[4 * i + 2] = convfunc(s[4 * i + 2]);          \
      d[4 * i + 3] = oneval;                          \
    }                                                 \
  } while (0);                                        \
  break;

#define COPY_BGRX(dsttype, srctype, convfunc, oneval) \
  do {                                                \
    srctype* s = (srctype*)src;                       \
    dsttype* d = (dsttype*)dst;                       \
    for (i = 0; i < pixels; i++) {                    \
      d[4 * i] = convfunc(s[4 * i + 2]);              \
      d[4 * i + 1] = convfunc(s[4 * i + 1]);          \
      d[4 * i + 2] = convfunc(s[4 * i]);              \
      d[4 * i + 3] = oneval;                          \
    }                                                 \
  } while (0);                                        \
  break;

#define COPY_RGBA(dsttype, srctype, convfunc, oneval) \
  do {                                                \
    srctype* s = (srctype*)src;                       \
    dsttype* d = (dsttype*)dst;                       \
    for (i = 0; i < pixels; i++) {                    \
      d[4 * i] = convfunc(s[4 * i]);                  \
      d[4 * i + 1] = convfunc(s[4 * i + 1]);          \
      d[4 * i + 2] = convfunc(s[4 * i + 2]);          \
      d[4 * i + 3] = convfunc(s[4 * i + 3]);          \
    }                                                 \
  } while (0);                                        \
  break;

#define COPY_BGRA(dsttype, srctype, convfunc, oneval) \
  do {                                                \
    srctype* s = (srctype*)src;                       \
    dsttype* d = (dsttype*)dst;                       \
    for (i = 0; i < pixels; i++) {                    \
      d[4 * i] = convfunc(s[4 * i + 2]);              \
      d[4 * i + 1] = convfunc(s[4 * i + 1]);          \
      d[4 * i + 2] = convfunc(s[4 * i]);              \
      d[4 * i + 3] = convfunc(s[4 * i + 3]);          \
    }                                                 \
  } while (0);                                        \
  break;

#define COPY_L(dsttype, srctype, convfunc, oneval) \
  do {                                             \
    srctype* s = (srctype*)src;                    \
    dsttype* d = (dsttype*)dst;                    \
    for (i = 0; i < pixels; i++) {                 \
      d[4 * i] = convfunc(s[i]);                   \
      d[4 * i + 1] = convfunc(s[i]);               \
      d[4 * i + 2] = convfunc(s[i]);               \
      d[4 * i + 3] = oneval;                       \
    }                                              \
  } while (0);                                     \
  break;

#define COPY_LA(dsttype, srctype, convfunc, oneval) \
  do {                                              \
    srctype* s = (srctype*)src;                     \
    dsttype* d = (dsttype*)dst;                     \
    for (i = 0; i < pixels; i++) {                  \
      d[4 * i] = convfunc(s[2 * i]);                \
      d[4 * i + 1] = convfunc(s[2 * i]);            \
      d[4 * i + 2] = convfunc(s[2 * i]);            \
      d[4 * i + 3] = convfunc(s[2 * i + 1]);        \
    }                                               \
  } while (0);                                      \
  break;

  int i;
  switch (method) {
    case R8_TO_RGBA8:
      COPY_R(uint8_t, uint8_t, id, 0xFF);
    case RG8_TO_RGBA8:
      COPY_RG(uint8_t, uint8_t, id, 0xFF);
    case RGB8_TO_RGBA8:
      COPY_RGB(uint8_t, uint8_t, id, 0xFF);
    case RGBA8_TO_RGBA8:
      COPY_RGBA(uint8_t, uint8_t, id, 0xFF);
    case BGR8_TO_RGBA8:
      COPY_BGR(uint8_t, uint8_t, id, 0xFF);
    case BGRA8_TO_RGBA8:
      COPY_BGRA(uint8_t, uint8_t, id, 0xFF);
    case RGBX8_TO_RGBA8:
      COPY_RGBX(uint8_t, uint8_t, id, 0xFF);
    case BGRX8_TO_RGBA8:
      COPY_BGRX(uint8_t, uint8_t, id, 0xFF);
    case L8_TO_RGBA8:
      COPY_L(uint8_t, uint8_t, id, 0xFF);
    case LA8_TO_RGBA8:
      COPY_LA(uint8_t, uint8_t, id, 0xFF);

    case R16F_TO_RGBA16F:
      COPY_R(uint16_t, uint16_t, id, 0x3C00);
    case RG16F_TO_RGBA16F:
      COPY_RG(uint16_t, uint16_t, id, 0x3C00);
    case RGB16F_TO_RGBA16F:
      COPY_RGB(uint16_t, uint16_t, id, 0x3C00);
    case RGBA16F_TO_RGBA16F:
      COPY_RGBA(uint16_t, uint16_t, id, 0x3C00);
    case BGR16F_TO_RGBA16F:
      COPY_BGR(uint16_t, uint16_t, id, 0x3C00);
    case BGRA16F_TO_RGBA16F:
      COPY_BGRA(uint16_t, uint16_t, id, 0x3C00);
    case L16F_TO_RGBA16F:
      COPY_L(uint16_t, uint16_t, id, 0x3C00);
    case LA16F_TO_RGBA16F:
      COPY_LA(uint16_t, uint16_t, id, 0x3C00);

    case R16_TO_RGBA16F:
      COPY_R(uint16_t, uint16_t, u16_sf16, 0x3C00);
    case RG16_TO_RGBA16F:
      COPY_RG(uint16_t, uint16_t, u16_sf16, 0x3C00);
    case RGB16_TO_RGBA16F:
      COPY_RGB(uint16_t, uint16_t, u16_sf16, 0x3C00);
    case RGBA16_TO_RGBA16F:
      COPY_RGBA(uint16_t, uint16_t, u16_sf16, 0x3C00);
    case BGR16_TO_RGBA16F:
      COPY_BGR(uint16_t, uint16_t, u16_sf16, 0x3C00);
    case BGRA16_TO_RGBA16F:
      COPY_BGRA(uint16_t, uint16_t, u16_sf16, 0x3C00);
    case L16_TO_RGBA16F:
      COPY_L(uint16_t, uint16_t, u16_sf16, 0x3C00);
    case LA16_TO_RGBA16F:
      COPY_LA(uint16_t, uint16_t, u16_sf16, 0x3C00);

    case R32F_TO_RGBA16F:
      COPY_R(uint16_t, uint32_t, f32_sf16, 0x3C00);
    case RG32F_TO_RGBA16F:
      COPY_RG(uint16_t, uint32_t, f32_sf16, 0x3C00);
    case RGB32F_TO_RGBA16F:
      COPY_RGB(uint16_t, uint32_t, f32_sf16, 0x3C00);
    case RGBA32F_TO_RGBA16F:
      COPY_RGBA(uint16_t, uint32_t, f32_sf16, 0x3C00);
    case BGR32F_TO_RGBA16F:
      COPY_BGR(uint16_t, uint32_t, f32_sf16, 0x3C00);
    case BGRA32F_TO_RGBA16F:
      COPY_BGRA(uint16_t, uint32_t, f32_sf16, 0x3C00);
    case L32F_TO_RGBA16F:
      COPY_L(uint16_t, uint32_t, f32_sf16, 0x3C00);
    case LA32F_TO_RGBA16F:
      COPY_LA(uint16_t, uint32_t, f32_sf16, 0x3C00);
  };
}

void GetCompressionSpeedParameters(
    const CompressionSpeed speed, const int footprint_x, const int footprint_y,
    const int footprint_z, int* const plimit_autoset,
    float* const oplimit_autoset, float* const dblimit_autoset,
    float* const bmc_autoset, float* const mincorrel_autoset,
    int* const maxiters_autoset, int* const pcdiv) {
  float log10_texels =
      std::log(static_cast<float>(footprint_x * footprint_y * footprint_z)) /
      std::log(10.0f);

  if (speed == CompressionSpeed::kVeryFast) {
    *plimit_autoset = 2;
    *oplimit_autoset = 1.0;
    *dblimit_autoset = std::max(70 - 35 * log10_texels, 53 - 19 * log10_texels);
    *bmc_autoset = 25;
    *mincorrel_autoset = 0.5;
    *maxiters_autoset = 1;

    switch (footprint_y) {
      case 4:
        *pcdiv = 240;
        break;
      case 5:
        *pcdiv = 56;
        break;
      case 6:
        *pcdiv = 64;
        break;
      case 8:
        *pcdiv = 47;
        break;
      case 10:
        *pcdiv = 36;
        break;
      case 12:
        *pcdiv = 30;
        break;
      default:
        *pcdiv = 30;
        break;
    }
  } else if (speed == CompressionSpeed::kFast) {
    *plimit_autoset = 4;
    *oplimit_autoset = 1.0;
    *mincorrel_autoset = 0.5;
    *dblimit_autoset = std::max(85 - 35 * log10_texels, 63 - 19 * log10_texels);
    *bmc_autoset = 50;
    *maxiters_autoset = 1;

    switch (footprint_y) {
      case 4:
        *pcdiv = 60;
        break;
      case 5:
        *pcdiv = 27;
        break;
      case 6:
        *pcdiv = 30;
        break;
      case 8:
        *pcdiv = 24;
        break;
      case 10:
        *pcdiv = 16;
        break;
      case 12:
        *pcdiv = 20;
        break;
      default:
        *pcdiv = 20;
        break;
    }
  } else if (speed == CompressionSpeed::kMedium) {
    *plimit_autoset = 25;
    *oplimit_autoset = 1.2f;
    *mincorrel_autoset = 0.75f;
    *dblimit_autoset = std::max(95 - 35 * log10_texels, 70 - 19 * log10_texels);
    *bmc_autoset = 75;
    *maxiters_autoset = 2;

    switch (footprint_y) {
      case 4:
        *pcdiv = 25;
        break;
      case 5:
        *pcdiv = 15;
        break;
      case 6:
        *pcdiv = 15;
        break;
      case 8:
        *pcdiv = 10;
        break;
      case 10:
        *pcdiv = 8;
        break;
      case 12:
        *pcdiv = 6;
        break;
      default:
        *pcdiv = 6;
        break;
    }
  } else if (speed == CompressionSpeed::kThorough) {
    *plimit_autoset = 100;
    *oplimit_autoset = 2.5f;
    *mincorrel_autoset = 0.95f;
    *dblimit_autoset =
        std::max(105 - 35 * log10_texels, 77 - 19 * log10_texels);
    *bmc_autoset = 95;
    *maxiters_autoset = 4;

    switch (footprint_y) {
      case 4:
        *pcdiv = 12;
        break;
      case 5:
        *pcdiv = 7;
        break;
      case 6:
        *pcdiv = 7;
        break;
      case 8:
        *pcdiv = 5;
        break;
      case 10:
        *pcdiv = 4;
        break;
      case 12:
        *pcdiv = 3;
        break;
      default:
        *pcdiv = 3;
        break;
    }
  } else if (speed == CompressionSpeed::kExhaustive) {
    *plimit_autoset = PARTITION_COUNT;
    *oplimit_autoset = 1000.0f;
    *mincorrel_autoset = 0.99f;
    *dblimit_autoset = 999.0f;
    *bmc_autoset = 100;
    *maxiters_autoset = 4;

    switch (footprint_y) {
      case 4:
        *pcdiv = 3;
        break;
      case 5:
        *pcdiv = 1;
        break;
      case 6:
        *pcdiv = 1;
        break;
      case 8:
        *pcdiv = 1;
        break;
      case 10:
        *pcdiv = 1;
        break;
      case 12:
        *pcdiv = 1;
        break;
      default:
        *pcdiv = 1;
        break;
    }
  } else {
    assert(false);
  }
}

#ifndef WIN32
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

double get_time() {
  timeval tv;
  gettimeofday(&tv, 0);

  return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
}

int astc_codec_unlink(const char* filename) { return unlink(filename); }

#else
// Windows.h defines IGNORE, so we must #undef our own version.
#undef IGNORE

// Define pthread-like functions in terms of Windows threading API
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef HANDLE pthread_t;
typedef int pthread_attr_t;

int pthread_create(pthread_t* thread, const pthread_attr_t* attribs,
                   void* (*threadfunc)(void*), void* thread_arg) {
  *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadfunc,
                         thread_arg, 0, NULL);
  return 0;
}

int pthread_join(pthread_t thread, void** value) {
  WaitForSingleObject(thread, INFINITE);
  return 0;
}

double get_time() {
  FILETIME tv;
  GetSystemTimeAsFileTime(&tv);

  unsigned __int64 ticks = tv.dwHighDateTime;
  ticks = (ticks << 32) | tv.dwLowDateTime;

  return ((double)ticks) / 1.0e7;
}

// Define an unlink() function in terms of the Win32 DeleteFile function.
int astc_codec_unlink(const char* filename) {
  BOOL res = DeleteFileA(filename);
  return (res ? 0 : -1);
}
#endif

#if defined(__APPLE__)
#define _DARWIN_C_SOURCE
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#else
#include <unistd.h>
#endif

unsigned get_number_of_cpus(void) {
  unsigned n_cpus = 1;

#ifdef __linux__
  cpu_set_t mask;
  CPU_ZERO(&mask);
  sched_getaffinity(getpid(), sizeof(mask), &mask);
  n_cpus = 0;
  for (unsigned i = 0; i < CPU_SETSIZE; ++i) {
    if (CPU_ISSET(i, &mask)) n_cpus++;
  }
  if (n_cpus == 0) n_cpus = 1;

#elif defined(_WIN32) || defined(__CYGWIN__)
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  n_cpus = sysinfo.dwNumberOfProcessors;

#elif defined(__APPLE__)
  int mib[4];
  size_t length = 100;
  mib[0] = CTL_HW;
  mib[1] = HW_AVAILCPU;
  sysctl(mib, 2, &n_cpus, &length, NULL, 0);
#endif

  return n_cpus;
}

void astc_codec_internal_error(const char* filename, int linenum) {
  printf("Internal error: File=%s Line=%d\n", filename, linenum);
  exit(1);
}

int suppress_progress_counter = 0;
int progress_counter_divider = 1;
int rgb_force_use_of_hdr = 0;
int alpha_force_use_of_hdr = 0;
int perform_srgb_transform = 0;

int print_tile_errors = 0;
int print_statistics = 0;

#ifdef DEBUG_PRINT_DIAGNOSTICS
int print_diagnostics = 0;
int diagnostics_tile = -1;
#endif

struct encode_astc_image_info {
  int xdim;
  int ydim;
  int zdim;
  const error_weighting_params* ewp;
  uint8_t* buffer;
  int* counters;
  int pack_and_unpack;
  int thread_id;
  int threadcount;
  astc_decode_mode decode_mode;
  swizzlepattern swz_encode;
  swizzlepattern swz_decode;
  int* threads_completed;
  const astc_codec_image* input_image;
  astc_codec_image* output_image;
};

void* encode_astc_image_threadfunc(void* vblk) {
  const encode_astc_image_info* blk = (const encode_astc_image_info*)vblk;
  int xdim = blk->xdim;
  int ydim = blk->ydim;
  int zdim = blk->zdim;
  uint8_t* buffer = blk->buffer;
  const error_weighting_params* ewp = blk->ewp;
  int thread_id = blk->thread_id;
  int threadcount = blk->threadcount;
  int* counters = blk->counters;
  int pack_and_unpack = blk->pack_and_unpack;
  astc_decode_mode decode_mode = blk->decode_mode;
  swizzlepattern swz_encode = blk->swz_encode;
  swizzlepattern swz_decode = blk->swz_decode;
  int* threads_completed = blk->threads_completed;
  const astc_codec_image* input_image = blk->input_image;
  astc_codec_image* output_image = blk->output_image;

  imageblock pb;
  int ctr = thread_id;
  int pctr = 0;

  int x, y, z, i;
  int xsize = input_image->xsize;
  int ysize = input_image->ysize;
  int zsize = input_image->zsize;
  int xblocks = (xsize + xdim - 1) / xdim;
  int yblocks = (ysize + ydim - 1) / ydim;
  int zblocks = (zsize + zdim - 1) / zdim;

  int owns_progress_counter = 0;

  // allocate memory for temporary buffers
  compress_symbolic_block_buffers temp_buffers;
  temp_buffers.ewb = new error_weight_block;
  temp_buffers.ewbo = new error_weight_block_orig;
  temp_buffers.tempblocks = new symbolic_compressed_block[4];
  temp_buffers.temp = new imageblock;
  temp_buffers.planes2 = new compress_fixed_partition_buffers;
  temp_buffers.planes2->ei1 = new endpoints_and_weights;
  temp_buffers.planes2->ei2 = new endpoints_and_weights;
  temp_buffers.planes2->eix1 = new endpoints_and_weights[MAX_DECIMATION_MODES];
  temp_buffers.planes2->eix2 = new endpoints_and_weights[MAX_DECIMATION_MODES];
  temp_buffers.planes2->decimated_quantized_weights =
      new float[2 * MAX_DECIMATION_MODES * MAX_WEIGHTS_PER_BLOCK];
  temp_buffers.planes2->decimated_weights =
      new float[2 * MAX_DECIMATION_MODES * MAX_WEIGHTS_PER_BLOCK];
  temp_buffers.planes2->flt_quantized_decimated_quantized_weights =
      new float[2 * MAX_WEIGHT_MODES * MAX_WEIGHTS_PER_BLOCK];
  temp_buffers.planes2->u8_quantized_decimated_quantized_weights =
      new uint8_t[2 * MAX_WEIGHT_MODES * MAX_WEIGHTS_PER_BLOCK];
  temp_buffers.plane1 = temp_buffers.planes2;

  for (z = 0; z < zblocks; z++)
    for (y = 0; y < yblocks; y++)
      for (x = 0; x < xblocks; x++) {
        if (ctr == 0) {
          int offset = ((z * yblocks + y) * xblocks + x) * 16;
          uint8_t* bp = buffer + offset;
#ifdef DEBUG_PRINT_DIAGNOSTICS
          if (diagnostics_tile < 0 || diagnostics_tile == pctr) {
            print_diagnostics = (diagnostics_tile == pctr) ? 1 : 0;
#endif
            fetch_imageblock(input_image, &pb, xdim, ydim, zdim, x * xdim,
                             y * ydim, z * zdim, swz_encode);
            symbolic_compressed_block scb;
            compress_symbolic_block(input_image, decode_mode, xdim, ydim, zdim,
                                    ewp, &pb, &scb, &temp_buffers);
            if (pack_and_unpack) {
              decompress_symbolic_block(decode_mode, xdim, ydim, zdim, x * xdim,
                                        y * ydim, z * zdim, &scb, &pb);
              write_imageblock(output_image, &pb, xdim, ydim, zdim, x * xdim,
                               y * ydim, z * zdim, swz_decode);
            } else {
              physical_compressed_block pcb;
              pcb = symbolic_to_physical(xdim, ydim, zdim, &scb);
              *(physical_compressed_block*)bp = pcb;
            }
#ifdef DEBUG_PRINT_DIAGNOSTICS
          }
#endif

          counters[thread_id]++;
          ctr = threadcount - 1;

          pctr++;

          // routine to print the progress counter.
          if (suppress_progress_counter == 0 &&
              (pctr % progress_counter_divider) == 0 &&
              print_tile_errors == 0 && print_statistics == 0) {
            int do_print = 1;
            // the current thread has the responsibility for printing the
            // progress counter if every previous thread has completed. Also, if
            // we have ever received the responsibility to print the progress
            // counter, we are going to keep it until the thread is completed.
            if (!owns_progress_counter) {
              for (i = thread_id - 1; i >= 0; i--) {
                if (threads_completed[i] == 0) {
                  do_print = 0;
                  break;
                }
              }
            }
            if (do_print) {
              owns_progress_counter = 1;
              int summa = 0;
              for (i = 0; i < threadcount; i++) summa += counters[i];
              printf("\r%d", summa);
              fflush(stdout);
            }
          }
        } else
          ctr--;
      }

  delete[] temp_buffers.planes2->decimated_quantized_weights;
  delete[] temp_buffers.planes2->decimated_weights;
  delete[] temp_buffers.planes2->flt_quantized_decimated_quantized_weights;
  delete[] temp_buffers.planes2->u8_quantized_decimated_quantized_weights;
  delete[] temp_buffers.planes2->eix1;
  delete[] temp_buffers.planes2->eix2;
  delete temp_buffers.planes2->ei1;
  delete temp_buffers.planes2->ei2;
  delete temp_buffers.planes2;
  delete[] temp_buffers.tempblocks;
  delete temp_buffers.temp;
  delete temp_buffers.ewbo;
  delete temp_buffers.ewb;

  threads_completed[thread_id] = 1;
  return NULL;
}

void encode_astc_image(const astc_codec_image* input_image,
                       astc_codec_image* output_image, int xdim, int ydim,
                       int zdim, const error_weighting_params* ewp,
                       astc_decode_mode decode_mode, swizzlepattern swz_encode,
                       swizzlepattern swz_decode, uint8_t* buffer,
                       int pack_and_unpack, int threadcount) {
  int i;
  int* counters = new int[threadcount];
  int* threads_completed = new int[threadcount];

  // before entering into the multi-threaded routine, ensure that the block size
  // descriptors and the partition table descriptors needed actually exist.
  get_block_size_descriptor(xdim, ydim, zdim);
  get_partition_table(xdim, ydim, zdim, 0);

  encode_astc_image_info* ai = new encode_astc_image_info[threadcount];
  for (i = 0; i < threadcount; i++) {
    ai[i].xdim = xdim;
    ai[i].ydim = ydim;
    ai[i].zdim = zdim;
    ai[i].buffer = buffer;
    ai[i].ewp = ewp;
    ai[i].counters = counters;
    ai[i].pack_and_unpack = pack_and_unpack;
    ai[i].thread_id = i;
    ai[i].threadcount = threadcount;
    ai[i].decode_mode = decode_mode;
    ai[i].swz_encode = swz_encode;
    ai[i].swz_decode = swz_decode;
    ai[i].threads_completed = threads_completed;
    ai[i].input_image = input_image;
    ai[i].output_image = output_image;
    counters[i] = 0;
    threads_completed[i] = 0;
  }

  if (threadcount == 1)
    encode_astc_image_threadfunc(&ai[0]);
  else {
    pthread_t* threads = new pthread_t[threadcount];
    for (i = 0; i < threadcount; i++)
      pthread_create(&(threads[i]), NULL, encode_astc_image_threadfunc,
                     (void*)(&(ai[i])));

    for (i = 0; i < threadcount; i++) pthread_join(threads[i], NULL);
    delete[] threads;
  }

  delete[] ai;
  delete[] counters;
  delete[] threads_completed;
}

void encode_astc_image(const astc_codec_image* input_image, int xdim, int ydim,
                       int zdim, const error_weighting_params* ewp,
                       astc_decode_mode decode_mode, swizzlepattern swz_encode,
                       int threadcount, uint8_t** out_data, size_t* out_size) {
  int xsize = input_image->xsize;
  int ysize = input_image->ysize;
  int zsize = input_image->zsize;

  int xblocks = (xsize + xdim - 1) / xdim;
  int yblocks = (ysize + ydim - 1) / ydim;
  int zblocks = (zsize + zdim - 1) / zdim;

  const size_t buffer_size = xblocks * yblocks * zblocks * 16;
  uint8_t* buffer = (uint8_t*)malloc(buffer_size);
  if (!buffer) {
    std::cout << "Ran out of memory" << std::endl;
    exit(1);
  }

  if (!suppress_progress_counter) {
    printf("%d blocks to process ..\n", xblocks * yblocks * zblocks);
  }

  encode_astc_image(input_image, NULL, xdim, ydim, zdim, ewp, decode_mode,
                    swz_encode, swz_encode, buffer, 0, threadcount);

  *out_data = buffer;
  *out_size = buffer_size;

  /*astc_header hdr;
  hdr.magic[0] = MAGIC_FILE_CONSTANT & 0xFF;
  hdr.magic[1] = (MAGIC_FILE_CONSTANT >> 8) & 0xFF;
  hdr.magic[2] = (MAGIC_FILE_CONSTANT >> 16) & 0xFF;
  hdr.magic[3] = (MAGIC_FILE_CONSTANT >> 24) & 0xFF;
  hdr.blockdim_x = xdim;
  hdr.blockdim_y = ydim;
  hdr.blockdim_z = zdim;
  hdr.xsize[0] = xsize & 0xFF;
  hdr.xsize[1] = (xsize >> 8) & 0xFF;
  hdr.xsize[2] = (xsize >> 16) & 0xFF;
  hdr.ysize[0] = ysize & 0xFF;
  hdr.ysize[1] = (ysize >> 8) & 0xFF;
  hdr.ysize[2] = (ysize >> 16) & 0xFF;
  hdr.zsize[0] = zsize & 0xFF;
  hdr.zsize[1] = (zsize >> 8) & 0xFF;
  hdr.zsize[2] = (zsize >> 16) & 0xFF;

  FILE *wf = fopen(filename, "wb");
  fwrite(&hdr, 1, sizeof(astc_header), wf);
  fwrite(buffer, 1, xblocks * yblocks * zblocks * 16, wf);
  fclose(wf);
  free(buffer);*/
}

astc_codec_image* CreateAstcCodecImageFromGl(const void* pixels, int size_x,
                                             int size_y, int size_z,
                                             uint32_t gl_format,
                                             uint32_t gl_type) {
  int components = GetNumComponentsFromGlFormat(gl_format);
  int bytes_per_component = 3;
  int bitness = 16;
  int padding = 0;  // MAX(ewp.mean_stdev_radius, ewp.alpha_radius);
  GetBitnessFromGlType(gl_type, bitness, bytes_per_component);

  astc_codec_image* astc_img =
      allocate_image(bitness, size_x, size_y, size_z, padding);

  uint32_t xstride = bytes_per_component * components * size_x;
  uint32_t ystride = xstride * size_y;

  scanline_copy_method cm = GetScanlineCopyMethod(gl_type, gl_format);

  for (int z = 0; z < size_z; z++) {
    int zdst = (size_z == 1) ? z : z + padding;
    for (int y = 0; y < size_y; y++) {
      int ydst = y + padding;
      void* dst;
      if (bitness == 16)
        dst = (void*)(astc_img->imagedata16[zdst][ydst] + 4 * padding);
      else
        dst = (void*)(astc_img->imagedata8[zdst][ydst] + 4 * padding);

      const uint8_t* src = reinterpret_cast<const uint8_t*>(pixels) +
                           (z * ystride) + (y * xstride);
      copy_scanline(dst, src, size_x, cm);
    }
  }

  return astc_img;
}

void EncodeAstc(const void* pixels, int width, int height, uint32_t gl_format,
                uint32_t gl_type, uint8_t** out_data, size_t* out_size,
                int footprint_x, int footprint_y,
                CompressionSpeed compression_speed) {
  // initialization routines
  prepare_angular_tables();
  build_quantization_mode_table();
  int footprint_z = 1;
  int size_z = 1;

  int thread_count = get_number_of_cpus();

  enum astc_decode_mode decode_mode = DECODE_HDR;
  //{DECODE_LDR_SRGB, DECODE_LDR, DECODE_HDR};

  error_weighting_params ewp;

  ewp.rgb_power = 1.0f;
  ewp.alpha_power = 1.0f;
  ewp.rgb_base_weight = 1.0f;
  ewp.alpha_base_weight = 1.0f;
  ewp.rgb_mean_weight = 0.0f;
  ewp.rgb_stdev_weight = 0.0f;
  ewp.alpha_mean_weight = 0.0f;
  ewp.alpha_stdev_weight = 0.0f;

  ewp.rgb_mean_and_stdev_mixing = 0.0f;
  ewp.mean_stdev_radius = 0;
  ewp.enable_rgb_scale_with_alpha = 0;
  ewp.alpha_radius = 0;

  ewp.block_artifact_suppression = 0.0f;
  const int components = GetNumComponentsFromGlFormat(gl_format);
  ewp.rgba_weights[0] = 1.0f;
  ewp.rgba_weights[1] = components > 1 ? 1.0f : 0.0f;
  ewp.rgba_weights[2] = components > 2 ? 1.0f : 0.0f;
  ewp.rgba_weights[3] = components > 3 ? 1.0f : 0.0f;
  ewp.ra_normal_angular_scale = 0;

  // HDR
  if (decode_mode == DECODE_HDR) {
    ewp.mean_stdev_radius = 0;
    ewp.rgb_power = 0.75;
    ewp.rgb_base_weight = 0;
    ewp.rgb_mean_weight = 1;
    ewp.alpha_base_weight = 0.05f;
    rgb_force_use_of_hdr = 1;
    alpha_force_use_of_hdr = 0;
  }

  int plimit_autoset = -1;
  float dblimit_autoset = 0.0;
  float oplimit_autoset = 0.0;
  float mincorrel_autoset = 0.0;
  float bmc_autoset = 0.0;
  int maxiters_autoset = 0;

  int pcdiv = 1;

  GetCompressionSpeedParameters(compression_speed, footprint_x, footprint_y,
                                footprint_z, &plimit_autoset, &oplimit_autoset,
                                &dblimit_autoset, &bmc_autoset,
                                &mincorrel_autoset, &maxiters_autoset, &pcdiv);

  int partitions_to_test = plimit_autoset;
  float dblimit = dblimit_autoset;
  float oplimit = oplimit_autoset;
  float mincorrel = mincorrel_autoset;
  int maxiters = maxiters_autoset;

  ewp.max_refinement_iters = maxiters;
  ewp.block_mode_cutoff = bmc_autoset / 100.0f;
  ewp.partition_1_to_2_limit = oplimit;
  ewp.lowest_correlation_cutoff = mincorrel;
  ewp.partition_search_limit = partitions_to_test;

  // Specifying the error weight of a color component as 0 is not allowed.
  // If weights are 0, then they are instead set to a small positive value.

  float max_color_component_weight =
      MAX(MAX(ewp.rgba_weights[0], ewp.rgba_weights[1]),
          MAX(ewp.rgba_weights[2], ewp.rgba_weights[3]));
  ewp.rgba_weights[0] =
      MAX(ewp.rgba_weights[0], max_color_component_weight / 1000.0f);
  ewp.rgba_weights[1] =
      MAX(ewp.rgba_weights[1], max_color_component_weight / 1000.0f);
  ewp.rgba_weights[2] =
      MAX(ewp.rgba_weights[2], max_color_component_weight / 1000.0f);
  ewp.rgba_weights[3] =
      MAX(ewp.rgba_weights[3], max_color_component_weight / 1000.0f);

  ewp.texel_avg_error_limit =
      std::pow(0.1f, dblimit * 0.1f) * 65535.0f * 65535.0f;

  swizzlepattern swz_encode = {0, 1, 2, 3};

  astc_codec_image* astc_img = CreateAstcCodecImageFromGl(
      pixels, width, height, size_z, gl_format, gl_type);

  expand_block_artifact_suppression(footprint_x, footprint_y, footprint_z,
                                    &ewp);

  compute_averages_and_variances(astc_img, ewp.rgb_power, ewp.alpha_power,
                                 ewp.mean_stdev_radius, ewp.alpha_radius,
                                 swz_encode);

  encode_astc_image(astc_img, footprint_x, footprint_y, footprint_z, &ewp,
                    decode_mode, swz_encode, thread_count, out_data, out_size);

  destroy_image(astc_img);
}