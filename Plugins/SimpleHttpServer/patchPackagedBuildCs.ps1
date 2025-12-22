param(
  [Parameter(Mandatory = $true)]
  [string] $Path,

  [Parameter(Mandatory = $true)]
  [ValidateSet('true','false')]
  [string] $Value
)

$ErrorActionPreference = 'Stop'

if ([string]::IsNullOrWhiteSpace($Path)) {
  throw "Build.cs path is empty."
}
if (-not (Test-Path -LiteralPath $Path)) {
  throw "Build.cs not found: $Path"
}

$lines = Get-Content -LiteralPath $Path
if ($null -eq $lines) { $lines = @() }

# 1) Replace existing bUsePrecompiled if present
$has = $false
$out = New-Object System.Collections.Generic.List[string]

foreach ($l in $lines) {
  if ($l -match '^\s*bUsePrecompiled\s*=') {
    $has = $true
    $indent = ($l -replace '^(\s*).*','$1')
    $out.Add($indent + "bUsePrecompiled = $Value;")
  } else {
    $out.Add($l)
  }
}

# 2) Insert if missing
if (-not $has) {
  $arr = $out.ToArray()

  # Prefer insert after PCHUsage (ModuleRules. optional)
  $idx = [Array]::FindIndex($arr, [Predicate[string]]{
    param($x)
    $x -match '^\s*PCHUsage\s*=\s*(?:ModuleRules\.)?PCHUsageMode\.UseExplicitOrSharedPCHs\s*;'
  })

  if ($idx -ge 0) {
    $indent = ($arr[$idx] -replace '^(\s*).*','$1')
    $out.Insert($idx + 1, $indent + "bUsePrecompiled = $Value;")
  } else {
    # Fallback: insert after first '{' inside file
    $idxBrace = [Array]::FindIndex($arr, [Predicate[string]]{ param($x) $x -match '\{' })
    if ($idxBrace -lt 0) { $idxBrace = 0 }

    $indent = '    '
    if ($idxBrace + 1 -lt $arr.Length) {
      $nextIndent = ($arr[$idxBrace + 1] -replace '^(\s*).*','$1')
      if (-not [string]::IsNullOrEmpty($nextIndent)) { $indent = $nextIndent }
    }

    $out.Insert($idxBrace + 1, $indent + "bUsePrecompiled = $Value;")
  }
}

Set-Content -LiteralPath $Path -Value $out -Encoding UTF8
Write-Host "Patched: $Path (bUsePrecompiled=$Value)"
