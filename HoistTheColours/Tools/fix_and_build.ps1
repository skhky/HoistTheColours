<#
Fix and build helper for HoistTheColours

Usage (Windows PowerShell):
  Open PowerShell as Administrator and run:
    cd <project root>\HoistTheColours
    .\Tools\fix_and_build.ps1

What this script does:
 1. Create a text backup of modified files (in Tools\backup)
 2. Normalize encoding of .cpp and .h files to UTF-8
 3. Remove common intermediate folders (Binaries, Intermediate, Saved)
 4. Regenerate Visual Studio project files using Unreal's GenerateProjectFiles.bat if available
 5. Invoke Unreal Build to build the editor and save log to Desktop (hoist_build_log.txt)

Note: This script does not modify engine files. Run only in Windows with UE installed.
#>

param()

function Log {
    param([string]$m)
    Write-Host "[fix_and_build] $m"
}

Set-StrictMode -Version Latest

$projectRoot = (Get-Location).Path
Log "Project root: $projectRoot"

# 1) Backup
$backupDir = Join-Path $projectRoot "Tools\backup"
if (-Not (Test-Path $backupDir)) { New-Item -ItemType Directory -Path $backupDir | Out-Null }
Log "Backing up source files to $backupDir"
Get-ChildItem -Path $projectRoot -Recurse -Include *.cpp,*.h,*.inl -ErrorAction SilentlyContinue | ForEach-Object {
    $rel = $_.FullName.Substring($projectRoot.Length).TrimStart('\')
    $dest = Join-Path $backupDir $rel
    $destDir = Split-Path $dest
    if (-not (Test-Path $destDir)) { New-Item -ItemType Directory -Path $destDir -Force | Out-Null }
    Copy-Item -Path $_.FullName -Destination $dest -Force
}

# 2) Normalize encodings to UTF8
Log "Normalizing file encodings to UTF-8 for .cpp and .h files"
Get-ChildItem -Path $projectRoot -Recurse -Include *.cpp,*.h -ErrorAction SilentlyContinue | ForEach-Object {
    $content = Get-Content -Raw -Encoding Default -Path $_.FullName
    # overwrite as UTF8 (PowerShell Core: no BOM; Windows PowerShell adds BOM)
    Set-Content -Path $_.FullName -Value $content -Encoding UTF8
}

# 3) Remove intermediate folders
@("Binaries","Intermediate","Saved") | ForEach-Object {
    $p = Join-Path $projectRoot $_\
    if (Test-Path $p) {
        Log "Removing $p"
        Remove-Item -Recurse -Force -Path $p -ErrorAction SilentlyContinue
    }
}

# 4) Regenerate project files if GenerateProjectFiles exists near engine install or project root
$engineRoot = "C:\Program Files\Epic Games\UE_5.7"
$genScript = Join-Path $engineRoot "Engine\Build\BatchFiles\GenerateProjectFiles.bat"
if (Test-Path $genScript) {
    Log "Regenerating Visual Studio project files using $genScript"
    & $genScript | Out-Null
} else {
    Log "GenerateProjectFiles.bat not found at $genScript. Skipping regeneration."
}

# 5) Build using Engine Build.bat
$buildScript = Join-Path $engineRoot "Engine\Build\BatchFiles\Build.bat"
if (Test-Path $buildScript) {
    $uproject = Get-ChildItem -Path $projectRoot -Filter *.uproject -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($uproject -eq $null) {
        Log "Could not find .uproject in project root. Aborting build."
        exit 1
    }

    $logPath = [Environment]::GetFolderPath('Desktop') + "\hoist_build_log.txt"
    $args = "HoistTheColoursEditor Win64 Development -Project=\"$($uproject.FullName)\" -WaitMutex -FromMsBuild"
    Log "Starting build. Logs will be written to $logPath"
    & $buildScript $args > $logPath 2>&1
    Log "Build finished. See $logPath"
} else {
    Log "Build.bat not found at $buildScript. Ensure Unreal Engine is installed at $engineRoot or edit this script."
}

Log "Done"
