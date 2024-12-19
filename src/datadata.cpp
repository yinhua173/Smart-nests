#include <iostream>
// #include <vector>
#include "datadata.h"
using namespace std;
#define Maxsize 33
volatile uint8_t n=0;
uint8_t dd = 0;//入队标识1,出队标识2,读取标识3
extern BME680 bme680;
extern beginn door;
// extern uint8_t humi;
// extern float temp;
TickType_t lasttick = xTaskGetTickCount();//初值
volatile uint8_t min_one=0;
volatile bool datadata_temp = false;
volatile bool datadata_humi = false;
volatile bool datadata_door = false;
volatile bool datadata_tra_state = false;
volatile bool datadata_state_stop = false;
volatile float datadata[32];
volatile int timedd[32];
volatile uint16_t tra=0;
volatile uint8_t data_stop=0;
volatile uint8_t time_size=sizeof(datadata)/sizeof(datadata[0]);
typedef  struct SqQueue{
  float *base; //基地址
  int front,rear; //头指针,尾指针
}SqQueue;

//循环队列的初始化
bool InitQueue(SqQueue &Q)//注意使用引用参数，否则出了函数，其改变无效
{
	Q.base=new float[Maxsize];//分配空间
	if(!Q.base) return false;
	Q.front=Q.rear=0; //头指针和尾指针置为零，队列为空
	return true;
}

//循环队列的入队
bool EnQueue(SqQueue &Q,float e)//将元素e放入Q的队尾
{
	if((Q.rear+1)%Maxsize==Q.front) //尾指针后移一位等于头指针，表明队满
	{ // 队列满的情况
        // 执行出队操作，删除队头元素
        Q.front = (Q.front + 1) % Maxsize;
    }
	Q.base[Q.rear]=e; //新元素插入队尾
	Q.rear=(Q.rear+1)%Maxsize; //队尾指针加1
	return true;
}

//循环队列的出队
bool DeQueue(SqQueue &Q, int &e) //删除Q的队头元素，用e返回其值
{
	if (Q.front==Q.rear)
		return false; //队空
	e=Q.base[Q.front]; //保存队头元素
	Q.front=(Q.front+1)%Maxsize; //队头指针加1
	return true;
}

//取循环队列的队头元素
int GetHead(SqQueue Q)//返回Q的队头元素，不修改队头指针
{
	if (Q.front!=Q.rear) //队列非空
		return Q.base[Q.front];
    return -1;
}
//循环队列的长度
int QueueLength(SqQueue Q)
{
	return (Q.rear-Q.front+Maxsize)%Maxsize;
}

// 遍历队列，不删除元素
void TraverseQueue(SqQueue Q) {
	if (Q.front == Q.rear){
		printf("Queue is empty.\n");
		return;
	}
	tra++;
	int i = Q.front;
	int j=0;
	while (i != Q.rear){
		printf("%f ", Q.base[i]);
		if(tra==1){//取出温度数组
			datadata[j]=Q.base[i];
		}else if(tra==2){
			timedd[j]=Q.base[i];
		}
		j++;
		i = (i + 1) % Maxsize;
	}
	printf("\n");
}

void datadata_task(void *parameter){
  SqQueue Q;//温度
  SqQueue H;//湿度
	SqQueue time1;//温度湿度记录时间
	SqQueue doorr;//门口历史
	SqQueue time2;//门口记录小时
	int n=0,x,a=0;
	uint8_t hour_in=0;
	bool door_in=false;
	InitQueue(Q);//初始化队列(一定要初始化，否则后面存储出错)
  InitQueue(H);
	InitQueue(doorr);
  InitQueue(time1);
	InitQueue(time2);
	while(1){
	  if(datadata_temp||datadata_humi||datadata_door){
			data_stop++;
			if(data_stop==1){
			  a=3;datadata_tra_state = true;
			}
		}
		if(hour_in!=rtc.minutes()){//每一小时入队一次
			hour_in=rtc.minutes();
			a=1;
		}
		if(door_in!=door.status){//门口状态改变入队
			door_in=door.status;
			EnQueue(doorr,door.status);
			EnQueue(time2,rtc.hours()*100+rtc.minutes());//02 03 04 05
		}
		switch(a){
			case 1: 
			//cout <<"入队：" <<endl;
		       	//cin>>x;
				n++;
				EnQueue(Q,bme680.temp);//入队
				EnQueue(H,bme680.humi);
				EnQueue(time1,hour_in);//02 03 04 05
				a=0;
			//cout<<endl;
	    	break;
	    case 2:
	    	//cout <<"队列内元素个数，即长度："<<QueueLength(Q)<<endl;
		    //cout <<"队头元素：" <<GetHead(Q)<<endl;
			//cout <<"元素依次出队：" <<endl;
			while(true)//如果栈不空，则依次出栈
			{
				if (DeQueue(Q, x))
					cout << x << "\t"; // 出队元素
				else
					break;
			}
			while (true) // 如果栈不空，则依次出栈
			{
				if (DeQueue(H, x))
					cout << x << "\t"; // 出队元素
				else
					break;
			}
				//cout <<endl;
		    break;
			case 3:
			if(datadata_temp){
				TraverseQueue(Q);
				TraverseQueue(time1);
			}
			if(datadata_humi){
				TraverseQueue(H);
				TraverseQueue(time1);
			}
			if(datadata_door){
				TraverseQueue(doorr);
				TraverseQueue(time2);
			}
				tra=0,a=0;datadata_tra_state = false;
			break;
		}
		vTaskDelay(100);
	}
}
// //vTaskDelayUntil测试
// void delay_test_task(void *parameter){
        
// 		while(1){
// 			printf("----------------------\n");
// 			printf("1.lasttick:%d,\txTaskGetTickCount:%d\n", lasttick, xTaskGetTickCount());
// 			vTaskDelayUntil(&lasttick, 10000);//60000ms
// 			min_one++;
// 			printf("min_one:%d\n",min_one);
// 		}
// }

