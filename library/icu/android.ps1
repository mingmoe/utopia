
Write-Host "this script only works on linux or WSL"

if(-not($IsLinux)){
    Write-Host "this script only support android!"
    return 1
}

if($null -eq $env:NDK_ROOT){
    Write-Host "need to set the NDK_ROOT envrionment variable to the root of the ndk!"
    return 1
}


$root = $env:NDK_ROOT

$tool_chain = $root + "/toolchains/llvm/prebuilt/linux-x86_64"

$source_root = "$PSScriptRoot/icu4c/source"

$cores = [System.Environment]::ProcessorCount()


# build for linux
if(-not(Test-Path -PathType Container build-linux)){
    mkdir build-linux
}
if(-not(Test-Path -PathType Container build-android)){
    mkdir build-android
}
Set-Location "$PSScriptRoot/build-linux"

&"$source_root/runConfigureICU" Linux
&"make" "-j$cores"


# build for android
Set-Location "$PSScriptRoot/build-android"
$env:CC = $tool_chain + "/bin/aarch64-linux-android33-clang"
$env:CXX = $tool_chain + "/bin/aarch64-linux-android33-clang++"
$env:CFLAGS = "-std=c11 -O3 -g3"
$env:CXXFLAGS = "-std=c++20 -O3 -g3"

&"$source_root/congifure" `
--with-data-packaging=static `
--with-cross-build="$PSScriptRoot/build-linux" `
--enable-tools=no `
--enable-tests=no `
--enable-samples=no `
--enable-shared=no `
--enable-static `
--enable-release `
--enable-icuio `
--prefix="$PSScriptRoot/android" `
--build=aarch64-linux-android `
--host=x86_64-linux-gnu

&"make" "-j$cores" "install"

Set-LocalGroup $PSScriptRoot
