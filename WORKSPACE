workspace(name = "com_github_yuikns_dblp_reader")

load("//tools:argcv_archive.bzl", "argcv_archive")

argcv_archive(
  name = "com_github_argcv_argcv",
  sha256 = "",
  git_commit = "e0a339382e7d1bd83b8dac0cbc8e4f3df4a37e2e",
)

load("@com_github_argcv_argcv//tools:version.bzl", "check_version")

check_version("0.16.0")

load("@com_github_argcv_argcv//argcv:argcv.bzl", "ws_argcv")
ws_argcv("com_github_argcv_argcv")

