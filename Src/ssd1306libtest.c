/**
  ******************************************************************************
  * File Name          : ssd1306libtest.c
  * Description        : 
  ******************************************************************************
  *
  * COPYRIGHT(c) 
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "ssd1306.h"
#include "stdlib.h"
#include "thegame.h"
#include <math.h>


void Points_Copy (SSD1306_pointTypeDef * src, SSD1306_pointTypeDef * dst, int num);
void ShootMove(MobileUnitTypeDef * Shoot );
void ShootMake(MobileUnitTypeDef * BattleStar );
void MoveObject( MobileUnitTypeDef * Object );
void BoostObject( MobileUnitTypeDef * Object );

MobileUnitTypeDef BattleStar;
MobileUnitTypeDef Meteorite;
        
SSD1306_pointTypeDef BattleStarShape[] = {
    {  -5, -5},
    {   4, -5},
    {   1, -2},
    {   7,  0},
    {   1,  2},
    {   4,  5},
    {  -5,  5},
    {   0,  0}
};

SSD1306_pointTypeDef MeteoriteShape[] = {
    {  -2 ,  -5 *2},
    {   2 ,  -2 *2},
    {   5 ,  -2 *2},
    {   5 ,   2 *2},
    {   2 ,   5 *2},
    {  -2 ,   5 *2},
    {  -5 ,   2 *2},
    {  -6 ,  -2 *2}
};

// temporary var
int  dfs, points;
char str[30];
	
int SSD1306LibTest(void)
{
	int i,j,k, time;
	static double l, m;
    points = 0;
    /*******************/
    /* Init BattleStar */
    /*******************/
    BattleStar.shape.SSD1306_points_pointer = BattleStarShape;
    BattleStar.shape.SSD1306_points_number = SSD1306_COUNTOF(BattleStarShape);
    for( i = 0; i < SHOOTS; i++)
    {
//        BattleStar.shoot[i].speed = SHOOT_SPEED;
        BattleStar.shoot[i].alive = 0;
        BattleStar.shoot[i].lenth = 0;
        BattleStar.shoot[i].x0 = 0;
        BattleStar.shoot[i].y0 = 0;
        BattleStar.shoot[i].angle = 0;
    }
	BattleStar.angle  =   0;
	BattleStar.scale  =   1;
	BattleStar.health =   1;
	BattleStar.posX   =  64;
	BattleStar.posY   =  60;
	BattleStar.velocityX =   0.3;
	BattleStar.velocityY =   0;
    RotateObject( &BattleStar,   180 );
    /* End of BattleStar init */

    /******************/
    /* Init Meteorite */
    /******************/
    Meteorite.shape.SSD1306_points_pointer = MeteoriteShape;
    Meteorite.shape.SSD1306_points_number = SSD1306_COUNTOF(MeteoriteShape);
    Meteorite.posX =  rand() % 127;
    Meteorite.posY =  5;
    Meteorite.angle =  0;
    Meteorite.scale =  1;
    Meteorite.health =  METEOHEALTH;
    Meteorite.velocityX = 0.00;
    Meteorite.velocityY = 0.00;
//    Meteorite.velocityX = -0.5 + (double) (rand() % 10) / 10;
//    Meteorite.velocityY = -0.5 + (double) (rand() % 10) / 10;
    /* End of BattleStar init */


	/* ---Test routine--- */
    /* Font tests */
    ssd1306ClearScreen(LAYER0|LAYER1);
    ssd1306SetFont((FONT_INFO*)&ubuntuMono_16ptFontInfo);
    ssd1306DrawString(0,  16, " > Fonts <", 1, WHITE, LAYER0); 
    ssd1306Refresh();
    SSD1306MSDELAY(3000);
    for(i = 128; i>=-256; i-=1)
    {
        ssd1306ClearScreen(LAYER0|LAYER1);
        ssd1306SetFont((FONT_INFO*)&ubuntuMono_8ptFontInfo);
        ssd1306DrawString((i+32) /2,  0, "ubuntuMono_8", 1, WHITE, LAYER0);  
        ssd1306SetFont((FONT_INFO*)&ubuntuMono_16ptFontInfo);
        ssd1306DrawString(128 - i,  16, "ubuntuMono_16", 1, WHITE, LAYER0);  
        ssd1306SetFont((FONT_INFO*)&ubuntuMono_24ptFontInfo);
        ssd1306DrawString(i,  32, "ubuntuMono_24", 1, WHITE, LAYER0);  
        ssd1306Refresh();
        SSD1306MSDELAY(10);
    }

    ssd1306ClearScreen(LAYER0|LAYER1);
    for(i = 0; i<=100; i+=1)
    {
        ssd1306SetFont((FONT_INFO*)&ubuntuMono_8ptFontInfo);
        ssd1306DrawString(16,  0, "ubuntuMono_48:", 1, WHITE, LAYER0);
        ssd1306SetFont((FONT_INFO*)&ubuntuMono_48ptFontInfo);
        sprintf(str,"%03d", i) ;
        ssd1306FillRect(0, 16, 128, 48, BLACK, LAYER0 | LAYER1);
        ssd1306DrawString(17,  16, str, 1, WHITE, LAYER0); 
        ssd1306Refresh();
        SSD1306MSDELAY(0);
    }
    ssd1306SetFont((FONT_INFO*)&ubuntuMono_16ptFontInfo);
    for(i = 000; i < 400; i++)
    {
      ssd1306ClearScreen( LAYER1 );
      if (i >= 0 && i < 100)
      {
        for(j = 0; j < 1; j++)
        {
          ssd1306DrawPixel( (rand() % 127), (rand() % 5), WHITE, LAYER0) ;
        }
        ssd1306DrawString( 10,  0, "          ",  1, WHITE, LAYER1);
        ssd1306DrawString( 10, 16, "          ",  1, WHITE, LAYER1);
        ssd1306DrawString( 14, 32, "          ",  1, WHITE, LAYER1);
        ssd1306ShiftFrameBuffer( 1, 1);
      } else if (i >= 100 && i < 200) {
        for(j = 0; j < 1; j++)
        {
          ssd1306DrawPixel( (rand() % 127), (58 + rand() % 5), WHITE, LAYER0) ;
        }
        ssd1306DrawString( 0,  0, " Draw/Clear",  1, WHITE, LAYER1);
        ssd1306DrawString( 0, 16, "   pixel  " ,  1, WHITE, LAYER1);
        ssd1306DrawString( 0, 32, " DualBuffer",  1, WHITE, LAYER1);
        ssd1306ShiftFrameBuffer( 1, 0);
      } else if (i >= 200 && i < 300) {
        for(j = 0; j < 1; j++)
        {
          ssd1306DrawPixel( (rand() % 10) + 118, rand() % 64, WHITE, LAYER0) ;
        }
        ssd1306DrawString( 0,  0, " Draw/Clear",  1, WHITE, LAYER1);
        ssd1306DrawString( 0, 16, "   pixel  " ,  1, WHITE, LAYER1);
        ssd1306DrawString( 0, 32, " DualBuffer",  1, WHITE, LAYER1);
        ssd1306ShiftFrameBuffer( 1, 2);
      }else if (i >= 300 && i < 400) {
        for(j = 0; j < 1; j++)
        {
          ssd1306DrawPixel( (rand() % 10) + 0, rand() % 64, WHITE, LAYER0) ;
        }
        ssd1306DrawString( 0,  0, " Draw/Clear",  1, WHITE, LAYER1);
        ssd1306DrawString( 0, 16, "   pixel  " ,  1, WHITE, LAYER1);
        ssd1306DrawString( 0, 32, " DualBuffer",  1, WHITE, LAYER1);
        ssd1306ShiftFrameBuffer( 1, 3);
      }
      
      ssd1306Refresh();
      SSD1306MSDELAY(40);
    }
    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    ssd1306SetFont((FONT_INFO*)&ubuntuMono_16ptFontInfo);
    /* message */
    ssd1306FillRect(0, 0, 128, 16, BLACK, LAYER0);
    ssd1306DrawString(0, 0, " DrawLine", 1, WHITE, LAYER0);
    ssd1306FillRect(32, 16,  64, 32, WHITE , LAYER0);
    ssd1306DrawLine(10, 20, 118, 20, WHITE , LAYER0);
    ssd1306DrawLine(10, 21, 118, 21, WHITE , LAYER0);
    ssd1306DrawLine(10, 20, 118, 36, BLACK , LAYER0);
    ssd1306DrawLine(10, 21, 118, 37, BLACK , LAYER0);
    ssd1306DrawLine(10, 20, 118, 44, INVERSE, LAYER0);
    ssd1306DrawLine(10, 21, 118, 45, INVERSE, LAYER0);
		ssd1306Refresh();
    /* */
    SSD1306MSDELAY(5000);
    
    /* ---Test routine--- */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    ssd1306FillRect(0, 16,  64, 47, WHITE, LAYER0);
    ssd1306DrawCircle(64, 39, 19, WHITE);
    ssd1306DrawCircle(64, 39, 22, BLACK);
    ssd1306DrawCircle(32, 39, 19, WHITE);
    ssd1306DrawCircle(32, 39, 22, BLACK);
    ssd1306DrawCircle(96, 39, 19, WHITE);
    ssd1306DrawCircle(96, 39, 22, BLACK);

    /* message */
    ssd1306FillRect(0, 0, 128, 16, BLACK, LAYER0);
    ssd1306DrawString(0, 0, " DrawCircle ", 1, WHITE, LAYER0);
		ssd1306Refresh();
    /* */
    SSD1306MSDELAY(5000);
    
    
    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    ssd1306FillRect(0, 16,  127, 24, WHITE, LAYER0);
    ssd1306FillCircle(32, 40, 20, WHITE, LAYER0);
    ssd1306FillCircle(64, 40, 20, BLACK, LAYER0);
    ssd1306FillCircle(96, 40, 20, WHITE, LAYER0);

    /* message */
    ssd1306FillRect(0, 0, 128, 16, BLACK, LAYER0);
    ssd1306DrawString(0, 0, " FillCircle", 1, WHITE, LAYER0);
		ssd1306Refresh();
    /* */
    SSD1306MSDELAY(5000);

    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    ssd1306FillRect(26, 24, 73, 24, WHITE, LAYER0);
    ssd1306DrawRect(28, 26, 73, 24, WHITE, LAYER0);
    ssd1306DrawString(28, 28, "Window", 1, INVERSE, LAYER0);

    /* message */
    ssd1306FillRect(0, 0, 128, 16, BLACK, LAYER0);
    ssd1306DrawString(21, 0, "DrawRect", 1, WHITE, LAYER0);
    ssd1306Refresh();
    /* */
    SSD1306MSDELAY(5000);

    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    /* message */
		/* Intro */
    ssd1306SetFont((FONT_INFO*)&ubuntuMono_16ptFontInfo);
    ssd1306DrawString(3,  0, "Battle Star", 1, WHITE, LAYER0);
    ssd1306Refresh();
    for(i = 128; i>=-256; i-=1)
    {
        ssd1306FillRect(0, 16, 128, 48, BLACK, LAYER0 | LAYER1);
         ssd1306SetFont((FONT_INFO*)&ubuntuMono_48ptFontInfo);
        ssd1306DrawString(i,  16, "THE GAME", 1, WHITE, LAYER0);  // SIZE 6 - FULL SCREEN 
        ssd1306Refresh();
        SSD1306MSDELAY(3);
    }
		/* END OF INTRO */
    /* Game loop */
    while(1)
    {
        ssd1306FillRect(   0,  0, 128, 64, BLACK, LAYER0 | LAYER1 );
        ssd1306SetFont((FONT_INFO*)&ubuntuMono_16ptFontInfo);
//        ssd1306DrawString( 3,  0, "Battle Star", 1, WHITE, LAYER0 );
        /* draw Battle Star */

        /* draw Meteorite */
        if(!HAL_GPIO_ReadPin  ( S2_GPIO_Port,  S2_Pin ) ){
            RotateObject( &BattleStar,  5 );            
        }
        if(!HAL_GPIO_ReadPin  ( S3_GPIO_Port,  S3_Pin ) ){
            RotateObject( &BattleStar,  -5 ); 
        }
        double radians = atan2(Meteorite.posY - BattleStar.posY, Meteorite.posX - BattleStar.posX);
        double diff = BattleStar.angle - radiansToDegrees(radians);
        diff = sin(degreesToRadians(diff));
        if(diff < 0){
            RotateObject( &BattleStar,  (0.5 + fabs(diff)*30));            
        }
        if(diff > 0){
            RotateObject( &BattleStar,  (-0.5 - fabs(diff)*30)); 
        }
        BoostObject( &BattleStar ); 
        RotateObject( &Meteorite,   1.0 );
        MoveObject( &Meteorite);
        MoveObject( &BattleStar);
        DrawObject( &BattleStar, 0 );
        if ( dfs == 0 )
        {
            DrawObject( &Meteorite,  0 );
        }
        else
        {
            DrawObject( &Meteorite,  1 );
        }
//        ssd1306DrawLine(Meteorite.posX, Meteorite.posY,  BattleStar.posX, BattleStar.posY, WHITE , LAYER0);
        ssd1306SetFont((FONT_INFO*)&ubuntuMono_8ptFontInfo);
        sprintf(str,"PTS: %-d", points) ;
        ssd1306DrawString(0,  0, str, 1, WHITE, LAYER0);
//        sprintf(str,"Dif: %+03.1f", diff) ;
//        ssd1306DrawString(0,  0, str, 1, WHITE, LAYER0);
//        sprintf(str,"AnglH: %03.0f", radiansToDegrees(radians)) ;
//        ssd1306DrawString(0, 8, str, 1, WHITE, LAYER0);
//        sprintf(str,"Sin: %+1.3f", sin(degreesToRadians(diff))) ;
//        ssd1306DrawString(0, 8, str, 1, WHITE, LAYER0);
        dfs =  0;
        
        /* auto shoot */
        if ((time%2 == 0) && (fabs(sin(degreesToRadians(diff))) < 0.003)){
            ShootMake(&BattleStar);
        }
        ShootMove(&BattleStar);
        ssd1306Refresh();
        SSD1306MSDELAY(10);
        time++;
    }
    return 0;
}	
/*
 *  Copy polygon to polygon
 */
void Points_Copy (SSD1306_pointTypeDef * src, SSD1306_pointTypeDef * dst, int num)
{
	while (num)
	{
		dst->x = src->x ;
		dst->y = src->y ;
		dst++; src++;
		num--;
	}
}

/**************************************************************
 *              Function draws polygonal ojects               * 
 **************************************************************/
void DrawObject(MobileUnitTypeDef * Object, int fill)
{
    if (Object->health > 0){
        if (fill)
        {
            ssd1306_FillPolygon( &Object->shape, round(Object->posX), round(Object->posY), WHITE, LAYER0 );
        }
        else
        {
            ssd1306_DrawPolygon( &Object->shape, round(Object->posX), round(Object->posY), WHITE, LAYER0 );
        }
    }
}

/**************************************************************
 * Function rotates MobileUnitTypeDef object to angle degrees *
 **************************************************************/
void RotateObject( MobileUnitTypeDef * Object, double angle )
{
	int i;
    double  x, y ;
	double xsina, ysina, xcosa, ycosa;
	
	for ( i = 0; i < Object->shape.SSD1306_points_number; i++ )
	{
		x = Object->shape.SSD1306_points_pointer[i].x;
		y = Object->shape.SSD1306_points_pointer[i].y;
		xcosa = x * cos(degreesToRadians(angle));
		ysina = y * sin(degreesToRadians(angle));
		Object->shape.SSD1306_points_pointer[i].x = xcosa - ysina;
		ycosa = y * cos(degreesToRadians(angle));
		xsina = x * sin(degreesToRadians(angle));
		Object->shape.SSD1306_points_pointer[i].y = ycosa + xsina;
	}
    Object->angle = Object->angle + angle;
    while (Object->angle > 360)
    {
        Object->angle -= 360;
    }
    while (Object->angle < 0)
    {
        Object->angle += 360;
    }
}

/**************************************************************
 *  Function moves MobileUnitTypeDef object to angle degrees  *
 **************************************************************/
void MoveObject( MobileUnitTypeDef * Object )
{
    Object->posX += Object->velocityX;
    Object->posY += Object->velocityY;
    while (Object->posX < 0) {
//        Object->posX += SSD1306_LCDWIDTH;
        Object->posX = 0;
        Object->velocityX = 0 - Object->velocityX;
    }
    while (Object->posX > SSD1306_LCDWIDTH)
    {
//        Object->posX -= SSD1306_LCDWIDTH;
        Object->posX = SSD1306_LCDWIDTH;
        Object->velocityX = 0 - Object->velocityX;
    }
    while (Object->posY < 0) {
//        Object->posY += SSD1306_LCDHEIGHT;
        Object->posY = 0;
        Object->velocityY = 0 - Object->velocityY;
    }
    while (Object->posY > SSD1306_LCDHEIGHT)
    {
//        Object->posY -= SSD1306_LCDHEIGHT;
        Object->posY = SSD1306_LCDHEIGHT;
        Object->velocityY = 0 - Object->velocityY;
    }
}
	

/**************************************************************
 *  Function moves MobileUnitTypeDef object to angle degrees  *
 **************************************************************/
void BoostObject( MobileUnitTypeDef * Object )
{
    double scale_x = cos(degreesToRadians(Object->angle));
    double scale_y = sin(degreesToRadians(Object->angle));
    Object->velocityX += scale_x * ACCELERATION;
    Object->velocityY += scale_y * ACCELERATION;
    if (Object->velocityX > 0 && Object->velocityX > MAXSPEED) {
        Object->velocityX = MAXSPEED;
    } else if (Object->velocityX < 0 && fabs(Object->velocityX) > MAXSPEED) {
        Object->velocityX = 0 - MAXSPEED;
    } 
    if (Object->velocityY > 0 && Object->velocityY > MAXSPEED) {
        Object->velocityY = MAXSPEED;
    } else if (Object->velocityY < 0 && fabs(Object->velocityY) > MAXSPEED) {
        Object->velocityY = 0 - MAXSPEED;
    } 
   
}

void ShootMove(MobileUnitTypeDef * BattleStar )
{
	int i, j;
    static int pause;
	for( i = 0; i < SHOOTS; i++)
	{
		if (BattleStar->shoot[i].alive)
		{
//			double scale_x = cos(degreesToRadians(BattleStar->shoot[i].angle));
//			double scale_y = sin(degreesToRadians(BattleStar->shoot[i].angle));
//			double velocity_x = (BattleStar->shoot[i].speed * scale_x);
//			double velocity_y = (BattleStar->shoot[i].speed * scale_y);
			BattleStar->shoot[i].x0 += BattleStar->shoot[i].velocityX;
			BattleStar->shoot[i].y0 += BattleStar->shoot[i].velocityY;
			BattleStar->shoot[i].alive--;
			ssd1306DrawPixel( BattleStar->shoot[i].x0, BattleStar->shoot[i].y0 , WHITE, LAYER0) ;
			if( (BattleStar->shoot[i].x0 >= SSD1306_LCDWIDTH)           || // Clip right
					(BattleStar->shoot[i].y0 >= SSD1306_LCDHEIGHT)      || // Clip bottom
					(BattleStar->shoot[i].x0 < 0)                       || // Clip left
					(BattleStar->shoot[i].y0 < 0)  )
			{
			BattleStar->shoot[i].alive = 0;
            BattleStar->shoot[i].y0 = 0;
            BattleStar->shoot[i].x0 = 0;
			}
            //debug
            if (pointInPolygon( (MobileUnitTypeDef *)&Meteorite, Meteorite.shape.SSD1306_points_number,
                BattleStar->shoot[i].x0, BattleStar->shoot[i].y0 )){
                dfs ++;
                Meteorite.health--;
                Meteorite.velocityX += BattleStar->shoot[i].velocityX*0.1;
                Meteorite.velocityY += BattleStar->shoot[i].velocityY*0.1;
//                BattleStar->velocityX -= BattleStar->shoot[i].velocityX*0.05;
//                BattleStar->velocityY -= BattleStar->shoot[i].velocityY*0.05;
                if ((Meteorite.health <= 0) ) {
                    Meteorite.posX =  rand() % 127;
                    Meteorite.posY =  rand() % 10;
                    Meteorite.scale =  (rand() % 5)/10 + 0.5 ;
                    Meteorite.health = METEOHEALTH;
                    Meteorite.velocityX =    2 - (double) (rand() % 40) / 10;
                    Meteorite.velocityY = -0.5 + (double) (rand() % 10) / 10;
//                    Meteorite.velocityX = 0;
//                    Meteorite.velocityY = 0;
                    points++;
                }  

                BattleStar->shoot[i].alive = 0;
            }

      // end of debug
		}
	}
}

void ShootMake(MobileUnitTypeDef * Object )
{
	static int i;
	for( i = 0; i < SHOOTS; i++)
	{
		if (Object->shoot[i].alive == 0)
		{
			Object->shoot[i].alive = SHOOT_LIFE + rand()%50;
			Object->shoot[i].angle = Object->angle;
			Object->shoot[i].velocityX = Meteorite.velocityX + SHOOT_SPEED * cos(degreesToRadians(Object->angle));
			Object->shoot[i].velocityY = Meteorite.velocityY + SHOOT_SPEED * sin(degreesToRadians(Object->angle));
			Object->shoot[i].x0 = Object->posX;
			Object->shoot[i].y0 = Object->posY;
			return;
		}
	}
}

//  Globals which should be set before calling this function:
//
//  int    polyCorners  =  how many corners the polygon has
//  float  polyX[]      =  horizontal coordinates of corners
//  float  polyY[]      =  vertical coordinates of corners
//  float  x, y         =  point to be tested
//
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.

int pointInPolygon(MobileUnitTypeDef * poly, int polyCorners, double x, double y  ) {
  int   nodes, nodeX[MAX_POLY_CORNERS], pixelX, pixelY;
  int   i, j = polyCorners - 1;
  int   oddNodes = 0;

  x = x - poly->posX;
  y = y - poly->posY;
  
  for ( i = 0; i < polyCorners; i++ ) 
  {
    if ((( poly->shape.SSD1306_points_pointer[i].y < y  && poly->shape.SSD1306_points_pointer[j].y >= y ) 
       || (poly->shape.SSD1306_points_pointer[j].y < y  && poly->shape.SSD1306_points_pointer[i].y >= y )) 
       && (poly->shape.SSD1306_points_pointer[i].x <= x || poly->shape.SSD1306_points_pointer[j].x <= x )) 
    {
      oddNodes ^= ( poly->shape.SSD1306_points_pointer[i].x + (y-poly->shape.SSD1306_points_pointer[i].y) 
      / (poly->shape.SSD1306_points_pointer[j].y-poly->shape.SSD1306_points_pointer[i].y) 
      * (poly->shape.SSD1306_points_pointer[j].x-poly->shape.SSD1306_points_pointer[i].x)<x); 
    }
    j=i; 
  }
  return oddNodes; 
}

/***************************************************************END OF FILE****/
