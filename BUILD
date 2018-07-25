cc_binary(
    name = "tool",
    srcs = ["main.cc", "astc.cc", "astc.h"],
    deps = [
        "@stb//:image",
        "@glfw//:glfw",
        "@mathfu//:mathfu",
        "@astc_encoder//:astc_encoder",
        "//third_party/glad:glad",
        "//ktx",
    ],
    data = [
        "data/source.hdr",
        "data/equirectangular_to_cubemap.glslf",
        "data/equirectangular_to_cubemap.glslv",
        "data/irradiance_convolution.glslf",
        "data/irradiance_convolution.glslv",
        "data/prefilter.glslf",
        "data/prefilter.glslv",
        "data/cubemap.glslf",
        "data/cubemap.glslv",
    ],
    visibility = ["//visibility:public"],
)