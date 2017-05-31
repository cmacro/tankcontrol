//#define STM32F10X_MD
#include <stm32f10x.h>

void SystemClock_Config(void)
{
    unsigned char dump = 0;
    
    // 使用外部8MHz晶振，启用PLL设置系统时钟为 72HMz
    // USB 可用
    // APB1 低速总线 36HMz
    // APB2 高速总线 72HMz
    RCC->CFGR = 0x001D0402;
    RCC->CR   = 0x01010083;
    
    // 闪存访问延迟，48MHz ~ 72MHz = 010。 复位值：0x30
    FLASH->ACR = 0x32;
    
    // 确定外部高速晶振起效
    while (!(RCC->CR>>17));
    // 确定PLL设置起效
    while (!(RCC->CR>>25));
    // 确定PLL为系统时钟源
    while(dump != 0x02) {
        dump = RCC->CFGR >> 2;
        dump &= 0x03;
    }              
}

void LEDPWM_Init(void)
{
  // 总线 TIM2 和 A组引脚 使能
  RCC->APB1ENR = 0x00000001;
  RCC->APB2ENR = 0x00000004;
  // PA1 脚使用复用输出，打开TIM2第二通道
  GPIOA->CRL = 0x444444A4;
  // 设置自动重载计数器
  TIM2->ARR = 900;
  // 设置PWM
  //   通道2 PWM模式1
  //   设置比较输出
  //   设置比较脉冲宽度
  TIM2->CCMR1 = 0x6000;        
  TIM2->CCER = 0x0030;        
  TIM2->CCR2 = 0x0000;
  // 设置为比较输出，使能TIM2
  TIM2->CR2 = 0x0030;
  TIM2->CR1 = 0x0001;
}

int main(void)
{
  unsigned int x, y;
  unsigned char iDir;
  unsigned int pwmwidth;  
  
  SystemClock_Config();
  LEDPWM_Init();
  
  iDir = 0;
  pwmwidth = 0;
  while (1) {
    // 延迟
    //for (iDelay=0; iDelay < 1; iDelay++)
        for (x=0; x < 100; x++)
            for (y=0; y < 1000; y++)
                ;

    if ((pwmwidth == 0) || (pwmwidth == 300) || (pwmwidth == 600) || (pwmwidth == 900)) {
      TIM2->CCR2 = pwmwidth; 
      TIM2->CCR1 = pwmwidth;
    }

    if (!iDir) pwmwidth++;
    else pwmwidth --;
    if (pwmwidth > 900) iDir = 1;
    else if (pwmwidth < 1) iDir = 0;
  }
}
