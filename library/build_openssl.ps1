
$config_args = @("Configure")

# Launch-VsDevShell.ps1 will change the cwd
$backer_cwd = Get-Location

if($args.Length -eq 0 -or $null -eq $env:UTOPIA_BUILD_ANDROID){
    Write-Error "this script need run under the build.py!"
    exit 1
}

$Android = [bool]::Parse($env:UTOPIA_BUILD_ANDROID)

# prepare
if($IsWindows -and -not $Android){
    $vswhere = "${env:ProgramFiles(x86)}" + "\Microsoft Visual Studio\Installer\vswhere.exe"

    $install = &$vswhere -utf8 -latest -property installationPath

    $install = $install.Trim()

    Write-Host "Use" "$install\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64 -HostArch amd64
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

    
    $env:CC = "cl"
    $env:CXX = "cl"

    Write-Host "use $env:CC"
    Write-Host "use $env:CXX"

    $config_args += "VC-WIN64A"
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
if($Android){
    $config_args += "android-arm64"
}

Set-Location $backer_cwd

$command_perl_args = @() + ( $config_args | ForEach-Object -Process {$_.ToString().Trim('"')}) + ( $args | ForEach-Object -Process {$_.ToString().Trim('"')})
Write-Host "execute" $perl_exe $command_perl_args

if($IsWindows){
    $env:CFLAGS = "/utf-8"
    $env:CXXFLAGS = "/utf-8"
    Write-Host "enable /utf-8 msvc options"
}

&$perl_exe $command_perl_args
if(-not $?) { exit 1}

&$make
if(-not $?) { exit 1}

&$make "install"
if(-not $?) { exit 1}
