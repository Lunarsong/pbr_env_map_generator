cc_binary(
    name = "tool",
    srcs = glob(["main.cc"]),
    deps = [
        "@stb//:image",
        "@glfw//:glfw",
        "@mathfu//:mathfu",
        "//third_party/glad:glad",
        "//ktx",
    ],
    data = [
        "data/newport_loft.hdr",
        "data/back.jpg",
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