param(
  [string]$SketchbookDirectory = $env:ARDUINO_DIRECTORIES_USER
)

$ErrorActionPreference = 'Stop'

$previousUserDirectory = $env:ARDUINO_DIRECTORIES_USER
$userDirectory = $SketchbookDirectory
if ([string]::IsNullOrWhiteSpace($userDirectory)) {
  $userDirectory = Join-Path $env:USERPROFILE 'Documents\Arduino'
}

$libraryPaths = @(
  'ArduinoJson'
  'Adafruit_GFX_Library'
  'Adafruit_BusIO'
  'U8g2_for_Adafruit_GFX'
  'ESP_SSLClient'
  'ESP_Mail_Client'
) | ForEach-Object { Join-Path (Join-Path $userDirectory 'libraries') $_ }

foreach ($libraryPath in $libraryPaths) {
  if (-not (Test-Path $libraryPath)) {
    throw "Required Arduino library not found: $libraryPath"
  }
}

$isolatedUserDirectory = Join-Path $PSScriptRoot 'third_party\arduino-user'
New-Item -ItemType Directory -Force -Path (Join-Path $isolatedUserDirectory 'libraries') | Out-Null

$buildArguments = @(
  'compile'
  '--fqbn'
  'esp32:esp32:dfrobot_firebeetle2_esp32e'
)

foreach ($libraryPath in $libraryPaths) {
  $buildArguments += @('--library', $libraryPath)
}

$buildArguments += @('--jobs', '0', $PSScriptRoot)
$buildExitCode = 0

try {
  $env:ARDUINO_DIRECTORIES_USER = $isolatedUserDirectory
  & arduino-cli @buildArguments
  $buildExitCode = $LASTEXITCODE
}
finally {
  if ([string]::IsNullOrWhiteSpace($previousUserDirectory)) {
    Remove-Item Env:ARDUINO_DIRECTORIES_USER -ErrorAction SilentlyContinue
  }
  else {
    $env:ARDUINO_DIRECTORIES_USER = $previousUserDirectory
  }
}

exit $buildExitCode