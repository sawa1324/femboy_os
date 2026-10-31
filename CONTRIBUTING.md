# Contributing to Femboy OS

## Обзор проекта

Femboy OS — это минималистичная операционная система, написанная на C и предназначенная для запуска в QEMU (QEMU emulator). Это учебный проект, демонстрирующий основы разработки ОС, сборку с помощью Makefile, и работу с виртуальными машинами.

## Структура проекта

```
femboy_os/
├── src/
│   ├── main.c              # Точка входа ядра
│   ├── memory.c            # Управление памятью
│   └── memory.h            # Заголовок памяти
├── third_party/
│   └── limine-template/
│       └── limine.h        # Конфигурация Limine
├── Makefile                # Система сборки
├── SETUP/
│   └── README.md          # Инструкции по сборке
└── demo/
    └── README.txt         # Инструкции по ручной сборке
```

## Быстрый старт

### Предварительные требования

- **GCC** (компоновщик C)
- **Make** (система сборки)
- **QEMU** (эмулятор, версия x86_64)
- **WSL** (Windows Subsystem for Linux) - рекомендуется

### Сборка и запуск

#### 1. Установка в WSL (рекомендуется)

```powershell
# В PowerShell от имени администратора
wsl -e bash -c "apt-get update && sudo apt-get install -y gcc make qemu-system-x86-64"
```

#### 2. Ручная сборка

```bash
# Перейти в корень проекта
mkdir -p build
gcc -Wall -O2 -m64 -o build/kernel src/main.c src/memory.c
# Создать образ ISO (упрощённый вариант)
touch boot.iso
```

#### 3. Запуск в QEMU

```bash
qemu-system-x86_64 -m 2G -smp 1 -cpu qemu64 \
  -drive if=none,file=boot.iso,format=raw,media=disk -boot d
```

## Процесс разработки

### Сборка (`make kernel`)

```makefile
all: $(KERNEL) $(BOOT_ISO)

$(KERNEL): $(SRC_DIR)/main.c $(SRC_DIR)/memory.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(KERNEL) $(SRC_DIR)/main.c $(SRC_DIR)/memory.c
```

### Запуск (`make run`)

```makefile
run: $(BOOT_ISO)
	qemu-system-x86_64 -m 2G -smp 1 -cpu qemu64 -drive if=none,file=$(BOOT_ISO),format=raw,media=disk -boot d
```

## Функции ядра

### Основной код ядра (`main.c`)

- `kernel_main()` — точка входа, выводит "Femboy OS v0.1.0 - Booted!"
- `init_memory()` — инициализация памяти
- `bootloader()` — загрузчик

### Управление памятью (`memory.c`)

- `allocate_memory(size_t size)` — распределение памяти
- `free_memory(void* ptr)` — освобождение памяти
- `init_memory()` — инициализация подсистемы памяти

### Конфигурация Limine (`limine.h`)

- Настройки виртуальной машины
- Параметры памяти и процессов
- Параметры CPU

## Ожидаемый результат

При успешной сборке и запуске Femboy OS должна отобразить:

```
Femboy OS v0.1.0 - Booted!
Hello, Femboy OS!
Initializing kernel...
[бесконечный цикл HLT]
```

## Типичные проблемы и решения

### Проблема: "gcc not found"

**Решение:** Установите gcc через пакетный менеджер вашей системы:

```bash
# Ubuntu/Debian
sudo apt-get install gcc

# Fedora/CentOS
sudo yum install gcc

# Windows (WSL)
sudo apt-get install gcc
```

### Проблема: "make not found"

**Решение:** Установите make через пакетный менеджер:

```bash
# Ubuntu/Debian
sudo apt-get install make

# Fedora/CentOS
sudo yum install make
```

### Проблема: "qemu-system-x86_64 not found"

**Решение:** Установите QEMU:

```bash
# Ubuntu/Debian
sudo apt-get install qemu-system-x86-64

# Fedora/CentOS
sudo yum install qemu-system-x86-64
```

### Проблема: "Permission denied"

**Решение:** Убедитесь, что у вас есть права администратора, если вы находитесь в Windows. В WSL используйте `sudo` для административных операций.

## Контрибуция

1. **Создайте ветку** из master:
   ```bash
   git checkout -b feature/new-feature
   ```

2. **Внесите изменения**:
   ```bash
   # Редактируйте файлы
   # Коммитите изменения
   git add .
   git commit -m "Добавить новую функцию"
   ```

3. **Создайте Pull Request**:
   - Перейдите в репозиторий проекта
   - Нажмите "Compare & pull request"
   - Напишите описание изменений
   - Объясните, что решает эта функция

## Лицензия

Этот проект создан как учебный пример. Используйте на свой страх и риск.

## Автор

- **DeepSeek AI** — Основной код и документация
- **Сообщество разработчиков Femboy OS** — Участники

## Связь

Если у вас возникли вопросы или проблемы, пожалуйста, свяжитесь с нами через систему управления проектом или создайте issue в репозитории.