#ifndef __LED_H__
#define __LED_H__

#include "sys.h"

#define LED0 PAout(8)
#define LED1 PDout(2)

//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 

#endif


