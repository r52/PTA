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
    $qtpath = "C:\Qt\5.12.5\msvc2017_64"
}

$windeploy = "$($qtpath)\bin\windeployqt.exe"

# Copy files
$pkpath = "PTA_x64"
$binpath = "$($pkpath)\PTA.exe"
$dllpath = "$($pkpath)\QHotkey.dll"

New-Item $pkpath -Type Directory -Force > $null
Copy-Item ("x64\" + $rel + "\PTA.exe") -Destination $binpath -Force
Copy-Item ("x64\" + $rel + "\QHotkey.dll") -Destination $dllpath -Force

# Deploy Qt
& $windeploy $rels $binpath

$pkgname = "$($pkpath).7z"
Write-Host "Packaging $($pkgname)..."

& 7z a -t7z $pkgname .\$pkpath

if ($env:APPVEYOR -eq $true) {
    Get-ChildItem $pkgname | % { Push-AppveyorArtifact $_.FullName -FileName $_.Name }
}
