cc_binary(
    name = "paper_collecter",
    srcs = glob(
        [
          "main.cc",
          "include/*.hpp",
          "include/*.h",
        ],
    ),
    deps = [
        "//argcv/cxx:argcv_str",
        "//argcv/c:argcv_char",
        "//argcv/cxx/helper:xml_sax_parser",
        "//external:glog",
        "//external:gflags",
        "//external:rapidjson",
    ],
    visibility = ["//visibility:public"],
)
