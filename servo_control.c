/*
 *    Atividade: Controle de Servo Motor com PWM e Experimento com LED RGB
 *
 *    Descrição:
 *    Este código implementa o controle de um servomotor através de PWM na GPIO 22 e demonstra,
 *    também, o comportamento de um LED RGB na GPIO 12 utilizando a ferramenta BitDogLab.
 *
 *    Requisitos e Funcionalidades:
 *    1) PWM na GPIO 22:
 *       - Configurar a frequência de PWM para aproximadamente 50Hz, o que corresponde a um período de 20ms.
 *
 *    2) Posição de 180 graus:
 *       - Ajustar o ciclo ativo do PWM para 2.400µs (duty cycle de ~0,12%), posicionando a flange (braço)
 *         do servomotor a aproximadamente 180 graus.
 *       - Manter essa posição por 5 segundos.
 *
 *    3) Posição de 90 graus:
 *       - Ajustar o ciclo ativo do PWM para 1.470µs (duty cycle de ~0,0735%), posicionando a flange a
 *         aproximadamente 90 graus.
 *       - Manter essa posição por 5 segundos.
 *
 *    4) Posição de 0 graus:
 *       - Ajustar o ciclo ativo do PWM para 500µs (duty cycle de ~0,025%), posicionando a flange a
 *         aproximadamente 0 graus.
 *       - Manter essa posição por 5 segundos.
 *
 *    5) Movimento Periódico e Suave:
 *       - Após as posições anteriores, implementar uma rotina que movimente periodicamente o braço do
 *         servomotor entre 0 e 180 graus.
 *       - A movimentação deve ser suave, utilizando incremento/decremento do ciclo ativo de ±5µs,
 *         com um atraso de 10ms entre cada ajuste.
 *
 *    6) Experimento com LED RGB (GPIO 12) utilizando BitDogLab:
 *       - Executar o código com a ferramenta BitDogLab, observando o comportamento do LED RGB.
 *       - Observação típica: o LED RGB apresenta variações de brilho (e possivelmente de cor) em sincronia
 *         com as alterações do PWM, evidenciando a modulação do sinal.
 *
 *    Autor: Heitor Rodrigues Lemos Dias
 *    Data: Fevereiro de 2025
 *
 */

 #include "pico/stdlib.h"
 #include "hardware/pwm.h"
 
 // ================================
 // DEFINIÇÕES DE CONSTANTES E PINOS
 // ================================
 
 #define SERVO_PIN 22
 #define LED_PIN   12  // LED RGB para o experimento com BitDogLab
 
 // Para uma resolução de 1µs, configuramos o PWM para 1MHz (125MHz/125)
 // Período do servo: 20ms = 20000µs
 const uint32_t SERVO_PERIOD_US = 20000; 
 // Como o contador vai de 0 até wrap, definimos:
 const uint16_t WRAP = SERVO_PERIOD_US - 1;  // 19999
 
 // Pulsos em microsegundos correspondentes às posições do servo:
 const uint16_t POS_180 = 2400;  // Aproximadamente 180 graus (12% de 20ms)
 const uint16_t POS_90  = 1470;  // Aproximadamente 90 graus  (7.35% de 20ms)
 const uint16_t POS_0   = 500;   // Aproximadamente 0 graus   (2.5% de 20ms)
 
 // Parâmetros para movimentação suave:
 const uint16_t STEP_US = 5;       // Incremento/decremento de 5µs
 const uint16_t STEP_DELAY_MS = 10;  // 10ms de atraso entre cada ajuste
 
 // ================================
 //    FUNÇÃO DE CONFIGURAR O PWM
 // ================================
 
 void pwm_config_servo(){
     // Configura a GPIO do servo como PWM
     gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
 
     // Obtém o número do slice associado à GPIO
     uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
 
     // Configura o divisor de clock para que o clock do PWM seja de 1MHz
     float divider = 125.0f;
     pwm_set_clkdiv(slice_num, divider);
 
     // Configura o wrap (valor máximo do contador) para gerar um período de 20000µs
     pwm_set_wrap(slice_num, WRAP);
 
     // Habilita o PWM
     pwm_set_enabled(slice_num, true);
 }
 
 void pwm_config_led(){
     // Configura a GPIO do servo como PWM
     gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
 
     // Obtém o número do slice associado à GPIO
     uint slice_num_led = pwm_gpio_to_slice_num(LED_PIN);
 
     // Configura o divisor de clock para que o clock do PWM seja de 1MHz
     // (considerando um clock do sistema de 125MHz: 125MHz/125 = 1MHz)
     float divider = 125.0f;
     pwm_set_clkdiv(slice_num_led, divider);
 
     // Configura o wrap (valor máximo do contador) para gerar um período de 20000µs
     pwm_set_wrap(slice_num_led, WRAP);
 
     // Habilita o PWM
     pwm_set_enabled(slice_num_led, true);
 }
 
 // Função para definir o pulso do servo (em microsegundos)
 void set_servo_pulse(uint16_t pulse_us) {
     uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
     // Assume-se o canal A para a SERVO_PIN
     pwm_set_chan_level(slice_num, PWM_CHAN_A, pulse_us);
 }
 
 void set_led_pulse(uint16_t pulse_us) {
     uint slice_num_led = pwm_gpio_to_slice_num(LED_PIN);
     // Assume-se o canal A para a SERVO_PIN
     pwm_set_chan_level(slice_num_led, PWM_CHAN_A, pulse_us);
 
 }
 
 
 int main() {
     stdio_init_all();
 
     // Configura PWM para o servo e a GPIO do LED
     pwm_config_servo();
     pwm_config_led();
 
     // =======================================================
     // 1) Posição de 180 graus: Pulso de 2400µs – 5 segundos
     // =======================================================
     set_servo_pulse(POS_180);
     set_led_pulse(POS_180);
     sleep_ms(5000);
 
     // =======================================================
     // 2) Posição de 90 graus: Pulso de 1470µs – 5 segundos
     // =======================================================
     set_servo_pulse(POS_90);
     set_led_pulse(POS_90);
     sleep_ms(5000);
 
     // =======================================================
     // 3) Posição de 0 graus: Pulso de 500µs – 5 segundos
     // =======================================================
     set_servo_pulse(POS_0);
     set_led_pulse(POS_0);
     sleep_ms(5000);
 
     // =======================================================
     // 4) Movimento Periódico e Suave entre 0 e 180 graus
     // =======================================================
     // O servo se movimentará suavemente incrementando/decrementando 5µs a cada 10ms
     while (true) {
         // Movimento de 0 para 180 graus (incremento suave)
         for (uint16_t pulse = POS_0; pulse <= POS_180; pulse += STEP_US) {
             set_servo_pulse(pulse);
             set_led_pulse(pulse);
             sleep_ms(STEP_DELAY_MS);
         }
         // Movimento de 180 para 0 graus (decremento suave)
         // Para evitar underflow, verificamos quando chegamos a POS_0
         for (uint16_t pulse = POS_180; pulse >= POS_0; pulse -= STEP_US) {
             set_servo_pulse(pulse);
             set_led_pulse(pulse);
             sleep_ms(STEP_DELAY_MS);
             if(pulse == POS_0) break;
         }
     }
 
     return 0;
 }
 