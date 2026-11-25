param([string]$SdkPath = $env:EMSDK, [string]$RaylibSource = '')
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
if ($SdkPath) {
    $env:EMSDK_PYTHON = (Get-ChildItem -Path (Join-Path $SdkPath 'python/*/python.exe') | Select-Object -First 1).FullName
    $emcmake = Join-Path $SdkPath 'upstream/emscripten/emcmake.bat'
} else { $emcmake = 'emcmake' }
$buildArgs = @('-S', $projectRoot, '-B', "$projectRoot/build/web", '-G', 'Ninja', '-DPLATFORM=Web', '-DCMAKE_BUILD_TYPE=Release', '-DBUILD_TESTING=OFF')
if ($RaylibSource) { $buildArgs += "-DFETCHCONTENT_SOURCE_DIR_RAYLIB=$RaylibSource" }
& $emcmake cmake @buildArgs
if ($LASTEXITCODE -ne 0) { throw 'Falha ao configurar Emscripten.' }
cmake --build "$projectRoot/build/web"
if ($LASTEXITCODE -ne 0) { throw 'Falha na compilacao web.' }
Write-Output "Exportacao pronta: $projectRoot/dist/web"
