
# Launch-VsDevShell.ps1 will change the cwd
$backer_cwd = Get-Location

if($args.Length -eq 0 -or $null -eq $env:UTOPIA_BUILD_ANDROID){
    Write-Error "this script need run under the build.py!"
    exit 1
}

$Android = [bool]::Parse($env:UTOPIA_BUILD_ANDROID)

# prepare
if($IsWindows){
    $vswhere = "${env:ProgramFiles(x86)}" + "\Microsoft Visual Studio\Installer\vswhere.exe"

    $install = &$vswhere -utf8 -latest -property installationPath

    $install = $install.Trim()

    &"$install\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64 -HostArch amd64


    $nasm_exe = Get-ChildItem -Path "$PSScriptRoot/tools/nasm" -Filter "nasm.exe" -Recurse -ErrorAction SilentlyContinue -Force
    $perl_exe = Get-ChildItem -Path "$PSScriptRoot/tools/perl" -Filter "perl.exe" -Recurse -ErrorAction SilentlyContinue -Force

    Write-Host "use $nasm_exe"
    Write-Host "use $perl_exe"

    $nasm = $nasm_exe.Directory.FullName
    $perl = $perl_exe.Directory.FullName

    Write-Host "add PATH $nasm"
    Write-Host "add PATH $perl"

    $env:PATH = "$nasm;" + $env:PATH
    $env:PATH = "$perl;" + $env:PATH
    
    $make = "nmake.exe"
}
else{
    $nasm_exe = "nasm"
    $perl_exe = "perl"
    $make = "make"
}

&$nasm_exe --version
if(-not $?) { exit 1}

&$perl_exe --version
if(-not $?) { exit 1}

# start build
$config_args = @("Configure")

if($IsWindows){
    $config_args += "VC-WIN64A"
}
elseif($Android){
    $config_args += "android-arm64"
}

Set-Location $backer_cwd
Write-Host "execute" $perl_exe $config_args $args

if($IsWindows){
    $env:CFLAGS = "/utf-8"
    $env:CXXFLAGS = "/utf-8"
}

&$perl_exe $config_args $args
if(-not $?) { exit 1}

&$make
if(-not $?) { exit 1}

&$make "install"
if(-not $?) { exit 1}
