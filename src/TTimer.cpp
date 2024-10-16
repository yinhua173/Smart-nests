#include "TTimer.h"
#include "fingermain.h"
extern volatile bool enroll_flag;
extern volatile bool delete_flag;

void timeTask1(void);
void timeCallBackTask(void);
void timeCallBackTask2(void);
uint8_t fingermain_run();
//任务控制权柄
TaskHandle_t xHandleTsak[4];
// 定时器控制权柄
TimerHandle_t xTimeHandle[2];
void creat(void){
	// BaseType_t xReturn[2];
	xTimeHandle[0] = xTimerCreate(
								(const char *)"task1",// 定时器名字
								(TickType_t)1000,// 定时器的周期
								pdFALSE,// 定时器是单次执行还是周期执行, pdTRUE为周期执行，pdFALSE为单次执行
								(void*)1,// 给定时器分配的唯一ID
								(TimerCallbackFunction_t)timeCallBackTask// 定时器的回调函数
								);
	xTimeHandle[1] = xTimerCreate(
								(const char *)"task2",// 定时器名字
								(TickType_t)2000,// 定时器的周期
								pdTRUE,// 定时器是单次执行还是周期执行, pdTRUE为周期执行，pdFALSE为周期执行
								(void*)2,// 给定时器分配的唯一ID
								(TimerCallbackFunction_t)timeCallBackTask2// 定时器的回调函数
								);
	// xReturn[0] = xTaskCreate(
	// 			(TaskFunction_t )timeTask1,//任务入口函数
	// 			(const char *)"timeTask1",//任务名字
	// 			(uint16_t)1024,//任务栈大小
	// 			(void*)NULL,//任务入口参数
	// 			1,//任务优先级  优先级越高，任务优先选越高
	// 			&xHandleTsak[2]//任务控制块
	// 			);
	// if(xReturn[0] == pdFALSE){}
		//changeLedStateByLocation(LED7,ON);
	//打开任务调度器
	vTaskStartScheduler();
}

/*****************************************
* 函数功能：定时器管理任务1
* 函数参数：无
* 函数返回值：无
*****************************************/
void timeTask1(void){
	uint32_t count = 0;
	const volatile TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );
	
		//LED3闪烁
		if(++count % 2){}
			//rollbackLedByLocation(LED3);
		//开启定时器任务1  关闭定时器任务2
		if(count % 50 == 0)
		{
			xTimerStart(xTimeHandle[0],0);
			xTimerStop(xTimeHandle[1],0);
		}
		//开启定时器任务2  关闭定时器任务1
		if(count % 50 == 25)
		{
			xTimerStart(xTimeHandle[1],0);
			xTimerStop(xTimeHandle[0],0);
		}
		//非阻塞延时1s
		vTaskDelay( xDelay500ms );
	
}

/*****************************************
* 函数功能：定时器任务
* 函数参数：无
* 函数返回值：无
*****************************************/
void timeCallBackTask(void){
  printf("timeCallBackTask\r\n");
  enroll_flag = false;
  fingermain_run();
	//rollbackLedByLocation(LED1);
}

/*****************************************
* 函数功能：定时器任务2
* 函数参数：无
* 函数返回值：无
*****************************************/
void timeCallBackTask2(void){
  printf("timeCallBackTask2\r\n");
	//rollbackLedByLocation(LED2);
}
