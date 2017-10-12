#include "control.h"
#include "stdint.h"
#include "protocol.h"
#include "board_gimbal_cmd_process.h"
#include "stm32_bsp_uart.h"

void	System_Run(int8_t	uc_uid_result_p)
{
	if(uc_uid_result_p)
	{
		/*´®¿Ú´¦Àí*/
			Task_Anasyse_Protocol(SET_CMD_RXBUF,Board_Gimbal_Cmd_Trans_Process);
		
	}
	
	
}



