 Инструкция по сборке Qt-версии приложения

## Требования

1. **Qt 5.15+ или Qt 6.x** - установленный и настроенный
2. **CMake 3.16+**
3. **Компилятор C++20** (MSVC, GCC, Clang)

## Способы сборки

### Способ 1: Использование CMakeLists_qt.txt напрямую

```bash
# Создайте директорию для сборки
mkdir build_qt
cd build_qt

# Укажите путь к Qt (примеры):
# Windows (если Qt установлен в стандартное место):
cmake -DCMAKE_PREFIX_PATH="C:/Qt/6.5.0/msvc2019_64" -f ../CMakeLists_qt.txt ..

# Linux:
cmake -DCMAKE_PREFIX_PATH="/usr/lib/qt5" -f ../CMakeLists_qt.txt ..

# macOS:
cmake -DCMAKE_PREFIX_PATH="/usr/local/opt/qt@5" -f ../CMakeLists_qt.txt ..

# Сборка
cmake --build .
```

### Способ 2: Переименовать файл в CMakeLists.txt

```bash
# Переименуйте CMakeLists_qt.txt в CMakeLists.txt (временно)
cp CMakeLists_qt.txt CMakeLists.txt.backup
mv CMakeLists_qt.txt CMakeLists.txt

# Затем используйте стандартную сборку CMake
mkdir build_qt
cd build_qt
cmake -DCMAKE_PREFIX_PATH="путь_к_Qt" ..
cmake --build .
```

### Способ 3: Использование в CLion

1. Откройте проект в CLion
2. File → Settings → Build, Execution, Deployment → CMake
3. Добавьте новый профиль:
   - Name: `Qt Build`
   - CMake options: `-DCMAKE_PREFIX_PATH="путь_к_Qt" -f CMakeLists_qt.txt`
   - Или скопируйте `CMakeLists_qt.txt` в `CMakeLists.txt` временно

## Настройка пути к Qt

### Windows
```bash
# Если Qt установлен через установщик:
set CMAKE_PREFIX_PATH=C:/Qt/6.5.0/msvc2019_64

# Или для Qt5:
set CMAKE_PREFIX_PATH=C:/Qt/5.15.2/msvc2019_64
```

### Linux
```bash
# Для Qt5 (установленного через пакетный менеджер):
export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/qt5

# Или если Qt установлен вручную:
export CMAKE_PREFIX_PATH=/opt/qt/6.5.0/gcc_64
```

### macOS
```bash
# Если Qt установлен через Homebrew:
export CMAKE_PREFIX_PATH=/usr/local/opt/qt@5

# Или для Qt6:
export CMAKE_PREFIX_PATH=/opt/homebrew/opt/qt6
```

## Проверка установки Qt

```bash
# Проверьте, что Qt найден:
cmake --find-package -DNAME=Qt6 -DCOMPILER_ID=GNU -DLANGUAGE=CXX -DMODE=COMPILE

# Или для Qt5:
qmake --version
```

## Запуск приложения

После успешной сборки:

```bash
# Windows
.\kursovayacpp_qt.exe

# Linux/macOS
./kursovayacpp_qt
```

## Решение проблем

### Ошибка: "Could not find a package configuration file provided by Qt6/Qt5"

**Решение:** Убедитесь, что указан правильный путь к Qt через `CMAKE_PREFIX_PATH`:
```bash
cmake -DCMAKE_PREFIX_PATH="полный_путь_к_Qt" ..
```

### Ошибка: "MOC failed"

**Решение:** Убедитесь, что `CMAKE_AUTOMOC` включен (он включен по умолчанию в файле).

### Ошибка компиляции с Q_OBJECT

**Решение:** Убедитесь, что файлы с Q_OBJECT (qt_ui.h, qt_ui.cpp) включены в список SOURCES и что AUTOMOC включен.

## Примечания

- Убедитесь, что все исходные файлы проекта находятся в той же директории, что и `CMakeLists_qt.txt`















