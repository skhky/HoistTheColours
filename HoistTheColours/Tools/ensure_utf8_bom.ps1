<#
Ensure a file is saved as UTF-8 with BOM.
Usage:
  powershell -ExecutionPolicy Bypass -File .\Tools\ensure_utf8_bom.ps1 <path-to-file>
#>
param(
    [Parameter(Mandatory=$true)]
    [string]$FilePath
)

if (-not (Test-Path $FilePath)) {
    Write-Error "File not found: $FilePath"
    exit 1
}

Write-Host "Converting $FilePath to UTF-8 with BOM..."

$txt = Get-Content -Raw -Encoding Default -Path $FilePath
# In Windows PowerShell, Set-Content -Encoding UTF8 writes with BOM
Set-Content -Path $FilePath -Value $txt -Encoding UTF8

Write-Host "Done. $FilePath is now saved as UTF-8 (with BOM on Windows PowerShell)."
