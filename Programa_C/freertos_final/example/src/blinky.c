/*When using ISR based freertos applications, when an ISR happens, the CPU hangs at a hardwault while(1) handler.
The issues is bypassed when replacing #if __CORTEX_M == 0U to #if 0 in vendor_interrupt.c file.*/
#include "board.h"
#include "stdio.h"
#include "EDU_CIAA.h"
#include "arm_math.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define PORT(n)				((uint8_t) n)
#define PIN(n)				((uint8_t) n)
#define GRUPO(n)			((uint8_t) n)
#define MATCH(n)			((uint8_t) n)
#define LPC_SSP           LPC_SSP1
#define SSP_IRQ           SSP1_IRQn
#define BUFFER_SIZE                         1
#define SSP_DATA_BITS                       (SSP_BITS_16)

#define WAVE_SINE     0x2000

static uint16_t Tx_Buf[BUFFER_SIZE];	// Buffer de transmision
static uint16_t Rx_Buf[BUFFER_SIZE]; // Buffer de recepcion
//static uint16_t counter=100;
static SSP_ConfigFormat ssp_format;
static Chip_SSP_DATA_SETUP_T xf_setup;
long int dato = 0;
static char aux[8];
long int counter = 2000000;
long int counter_ant = 0;
long int function = 0;
long int function_ant = 0;
int multi = 0;
int func = 0;
int func_ant = 0;
int i, j;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
static void prvSetupHardware(void){
	SystemCoreClockUpdate();
	Chip_GPIO_Init(LPC_GPIO_PORT);

	Chip_SCU_PinMux( GRUPO(1) , PIN(0) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC0 );
	Chip_SCU_PinMux( GRUPO(1) , PIN(1) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC0 );
	Chip_SCU_PinMux( GRUPO(1) , PIN(2) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC0 );
	Chip_SCU_PinMux( GRUPO(1) , PIN(6) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC0 );

	Chip_SCU_PinMux( GRUPO(2) , PIN(10) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC0 );
	Chip_SCU_PinMux( GRUPO(2) , PIN(11) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC0 );
	Chip_SCU_PinMux( GRUPO(2) , PIN(12) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC0 );

	Chip_SCU_PinMux( GRUPO(2) , PIN(0) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC4 );
	Chip_SCU_PinMux( GRUPO(2) , PIN(1) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC4 );
	Chip_SCU_PinMux( GRUPO(2) , PIN(2) , SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS , SCU_MODE_FUNC4 );

	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT , PORT(5) , PIN(0));
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT , PORT(5) , PIN(1));
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT , PORT(5) , PIN(2));
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT , PORT(3) , PIN(3));

	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT , 0, 4);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT , 0, 8);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT , 0, 9);

	Chip_GPIO_SetPinState(LPC_GPIO_PORT, PORT(1) , PIN(11) , FALSE);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, PORT(1) , PIN(12) , FALSE);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, PORT(0) , PIN(14) , FALSE);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, PORT(3) , PIN(3) , FALSE);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, PORT(5) , PIN(0) , FALSE);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, PORT(5) , PIN(1) , FALSE);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, PORT(5) , PIN(2) , FALSE);


	//Leds
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 5 , 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 5 , 1);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 5 , 2);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 0 , 14);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 1 , 11);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 1 , 12);

	//Leds_OFF
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 5, 0, FALSE); //R
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 5, 1, FALSE); //G
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 5, 2, FALSE); //B
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 0,14, FALSE); // Rojo
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1,11, FALSE); // Amarillo
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1,12, FALSE); // Verde
}

void delay(int j){
	for(i=j;i;i--){
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 5, 1, TRUE); // Verde
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 5, 1, FALSE); // Verde
	}
}

void UART_Init(void){

   //Initialize peripheral
   Chip_UART_Init(CIAA_BOARD_UART);

   // Set Baud rate
   Chip_UART_SetBaud(CIAA_BOARD_UART, SYSTEM_BAUD_RATE);

   // Enable UART Transmission
   Chip_UART_TXEnable(CIAA_BOARD_UART);


   Chip_SCU_PinMux(7, 1, MD_PDN, FUNC6);              /* P7_1: UART2_TXD */
   Chip_SCU_PinMux(7, 2, MD_PLN|MD_EZI|MD_ZI, FUNC6); /* P7_2: UART2_RXD */

   //Enable UART Rx Interrupt
   Chip_UART_IntEnable(CIAA_BOARD_UART,UART_IER_RBRINT);   //Receiver Buffer Register Interrupt

   NVIC_EnableIRQ(USART2_IRQn);

   NVIC_SetPriority(USART2_IRQn, 6);

}

void Config_SSP(void){

	Board_SSP_Init(LPC_SSP);
	Chip_SSP_Init(LPC_SSP);
	SCU->SFSP[15][4]=(SCU_MODE_FUNC0 | SCU_MODE_DES);		// Reloj
	SCU->SFSP[1][20]=(SCU_MODE_FUNC1 | SCU_MODE_DES);

	ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
	ssp_format.bits = SSP_DATA_BITS;
	ssp_format.clockMode = SSP_CLOCK_MODE2;
	Chip_SSP_SetFormat(LPC_SSP, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
	Chip_SSP_Enable(LPC_SSP);

	/* Initialize GPDMA controller */
	Chip_GPDMA_Init(LPC_GPDMA);

	/* Setting GPDMA interrupt */
	NVIC_DisableIRQ(DMA_IRQn);
	NVIC_SetPriority(DMA_IRQn, ((0x01 << 3) | 0x01));
	NVIC_EnableIRQ(DMA_IRQn);

	/* Setting SSP interrupt */
	NVIC_EnableIRQ(SSP_IRQ);

	/* Select SSP Mode */
	Chip_SSP_SetMaster(LPC_SSP, 1);  // Master
}

/* Select the Transfer mode : Polling */
static void enviar(uint16_t dato) {

	Tx_Buf[0] = dato;			// INST1

	xf_setup.length = BUFFER_SIZE;
	xf_setup.tx_data = Tx_Buf;
	xf_setup.rx_data = Rx_Buf;

	xf_setup.rx_cnt = xf_setup.tx_cnt = 0;

	// Envio de instrucciones para lectura
	Chip_SSP_WriteFrames_Blocking(LPC_SSP, Tx_Buf, BUFFER_SIZE);
	dato=0;
}

void AD9833setFrequency(long frequency){
   long FreqWord = (frequency * pow(2, 28)) / 25.0E6;
   int MSB = (int)((FreqWord & 0xFFFC000) >> 14);
   int LSB = (int)(FreqWord & 0x3FFF);
   LSB |= 0x4000;
   MSB |= 0x4000;
   enviar(0x2100);
   enviar(LSB);
   enviar(MSB);
   enviar(0xC000);
   enviar(WAVE_SINE);
}

void actualiza_func(){

   if(counter_ant != counter){
      AD9833setFrequency(counter);
      dato = counter;

      aux[0] = (dato/10000000) +'0';
      aux[1] = ((dato%10000000)/1000000) +'0';
      aux[2] = (((dato%10000000)%1000000)/100000) +'0';
      aux[3] = ((((dato%10000000)%1000000)%100000)/10000) +'0';
      aux[4] = (((((dato%10000000)%1000000)%100000)%10000)/1000) +'0';
      aux[5] = ((((((dato%10000000)%1000000)%100000)%10000)%1000)/100) +'0';
      aux[6] = (((((((dato%10000000)%1000000)%100000)%10000)%1000)%100)/10) +'0';
      aux[7] = (((((((dato%10000000)%1000000)%100000)%10000)%1000)%100)%10) +'0';

	  Chip_UART_SendBlocking(LPC_USART2, aux, sizeof(aux));
	  Chip_UART_SendBlocking(LPC_USART2, "\r\n\r", sizeof(1));
   }
   counter_ant = counter;

}

void AD9833reset(){
   enviar(0x100);
   actualiza_func();
}

int main(void) {

	prvSetupHardware();
	Board_Init();
	UART_Init();
	Config_SSP();
	AD9833reset();

while(1){

	if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 9) == 0){//tecla 3 Multi
		multi++;
		delay(500000);
	}

	if (multi == 0){
		if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 4) == 0){//tecla 1 UP
			if(counter<12500000){
				counter++;
				actualiza_func(counter);
				delay(300000);
			}
		}
		if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 8) == 0){//tecla 2 DOWN
			if(counter>0){
				counter--;
				actualiza_func(counter);
				delay(300000);
			}
		}
	}

	if (multi == 1){
		if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 4) == 0){//tecla 1 UP
			if(counter<=12499000){
				counter = counter + 1000;
				actualiza_func(counter);
				delay(300000);
			}
		}
		if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 8) == 0){//tecla 2 DOWN
			if(counter>1000){
				counter = counter - 1000;
				actualiza_func(counter);
				delay(300000);
			}
		}
	}

	if (multi == 2){
		if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 4) == 0){//tecla 1 UP
			if(counter<12000000){
				counter = counter + 1000000;
				actualiza_func(counter);
				delay(300000);
			}
		}
		if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 8) == 0){//tecla 2 DOWN
			if(counter>1000000){
				counter = counter - 1000000;
				actualiza_func(counter);
				delay(300000);
			}
		}
	}

	if (multi >= 3){
		multi = 0;
	}
	if (multi == 0){
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 0,14, FALSE); // Rojo
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1,11, FALSE); // Amarillo
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1,12, TRUE); // Verde
	}
	if (multi == 1){
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 0,14, FALSE); // Rojo
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1,11, TRUE); // Amarillo
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1,12, FALSE); // Verde
	}
	if (multi == 2){
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 0,14, TRUE); // Rojo
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1,11, FALSE); // Amarillo
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1,12, FALSE); // Verde
	}

		Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, 5, 2); //Rojo

	}
	return 0;
}

