# 解析参数
$index = 0

$android = $false
$mode = "Debug"
$ndk_root = $env:NDK_ROOT

while($index -ne $args.Length){
    $arg = $args[$index]

    if($arg -eq "android"){
        $android = $true
    }elseif($arg -eq "debug"){
        $mode = "Debug"
    }elseif($arg -eq "release"){
        $mode = "Release"
    }elseif($arg -eq "--ndk"){
        $index += 1

        if($index -ge $args.Length){
            Write-Error "miss ndk path in arguments"
            exit 1
        }
        else{
            $ndk_root = $args[$index]
        }
    }
    else{
        Write-Error "unknown argument $arg"
        exit 1
    }
    $index += 1
}

if($android){
    $sysroot_name = "android-arm64-$mode"
}elseif($IsWindows){
    $sysroot_name = "windows-x64-$mode"
}elseif($IsLinux){
    $sysroot_name = "linux-x64-$mode"
}elseif($IsMacOS){
    $sysroot_name = "macos-x64-$mode"
}else{
    Write-Error "unknown platform"
    exit 1
}

# 设置ndk
function fail($result){
    if($result -eq $false){
        Set-Location $PSScriptRoot
        Write-Host "failed"
        exit 1
    }
}

$global_cmake_options = @()
if($android){
    $MINSDKVERSION = 28
    Write-Host "android api version $MINSDKVERSION"

    if($null -eq $ndk_root){
        Write-Error "not found ndk. Set NDK_ROOT environment variable or argument"
        exit 1
    }

    $global_cmake_options += "-DCMAKE_TOOLCHAIN_FILE=$ndk_root/build/cmake/android.toolchain.cmake"
    $global_cmake_options += "-DANDROID_PLATFORM=android-$MINSDKVERSION"
    $global_cmake_options += "-DANDROID_ABI=arm64-v8a"
    $global_cmake_options += "-DANDROID_STL=c++_shared"
    $global_cmake_options += "-G"
    $global_cmake_options += "Ninja"
}

# 准备构建
mkdir "$PSScriptRoot/build" -Force
mkdir "$PSScriptRoot/install-tree" -Force
mkdir "$PSScriptRoot/install-tree/$sysroot_name" -Force

$InstallPath = ("$PSScriptRoot/install-tree/$sysroot_name").Replace("\","/")

$cores = [System.Environment]::ProcessorCount

Set-Location "$PSScriptRoot/build"

#==============================
# BUILD FOR CMAKE PROJECT
#==============================
function build($name,$cmake_args){
    $spath = "$PSScriptRoot/$name"
    $ipath = "$InstallPath/$name"
    $bpath = "$PSScriptRoot/build/$sysroot_name/$name"
    $outf = "$PSScriptRoot/$sysroot_name-$name-out.log"
    $errf = "$PSScriptRoot/$sysroot_name-$name-err.log"

    &cmake "-S" "$spath" "-B" "$bpath"  `
    $global_cmake_options `
    "-DCMAKE_BUILD_TYPE=$mode"  `
    "-DCMAKE_MODULE_PATH=$InstallPath" `
    "-DCMAKE_INSTALL_PREFIX=$ipath" `
    $cmake_args 1>$outf  2>$errf
    fail($?)

    &cmake "--build" "$bpath" "-j$cores" "--config" "$mode" 1>>$outf  2>>$errf
    fail($?)

    &cmake "--install" "$bpath" "--prefix" "$ipath" "--config" "$mode"  1>>$outf  2>>$errf
    fail($?)
}

Write-Host "build zlib"
build -name zlib -cmake_args @()

Write-Host "build libpng"
build -name libpng -cmake_args @(
    "-DPNG_SHARED=OFF","-DPNG_STATIC=ON","-DPNG_TEST=OFF",
    "-DZLIB_LIBRARY=$InstallPath/zlib/lib/zlibstatic.lib",
    "-DZLIB_INCLUDE_DIR=$InstallPath/zlib/include")

Write-Host "build SDL"
build -name SDL -cmake_args @("-DSDL_SHARED=OFF","-DHAVE_GCC_WDECLARATION_AFTER_STATEMENT=OFF")

Write-Host "build freetype"
build -name freetype -cmake_args @(
    "-DZLIB_INCLUDE_DIR=$InstallPath/zlib/include",
    "-DZLIB_LIBRARY=$InstallPath/zlib/lib/zlibstatic.lib",
    "-DPNG_PNG_INCLUDE_DIR=$InstallPath/libpng/include"
    "-DPNG_LIBRARY=$InstallPath/libpng/lib/libpng16_static.lib"
)

#==============================
# BUILD FOR OTHER PROJECT
#==============================
Write-Host "build harfbuzz"
mkdir "$PSScriptRoot/build/$sysroot_name/harfbuzz" -Force
$build_dir =  "$PSScriptRoot/build/$sysroot_name/harfbuzz" 

# generate cross build file
$global_meson_args = @()
if($android){
    $TOOL_CHAIN_PATH = (Get-Item -Path "C:\DirectProgram\Tools\android-ndk-r25\toolchains\llvm\prebuilt\*\bin\").ToString().Replace("\","/")

    if($IsWindows){
        $EXE = ".exe"
    }else{
        $EXE = ""
    }

    $txt = (Get-Content -Path "$PSScriptRoot/meson-android.txt" -Raw)

    $txt = $txt.Replace("(BINARY_PATH)",$TOOL_CHAIN_PATH)
    $txt = $txt.Replace("(EXE)",$EXE)
    $txt = $txt.Replace("(API_VERSION)",$MINSDKVERSION)

    [System.IO.File]::WriteAllText("$build_dir/machine.txt",$txt)

    $global_meson_args = @("--cross-file","$build_dir/machine.txt")
}

# build
Set-Location "$build_dir"

&"meson" "setup" "--buildtype=$($mode.ToLower())" `
        "-Dcpp_rtti=true" "-Dtests=disabled" `
        "-Dcpp_std=c++20" "-Ddefault_library=static" `
        "--prefix=$InstallPath/harfbuzz" "$PSScriptRoot/harfbuzz" `
        $global_meson_args
fail($?)

&"ninja"
fail($?)

&"meson" "install"
fail($?)

#==============================
# DONE
#==============================
Set-Location "$PSScriptRoot"
