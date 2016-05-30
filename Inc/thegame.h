#ifndef __THEGAME_H__
#define __THEGAME_H__

#include "ssd1306.h"

#define			SHOOTS  			1
#define 		SHOOT_SPEED			4
#define 		SHOOT_LIFE			25
#define         MAXSPEED            3.0
#define         ACCELERATION        0.0
#define         METEOHEALTH         5
// Converts degrees to radians to degrees macro
#define		M_PI												3.141593
#define 	degreesToRadians(angleDegrees)		(double)(angleDegrees * M_PI / 180.0)
#define 	radiansToDegrees(angleRadians)		(double)(angleRadians * 180.0 / M_PI)
//#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#define     LED1_ON()     				HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
#define     LED1_OFF()     				HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_SET)
#define     LED1_TOGGLE()				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9)
#define     LED2_ON()     				HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)
#define     LED2_OFF()    				HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_SET)
#define     LED2_TOGGLE()   			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8)

/*
 *  Shoot structure
 */
typedef struct {
	double      x0, y0, angle, velocityX, velocityY;
	int 		alive, lenth;
}	ShootTypeDef;

/*
 *  Mobile unit structure
 */
typedef struct {
		SSD1306_polyTypeDef shape;
		double  posX, posY;
        double  velocityX, velocityY;
		double	angle;
		double  scale;
        int  health;
		ShootTypeDef shoot[SHOOTS];
} MobileUnitTypeDef ;


void DrawObject(MobileUnitTypeDef * Object, int fill);
void RotateObject( MobileUnitTypeDef * Object, double angle );
int pointInPolygon(MobileUnitTypeDef * poly, int polyCorners, double x, double y  );

#endif
