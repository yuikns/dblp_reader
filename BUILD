cc_binary(
    name = "paper_collecter",
    srcs = [
        "collecter.cc",
    ],
    deps = [
        "@com_github_argcv_argcv//argcv/c:char",
        "@com_github_argcv_argcv//argcv/cxx:par",
        "@com_github_argcv_argcv//argcv/cxx:str",
        "@com_github_argcv_argcv//argcv/cxx:encoding",
        "//external:glog",
        "//external:gflags",
        "@com_github_argcv_argcv//third_party/json:json",
    ],
    visibility = ["//visibility:public"],
)
