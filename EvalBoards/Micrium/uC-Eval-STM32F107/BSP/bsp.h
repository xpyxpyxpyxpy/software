#include "includes.h"

#define  BSP_REG_DEM_CR                           (*(CPU_REG32 *)0xE000EDFC)	//DEMCR�Ĵ���
#define  BSP_REG_DWT_CR                           (*(CPU_REG32 *)0xE0001000)    //DWT���ƼĴ���
#define  BSP_REG_DWT_CYCCNT                       (*(CPU_REG32 *)0xE0001004)	//DWTʱ�Ӽ����Ĵ���	
#define  BSP_REG_DBGMCU_CR                        (*(CPU_REG32 *)0xE0042004)

//DEMCR�Ĵ����ĵ�24λ,���Ҫʹ��DWT ETM ITM��TPIU�Ļ�DEMCR�Ĵ����ĵ�24λ��1
#define  BSP_BIT_DEM_CR_TRCENA                    DEF_BIT_24			

//DWTCR�Ĵ����ĵ�0λ,��Ϊ1��ʱ��ʹ��CYCCNT������,ʹ��CYCCNT֮ǰӦ���ȳ�ʼ��
#define  BSP_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
* Argument(s) : none.
* Return(s)   : The CPU clock frequency, in Hz.
* Caller(s)   : Application.
* Note(s)     : none.
*********************************************************************************************************
*/
CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    return HAL_RCC_GetHCLKFreq();//����HCLKʱ��Ƶ��
}

/*
*********************************************************************************************************
*                                            BSP_Tick_Init()
* Description : BSP_Tick_Init.
* Argument(s) : none.
* Return(s)   : none.
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_Tick_Init(void)
{
	CPU_INT32U cpu_clk_freq;
	CPU_INT32U cnts;
	cpu_clk_freq = BSP_CPU_ClkFreq();
	
	#if(OS_VERSION>=3000u)
		cnts = cpu_clk_freq/(CPU_INT32U)OSCfg_TickRate_Hz;
	#else
		cnts = cpu_clk_freq/(CPU_INT32U)OS_TICKS_PER_SEC;
	#endif
	OS_CPU_SysTickInit(cnts);
}

void BSP_Init(void)
{
	BSP_Tick_Init();//�˺������ʼ��OSϵͳʱ�ӣ������ֲ������ԭ�ӵ�delay�ļ��������������е�delay_init(168)ֻ��Ҫ����һ������
}

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  cpu_clk_freq_hz;

    BSP_REG_DEM_CR     |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA; 	//ʹ��DWT  /* Enable Cortex-M4's DWT CYCCNT reg.*/
    BSP_REG_DWT_CYCCNT  = (CPU_INT32U)0u;					 	//��ʼ��CYCCNT�Ĵ���
    BSP_REG_DWT_CR     |= (CPU_INT32U)BSP_BIT_DWT_CR_CYCCNTENA;	//����CYCCNT

    cpu_clk_freq_hz = BSP_CPU_ClkFreq();
    CPU_TS_TmrFreqSet(cpu_clk_freq_hz);
}
#endif


#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return ((CPU_TS_TMR)BSP_REG_DWT_CYCCNT);
}
#endif


#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
	CPU_INT64U  ts_us;
  CPU_INT64U  fclk_freq;

 
  fclk_freq = BSP_CPU_ClkFreq();
  ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

  return (ts_us);
}
#endif
 
 
#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
	CPU_INT64U  ts_us;
	CPU_INT64U  fclk_freq;


  fclk_freq = BSP_CPU_ClkFreq();
  ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);
	
  return (ts_us);
}
#endif

