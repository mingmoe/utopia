
$global:UTOPIA_PROJECT_ROOT = [System.IO.Path]::GetFullPath("$PSScriptRoot/../".Replace("\","/"))

function build-library($build_args){
    &python "$UTOPIA_PROJECT_ROOT/library/build.py" $build_args
}

function build-android-library($build_args){
    $ndk_path = $null

    if($null -eq $build_args){$build_args = @()}

    if($null -ne $env:NDK_ROOT){
        $ndk_path = $env:NDK_ROOT
    }
    if($null -ne $env:NDK){
        $ndk_path = $env:NDK
    }
    if($null -ne $NDK_PATH){
        $ndk_path = $NDK_PATH
    }

    if($null -eq $ndk_path){
        Write-Error 
        "unknown ndk path. Set up NDK_ROOT or NDK environment. Or set NDK_PATH powershell environment"
        return
    }

    $x = @("--android","--ndk","$ndk_path")

    $z = [object[]]::new($x.Length + $build_args.build_argsLength)
    $x.CopyTo($z, 0);
    $build_args.CopyTo($z, $x.Length);

    build-library $z
}

<#
    .SYNOPSIS 
    IF THERE ARE SOMETHING WRONG WITH GOLANG.
    DON'T TELL ME! IT'S A LITTER.
#>
function go{
    param(
        [Parameter(Position = 1,Mandatory = $false,ValueFromPipeline = $true)]
        [string]
        $Path = "root"
    )

    if($Path -eq "root"){
        Set-Location "$global:UTOPIA_PROJECT_ROOT"
        return
    }

    if(Test-Path -PathType Container -Path "$global:UTOPIA_PROJECT_ROOT/library/$Path"){
        Set-Location "$global:UTOPIA_PROJECT_ROOT/library/$Path"
        return
    }

    if(Test-Path -PathType Container -Path "$global:UTOPIA_PROJECT_ROOT/$Path"){
        Set-Location "$global:UTOPIA_PROJECT_ROOT/$Path"
        return
    }

    Set-Location "$Path"
}

function update-library(){
    $c = Get-Location
    go-root
    &git submodule update --init --recursive
    Set-Location $c
}

function clear-logs(){
    Remove-Item -Force -Recurse -Path "$UTOPIA_PROJECT_ROOT/library/*.log"
}
