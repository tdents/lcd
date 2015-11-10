#include <SPI.h>
#include "symbols.h"
#define DATCTL 0xBC         // Data Control (data handling in RAM)

#define DISCTL 0xCA         // Display Control

#define GCP64 0xCB          // pulse set for 64 gray scale

#define GCP16 0xCC          // pulse set for 16 gray scale

#define OSSEL 0xD0          // Oscillator select

#define GSSET 0xCD          // set for gray scales

#define ASCSET 0xAA         // aerea scroll setting

#define SCSTART 0xAB        // scroll start setting

#define DISON 0xAF          // Display ON (no parameter)

#define DISOFF 0xAE         // Display OFF (no parameter)

#define DISINV 0xA7         // Display Invert (no parameter)

#define DISNOR 0xA6         // Display Normal (no parameter)

#define SLPIN  0x95         // Display Sleep (no parameter)

#define SLPOUT 0x94         // Display out of sleep (no parameter)

#define RAMWR 0x5C          // Display Memory write

#define PTLIN 0xA8          // partial screen write

#define SD_CSET 0x15        // column address setting

#define SD_PSET 0x75        // page address setting
#define DISP_W 132

#define DISP_H 176

#define CHAR_H 14

#define CHAR_W 8

#define TEXT_COL 16

#define TEXT_ROW 12


#define LCD_RESET        P2_1
#define LCD_CS         P2_0
#define LCD_RS         P2_2


//Commands define
#define LCD_RS_ON       digitalWrite(LCD_RS,HIGH);
#define LCD_RS_OFF      digitalWrite(LCD_RS,LOW);

#define LCD_CS_OFF      digitalWrite(LCD_CS,LOW);
#define LCD_CS_ON       digitalWrite(LCD_CS,HIGH);

#define LCD_RESET_ON    digitalWrite(LCD_RESET,HIGH);
#define LCD_RESET_OFF   digitalWrite(LCD_RESET,LOW);

void setup() {
   port_init();
   lcd_init_c();

   
}

void loop() {
   fill_screen(random(1,65535)); 
   put_char(0,0,'h',random(0,65535));
   put_char(8,0,'e',random(0,65535));
   put_char(16,0,'l',random(0,65535));
   put_char(24,0,'l',random(0,65535));
   put_char(32,0,'o',random(0,65535));
   delay(1000);
}



void lcd_write(uint8_t dat)

{

  SPI.transfer(dat);

}

void lcd_dat0(uint8_t dat)

{

  lcd_write(dat);

  lcd_write(0x00);  

}



void lcd_dat16(uint16_t dat)

{

  lcd_write(dat>>8);

  lcd_write(dat);

}

void lcd_cmd(uint8_t dat)

{

  LCD_RS_OFF;

  lcd_write(dat);

  lcd_write(0x00);

  LCD_RS_ON;

}

void port_init()

{
  pinMode (LCD_RESET, OUTPUT);
  pinMode (LCD_CS, OUTPUT);
  pinMode (LCD_RS, OUTPUT);
  SPI.setClockDivider(1);
  SPI.begin();
}

void lcd_init_c(void)

/*

  Init the Siemens S65 Display with EPSON controller



  (c) Christian Kranz, 10/2005

*/

{

  uint8_t i;

  static const uint8_t disctl[9] = {0x4C, 0x01, 0x53, 0x00, 0x02, 0xB4, 0xB0, 0x02, 0x00};

  static const uint8_t gcp64_0[29] =

                      {0x11,0x27,0x3C,0x4C,0x5D,0x6C,0x78,0x84,0x90,0x99,0xA2,0xAA,0xB2,0xBA,

                       0xC0,0xC7,0xCC,0xD2,0xD7,0xDC,0xE0,0xE4,0xE8,0xED,0xF0,0xF4,0xF7,0xFB,

                       0xFE};

  static const uint8_t gcp64_1[34] =

                     {0x01,0x03,0x06,0x09,0x0B,0x0E,0x10,0x13,0x15,0x17,0x19,0x1C,0x1E,0x20,

                      0x22,0x24,0x26,0x28,0x2A,0x2C,0x2D,0x2F,0x31,0x33,0x35,0x37,0x39,0x3B,

                      0x3D,0x3F,0x42,0x44,0x47,0x5E};

  static const uint8_t gcp16[15] =

                      {0x13,0x23,0x2D,0x33,0x38,0x3C,0x40,0x43,0x46,0x48,0x4A,0x4C,0x4E,0x50,0x64};



  // generate clean display reset
  LCD_RESET_OFF;
  LCD_CS_ON;
  LCD_RS_ON;
  delay(10);
  LCD_RESET_ON; 

  delay(35);

  LCD_CS_OFF;


  lcd_cmd(DATCTL);

  lcd_dat0(0x2A);  // 0x2A=565 mode, 0x0A=666mode, 0x3A=444mode

  LCD_CS_ON;
  LCD_CS_OFF;


  lcd_cmd(DISCTL);

  for (i=0; i<9; i++)

  {

    lcd_dat0(pgm_read_byte(&disctl[i]));

  }

  

  lcd_cmd(GCP64);

  for (i=0; i<29; i++)

  {

    lcd_dat0(pgm_read_byte(&gcp64_0[i]));

    lcd_dat0(0x00);

  }

  for (i=0; i<34; i++)

  {

    lcd_dat0(pgm_read_byte(&gcp64_1[i]));

    lcd_dat0(0x01);

  }



  lcd_cmd(GCP16);

  for (i=0; i<15; i++)

  {

    lcd_dat0(pgm_read_byte(&gcp16[i]));

  }



  lcd_cmd(GSSET);

  lcd_dat0(0x00);



  lcd_cmd(OSSEL);

  lcd_dat0(0x00);



  lcd_cmd(SLPOUT);

  

//  _delay_ms(7);



  lcd_cmd(SD_CSET);

  lcd_dat0(0x08);

  lcd_dat0(0x01);

  lcd_dat0(0x8B);

  lcd_dat0(0x01);



  lcd_cmd(SD_PSET);

  lcd_dat0(0x00);

  lcd_dat0(0x8F);





  lcd_cmd(ASCSET);

  lcd_dat0(0x00);

  lcd_dat0(0xAF);

  lcd_dat0(0xAF);

  lcd_dat0(0x03);

  

  lcd_cmd(SCSTART);

  lcd_dat0(0x00);


  LCD_RS_OFF;

  lcd_dat0(DISON);

  LCD_CS_ON;
}

void fill_screen(uint16_t color)

{

  uint16_t i;


  LCD_CS_OFF;
  

  lcd_cmd(SD_CSET);

  lcd_dat0(0x08);  // start is 8, not 0

  lcd_dat0(0x01);

  lcd_dat0(0x8B);  // is 00x8B = 0x83+8

  lcd_dat0(0x01);



  lcd_cmd(SD_PSET);

  lcd_dat0(0x00);

  lcd_dat0(0xAF);

  

  lcd_cmd(RAMWR);



  // start data transmission  

  for (i=0; i<DISP_W*DISP_H; i++)
    lcd_dat16(color);

  LCD_CS_ON;
}

// ascii table, starting with character blank (32)

// size is 8x14

uint16_t backcolor=0xFFFF;
uint16_t textcolor=0x0000;

void put_char(uint8_t x, uint8_t y, char c, uint8_t rot)

{

  uint8_t h,ch,p,mask,he;


  LCD_CS_OFF;  

  lcd_cmd(SD_CSET);

  lcd_dat0(0x08+x);  // start is 8, not 0

  lcd_dat0(0x01);

  lcd_dat0(0x08+x+CHAR_W-1);  // end is 00x8B = 0x83+8

  lcd_dat0(0x01);

  

  lcd_cmd(SD_PSET);

  lcd_dat0(y);

  if (y < DISP_H-CHAR_H)

  {

    he=CHAR_H;

    lcd_dat0(y+CHAR_H-1);

  }

  else

  {

    he=DISP_H-y;

    lcd_dat0(DISP_H-1);

  }



  lcd_cmd(RAMWR);

  

  for (h=0; h<he; h++) // every column of the character

  {        

        ch=pgm_read_byte(&ascii_tab[ c-32 ][h]);



        mask=0x80;
        
        for (p=0; p<CHAR_W; p++)  // write the pixels

        {

          if (ch&mask)

          {
            lcd_dat16(rot);                
          }

          else

          {
            lcd_dat16(backcolor);

          }

          mask=mask/2;

        }  // for p

  }
  
  LCD_CS_ON;



}
