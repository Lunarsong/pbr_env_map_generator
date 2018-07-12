cc_library(
    name = "mathfu",
    includes = ["include"],
    srcs = glob(["include/mathfu/**/*.h"]),
    visibility = ["//visibility:public"],
    deps = [
        ":vectorial",
    ],
    defines = [
        "MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT",
    ],
)

cc_library(
    name = "vectorial",
    includes = ["dependencies/vectorial/include"],
    visibility = ["//visibility:public"],
)
