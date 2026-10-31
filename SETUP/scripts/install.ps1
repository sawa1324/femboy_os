# ================================================================================
# Установка QEMU для Windows (PowerShell)
# ================================================================================
# Этот скрипт поможет тебе установить QEMU и Make на Windows.
# 
# Инструкция:
# 1. Открой PowerShell как администратор
# 2. Скопируй и вставь содержимое этого файла
# 3. Нажми Enter для запуска
# ================================================================================

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Установка QEMU и Make для Femboy OS" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Проверить наличие Chocolatey
$chocoPath = "$env:ProgramFiles\Chocolatey\bin\choco.exe"
if (-not (Test-Path $chocoPath)) {
    Write-Host "Chocolatey не найден. Устанавливаем..." -ForegroundColor Yellow
    
    # Скачиваем Chocolatey
    Invoke-WebRequest -Uri "https://community.chocolatey.org/install.ps1" -OutFile "$env:TEMP\chocolatey-install.ps1"
    & "$env:TEMP\chocolatey-install.ps1" -NoRestart
}

Write-Host ""
Write-Host "[1/3] Установка Chocolatey..." -ForegroundColor Green
& $chocoPath install chocolatey --yes

Write-Host ""
Write-Host "[2/3] Установка QEMU System x86_64..." -ForegroundColor Green
& $chocoPath install qemu-system make --yes

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Установка завершена!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Проверка установки:" -ForegroundColor Yellow
& $chocoPath list | Select-String "qemu-system|make"
