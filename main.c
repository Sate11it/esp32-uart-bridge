#include <driver/uart.h>

#define UART_PC UART_NUM_0
#define UART_NFC UART_NUM_2
#define PC_TX 1
#define PC_RX 3
#define NFC_TX 17
#define NFC_RX 16
#define BAUD 115200
#define BUF 256

void setup() {
  uart_config_t cfg = {
    .baud_rate = BAUD,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh = 0,
    .source_clk = UART_SCLK_APB,
  };
  
  uart_param_config(UART_PC, &cfg);
  uart_set_pin(UART_PC, PC_TX, PC_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_PC, BUF, BUF, 0, NULL, 0);
  
  uart_param_config(UART_NFC, &cfg);
  uart_set_pin(UART_NFC, NFC_TX, NFC_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_NFC, BUF, BUF, 0, NULL, 0);
}

void loop() {
  uint8_t buf[BUF];
  int n;
  
  n = uart_read_bytes(UART_PC, buf, BUF, 0);
  if (n > 0) uart_write_bytes(UART_NFC, buf, n);
  
  n = uart_read_bytes(UART_NFC, buf, BUF, 0);
  if (n > 0) uart_write_bytes(UART_PC, buf, n);
}
