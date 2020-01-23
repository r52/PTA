param (
    [switch]$debug = $false
)

$qtpath = $env:QTDIR

$rel = "Release"
$rels = "--release"

if ($debug)
{
    $rel = "Debug"
    $rels = "--debug"
}

if ($null -eq $qtpath)
{
    $qtpath = "C:\Qt\5.12.6\msvc2017_64"
}

$windeploy = "$($qtpath)\bin\windeployqt.exe"

# Copy files
$pkpath = "PTA_x64"
$binpath = "$($pkpath)\PTA.exe"
$dllpath = "$($pkpath)\QHotkey.dll"

New-Item $pkpath -Type Directory -Force > $null
Copy-Item ("x64\" + $rel + "\PTA.exe") -Destination $binpath -Force
Copy-Item ("x64\" + $rel + "\QHotkey.dll") -Destination $dllpath -Force

# Copy templates
Copy-Item .\PTA\templates\ -Destination ($pkpath + "\templates\") -Recurse -Force

# Copy SSL lib
if (($env:OPENSSL) -and (Test-Path $env:OPENSSL -pathType container)) {
    Copy-Item $env:OPENSSL\libcrypto-1_1-x64.dll -Destination $pkpath -Force
    Copy-Item $env:OPENSSL\libssl-1_1-x64.dll -Destination $pkpath -Force
}

# Deploy Qt
& $windeploy $rels $binpath

$pkgname = "$($pkpath)_no_stylesheets.7z"
Write-Host "Packaging $($pkgname)..."

& 7z a -t7z $pkgname .\$pkpath

if ($env:APPVEYOR -eq $true) {
    Get-ChildItem $pkgname | % { Push-AppveyorArtifact $_.FullName -FileName $_.Name }
}

# Copy stylesheets
Copy-Item .\PTA\stylesheets\ -Destination ($pkpath + "\stylesheets\") -Recurse -Force

$pkgname = "$($pkpath).7z"
Write-Host "Packaging $($pkgname)..."

& 7z a -t7z $pkgname .\$pkpath

if ($env:APPVEYOR -eq $true) {
    Get-ChildItem $pkgname | % { Push-AppveyorArtifact $_.FullName -FileName $_.Name }
}
