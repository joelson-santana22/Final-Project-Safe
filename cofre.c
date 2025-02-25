// UNIDADE 7 | CAPÍTULO 1 | PROJETO FINAL: Simulação de um cofre mecânico na placa de desenvolvimento BitDogLab //

#include <stdio.h> 
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"

// Definições de constantes
#define IS_RGBW false               // Define se os LEDs são RGBW (falso para RGB)
#define NUM_PIXELS 25               // Número de LEDs na matriz
#define WS2812_PIN 7                // Pino GPIO conectado aos LEDs WS2812
#define BTN_A_PIN 5                 // Pino GPIO para o botão A
#define BTN_B_PIN 6                 // Pino GPIO para o botão B
#define DEBOUNCE_DELAY_US 300000    // Tempo de debounce para os botões (300ms)

// Variáveis globais
volatile int digit = 0;             // Dígito atual selecionado pelo botão A
volatile int password[6] = {1, 1, 2, 2, 3, 3 }; // Senha correta
volatile int user_input[6] = {0, 0, 0, 0, 0, 0}; // Entrada do usuário
volatile int input_index = 0;       // Índice para a entrada do usuário
volatile bool reset_pending = false; // Flag para indicar se um reset está pendente
volatile bool first_click = false;  // Flag para rastrear o primeiro clique no botão A


bool numbers[10][NUM_PIXELS] = { { // Número 0
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0
},
{ // Número 1
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0
},
{ // Número 2
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0
},
{ // Número 3
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0
},
{ // Número 4
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0
},
{ // Número 5
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0
},
{ // Número 6
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0
},
{ // Número 7
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0
},
{ // Número 8
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0
},
{ // Número 9
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0
}
};
bool X_pattern[NUM_PIXELS] = { // x para erro

    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1

};
bool V_pattern[NUM_PIXELS] = { // v para acerto

    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
};

bool netx_try[NUM_PIXELS] = {// Seta para o botão A
    0, 0, 1, 0, 0,
    0, 1, 0, 0, 0,
    1, 1, 1, 1, 1,
    0, 1, 0, 0, 0,
    0, 0, 1, 0, 0
    };
    
   
    
    // Função para enviar um pixel para a matriz de LEDs
    static inline void put_pixel(uint32_t pixel_grb) {
        pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
    }
    
    // Função para criar uma cor no formato GRB
    static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
        return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
    }
    
    // Função para exibir um padrão na matriz de LEDs
    void display_number(bool *buffer, uint8_t r, uint8_t g, uint8_t b) {
        uint32_t color = urgb_u32(r, g, b); // Cria a cor
        for (int i = 0; i < NUM_PIXELS; i++) {
            put_pixel(buffer[i] ? color : 0); // Envia a cor para cada LED
        }
    }
   
    // Função para resetar a entrada do usuário e exibir a seta
    void reset_input() {
        input_index = 0;                // Reinicia o índice de entrada
        digit = 0;                      // Reinicia o dígito atual
        reset_pending = false;          // Desativa o flag de reset pendente
        first_click = false;            // Reinicia o flag de primeiro clique
        display_number(netx_try, 0, 70, 0); // Exibe a seta
    }
    
    // Callback do timer para resetar a entrada após 3 segundos
    bool reset_timer_callback(struct repeating_timer *t) {
        reset_input(); // Chama a função de reset
        return false;  // Não repete o timer
    }
    
    // Função para verificar se a senha está correta
    void check_password() {
        for (int i = 0; i < 4; i++) {
            if (user_input[i] != password[i]) { // Compara a entrada com a senha
                display_number(X_pattern, 70, 0, 0); // Exibe o padrão de erro (X)
                struct repeating_timer timer;
                add_repeating_timer_ms(3000, reset_timer_callback, NULL, &timer); // Configura o timer para resetar
                reset_pending = true; // Ativa o flag de reset pendente
                return; // Sai da função
            }
        }
        // Se a senha estiver correta
        display_number(V_pattern, 0, 70, 0); // Exibe o padrão de acerto (V)
        struct repeating_timer timer;
        add_repeating_timer_ms(3000, reset_timer_callback, NULL, &timer); // Configura o timer para resetar
        reset_pending = true; // Ativa o flag de reset pendente
    }
    
    // Callback para os botões A e B
    void button_callback(uint gpio, uint32_t events) {
        static volatile uint64_t last_interrupt_time = 0;
        uint64_t current_time = time_us_64();
        
        // Verifica o debounce e se um reset não está pendente
        if (current_time - last_interrupt_time > DEBOUNCE_DELAY_US && !reset_pending) {
            if (gpio == BTN_A_PIN) { // Botão A pressionado
                if (!first_click) {
                    first_click = true; // Marca o primeiro clique
                }
                digit = (digit + 1) % 10; // Incrementa o dígito (0-9)
            } else if (gpio == BTN_B_PIN && first_click) { // Botão B pressionado após o primeiro clique
                user_input[input_index++] = digit; // Armazena o dígito na entrada do usuário
                if (input_index == 6) { // Se 6 dígitos foram inseridos
                    check_password(); // Verifica a senha
                }
            }
            last_interrupt_time = current_time; // Atualiza o tempo do último clique
        }
    }
    
    // Função principal
    int main() {
        stdio_init_all(); // Inicializa a stdio (para debug, se necessário)
        
        // Configura os pinos dos botões
        gpio_init(BTN_A_PIN);
        gpio_set_dir(BTN_A_PIN, GPIO_IN);
        gpio_pull_up(BTN_A_PIN); // Habilita o pull-up interno
        gpio_init(BTN_B_PIN);
        gpio_set_dir(BTN_B_PIN, GPIO_IN);
        gpio_pull_up(BTN_B_PIN); // Habilita o pull-up interno
    
        // Configura o PIO para controlar os LEDs WS2812
        PIO pio = pio0;
        int sm = 0;
        uint offset = pio_add_program(pio, &ws2812_program);
        ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    
        // Configura as interrupções dos botões
        gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_RISE, true, &button_callback);
        gpio_set_irq_enabled_with_callback(BTN_B_PIN, GPIO_IRQ_EDGE_RISE, true, &button_callback);
    
        // Exibe a seta inicial
        reset_input();
    
        // Loop principal
        while (1) {
            if (!reset_pending) { // Se não houver reset pendente
                if (first_click) {
                    // Exibe o dígito atual após o primeiro clique no botão A
                    display_number(numbers[digit], 70, 70, 70);
                } else {
                    // Exibe a seta antes do primeiro clique no botão A
                    display_number(netx_try, 0, 70, 0);
                }
            }
            sleep_ms(100); // Aguarda 100ms para evitar uso excessivo da CPU
        }
    }