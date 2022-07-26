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


def get_executable_path(executable: str) -> str:
    path = shutil.which(executable)

    if path is None:
        raise Exception(f"couldn't found {executable}")

    return path


def mkdirs(path: str) -> None:
    pathlib.Path(path).mkdir(parents=True, exist_ok=True)


def get_root() -> str:
    return os.path.dirname(os.path.realpath(__file__)).replace("\\","/")


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

    
def get_os_lib_ext() -> str:
    if is_android() or is_linux():
        return ".a"
    if is_window():
        return ".lib"
    else:
        raise Exception("not supported platform")

def get_os_lib_prefix() -> str:
    if is_android() or is_linux():
        return "lib"
    else:
        return ""


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
        stdout.write(f"running {ninja}\n")
        stderr.write(f"running {ninja}\n")
        stdout.flush()
        stderr.flush()

        ninja = subprocess.Popen(args=ninja,
                                 stdout=stdout,
                                 stderr=stderr,
                                 encoding='utf-8',
                                 bufsize=0,
                                 cwd=work_dir)

        ninja.wait()

        if ninja.returncode != 0:
            raise Exception(f"ninja failed with return code {ninja.returncode}")

def call_git(args:List[str],work_dir:str):
    git = [get_executable_path("git")]
    git.extend(args)
    
    print(f"running {git}")

    git = subprocess.Popen(args=git,
        encoding='utf-8',
        bufsize=0,
        cwd=work_dir)
    
    git.wait()

    if git.returncode != 0:
        raise Exception(f"git failed with return code {git.returncode}")

def clear_source(source:str):
    call_git(['reset','--hard','HEAD'],source)
    call_git(['clean','-fd'],source)

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
        with open_log(self.stdout_file) as stdout, \
            open_log(self.stderr_file) as stderr:
            args = [self.cmake_path]
            args.extend(options)
            
            print(f"running {args}")
            stdout.write(f"running {args}\n")
            stderr.write(f"running {args}\n")
            stdout.flush()
            stderr.flush()

            cmake = subprocess.Popen(
                args=args,
                cwd=self.work_dir,
                encoding="utf-8",
                stdout=stdout,
                stderr=stderr,
                bufsize=0)
            cmake.wait()
            if cmake.returncode != 0:
                raise Exception(f"cmake failed with return code {cmake.returncode}")

    def build(self):
        # clear build
        clear_source(self.source_path)

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
            stdout.write(f"running {args}\n")
            stderr.write(f"running {args}\n")
            stdout.flush()
            stderr.flush()

            meson = subprocess.Popen(
                args=args,
                cwd=self.work_dir,
                encoding="utf-8",
                stdout=stdout,
                stderr=stderr,
                bufsize=0)

            meson.wait()
            if meson.returncode != 0:
                raise Exception(f"meson failed with return code {meson.returncode}")

    def build(self, name: str):
        # clear build
        clear_source(self.source_path)

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


def get_standard_cmake_task(name:str,options:List[str]) -> CMakeTask:
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
        task.generate_options.extend([
            "-DANDROID_PLATFORM=" + str(get_android_api_version()),
            "-DANDROID_ABI=" + get_android_abi(),
            "-DANDROID_STL=c++_shared"
        ])

    return task


def build_library(name: str, options: List[str]) -> None:
    get_standard_cmake_task(name,options).build()


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


def get_library_name_or_unix(name:str,unix_name:str) -> str:
    if is_android() or is_linux():
        return f"lib{unix_name}.a"
    else:
        return get_library_name(name)


def get_list_info_file() -> str:
    return f"{get_install_root()}/library-info.cmake"


def generate_list_info() -> Mapping[str,List[str]]:
    lists:Mapping[List[str]] = {}

    lists['zlib'] = {
        "library":[os.path.abspath(f"{get_install_path_of_library('zlib')}/lib/{get_library_name_or_unix('zlibstatic','z')}").replace("\\","/")],
        "include":[os.path.abspath(f"{get_install_path_of_library('zlib')}/include").replace("\\","/")]
    }

    lists['libpng'] = {
        "library":[os.path.abspath(f"{get_install_path_of_library('libpng')}/lib/{get_library_name_or_unix('libpng16_static','png')}").replace("\\","/")],
        "include":[os.path.abspath(f"{get_install_path_of_library('libpng')}/include").replace("\\","/")]
    }
    
    lists['flac'] = {
        "library":[os.path.abspath(f"{get_install_path_of_library('flac')}/lib/{get_os_lib_prefix()}FLAC{get_os_lib_ext()}").replace("\\","/"),
        os.path.abspath(f"{get_install_path_of_library('flac')}/lib/{get_os_lib_prefix()}FLAC++{get_os_lib_ext()}").replace("\\","/")],
        "include":[os.path.abspath(f"{get_install_path_of_library('flac')}/include").replace("\\","/")]
    }

    return lists


def check_lists(lists:Mapping[str,Mapping[str,str]]):
    for key,value in lists.items():
        for inc in value['include']:
            if not os.path.exists(inc) or not os.path.isdir(inc):
                raise Exception(f"not found directory {inc}")

        for lib in value['library']:
            if not os.path.exists(lib) or not os.path.isfile(lib):
                raise Exception(f"not found file {lib}")


list_info = generate_list_info()

build_library('zlib', [])

build_library('libpng', ["-DPNG_SHARED=OFF", "-DPNG_STATIC=ON", "-DPNG_TEST=OFF",
                         f"-DZLIB_INCLUDE_DIR={';'.join(list_info['zlib']['include'])}",
                         f"-DZLIB_LIBRARY={';'.join(list_info['zlib']['library'])}"])

build_library('SDL', ["-DSDL_SHARED=OFF"])

build_library('freetype', [
    f"-DZLIB_INCLUDE_DIR={';'.join(list_info['zlib']['include'])}",
    f"-DZLIB_LIBRARY={';'.join(list_info['zlib']['library'])}",
    f"-DPNG_PNG_INCLUDE_DIR={';'.join(list_info['libpng']['include'])}"
    f"-DPNG_LIBRARY={';'.join(list_info['libpng']['library'])}"])

build_library('libjpeg-turbo', [
    "-DENABLE_SHARED=OFF",
    "-DWITH_CRT_DLL=ON"
])

zstd = get_standard_cmake_task('zstd', [
    "-DZSTD_BUILD_PROGRAMS=OFF",
])
zstd.source_path = f"{get_source_path_of_library('zstd')}/build/cmake/"
zstd.build()

build_library('ogg', [
    "-DBUILD_SHARED_LIBS=OFF",
    "-DINSTALL_DOCS=OFF",
    "-DINSTALL_PKG_CONFIG_MODULE=OFF",
    "-DINSTALL_CMAKE_PACKAGE_MODULE=ON"
])

build_library('vorbis', [
    "-DBUILD_SHARED_LIBS=OFF",
    f"-DOGG_LIBRARY={get_install_path_of_library('ogg')}/lib/{get_os_lib_prefix()}ogg{get_os_lib_ext()}",
    f"-DOGG_INCLUDE_DIR={get_install_path_of_library('ogg')}/include"
])

build_library('flac', [
    "-DWITH_OGG=OFF",
    "-DBUILD_CXXLIBS=ON",
    "-DBUILD_PROGRAMS=OFF",
    "-DBUILD_EXAMPLES=OFF",
    "-DBUILD_TESTING=OFF",
    "-DBUILD_DOCS=OFF",
    "-DINSTALL_MANPAGES=OFF",
    "-DINSTALL_PKGCONFIG_MODULES=OFF",
    "-DINSTALL_CMAKE_CONFIG_MODULE=OFF",
    "-DBUILD_SHARED_LIBS=OFF",
    f"-DOGG_LIBRARY={get_install_path_of_library('ogg')}/lib/{get_os_lib_prefix()}ogg{get_os_lib_ext()}",
    f"-DOGG_INCLUDE_DIR={get_install_path_of_library('ogg')}/include"
])


# ========================
build_meson_library('harfbuzz', [], ['cpp_rtti=true', 'tests=disabled', 'cpp_std=c++20', 'default_library=static'])

# generate
check_lists(list_info)

with open(get_list_info_file(),"w",encoding='utf-8') as fs:
    fs.write('# This file is generate by library/build.ps\n')
    fs.write('# Those library has no CMakeConfig file,so we use them manual\n')

    for key, value in list_info.items():
        include = ';'.join(value['include'])
        library = ';'.join(value['library'])

        fs.write(f"set({key}_INCLUDE_DIR \"{include}\")\n")
        fs.write(f"set({key}_LIBRARY \"{library}\")\n")
