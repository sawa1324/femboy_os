# ================================================================================
# Инструкция по сборке Femboy OS
# ================================================================================
# Эта инструкция поможет тебе собрать и запустить Femboy OS в QEMU.
# 
# Текущая точка: Ядро написано, Makefile создан, но QEMU не установлен.
# Следующие шаги: Установить QEMU → Собрать ядро → Запустить в QEMU
# ================================================================================

## 📋 Шаг 1: Проверка текущего состояния

Открой PowerShell и выполни следующие команды:

```powershell
# Проверить наличие gcc (должно быть в WSL)
wsl -e bash -c "gcc --version"

# Проверить наличие make (должно быть в WSL)  
wsl -e bash -c "make --version"

# Проверить наличие QEMU (если нет - нужно установить)
qemu-system-x86_64 --version
```

## 📋 Шаг 2: Установка QEMU и Make через Chocolatey

### Вариант А: Через PowerShell скрипт (рекомендуется)

1. Открой **PowerShell как администратор** (Ctrl+Shift+Enter, затем "Run as administrator")
2. Скопируй содержимое файла `SETUP/scripts/install.ps1` и вставь в консоль
3. Нажми Enter для запуска

### Вариант Б: Через Chocolatey вручную

1. Открой PowerShell как администратор
2. Выполни команду:
   ```powershell
   choco install qemu-system make --yes
   ```

## 📋 Шаг 3: Сборка ядра Femboy OS

После установки QEMU выполни:

```powershell
# Собрать ядро через gcc (в WSL)
wsl -e bash -c "cd /mnt/c/Projects/femboy_os && make kernel"

# Проверить, что файл создан
ls -lh /mnt/c/Projects/femboy_os/kernel/boot/kernel.bin
```

## 📋 Шаг 4: Запуск в QEMU

### Вариант А: Через Make (если установлен)

```powershell
make run
```

### Вариант Б: Ручной запуск через PowerShell

```powershell
# Собрать образ ISO (64МБ)
wsl -e bash -c "cd /mnt/c/Projects/femboy_os && make boot"

# Запустить QEMU вручную
qemu-system-x86_64 \
  -m 2G \
  -smp 1 \
  -cpu qemu64 \
  -drive if=none,file=femboy-os.iso,format=raw,media=disk \
  -cdrom boot.iso \
  -boot d
```

## 📋 Шаг 5: Что должно произойти при запуске

1. Загрузчик Limine должен появиться в меню выбора ОС
2. Выбери "Femboy OS" (или нажми Enter)
3. Должно появиться сообщение: **"Femboy OS v0.1.0 - Booted!"** ✅
4. Затем: **"Initializing kernel..."**
5. В конце бесконечный цикл с HLT (ядро работает!)

## 📋 Шаг 6: Очистка артефактов

```powershell
make clean
```

---

## 🔍 Если что-то не работает

### Проблема: "gcc not found"
**Решение:** Установи gcc через Chocolatey или скачай вручную для WSL.

### Проблема: "qemu-system-x86_64 not found"  
**Решение:** Установи QEMU System x86_64 (см. Шаг 2)

### Проблема: "make not found"
**Решение:** Установи Make через Chocolatey или скачай вручную

---

## 📞 Помощь

Если возникли проблемы, напиши мне с описанием ошибки и я помогу её решить!
