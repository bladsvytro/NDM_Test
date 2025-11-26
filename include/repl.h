#ifndef REPL_H
#define REPL_H

#include <stdbool.h>
#include <time.h>
#include <stdio.h>

// Инициализация командной системы клиента
void client_commands_init(void);

// Обработка команды от пользователя
bool client_handle_command(const char *command);

// Получить текущее время и дату в формате "2025-11-10 17:28:45"
void get_current_time_formatted(char *buffer, size_t size);

// Проверить, является ли строка командой (начинается с '/')
bool is_command(const char *input);

#endif