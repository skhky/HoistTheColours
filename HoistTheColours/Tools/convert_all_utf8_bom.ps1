<# Convert all .cpp and .h files under Source to UTF-8 with BOM.
   Usage: powershell -ExecutionPolicy Bypass -File .\Tools\convert_all_utf8_bom.ps1
#>

Set-StrictMode -Version Latest

$projectRoot = (Get-Location).Path
$sourceDir = Join-Path $projectRoot 'Source'
if (-not (Test-Path $sourceDir)) {
    Write-Error "Source directory not found: $sourceDir"
    exit 1
}

$files = Get-ChildItem -Path $sourceDir -Recurse -Include *.cpp,*.h -File -ErrorAction SilentlyContinue
foreach ($f in $files) {
    try {
        $text = Get-Content -Raw -Encoding Default -Path $f.FullName
        # Write UTF8 with BOM (Windows PowerShell Set-Content -Encoding UTF8 writes BOM)
        Set-Content -Path $f.FullName -Value $text -Encoding UTF8
        Write-Host "Converted: $($f.FullName)"
    } catch {
        Write-Error "Failed: $($f.FullName) : $_"
    }
}

Write-Host "Done converting files."