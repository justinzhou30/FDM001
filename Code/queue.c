#include "All_Function.h"

#define Q_LEN		32
#define Q_LEN_MASK	(Q_LEN-1)

UINT8 qBuffer[Q_LEN];

UINT8 front;
UINT8 rear;

void q_init(void)
{
	UINT8 temp;
	
	for(temp = 0 ; temp < Q_LEN ; temp++)
		qBuffer[temp] = 0;
	
	front = 0;
	rear = 0;
}

void q_push(UINT8 qData)
{
//	clr_EA;
	
	if(((rear+1) & Q_LEN_MASK) == front)
	{
//		set_EA;
		return;
	}
	
	qBuffer[rear++] = qData;
	
	rear &= Q_LEN_MASK;
	
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
	
	front &= Q_LEN_MASK;
	
//	set_EA;
	
	return q_get_dataSize()+1;

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
		return (rear+Q_LEN-front);
	}
	else
	{
//		set_EA;
		return (rear-front);
	}
		
}

void q_server(void)
{
	
}


