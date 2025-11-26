#include "repl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
//vozmojno trushnee peremestit v > src

//Основные функции:
    //Автодополнение команд по нажатию Tab
    //История команд - навигация стрелками Вверх/Вниз
    //Обработка команд начинающихся с '/'
    //Локальное выполнение некоторых команд (/help, /time)
static const char* client_commands[] = {
    "/time", "/stats", "/shutdown", "/help", NULL
};

static int server_users_count = 0; 

static char* client_command_generator(const char* text, int state) {
    static int list_index;
    static size_t text_len;
    const char* cmd;

    if (!state) {
        list_index = 0;
        text_len = strlen(text);
    }

    while ((cmd = client_commands[list_index++])) {
        if (strncmp(cmd, text, text_len) == 0) {
            return strdup(cmd);
        }
    }
    return NULL;
}


static char** client_completion(const char* text, int start, int end) {
    (void)start;  // 33-34 dont tach weror atata
    (void)end;
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, client_command_generator);
}

void set_server_users_count(int count) {
    server_users_count = count;
}

void client_commands_init(void) {
    rl_attempted_completion_function = client_completion;
}

void get_current_time_formatted(char* buffer, size_t size) {
    time_t raw_time;
    struct tm* time_info;
    
    time(&raw_time);
    time_info = localtime(&raw_time);
    
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", time_info);
}


static void handle_time_command(void) {
    char time_buffer[64];
    get_current_time_formatted(time_buffer, sizeof(time_buffer));
    printf("Текущее время: %s\n", time_buffer);
}


static void handle_stats_command(void) {
    if (server_users_count > 0) {
        printf("Статистика сервера:\n");
        printf(" - Сейчас онлайн: %d\n", server_users_count);
    } else {
        printf("Запрос статистики отправлен на сервер...\n");
    }
}

// Обработка команды /shutdown
static void handle_shutdown_command(void) {
    printf("Завершение работы клиента...\n");
    exit(0);
}

static void handle_help_command(void) {
   printf("\n=== ДОСТУПНЫЕ КОМАНДЫ ==="
         "/time     - текущее время и дата"
         "/stats    - статистика сервера"
         "/shutdown - завершение работы"
         "/help     - эта справка");
}


bool client_handle_command(const char* command) {
    if (command == NULL || strlen(command) == 0) {
        return false;
    }

    if (command[0] == '/') {
        if (strcmp(command, "/time") == 0) {
            handle_time_command();
        }
        else if (strcmp(command, "/stats") == 0) {
            handle_stats_command();
        }
        else if (strcmp(command, "/shutdown") == 0) {
            handle_shutdown_command();
        }
        else if (strcmp(command, "/help") == 0) {
            handle_help_command();
        }
        else {
            printf("Неизвестная команда: %s\n", command);
            printf("Введите /help для списка команд\n");
        }
        return true; 
    }
    
    return false;
}

bool is_command(const char* input) {
    return input && input[0] == '/';
}