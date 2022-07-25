#!/usr/bin/python3
import shutil
import subprocess
import sys
from typing import *
from sys import platform
import os
import pathlib
import multiprocessing


# ================
# helper function
# ================
def is_window() -> bool:
    return platform == "win32"


def is_linux() -> bool:
    return platform == "linux"


def is_mac() -> bool:
    return platform == "darwin"


def get_os_lib_ext() -> str:
    if is_window():
        return ".lib"
    elif is_linux():
        return ".a"
    else:
        raise Exception("not supported platform")


def get_executable_path(executable: str) -> str:
    path = shutil.which(executable)

    if path is None:
        raise Exception(f"couldn't found {executable}")

    return path


def mkdirs(path: str) -> None:
    pathlib.Path(path).mkdir(parents=True, exist_ok=True)


def get_root() -> str:
    return os.path.dirname(os.path.realpath(__file__))


def get_cpu_cores() -> int:
    return multiprocessing.cpu_count()


# ================
# parse arguments
# ================
class BuildInfo:
    # when android is True, ndk_path is not None
    android: bool = False
    ndk_path: Optional[str] = None
    # mode may be "Debug" or "Release"
    mode: str = "Debug"


def parse_args() -> BuildInfo:
    argv = sys.argv[1:]
    index = 0

    info = BuildInfo()

    while index != len(argv):
        arg = argv[index]

        if arg == "--ndk":
            index += 1
            if index == len(argv):
                raise Exception("--ndk requires an argument")
            info.ndk_path = argv[index]
        elif arg == "--android":
            info.android = True
        elif arg == "--release":
            info.mode = "Release"
        elif arg == "--debug":
            info.mode = "Debug"
        else:
            raise Exception("Unknown argument: " + arg)

        index += 1

    if info.android and info.ndk_path is None:
        raise Exception("--ndk is required when building for Android")

    if info.ndk_path is not None and not info.android:
        raise Exception("--ndk is only available when building for Android")

    return info


global_info = parse_args()


# ================
# helper function
# ================
def is_android() -> bool:
    return global_info.android

def is_debug() -> bool:
    return global_info.mode == "Debug"


def is_release() -> bool:
    return global_info.mode == "Release"


def get_android_host_tag() -> str:
    if not is_android():
        raise Exception("get android llvm path is only available on android")

    if is_window():
        return "windows-x86_64"
    elif is_mac():
        return "darwin-x86_64"
    elif is_linux():
        return "linux-x86_64"
    else:
        raise Exception("unknown host for android building")


def get_android_llvm_root_path() -> str:
    if not is_android():
        raise Exception("get android llvm path is only available on android")

    return os.path.join(global_info.ndk_path, "toolchains", "llvm", "prebuilt", get_android_host_tag()).replace("\\",
                                                                                                                "/")


def get_triad_name() -> str:
    if is_android():
        return f"android-arm64-{global_info.mode}"

    name = sys.platform.replace("darwin", "macos")
    name = name.replace("win32", "windows")
    return f"{name}-x64-{global_info.mode}"


def get_source_path_of_library(name: str) -> str:
    return get_root() + f"/{name}"


def get_build_path() -> str:
    return get_root() + "/build/" + get_triad_name() + "/"


def get_binary_path_of_library(name: str) -> str:
    return get_build_path() + f"/{name}"


def get_install_root() -> str:
    return get_root() + "/install-tree/" + get_triad_name() + "/"


def get_install_path_of_library(name: str) -> str:
    return get_install_root() + f"/{name}"


ANDROID_API_VERSION = 28


def get_android_abi() -> str:
    if not is_android():
        raise Exception("get android abi is only available on android")
    return "arm64-v8a"


def get_android_api_version() -> int:
    if not is_android():
        raise Exception("get android api version is only available on android")
    return ANDROID_API_VERSION


if is_android():
    print(f"build for {get_android_abi()}-android-{get_android_api_version()}")


def get_stdout_log_of_library(name: str) -> str:
    return get_root() + "/" + get_triad_name() + "-" + name + ".stdout.log"


def get_stderr_log_of_library(name: str) -> str:
    return get_root() + "/" + get_triad_name() + "-" + name + ".stderr.log"


def open_log(path:str):
    broke = False
    if os.path.isfile(path) and os.path.exists(path) and os.path.getsize(path) >= 1024 * 1024 * 4:
        broke = True

    handle =  open(path,encoding="utf-8",mode="a")
    
    # 截断过大的log文件
    if broke:
        handle.truncate(0)
        
    return handle


# ================
# ninja build
# ================
def call_ninja(args: List[str], stdout_file: str, stderr_file: str, work_dir: str) -> None:
    with open_log(stdout_file) as stdout, \
        open_log(stderr_file) as stderr:
        ninja = [get_executable_path("ninja")]
        ninja.extend(args)

        print(f"running {ninja}")

        ninja = subprocess.Popen(args=ninja,
                                 stdout=stdout,
                                 stderr=stderr,
                                 encoding='utf-8',
                                 bufsize=512,
                                 cwd=work_dir)

        ninja.wait()

        if ninja.returncode != 0:
            raise Exception(f"ninja failed with return code {ninja.returncode}")


# ================
# cmake build
# ================
class CMakeTask:
    cmake_path: str = get_executable_path("cmake")
    work_dir: str = "."
    build_type: str = global_info.mode
    generate_options: List[str] = []
    tool_chain: Optional[str] = None
    generator: Optional[str] = None
    binary_dir: str = ""
    install_path: str = ""
    source_path: str = ""
    stdout_file: str = ""
    stderr_file: str = ""

    def run_once(self, options: List[str]):
        with open(self.stdout_file) as stdout, \
            open(self.stderr_file) as stderr:
            args = [self.cmake_path]
            args.extend(options)
            print(f"running {args}")

            cmake = subprocess.Popen(
                args=args,
                cwd=self.work_dir,
                encoding="utf-8",
                stdout=stdout,
                stderr=stderr,
                bufsize=512)
            cmake.wait()
            if cmake.returncode != 0:
                raise Exception(f"cmake failed with return code {cmake.returncode}")

    def build(self):
        g_opts = [
            '-S', self.source_path,
            '-B', self.binary_dir,
            '-DCMAKE_BUILD_TYPE=' + self.build_type.replace("\\", "/"),
            '-DCMAKE_INSTALL_PREFIX=' + self.install_path.replace("\\", "/")
        ]

        if self.tool_chain is not None:
            g_opts.extend(["-DCMAKE_TOOLCHAIN_FILE=" + self.tool_chain.replace("\\", "/")])
        if self.generator is not None:
            g_opts.extend(["-G", self.generator.replace("\\", "/")])

        g_opts.extend(self.generate_options)

        self.run_once(g_opts)

        self.run_once([
            '--build', self.binary_dir.replace("\\", "/"),
            '-j', str(get_cpu_cores()),
            '--target', 'install',
            '--config', self.build_type
        ])


# ================
# meson build
# ================
class MesonTask:
    meson_path: str = get_executable_path("meson")
    work_dir: str = "."
    # note: meson build type is not the same as cmake build type
    # we need lowercase build type
    build_type: str = global_info.mode.lower()
    source_path: str = ""
    binary_dir: str = ""
    install_path: str = ""
    stdout_file: str = ""
    stderr_file: str = ""
    definitions: List[str] = []
    generate_options: List[str] = []
    cross_file: Optional[str] = None

    def run_once(self, options: List[str]):
        with open_log(self.stdout_file) as stdout, \
            open_log(self.stderr_file) as stderr:
            args = [self.meson_path]
            args.extend(options)

            print(f"running {args}")

            meson = subprocess.Popen(
                args=args,
                cwd=self.work_dir,
                encoding="utf-8",
                stdout=stdout,
                stderr=stderr,
                bufsize=512)

            meson.wait()
            if meson.returncode != 0:
                raise Exception(f"meson failed with return code {meson.returncode}")

    def build(self, name: str):
        g_opts = [
            'setup',
            '--buildtype=' + self.build_type,
            '--prefix=' + self.install_path,
            self.binary_dir,
            self.source_path,
        ]

        g_opts.extend(self.generate_options)

        if self.cross_file is not None:
            g_opts.extend(['--cross-file', self.cross_file])

        for definition in self.definitions:
            g_opts.extend(['-D' + definition])

        self.run_once(g_opts)

        call_ninja(['install'], self.stdout_file, self.stderr_file, self.binary_dir)


def build_library(name: str, options: List[str]) -> None:
    task = CMakeTask()

    task.source_path = get_source_path_of_library(name)
    task.binary_dir = get_binary_path_of_library(name)
    task.install_path = get_install_path_of_library(name)
    task.stdout_file = get_stdout_log_of_library(name)
    task.stderr_file = get_stderr_log_of_library(name)
    task.generate_options = options

    mkdirs(task.binary_dir)
    mkdirs(task.install_path)

    if is_android():
        task.tool_chain = (global_info.ndk_path + "/build/cmake/android.toolchain.cmake").replace("\\", "/")
        task.generator = "Ninja"
        task.generate_options = [
            "-DANDROID_PLATFORM=" + str(get_android_api_version()),
            "-DANDROID_ABI=" + get_android_abi(),
            "-DANDROID_STL=c++_shared"
        ]

    task.build()


def get_meson_cross_file_path_in_build():
    return get_build_path() + "/meson_cross_file.txt"


def get_meson_cross_file_raw() -> str:
    with open(get_root() + "/meson-android.txt", "r", encoding="utf-8") as f:
        return f.read()


def generate_meson_cross_file() -> str:
    raw = get_meson_cross_file_raw()

    raw = raw.replace("(BINARY_PATH)", get_android_llvm_root_path() + "/bin")

    if is_window():
        raw = raw.replace("(EXE)", ".exe")
    else:
        raw = raw.replace("(EXE)", "")

    raw = raw.replace("(API_VERSION)", str(get_android_api_version()))

    with open(get_meson_cross_file_path_in_build(), encoding='utf-8', mode="w") as f:
        f.write(raw)
        f.close()

    return get_meson_cross_file_path_in_build()


def build_meson_library(name: str, options: List[str], definition: List[str]) -> None:
    task = MesonTask()

    task.source_path = get_source_path_of_library(name)
    task.binary_dir = get_binary_path_of_library(name)
    task.install_path = get_install_path_of_library(name)
    task.stdout_file = get_stdout_log_of_library(name)
    task.stderr_file = get_stderr_log_of_library(name)
    task.work_dir = task.binary_dir
    task.generate_options = options
    task.definitions = definition

    mkdirs(task.binary_dir)
    mkdirs(task.install_path)

    if is_android():
        task.cross_file = generate_meson_cross_file()

    task.build(name)


# ========================
def get_library_name(name:str) -> str:
    if is_debug():
        return f"{name}d{get_os_lib_ext()}"
    elif is_release():
        return f"{name}{get_os_lib_ext()}"
    else:
        raise Exception("unknown mode")


build_library('zlib', [])

build_library('libpng', ["-DPNG_SHARED=OFF", "-DPNG_STATIC=ON", "-DPNG_TEST=OFF",
                         f"-DZLIB_LIBRARY={get_install_path_of_library('zlib')}/lib/{get_library_name('zlibstatic')}",
                         f"-DZLIB_INCLUDE_DIR={get_install_path_of_library('zlib')}/include"])

build_library('SDL', ["-DSDL_SHARED=OFF"])

build_library('freetype', [
    f"-DZLIB_INCLUDE_DIR={get_install_path_of_library('zlib')}/include",
    f"-DZLIB_LIBRARY={get_install_path_of_library('zlib')}/lib/{get_library_name('zlibstatic')}",
    f"-DPNG_PNG_INCLUDE_DIR={get_install_path_of_library('libpng')}/include"
    f"-DPNG_LIBRARY={get_install_path_of_library('libpng')}/lib/{get_library_name('libpng16_static')}"])

# ========================
build_meson_library('harfbuzz', [], ['cpp_rtti=true', 'tests=disabled', 'cpp_std=c++20', 'default_library=static'])
