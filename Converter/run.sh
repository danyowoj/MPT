#!/bin/bash

# Цвета для вывода
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Исполняемый файл
EXEC="./test"

# Файлы исходников (обязательные)
SOURCES="Editor.cpp Convert_10p.cpp Convert_p10.cpp test.cpp"

# Флаги компиляции
CXXFLAGS="-std=c++11 -Wall -Wextra"

# Показать помощь
show_help() {
    cat << EOF
Использование: $0 [ОПЦИИ] [-- [АРГУМЕНТЫ_ТЕСТА]]

Опции:
  -c, --clean     Удалить скомпилированный файл перед сборкой
  -h, --help      Показать эту справку

Аргументы теста (передаются в исполняемый файл):
  --editor, -e       Тестировать только Editor
  --convert10p, -c10 Тестировать только Convert_10p
  --convertp10, -cp10 Тестировать только Convert_p10
  --roundtrip, -rt   Тестировать только совместную работу конвертеров
  --all, -a          Тестировать всё (по умолчанию)
  --quiet            Показывать только упавшие тесты

Примеры:
  $0                                 # компилирует и запускает все тесты
  $0 -- -e                           # только Editor
  $0 -- --convert10p --quiet         # только Convert_10p, без вывода успешных
  $0 --clean -- --editor --roundtrip # пересобрать и запустить Editor + roundtrip

EOF
}

# Обработка опций скрипта
CLEAN=0
TEST_ARGS=()

while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--clean)
            CLEAN=1
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        --)
            shift
            # Все оставшиеся аргументы — для тестов
            TEST_ARGS=("$@")
            break
            ;;
        *)
            echo "Неизвестная опция: $1"
            show_help
            exit 1
            ;;
    esac
done

# Удаление старого исполняемого файла
if [[ $CLEAN -eq 1 ]]; then
    echo "Очистка: удаляем $EXEC"
    rm -f $EXEC
fi

# Компиляция
echo "Компиляция..."
g++ $CXXFLAGS -o $EXEC $SOURCES
if [[ $? -ne 0 ]]; then
    echo -e "${RED}Ошибка компиляции${NC}"
    exit 1
fi
echo -e "${GREEN}Компиляция успешна${NC}"

# Запуск тестов
echo "Запуск тестов..."
if [[ ${#TEST_ARGS[@]} -eq 0 ]]; then
    # Без аргументов — запускаем все тесты
    $EXEC
else
    $EXEC "${TEST_ARGS[@]}"
fi

# Сохраняем код возврата
EXIT_CODE=$?

if [[ $EXIT_CODE -eq 0 ]]; then
    echo -e "${GREEN}Все тесты пройдены успешно${NC}"
else
    echo -e "${RED}Некоторые тесты не пройдены${NC}"
fi

exit $EXIT_CODE
