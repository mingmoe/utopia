
$UTOPIA_PROJECT_ROOT = "$PSScriptRoot/../".Replace("\","/")

function build-library($build_args){
    &python "$UTOPIA_PROJECT_ROOT/library/build.py" $build_args
}

function go-library($library){
    Set-Location "$UTOPIA_PROJECT_ROOT/library/$library"
}

function go-out(){
    if(-not(Test-Path "$UTOPIA_PROJECT_ROOT/out" -Type Container)){
        New-Item -Type Directory "$UTOPIA_PROJECT_ROOT/out"
    }
    Set-Location "$UTOPIA_PROJECT_ROOT/out"
}

function go-build(){
    if(-not(Test-Path "$UTOPIA_PROJECT_ROOT/build" -Type Container)){
        New-Item -Type Directory "$UTOPIA_PROJECT_ROOT/build"
    }
    Set-Location "$UTOPIA_PROJECT_ROOT/build"
}

function go-root(){
    Set-Location $UTOPIA_PROJECT_ROOT
}

function go-script(){
    Set-Location "$UTOPIA_PROJECT_ROOT/script"
}

function go-inc($project){
    Set-Location "$UTOPIA_PROJECT_ROOT/utopia-$project/include"
}

function go-src($project){
    Set-Location "$UTOPIA_PROJECT_ROOT/utopia-$project/src"
}

function go-proj($project){
    Set-Location "$UTOPIA_PROJECT_ROOT/utopia-$project/"
}

function go-cmake(){
    Set-Location "$UTOPIA_PROJECT_ROOT/cmake/"
}

function resource(){
    . "$UTOPIA_PROJECT_ROOT/script/pwsh.ps1"
}

function clone-subproj(){
    &git submodule update --init --recursive
}

function update-library(){
    $cwd = Get-Location

    go-root

    &git submodule "foreach" git pull origin master

    Set-Location $cwd
}
