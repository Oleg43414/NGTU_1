CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -fPIC -I$(INCLUDE_DIR)
LDFLAGS = -shared
DLFLAGS = -ldl

SRC_DIR = src
LIB_DIR = lib
BIN_DIR = bin
INCLUDE_DIR = include

# Основные цели
all: directories $(LIB_DIR)/librsa.so $(LIB_DIR)/libthreeway.so $(LIB_DIR)/libmorse.so $(BIN_DIR)/crypto_system

# Создание директорий
directories:
	@mkdir -p $(LIB_DIR) $(BIN_DIR)

# Компиляция RSA библиотеки
$(LIB_DIR)/librsa.so: $(SRC_DIR)/rsa_lib.cpp $(INCLUDE_DIR)/rsa_crypto.h
	@echo "Компиляция RSA библиотеки..."
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(SRC_DIR)/rsa_lib.cpp

# Компиляция 3-WAY библиотеки
$(LIB_DIR)/libthreeway.so: $(SRC_DIR)/threeway_crypto.cpp $(INCLUDE_DIR)/threeway_crypto.h
	@echo "Компиляция 3-WAY библиотеки..."
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(SRC_DIR)/threeway_crypto.cpp

# Компиляция Morse библиотеки
$(LIB_DIR)/libmorse.so: $(SRC_DIR)/morse_standalone.cpp $(INCLUDE_DIR)/morse_standalone.h
	@echo "Компиляция Morse библиотеки..."
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(SRC_DIR)/morse_standalone.cpp

# Компиляция основной программы (с динамической загрузкой всех библиотек)
$(BIN_DIR)/crypto_system: $(SRC_DIR)/main.cpp
	@echo "Компиляция основной программы..."
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_DIR)/main.cpp $(DLFLAGS)

# Проверка символов (подробная)
check-symbols:
	@echo "Проверка символов в библиотеках:"
	@echo "RSA:"
	@nm -D $(LIB_DIR)/librsa.so 2>/dev/null | grep run_rsa_crypto || echo "Символ не найден"
	@echo "3-WAY:"
	@nm -D $(LIB_DIR)/libthreeway.so 2>/dev/null | grep run_threeway_crypto || echo "Символ не найден"
	@echo "Morse:"
	@nm -D $(LIB_DIR)/libmorse.so 2>/dev/null | grep run_morse_demo || echo "Символ не найден"

# Тихая проверка символов
quiet-check-symbols:
	@nm -D $(LIB_DIR)/librsa.so 2>/dev/null | grep -q run_rsa_crypto && echo "✓ RSA библиотека загружена" || echo "✗ RSA символ не найден"
	@nm -D $(LIB_DIR)/libthreeway.so 2>/dev/null | grep -q run_threeway_crypto && echo "✓ 3-WAY библиотека загружена" || echo "✗ 3-WAY символ не найден"
	@nm -D $(LIB_DIR)/libmorse.so 2>/dev/null | grep -q run_morse_demo && echo "✓ Morse библиотека загружена" || echo "✗ Morse символ не найден"

# Запуск основной программы (без проверки символов)
run: all
	@echo "Запуск основной программы..."
	@LD_LIBRARY_PATH=$(LIB_DIR) $(BIN_DIR)/crypto_system

# Запуск с тихой проверкой
run-quiet: all quiet-check-symbols
	@echo "Запуск основной программы..."
	@LD_LIBRARY_PATH=$(LIB_DIR) $(BIN_DIR)/crypto_system

# Быстрая компиляция только основной программы
main-only: directories $(BIN_DIR)/crypto_system
	@echo "Компиляция основной программы завершена"
	@echo "Используйте 'make run' для запуска"

# Компиляция только Morse библиотеки
morse-only: directories $(LIB_DIR)/libmorse.so
	@echo "Компиляция Morse библиотеки завершена"

# Компиляция только RSA библиотеки
rsa-only: directories $(LIB_DIR)/librsa.so
	@echo "Компиляция RSA библиотеки завершена"

# Компиляция только 3-WAY библиотеки
threeway-only: directories $(LIB_DIR)/libthreeway.so
	@echo "Компиляция 3-WAY библиотеки завершена"

# Тестирование Morse библиотеки
test-morse: $(LIB_DIR)/libmorse.so
	@echo "=== Тестирование Morse библиотеки ==="
	@echo '#include <iostream>\nextern "C" { void run_morse_demo(); }\nint main() { run_morse_demo(); return 0; }' > test_morse.cpp
	$(CXX) $(CXXFLAGS) -o test_morse test_morse.cpp -L$(LIB_DIR) -lmorse
	@LD_LIBRARY_PATH=$(LIB_DIR) ./test_morse || echo "Тест завершен"
	@rm -f test_morse test_morse.cpp

# Тестирование RSA библиотеки
test-rsa: $(LIB_DIR)/librsa.so
	@echo "=== Тестирование RSA библиотеки ==="
	@echo '#include <iostream>\nextern "C" { void run_rsa_crypto(); }\nint main() { run_rsa_crypto(); return 0; }' > test_rsa.cpp
	$(CXX) $(CXXFLAGS) -o test_rsa test_rsa.cpp -L$(LIB_DIR) -lrsa
	@LD_LIBRARY_PATH=$(LIB_DIR) ./test_rsa || echo "Тест завершен"
	@rm -f test_rsa test_rsa.cpp

# Тестирование 3-WAY библиотеки
test-threeway: $(LIB_DIR)/libthreeway.so
	@echo "=== Тестирование 3-WAY библиотеки ==="
	@echo '#include <iostream>\nextern "C" { void run_threeway_crypto(); }\nint main() { run_threeway_crypto(); return 0; }' > test_threeway.cpp
	$(CXX) $(CXXFLAGS) -o test_threeway test_threeway.cpp -L$(LIB_DIR) -lthreeway
	@LD_LIBRARY_PATH=$(LIB_DIR) ./test_threeway || echo "Тест завершен"
	@rm -f test_threeway test_threeway.cpp

# Тестирование всех библиотек
test-all: test-morse test-rsa test-threeway
	@echo "=== Все тесты завершены ==="

# Проверка зависимостей
check-deps:
	@echo "Проверка зависимостей..."
	@which $(CXX) > /dev/null && echo "✓ Компилятор $(CXX) найден" || echo "✗ Компилятор $(CXX) не найден"
	@[ -f "$(SRC_DIR)/main.cpp" ] && echo "✓ Основной файл найден" || echo "✗ Основной файл не найден"
	@[ -f "$(SRC_DIR)/morse_standalone.cpp" ] && echo "✓ Файл Морзе найден" || echo "✗ Файл Морзе не найден"
	@[ -f "$(SRC_DIR)/rsa_lib.cpp" ] && echo "✓ Файл RSA найден" || echo "✗ Файл RSA не найден"
	@[ -f "$(SRC_DIR)/threeway_crypto.cpp" ] && echo "✓ Файл 3-WAY найден" || echo "✗ Файл 3-WAY не найден"
	@[ -f "$(INCLUDE_DIR)/morse_standalone.h" ] && echo "✓ Заголовок Морзе найден" || echo "✗ Заголовок Морзе не найден"
	@[ -f "$(INCLUDE_DIR)/rsa_crypto.h" ] && echo "✓ Заголовок RSA найден" || echo "✗ Заголовок RSA не найден"
	@[ -f "$(INCLUDE_DIR)/threeway_crypto.h" ] && echo "✓ Заголовок 3-WAY найден" || echo "✗ Заголовок 3-WAY не найден"

# Отладочная сборка
debug: CXXFLAGS += -g -DDEBUG
debug: all

# Профилировочная сборка
profile: CXXFLAGS += -pg
profile: LDFLAGS += -pg
profile: all

# Релизная сборка
release: CXXFLAGS += -O3 -DNDEBUG
release: all

# Статическая компиляция (только для основной программы)
static: CXXFLAGS += -static
static: directories $(BIN_DIR)/crypto_system

# Очистка
clean:
	@rm -rf $(LIB_DIR) $(BIN_DIR)
	@rm -f test_* *.txt *.desktop
	@echo "Очистка завершена"

# Очистка только объектных файлов
clean-obj:
	@find . -name "*.o" -delete
	@echo "Объектные файлы удалены"

# Показать информацию о сборке
info:
	@echo "=== Информация о сборке ==="
	@echo "Компилятор: $(CXX)"
	@echo "Флаги: $(CXXFLAGS)"
	@echo "Исходники: $(SRC_DIR)"
	@echo "Библиотеки: $(LIB_DIR)"
	@echo "Бинарники: $(BIN_DIR)"
	@echo "Доступные библиотеки:"
	@ls -la $(LIB_DIR)/*.so 2>/dev/null || echo "  (библиотеки не собраны)"

# Показать список символов в библиотеках
symbols: check-symbols

# Показать тихий список символов
symbols-quiet: quiet-check-symbols

# ==================== УСТАНОВКА И УПАКОВКА ====================

# Пути для установки
PREFIX = /usr/local
BIN_INSTALL_DIR = $(PREFIX)/bin
LIB_INSTALL_DIR = $(PREFIX)/lib
DESKTOP_DIR = /usr/share/applications
ICON_DIR = /usr/share/icons/hicolor/256x256/apps

# Системная установка (требует sudo)
install: all
	@echo "=== Установка Crypto System ==="
	@echo "Установка в системные директории..."
	@mkdir -p $(BIN_INSTALL_DIR) $(LIB_INSTALL_DIR) $(DESKTOP_DIR)
	@install -m 755 $(BIN_DIR)/crypto_system $(BIN_INSTALL_DIR)/crypto-system
	@install -m 644 $(LIB_DIR)/librsa.so $(LIB_INSTALL_DIR)/
	@install -m 644 $(LIB_DIR)/libthreeway.so $(LIB_INSTALL_DIR)/
	@install -m 644 $(LIB_DIR)/libmorse.so $(LIB_INSTALL_DIR)/
	@echo "[Desktop Entry]" > $(DESKTOP_DIR)/crypto-system.desktop
	@echo "Version=1.0" >> $(DESKTOP_DIR)/crypto-system.desktop
	@echo "Type=Application" >> $(DESKTOP_DIR)/crypto-system.desktop
	@echo "Name=Crypto System" >> $(DESKTOP_DIR)/crypto-system.desktop
	@echo "Comment=Система шифрования с Morse кодом, RSA и 3-WAY" >> $(DESKTOP_DIR)/crypto-system.desktop
	@echo "Exec=$(BIN_INSTALL_DIR)/crypto-system" >> $(DESKTOP_DIR)/crypto-system.desktop
	@echo "Categories=Utility;Security;" >> $(DESKTOP_DIR)/crypto-system.desktop
	@echo "Terminal=true" >> $(DESKTOP_DIR)/crypto-system.desktop
	@echo "StartupNotify=true" >> $(DESKTOP_DIR)/crypto-system.desktop
	@echo "✓ Программа установлена!"
	@echo "Запуск: crypto-system"
	@echo "Ярлык создан в меню приложений"

# Установка для текущего пользователя (без прав root)
install-local: all
	@echo "=== Установка Crypto System для текущего пользователя ==="
	@mkdir -p $(HOME)/.local/bin $(HOME)/.local/lib $(HOME)/.local/share/applications
	@cp $(BIN_DIR)/crypto_system $(HOME)/.local/bin/crypto-system
	@cp $(LIB_DIR)/librsa.so $(HOME)/.local/lib/
	@cp $(LIB_DIR)/libthreeway.so $(HOME)/.local/lib/
	@cp $(LIB_DIR)/libmorse.so $(HOME)/.local/lib/
	@echo "[Desktop Entry]" > $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "Version=1.0" >> $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "Type=Application" >> $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "Name=Crypto System" >> $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "Comment=Система шифрования с Morse кодом, RSA и 3-WAY" >> $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "Exec=$(HOME)/.local/bin/crypto-system" >> $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "Categories=Utility;Security;" >> $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "Terminal=true" >> $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "StartupNotify=true" >> $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "✓ Программа установлена для текущего пользователя!"
	@echo "Запуск: crypto-system"
	@echo "Ярлык создан в меню приложений"

# Создание ярлыка на рабочем столе (простая версия)
desktop-shortcut: all
	@echo "=== Создание ярлыка на рабочем столе ==="
	@DESKTOP_PATH="$(HOME)/Desktop"; \
	if [ ! -d "$$DESKTOP_PATH" ]; then \
		DESKTOP_PATH="$(HOME)/Рабочий стол"; \
		if [ ! -d "$$DESKTOP_PATH" ]; then \
			DESKTOP_PATH="$(HOME)"; \
			echo "Директория рабочего стола не найдена, создаем в домашней директории"; \
		fi; \
	fi; \
	echo "Создание ярлыка в: $$DESKTOP_PATH"; \
	echo "[Desktop Entry]" > "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "Version=1.0" >> "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "Type=Application" >> "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "Name=Crypto System" >> "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "Comment=Система шифрования с Morse кодом, RSA и 3-WAY" >> "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "Exec=$(shell pwd)/$(BIN_DIR)/crypto_system" >> "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "Path=$(shell pwd)" >> "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "Categories=Utility;Security;" >> "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "Terminal=true" >> "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "StartupNotify=true" >> "$$DESKTOP_PATH/crypto-system.desktop"; \
	chmod +x "$$DESKTOP_PATH/crypto-system.desktop"; \
	echo "✓ Ярлык создан: $$DESKTOP_PATH/crypto-system.desktop"; \
	echo "Запуск: двойной клик по ярлыку на рабочем столе"

# Создание ярлыка в текущей директории (для ручного перемещения)
create-desktop-file: all
	@echo "=== Создание файла ярлыка ==="
	@echo "[Desktop Entry]" > crypto-system.desktop
	@echo "Version=1.0" >> crypto-system.desktop
	@echo "Type=Application" >> crypto-system.desktop
	@echo "Name=Crypto System" >> crypto-system.desktop
	@echo "Comment=Система шифрования с Morse кодом, RSA и 3-WAY" >> crypto-system.desktop
	@echo "Exec=$(shell pwd)/$(BIN_DIR)/crypto_system" >> crypto-system.desktop
	@echo "Path=$(shell pwd)" >> crypto-system.desktop
	@echo "Categories=Utility;Security;" >> crypto-system.desktop
	@echo "Terminal=true" >> crypto-system.desktop
	@echo "StartupNotify=true" >> crypto-system.desktop
	@chmod +x crypto-system.desktop
	@echo "✓ Файл ярлыка создан: crypto-system.desktop"
	@echo "Переместите его вручную на рабочий стол:"
	@echo "  mv crypto-system.desktop ~/Desktop/"
	@echo "Или:"
	@echo "  mv crypto-system.desktop ~/Рабочий\\ стол/"

# Удаление программы
uninstall:
	@echo "=== Удаление Crypto System ==="
	@rm -f $(BIN_INSTALL_DIR)/crypto-system
	@rm -f $(LIB_INSTALL_DIR)/librsa.so
	@rm -f $(LIB_INSTALL_DIR)/libthreeway.so
	@rm -f $(LIB_INSTALL_DIR)/libmorse.so
	@rm -f $(DESKTOP_DIR)/crypto-system.desktop
	@echo "✓ Программа удалена из системы"

# Удаление локальной установки
uninstall-local:
	@echo "=== Удаление локальной установки ==="
	@rm -f $(HOME)/.local/bin/crypto-system
	@rm -f $(HOME)/.local/lib/librsa.so
	@rm -f $(HOME)/.local/lib/libthreeway.so
	@rm -f $(HOME)/.local/lib/libmorse.so
	@rm -f $(HOME)/.local/share/applications/crypto-system.desktop
	@echo "✓ Локальная установка удалена"

# Создание DEB пакета (для Ubuntu/Debian)
deb-package: all
	@echo "=== Создание DEB пакета ==="
	@mkdir -p deb-package/DEBIAN
	@mkdir -p deb-package/usr/bin
	@mkdir -p deb-package/usr/lib
	@mkdir -p deb-package/usr/share/applications
	@cp $(BIN_DIR)/crypto_system deb-package/usr/bin/crypto-system
	@cp $(LIB_DIR)/librsa.so deb-package/usr/lib/
	@cp $(LIB_DIR)/libthreeway.so deb-package/usr/lib/
	@cp $(LIB_DIR)/libmorse.so deb-package/usr/lib/
	@echo "[Desktop Entry]" > deb-package/usr/share/applications/crypto-system.desktop
	@echo "Version=1.0" >> deb-package/usr/share/applications/crypto-system.desktop
	@echo "Type=Application" >> deb-package/usr/share/applications/crypto-system.desktop
	@echo "Name=Crypto System" >> deb-package/usr/share/applications/crypto-system.desktop
	@echo "Comment=Система шифрования с Morse кодом, RSA и 3-WAY" >> deb-package/usr/share/applications/crypto-system.desktop
	@echo "Exec=crypto-system" >> deb-package/usr/share/applications/crypto-system.desktop
	@echo "Categories=Utility;Security;" >> deb-package/usr/share/applications/crypto-system.desktop
	@echo "Terminal=true" >> deb-package/usr/share/applications/crypto-system.desktop
	@echo "StartupNotify=true" >> deb-package/usr/share/applications/crypto-system.desktop
	@echo "Package: crypto-system" > deb-package/DEBIAN/control
	@echo "Version: 1.0" >> deb-package/DEBIAN/control
	@echo "Section: utils" >> deb-package/DEBIAN/control
	@echo "Priority: optional" >> deb-package/DEBIAN/control
	@echo "Architecture: amd64" >> deb-package/DEBIAN/control
	@echo "Maintainer: $(USER) <$(USER)@localhost>" >> deb-package/DEBIAN/control
	@echo "Description: Система шифрования с Morse кодом, RSA и 3-WAY" >> deb-package/DEBIAN/control
	@echo " Программа для шифрования и дешифрования данных с использованием" >> deb-package/DEBIAN/control
	@echo " различных криптографических алгоритмов." >> deb-package/DEBIAN/control
	@dpkg-deb --build deb-package crypto-system_1.0_amd64.deb 2>/dev/null || (echo "Для создания DEB пакета установите: sudo apt install dpkg-dev" && false)
	@rm -rf deb-package
	@echo "✓ DEB пакет создан: crypto-system_1.0_amd64.deb"
	@echo "Установка: sudo dpkg -i crypto-system_1.0_amd64.deb"

# Создание portable версии (все в одной директории)
portable: all
	@echo "=== Создание portable версии ==="
	@mkdir -p portable-crypto-system
	@cp $(BIN_DIR)/crypto_system portable-crypto-system/
	@cp $(LIB_DIR)/*.so portable-crypto-system/
	@echo "#!/bin/bash" > portable-crypto-system/run.sh
	@echo "cd \$$(dirname \"\$$0\")" >> portable-crypto-system/run.sh
	@echo "LD_LIBRARY_PATH=. ./crypto_system" >> portable-crypto-system/run.sh
	@chmod +x portable-crypto-system/run.sh
	@echo "✓ Portable версия создана в директории portable-crypto-system/"
	@echo "Запуск: cd portable-crypto-system && ./run.sh"

# Показать помощь
help:
	@echo "Доступные цели:"
	@echo ""
	@echo "=== ОСНОВНЫЕ ЦЕЛИ ==="
	@echo "  all            - Полная сборка всех компонентов"
	@echo "  run            - Сборка и запуск основной программы"
	@echo "  main-only      - Сборка только основной программы"
	@echo "  clean          - Полная очистка"
	@echo ""
	@echo "=== УСТАНОВКА И ЯРЛЫКИ ==="
	@echo "  install        - Системная установка (требует sudo)"
	@echo "  install-local  - Установка для текущего пользователя"
	@echo "  desktop-shortcut - Создать ярлык на рабочем столе (авто)"
	@echo "  create-desktop-file - Создать файл ярлыка для ручного перемещения"
	@echo "  uninstall      - Удалить системную установку"
	@echo "  uninstall-local - Удалить локальную установку"
	@echo ""
	@echo "=== ПАКЕТЫ ==="
	@echo "  deb-package    - Создать DEB пакет (Ubuntu/Debian)"
	@echo "  portable       - Создать portable версию"
	@echo ""
	@echo "=== ТЕСТИРОВАНИЕ ==="
	@echo "  test-all       - Протестировать все библиотеки"
	@echo "  test-morse     - Тестирование Morse библиотеки"
	@echo "  test-rsa       - Тестирование RSA библиотеки"
	@echo "  test-threeway  - Тестирование 3-WAY библиотеки"
	@echo ""
	@echo "=== ОТЛАДКА ==="
	@echo "  debug          - Отладочная сборка"
	@echo "  check-deps     - Проверка зависимостей"
	@echo "  symbols        - Показать символы в библиотеках"
	@echo "  info           - Информация о сборке"
	@echo ""
	@echo "=== ДОПОЛНИТЕЛЬНО ==="
	@echo "  morse-only     - Сборка только Morse библиотеки"
	@echo "  rsa-only       - Сборка только RSA библиотеки"
	@echo "  threeway-only  - Сборка только 3-WAY библиотеки"
	@echo "  release        - Релизная сборка"
	@echo "  profile        - Профилировочная сборка"
	@echo "  static         - Статическая компиляция"
	@echo ""
	@echo "Для подробной информации: make <цель>"

.PHONY: all directories run run-quiet main-only debug profile release static clean clean-obj info help check-symbols quiet-check-symbols check-deps morse-only rsa-only threeway-only test-morse test-rsa test-threeway test-all symbols symbols-quiet install install-local desktop-shortcut create-desktop-file uninstall uninstall-local deb-package portable

