if(-not(Test-Path "$PSScriptRoot/build" -PathType Container)){
    mkdir "$PSScriptRoot/build"
}
if(-not(Test-Path "$PSScriptRoot/install-tree" -PathType Container)){
    mkdir "$PSScriptRoot/install-tree"
}

$InstallPath = ("$PSScriptRoot/install-tree").Replace("\","/")

if($args.Length -ge 1){
    Write-Host "provide too many arguments!"
    Write-Host "provide one argument as install-tree"
    Write-Host "or no argument to use default install tree"
    return 1
}
elseif($args.Length -eq 1){
    $InstallPath = $args[0].Replace("\","/")
}

$cores = [System.Environment]::ProcessorCount

Set-Location "$PSScriptRoot/build"

#==============================
# BUILD FOR CMAKE PROJECT
#==============================
function build($name,$cmake_args){
    &cmake "-S" "$PSScriptRoot/$name" "-B" "$PSScriptRoot/build/$name"  `
    "-DCMAKE_BUILD_TYPE=Release"  `
    "-DCMAKE_MODULE_PATH=$InstallPath" `
    "-DCMAKE_INSTALL_PREFIX=$InstallPath/$name" `
    $cmake_args

    &cmake "--build" "$PSScriptRoot/build/$name" "-j$cores" "--config" "Release"
 
    &cmake "--install" "$PSScriptRoot/build/$name" "--prefix" "$InstallPath/$name" "--config" "Release"
}

Write-Host "build zlib"
build -name zlib -cmake_args @()

Write-Host "build libpng"
build -name libpng -cmake_args @(
    "-DPNG_SHARED=OFF","-DPNG_STATIC=ON","-DPNG_TEST=OFF",
    "-DZLIB_LIBRARY=$InstallPath/zlib/lib/zlibstatic.lib",
    "-DZLIB_INCLUDE_DIR=$InstallPath/zlib/include")

Write-Host "build SDL"
build -name SDL -cmake_args @()

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
if(-not(Test-Path "$PSScriptRoot/build/harfbuzz" -PathType Container)){
    mkdir "$PSScriptRoot/build/harfbuzz"
}
Set-Location "$PSScriptRoot/build/harfbuzz"

&"meson" "setup" "--buildtype=release" `
        "-Dcpp_rtti=true" "-Dtests=disabled" `
        "-Dcpp_std=c++20" "-Ddefault_library=static" `
        "--prefix=$InstallPath/harfbuzz" "$PSScriptRoot/harfbuzz"

&"ninja"

&"meson" "install"

#==============================
# DONE
#==============================
Set-Location "$PSScriptRoot"
