#include "All_Function.h"

UINT8 qBuffer[64];

UINT8 front;
UINT8 rear;

void q_init(void)
{
	UINT8 temp;
	
	for(temp = 0 ; temp < 64 ; temp++)
		qBuffer[temp] = 0;
	
	front = 0;
	rear = 0;
}

void q_push(UINT8 qData)
{
//	clr_EA;
	
	if(((rear+1) & 0x3f) == front)
	{
//		set_EA;
		return;
	}
	
	qBuffer[rear++] = qData;
	
	rear &= 0x3f;
	
//	set_EA;
}

UINT8 q_pop(UINT8 *qData)
{
//	clr_EA;
	
	if(front == rear)		//队列为空，返回0
	{
//		set_EA;
		return 0;
	}
	
	*qData = qBuffer[front++];
	
	front &= 0x3f;
	
//	set_EA;
	
	return q_get_dataSize();

}

UINT8 q_get_dataSize(void)
{
//	clr_EA;
	
	if(front == rear)
	{
//		set_EA;
		return 0;
	}
	
	if(front > rear)
	{
//		set_EA;
		return (rear+64-front);
	}
	else
	{
//		set_EA;
		return (rear-front);
	}
		
}


