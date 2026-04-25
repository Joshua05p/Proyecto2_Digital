/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "Bitmaps.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fatfs_sd.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
SPI_HandleTypeDef hspi1;
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char buffer[100];

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
////////////////////////////////////////////
#define FILAS 30
#define COLUMNAS 11
#define OFFSET_X 88
#define OFFSET_Y 0
#define NUM_NEXT 4

const uint16_t *tablero[FILAS][COLUMNAS];

extern uint16_t menu[];

volatile int anim = 0;


volatile int pos_menu = 0;
volatile uint8_t modo_juego = 0;

volatile uint8_t b_derecha = 0;
volatile uint8_t b_izquierda = 0;
volatile uint8_t b1 = 0;
volatile uint8_t b2 = 0;



int pieza_x = OFFSET_X + (COLUMNAS / 2) * 8 - 16;
int pieza_y = 0;
int old_x = 0;

uint32_t lastFall = 0;


int (*pieza_actual)[4][4];
int (*pieza_siguiente)[4][4];
const uint16_t *color_siguiente;
const uint16_t *pieza_color = C_Rojo;

int (*cola_piezas[NUM_NEXT])[4][4];
const uint16_t *cola_colores[NUM_NEXT];


uint8_t estado_actual;
uint8_t estado_anterior = 255;

uint8_t rx5Data;

uint32_t tiempo_inicio = 0;
uint8_t modo_time_attack = 0;
static uint32_t lastPrint = 0;

int score = 0;
int lineas = 0;
int nivel = 1;

uint8_t musica_on = 1;
uint8_t opciones = 0;
uint8_t opciones_1 = 0;

volatile uint8_t salir_menu = 0;
int pieza_Z[4][4][4] = {
    // Rotación 0
    {
        {0,0,0,0},
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    // Rotación 1
    {
        {0,0,1,0},
        {0,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
    },
    // Rotación 2
    {
         {0,0,0,0},
         {0,1,1,0},
         {1,1,0,0},
         {0,0,0,0}
    },
    // Rotación 3
    {
         {0,1,0,0},
         {0,1,1,0},
         {0,0,1,0},
         {0,0,0,0}
    }

};
int pieza_L[4][4][4] = {
    // Rotación 0
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    // Rotación 1
    {
        {0,0,0,0},
        {1,1,1,0},
        {1,0,0,0},
        {0,0,0,0}
    },
    // Rotación 2
    {
         {0,1,1,0},
         {0,0,1,0},
         {0,0,1,0},
         {0,0,0,0}
    },
    // Rotación 3
    {
         {0,0,0,0},
         {0,0,0,1},
         {0,1,1,1},
         {0,0,0,0}
    }

};
int pieza_C[4][4][4] = {
    // Rotación 0
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    // Rotación 1
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    // Rotación 2
    {
         {0,0,0,0},
         {0,1,1,0},
         {0,1,1,0},
         {0,0,0,0}
    },
    // Rotación 3
    {
         {0,0,0,0},
         {0,1,1,0},
         {0,1,1,0},
         {0,0,0,0}
    }

};
int pieza_I[4][4][4] = {
    // Rotación 0
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0}
    },
    // Rotación 1
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    // Rotación 2
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0}
    },
    // Rotación 3
    {
         {0,0,0,0},
         {0,0,0,0},
         {1,1,1,1},
         {0,0,0,0}
    }

};

int pieza_R[4][4][4] = {
    // Rotación 0
    {
        {0,0,0,0},
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0}
    },
    // Rotación 1
    {
        {1,0,0,0},
        {1,1,0,0},
        {1,0,0,0},
        {0,0,0,0}
    },
    // Rotación 2
    {
         {1,1,1,0},
         {0,1,0,0},
         {0,0,0,0},
         {0,0,0,0}
    },
    // Rotación 3
    {
         {0,0,1,0},
         {0,1,1,0},
         {0,0,1,0},
         {0,0,0,0}
    }

};




void guardar_en_tablero();
////////////////////////////////////////////
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_UART5_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void borrar_pieza(int x, int y, int rot) {
    for (int f = 0; f < 4; f++) {
        for (int c = 0; c < 4; c++) {
            if (pieza_actual[rot][f][c] == 1) {
                FillRect(x + c*8, y + f*8, 8, 8, 0x0000);
            }
        }
    }
}
void dibujar_pieza(int x, int y, int rot) {
    for (int f = 0; f < 4; f++) {
        for (int c = 0; c < 4; c++) {
            if (pieza_actual[rot][f][c] == 1) {
                LCD_Sprite_Transparent(x + c*8, y + f*8, 8, 8, pieza_color, 1, 0, 0, 0, 0x0746);
            }
        }
    }
}

void guardar_en_tablero() {
    for (int f = 0; f < 4; f++) {
        for (int c = 0; c < 4; c++) {
            if (pieza_actual[anim][f][c] == 1) {
                int col = ((pieza_x - OFFSET_X) / 8) + c;
                int fila = ((pieza_y - OFFSET_Y) / 8) + f + 1;
                if (fila >= 0 && fila < FILAS && col >= 0 && col < COLUMNAS){

                	tablero[fila][col] = pieza_color;
                }
            }
        }
    }
}
void dibujar_tablero() {

	FillRect(OFFSET_X, OFFSET_Y, COLUMNAS * 8, FILAS * 8, 0x0000);
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
        	if (tablero[f][c] != NULL) {
        	    LCD_Sprite_Transparent(OFFSET_X + c * 8, OFFSET_Y + f * 8, 8, 8, tablero[f][c], 1, 0, 0, 0, 0x0746);
        	}
        }
    }
}

int colision_abajo() {
    for (int c = 0; c < 4; c++) {
        for (int f = 3; f >= 0; f--) {
            if (pieza_actual[anim][f][c] == 1) {
                int col = ((pieza_x - OFFSET_X) / 8) + c;
                int fila = ((pieza_y - OFFSET_Y) / 8) + f + 1;
                if (col < 0 || col >= COLUMNAS) return 1;
                if (fila >= FILAS) return 1;
                if (tablero[fila][col] != NULL) return 1;
                break;
            }
        }
    }

    return 0;
}

int colision_spawn() {
    for (int f = 0; f < 4; f++) {
        for (int c = 0; c < 4; c++) {
            if (pieza_actual[anim][f][c] == 1) {
                int col = ((pieza_x - OFFSET_X) / 8) + c;
                int fila = ((pieza_y - OFFSET_Y) / 8) + f;
                if (tablero[fila][col] != NULL) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int colision_derecha() {
    for (int f = 0; f < 4; f++) {
        for (int c = 3; c >= 0; c--) {
            if (pieza_actual[anim][f][c] == 1) {
                int col = ((pieza_x - OFFSET_X) / 8) + c + 1;
                int fila = ((pieza_y - OFFSET_Y) / 8) + f;
                if (col >= COLUMNAS) return 1;
                if (tablero[fila][col] != NULL) return 1;
                break;
            }
        }
    }
    return 0;
}

int colision_izquierda() {
    for (int f = 0; f < 4; f++) {
        for (int c = 0; c < 4; c++) {
            if (pieza_actual[anim][f][c] == 1) {
                int col = ((pieza_x - OFFSET_X) / 8) + c - 1;
                int fila = ((pieza_y - OFFSET_Y) / 8) + f;
                if (col < 0) return 1;
                if (tablero[fila][col] != NULL) return 1;
                break;
            }
        }
    }
    return 0;
}

void limpiar_tablero() {
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            tablero[f][c] = 0;
        }
    }
}

void generar_pieza(int (**pieza)[4][4], const uint16_t **color) {
    int r = rand() % 7;
    switch (r) {
        case 0:
            *pieza = pieza_Z;
            *color = C_Rojo;
            break;
        case 1:
            *pieza = pieza_Z;
            *color = C_verde;
            break;
        case 2:
            *pieza = pieza_L;
            *color = C_Azul;
            break;
        case 3:
        	*pieza = pieza_L;
			*color = C_Naranja;
			break;
        case 4:
            *pieza = pieza_C;
        	*color = C_Amarillo;
        	break;
        case 5:
            *pieza = pieza_I;
            *color = C_Celeste;
        	break;
        case 6:
            *pieza = pieza_R;
            *color = C_Morado;
            break;
    }
}
void dibujar_preview(int x, int y, int (*pieza)[4][4], const uint16_t *color){
    for (int f = 0; f < 4; f++) {
        for (int c = 0; c < 4; c++) {
            if (pieza[0][f][c] == 1) {
                LCD_Sprite_Transparent(x + c * 8, y + f * 8, 8, 8, color, 1, 0, 0, 0, 0x0000);
            }
        }
    }
}

void dibujar_siguientes() {
    // pieza 1
	FillRect(189, 28, 26, 32, 0x0000);
    dibujar_preview(189, 28, cola_piezas[0], cola_colores[0]);
    // pieza 2
    FillRect(224, 28, 26, 32, 0x0000);
    dibujar_preview(224, 28, cola_piezas[1], cola_colores[1]);
    // pieza 3
    FillRect(261, 28, 26, 32, 0x0000);
    dibujar_preview(261, 28, cola_piezas[2], cola_colores[2]);
    // pieza 4
    FillRect(261, 76, 26, 32, 0x0000);
    dibujar_preview(261, 76, cola_piezas[3], cola_colores[3]);
}





void eliminar_fila(int fila) {
    for (int f = fila; f > 0; f--) {
        for (int c = 0; c < COLUMNAS; c++) {
            tablero[f][c] = tablero[f-1][c];
        }
    }
    for (int c = 0; c < COLUMNAS; c++) {
        tablero[0][c] = NULL;
    }
}

int fila_llena(int f) {
    for (int c = 0; c < COLUMNAS; c++) {
        if (tablero[f][c] == 0) {
            return 0;
        }
    }
    return 1;
}

int revisar_filas() {
    int lineas_eliminadas = 0;
    for (int f = 0; f < FILAS; f++) {
        if (fila_llena(f)) {
            eliminar_fila(f);
            lineas_eliminadas++;
            f--;
        }
    }
    return lineas_eliminadas;
}

void limites_pieza(int rot, int *min_c, int *max_c) {
    *min_c = 4;
    *max_c = 0;
    for (int f = 0; f < 4; f++) {
        for (int c = 0; c < 4; c++) {
            if (pieza_actual[rot][f][c] == 1) {
                if (c < *min_c) *min_c = c;
                if (c > *max_c) *max_c = c;
            }
        }
    }
}

//////////////////////////////FUNCION PARA MOSTRAR IMAGENES DESDE LA SD//////////////////////////////////
void LCD_DrawRAW_FromSD(const char *filename, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    FIL fil;
    FRESULT res;
    UINT br;

    uint8_t buffer1[512];

    res = f_open(&fil, filename, FA_READ);
    if (res != FR_OK) return;

    LCD_CS_L();
    SetWindows(x, y, x + width - 1, y + height - 1);
    LCD_RS_H();

    uint32_t totalBytes = width * height * 2;
    uint32_t bytesRead = 0;

    while (bytesRead < totalBytes)
    {
        UINT toRead = (totalBytes - bytesRead > sizeof(buffer1)) ? sizeof(buffer1) : (totalBytes - bytesRead);

        if (f_read(&fil, buffer1, toRead, &br) != FR_OK) break;
        if (br == 0) break;
        for (uint16_t i = 0; i < br; i++)
        {
            LCD_DATA(buffer1[i]);
        }
        bytesRead += br;
    }
    LCD_CS_H();
    f_close(&fil);
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_UART5_Init();
  MX_USART3_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  srand(HAL_GetTick());
	LCD_Init();
	HAL_UART_Receive_IT(&huart5, &rx5Data, 1);

	LCD_Clear(0x00);
	for (int i = 0; i < NUM_NEXT; i++) {
	    generar_pieza(&cola_piezas[i], &cola_colores[i]);
	}

	pieza_actual = cola_piezas[0];
	pieza_color  = cola_colores[0];

	if (f_mount(&fs, "", 1) != FR_OK) {
	    LCD_Init();
	    LCD_Clear(0xF800);
	    while(1);
	}
	LCD_Clear(0x00);
	LCD_DrawRAW_FromSD("Inicio1.RAW", 0, 0, 320, 240);
	HAL_Delay(50);
	LCD_Clear(0x00);
	LCD_DrawRAW_FromSD("Inicio2.RAW", 0, 0, 320, 240);
	HAL_Delay(50);
	LCD_Clear(0x00);
	LCD_DrawRAW_FromSD("Inicio3.RAW", 0, 0, 320, 240);
	HAL_Delay(50);
	LCD_Clear(0x00);
	LCD_DrawRAW_FromSD("Inicio4.RAW", 0, 0, 320, 240);
	HAL_Delay(50);
	LCD_Clear(0x00);
	LCD_DrawRAW_FromSD("Inicio5.RAW", 0, 0, 320, 240);
	HAL_Delay(50);
	LCD_Clear(0x00);
	LCD_DrawRAW_FromSD("Inicio6.RAW", 0, 0, 320, 240);
	HAL_Delay(50);

	LCD_Clear(0x00);
	LCD_Sprite(59, 0, 101, 120, menu, 7, 0, 0, 0);
	LCD_Sprite(161, 0, 101, 120, menu, 7, 1, 0, 0);
	LCD_Sprite(59, 120, 101, 120, menu, 7, 3, 0, 0);
	LCD_Sprite(161, 120, 101, 120, menu, 7, 5, 0, 0);
	HAL_UART_Transmit(&huart3, &estado_actual, 1, HAL_MAX_DELAY);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    ////////////////LOGICA PARA TERMINAR LA PARTIDA//////////////////////////
		if (salir_menu && modo_juego == 1) {

		    salir_menu = 0;

		    modo_juego = 0;
		    modo_time_attack = 0;

		    pieza_x = OFFSET_X + (COLUMNAS / 2) * 8 - 16;
		    pieza_y = 0;
		    anim = 0;

		    score = 0;
		    lineas = 0;
		    nivel = 1;

		    limpiar_tablero();

		    for (int i = 0; i < NUM_NEXT; i++) {
		        generar_pieza(&cola_piezas[i], &cola_colores[i]);
		    }

		    pieza_actual = cola_piezas[0];
		    pieza_color  = cola_colores[0];

		    LCD_Clear(0x00);
		    LCD_Sprite(59, 0, 101, 120, menu, 7, 0, 0, 0);
		    LCD_Sprite(161, 0, 101, 120, menu, 7, 1, 0, 0);
		    LCD_Sprite(59, 120, 101, 120, menu, 7, 3, 0, 0);
		    LCD_Sprite(161, 120, 101, 120, menu, 7, 5, 0, 0);

		    pos_menu = 0;
		}

		///////////////SONIDO//////////////////////////////////
		if (modo_juego == 1 && musica_on == 1) {
		    estado_actual = '2';
		} else {
		    estado_actual = '1';
		}
		if (estado_actual != estado_anterior) {
		    HAL_UART_Transmit(&huart3, &estado_actual, 1, HAL_MAX_DELAY);
		    estado_anterior = estado_actual;
		}

		static uint32_t lastHUD = 0;

		if (modo_juego == 1) {

		    if (HAL_GetTick() - lastHUD > 200) {
		        lastHUD = HAL_GetTick();
		        char buffer[20];
		        // SCORE
		        sprintf(buffer, "%d", score);
			      FillRect(198, 225, 48, 10, 0x0000);
			      LCD_Print(buffer, 198, 225, 1.5, 0xFFFF, 0x0000);
		        // LINEAS
			      sprintf(buffer, "%d", lineas);
			      FillRect(198, 178, 15, 10, 0x0000);
		        LCD_Print(buffer, 198, 178, 1.5, 0xFFFF, 0x0000);

		        // NIVEL
		        sprintf(buffer, "%d", nivel);
		        FillRect(198, 128, 15, 10, 0x0000);
		        LCD_Print(buffer, 198, 128, 1, 0xFFFF, 0x0000);
		    }
		}
		////////////////////////LOGICA DE LOS BOTONES/////////////////////////////////////
		if (b_derecha == 1){
			b_derecha = 0;
			if (modo_juego == 0 && opciones == 0){
						if(pos_menu == 0){
							pos_menu = 3;
						}
			    		pos_menu--;
			    	    switch (pos_menu) {
			    	        case 1:
				            	LCD_Sprite(161, 0, 101, 120, menu, 7, 2, 0, 0);
				            	LCD_Sprite(59, 120, 101, 120, menu, 7, 3, 0, 0);
				            	opciones_1 = 0;
			    	            break;
			    	        case 2:
				            	LCD_Sprite(161, 120, 101, 120, menu, 7, 5, 0, 0);
				            	LCD_Sprite(59, 120, 101, 120, menu, 7, 4, 0, 0);
				            	opciones_1 = 0;
			    	            break;

			    	        default:
				            	LCD_Sprite(161, 0, 101, 120, menu, 7, 1, 0, 0);
				            	LCD_Sprite(161, 120, 101, 120, menu, 7, 6, 0, 0);
				            	pos_menu = 3;
				            	opciones_1 = 1;
			    	            break;
			    	    }
			    	}
			    	if (modo_juego == 1){
			    		int anim_old = anim;
			    		anim++;
			    		if(anim >= 4) anim = 0;
			    		borrar_pieza(pieza_x, pieza_y, anim_old);
			    		dibujar_tablero();
			    		dibujar_pieza(pieza_x, pieza_y, anim);
			    	}
		}
		if (b_izquierda == 1){
			b_izquierda = 0;
	    	if (modo_juego == 0 && opciones == 0){
	    		if(pos_menu >= 3){
	    			pos_menu = 0;
	    		}
	    		pos_menu++;

	    	    switch (pos_menu) {
	    	        case 1:
		            	LCD_Sprite(161, 0, 101, 120, menu, 7, 2, 0, 0);
		            	LCD_Sprite(161, 120, 101, 120, menu, 7, 5, 0, 0);
		            	opciones_1 = 0;
	    	            break;
	    	        case 2:
		            	LCD_Sprite(161, 0, 101, 120, menu, 7, 1, 0, 0);
		            	LCD_Sprite(59, 120, 101, 120, menu, 7, 4, 0, 0);
		            	opciones_1 = 0;
	    	            break;
	    	        default:
		            	LCD_Sprite(59, 120, 101, 120, menu, 7, 3, 0, 0);
		            	LCD_Sprite(161, 120, 101, 120, menu, 7, 6, 0, 0);
		            	opciones_1 = 1;
		            	pos_menu = 0;
	    	            break;
	    	    }

	    	}
	    	if (modo_juego == 1){
	    		int anim_old = anim;
	    		anim--;
	    		if(anim < 0) anim = 3;
	    		borrar_pieza(pieza_x, pieza_y, anim_old);
	    		dibujar_tablero();
	    		dibujar_pieza(pieza_x, pieza_y, anim);
	    	}

		}
		if (b1 == 1){
			b1 = 0;
	    	if (modo_juego == 0){
	    		if (pos_menu == 1){
	    	        modo_juego = 1;
	    	        modo_time_attack = 0;
	    	        LCD_DrawRAW_FromSD("ENDLESS.RAW", 0, 0, 320, 240);
	    	        dibujar_siguientes();
	    		    	}
	    	    if (pos_menu == 2){
	    	        modo_juego = 1;
	    	        modo_time_attack = 1;
	    	        tiempo_inicio = HAL_GetTick();
	    	        LCD_DrawRAW_FromSD("TIME.RAW", 0, 0, 320, 240);
	    	        dibujar_siguientes();
	    	    }

	    	    if (opciones_1 == 1) {
	    	        LCD_Clear(0x0000);
	    	        opciones = 1;
	    	        if (musica_on)
	    	            LCD_Print("MUSICA: ON", 80, 100, 2, 0xFFFF, 0x0000);
	    	        else
	    	            LCD_Print("MUSICA: OFF", 80, 100, 2, 0xFFFF, 0x0000);
	    	    }
	    	}else if (modo_juego == 1){
	    		borrar_pieza(pieza_x, pieza_y, anim);
	    		int min_c, max_c;
	    		limites_pieza(anim, &min_c, &max_c);
	    		if (!colision_derecha()) {
	    		    pieza_x += 8;
	    		}
	    		dibujar_pieza(pieza_x, pieza_y, anim);
	    	}
		}
		if(b2 == 1){
			b2 = 0;
	    	if (modo_juego == 1){
	    		borrar_pieza(pieza_x, pieza_y, anim);
	    		int min_c, max_c;
	    		limites_pieza(anim, &min_c, &max_c);
	    		if (!colision_izquierda()) {
	    		    pieza_x -= 8;
	    		}
	    		dibujar_pieza(pieza_x, pieza_y, anim);
	    	}
	    	if (modo_juego == 0 && opciones == 1) {
	    	    opciones = 0;
	    	    LCD_Clear(0x00);
	    	    LCD_Sprite(59, 0, 101, 120, menu, 7, 0, 0, 0);
	    	    LCD_Sprite(161, 0, 101, 120, menu, 7, 1, 0, 0);
	    	    LCD_Sprite(59, 120, 101, 120, menu, 7, 3, 0, 0);
	    	    LCD_Sprite(161, 120, 101, 120, menu, 7, 5, 0, 0);
	    	}
		}
		if (modo_juego == 1)
		{
		    if (HAL_GetTick() - lastFall >= 150)
		    {
		        lastFall = HAL_GetTick();
		        borrar_pieza(pieza_x, pieza_y, anim);
		        pieza_y += 8;
		        if (!colision_abajo()) {
		            dibujar_pieza(pieza_x, pieza_y, anim);
		        } else {
		        	pieza_y -= 8;
		            guardar_en_tablero();
		            int eliminadas = revisar_filas();
		            if (eliminadas > 0) {
		                lineas += eliminadas;
		                switch (eliminadas) {
		                    case 1: score += 100 * nivel; break;
		                    case 2: score += 300 * nivel; break;
		                    case 3: score += 500 * nivel; break;
		                    case 4: score += 800 * nivel; break;
		                }
		                nivel = (lineas / 10) + 1;
		            }
		            dibujar_tablero();
		            pieza_actual = cola_piezas[0];
		            pieza_color  = cola_colores[0];
		            for (int i = 0; i < NUM_NEXT - 1; i++) {
		                cola_piezas[i] = cola_piezas[i + 1];
		                cola_colores[i] = cola_colores[i + 1];
		            }
		            generar_pieza(&cola_piezas[NUM_NEXT - 1], &cola_colores[NUM_NEXT - 1]);
		            pieza_x = OFFSET_X + (COLUMNAS / 2) * 8 - 16;
		            pieza_y = 0;
		            anim = 0;
		            dibujar_siguientes();
		            if (colision_spawn()) {
		                modo_juego = 0;
		                pos_menu = 0;
		                limpiar_tablero();
		                score = 0;
		                lineas = 0;
		                nivel = 1;
		            	LCD_Clear(0x00);
		            	LCD_Sprite(59, 0, 101, 120, menu, 7, 0, 0, 0);
		            	LCD_Sprite(161, 0, 101, 120, menu, 7, 1, 0, 0);
		            	LCD_Sprite(59, 120, 101, 120, menu, 7, 3, 0, 0);
		            	LCD_Sprite(161, 120, 101, 120, menu, 7, 5, 0, 0);
		            }
		        }
		        if (pieza_y >= 210){
		        	pieza_y = 0;
		        }
		    }
		}
		///////////////////////////////////////////////////////////////////
		/////////////////////TIME ATTACK/////////////////////////////
		if (modo_juego == 1 && modo_time_attack == 1){
		    uint32_t restante = 60000 - (HAL_GetTick() - tiempo_inicio);
		    uint32_t segundos = restante / 1000;
		    uint32_t minutos = segundos / 60;
		    segundos = segundos % 60;
		    char buffer[10];
		    sprintf(buffer, "%02lu:%02lu", minutos, segundos);
		    if (HAL_GetTick() - lastPrint > 500) {
		        lastPrint = HAL_GetTick();
			    FillRect(34, 212, 48, 10, 0x0000);
			    LCD_Print(buffer, 34, 212, 1, 0xFFFF, 0x0000);
		    }
		    if (HAL_GetTick() - tiempo_inicio >= 60000){
		        modo_juego = 0;
		        pos_menu = 0;
		        modo_time_attack = 0;
		        pieza_x = OFFSET_X + (COLUMNAS / 2) * 8 - 16;
		        pieza_y = 0;
		        anim = 0;
		        score = 0;
		        lineas = 0;
		        nivel = 1;
		        limpiar_tablero();
		        LCD_Clear(0x00);
		        LCD_Sprite(59, 0, 101, 120, menu, 7, 0, 0, 0);
		        LCD_Sprite(161, 0, 101, 120, menu, 7, 1, 0, 0);
		        LCD_Sprite(59, 120, 101, 120, menu, 7, 3, 0, 0);
		        LCD_Sprite(161, 120, 101, 120, menu, 7, 5, 0, 0);
		    }
		}
///////////////////////LOGICA PARA APAGAR EL SONIDO////////////////////////////
    	if (modo_juego == 0 && opciones == 1) {

    	    if (b_derecha || b_izquierda) {
    	        b_derecha = 0;
    	        b_izquierda = 0;

    	        musica_on = !musica_on;


    	        FillRect(80, 100, 240, 20, 0x0000);

    	        if (musica_on)
    	            LCD_Print("MUSICA: ON", 80, 100, 2, 0xFFFF, 0x0000);
    	        else
    	            LCD_Print("MUSICA: OFF", 80, 100, 2, 0xFFFF, 0x0000);
    	    }
    	}




	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|LCD_D1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_SS_GPIO_Port, SD_SS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LCD_RST_Pin LCD_D1_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BOTON1_Pin BOTON2_Pin DERECHA_Pin */
  GPIO_InitStruct.Pin = BOTON1_Pin|BOTON2_Pin|DERECHA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RD_Pin LCD_WR_Pin LCD_RS_Pin LCD_D7_Pin
                           LCD_D0_Pin LCD_D2_Pin */
  GPIO_InitStruct.Pin = LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_CS_Pin LCD_D6_Pin LCD_D3_Pin LCD_D5_Pin
                           LCD_D4_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : IZQUIERDA_Pin */
  GPIO_InitStruct.Pin = IZQUIERDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(IZQUIERDA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_SS_Pin */
  GPIO_InitStruct.Pin = SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(SD_SS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/////////////////////////DATOS RECIBIDOS DESDE EL ESP32////////////////////////////
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART5)
    {

        switch(rx5Data)
        {
            case '1':
            	b_izquierda = 1;
                break;
            case '2':
            	b_derecha = 1;
                break;
            case '3':
            	b1 = 1;
                break;
            case '4':
            	b2 = 1;
                break;
            case '5':
            	salir_menu = 1;
                break;
            case '6':
                break;
        }


        HAL_UART_Receive_IT(&huart5, &rx5Data, 1);
    }
}






/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
