/******************************************************************************
* Copyright (C) 2018, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC
* components. This software is licensed by HDSC to be adapted only
* for use in systems utilizing HDSC components. HDSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. HDSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2019-03-26  1.0  Lux First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "gpio.h"
#include "sysctrl.h"
#include "user_sever.h"
#include "flash.h"
#include "wdt.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/*******************************************************************************
 * 中断服务函数
 ******************************************************************************/

void App_ClkInit(void);
void App_Rch4MHzTo24MHz(void);


//#define ENABLE_WDT

#ifdef ENABLE_WDT
static void App_WdtInit(void)
{
    ///< 开启WDT外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);
    ///< WDT 初始化
    Wdt_Init(WdtResetEn, WdtT820ms);
}
#endif

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
 
 
 
uint32_t u32Val;
int32_t main(void)
{    
    //时钟初始化配置
    App_ClkInit();
    //将时钟从RCH4MHz切换至RCH24MHz, PA01观察时钟波形，(3s后)按下按键继续运行
    App_Rch4MHzTo24MHz();
	///< WDT 初始化

		user_init();
#ifdef ENABLE_WDT
		App_WdtInit();
		Wdt_Start();
#endif
    while(1)
		{
			user_work();
			#ifdef ENABLE_WDT
			Wdt_Feed();//看门狗
			#endif
		}
}



//将时钟从RCH4MHz切换至RCH24MHz,
void App_Rch4MHzTo24MHz(void)
{    
///<============== 将时钟从RCH4MHz切换至RCH24MHz ==============================    
    ///< RCH时钟不同频率的切换，需要先将时钟切换到RCL，设置好频率后再切回RCH
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkRCL);
    
    ///< 加载目标频率的RCH的TRIM值
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);
    ///< 使能RCH（默认打开，此处可不需要再次打开）
    //Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);
    ///< 时钟切换到RCH
    Sysctrl_SysClkSwitch(SysctrlClkRCH);
    ///< 关闭RCL时钟
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE);
    
    ///< 使能HCLK从PA01输出
    //Gpio_SfHClkOutputCfg(GpioSfHclkOutEnable, GpioSfHclkOutDiv1);
    
    delay1ms(1000);

    ///< 禁止HCLK从PA01输出
    //Gpio_SfHClkOutputCfg(GpioSfHclkOutDisable, GpioSfHclkOutDiv1);

}  

//时钟初始化配置
void App_ClkInit(void)
{
    stc_sysctrl_clk_cfg_t stcCfg;
    
    ///< 开启FLASH外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralFlash, TRUE);
    
    ///<========================== 时钟初始化配置 ===================================
    ///< 因要使用的时钟源HCLK小于24M：此处设置FLASH 读等待周期为0 cycle(默认值也为0 cycle)
    Flash_WaitCycle(FlashWaitCycle0);
    
    ///< 时钟初始化前，优先设置要使用的时钟源：此处设置RCH为4MHz
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);
    
    ///< 选择内部RCH作为HCLK时钟源;
    stcCfg.enClkSrc    = SysctrlClkRCH;
    ///< HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    ///< PCLK 为HCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
    ///< 系统时钟初始化
    Sysctrl_ClkInit(&stcCfg);
}


