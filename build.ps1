# Build script for Windows (PowerShell)

Write-Host "Building UART Driver Project..." -ForegroundColor Cyan
Write-Host "===============================" -ForegroundColor Cyan
Write-Host ""

# Create build directory
if (!(Test-Path -Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

Set-Location build

# Run CMake
Write-Host "Running CMake..." -ForegroundColor Yellow
cmake ..

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

# Build
Write-Host "Building..." -ForegroundColor Yellow
cmake --build .

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Set-Location ..

Write-Host ""
Write-Host "Build successful!" -ForegroundColor Green
Write-Host ""
Write-Host "To run tests:" -ForegroundColor Cyan
Write-Host "  .\build\Debug\uart_tests.exe    (Visual Studio)"
Write-Host "  .\build\uart_tests.exe          (MinGW)"
Write-Host ""
Write-Host "To run demo:" -ForegroundColor Cyan
Write-Host "  .\build\Debug\uart_demo.exe     (Visual Studio)"
Write-Host "  .\build\uart_demo.exe           (MinGW)"
