#pragma once

inline void PrintDeclaration()
{
	console.SetTextColor(FORE_WHITE);
	printf("李思贤 3121004736 伏羲班\n");
	console.SetTextColor(FORE_YELLOW);
	printf("\
//////////////////////////////////\n\
[X] [Z] to control loop wait time\n\
[C] to pause\n\
[T] to switch data base tree view\n\
[Space] to switch\n\
[Esc] to exit\n\
Rejected Request will be mark as red\n\
//////////////////////////////////\n\n");
	console.SetTextColor(FORE_WHITE);
}

void PrintBankInfo(Bank bank, int time, int time_gap, enum Operation op)
{
	printf("Time: %d   Loop wait time: %d\n", time, time_gap);
	console.SetTextColor(FORE_YELLOW);
	printf("Bank Fund: %d\n", bank.fund);
	console.SetTextColor(FORE_WHITE);
	printf("Average duration time: %.2f\n", avg_durtime);
	printf("Total Served: %d\n", cust_num);
}

inline void PrintRequestInfo(const Request* cust)
{
	console.SetTextColor(FORE_GRAY);
	if (cust->client)
		printf("Serving:\
ID: %-4lld left time %-3d arrive time %-3d  \
amount %-8d account: %8d --> %-8d\n",
cust->client->id,
cust->required_time,
cust->arrtime,
cust->amount,
cust->client->account,
cust->client->account + cust->amount);
	else
		printf("Serving:\
[New Client] left time %-3d arrive time %-3d  \
amount %-8d\n",
cust->required_time,
cust->arrtime,
cust->amount);

	console.SetTextColor(FORE_WHITE);
}

void PrintRegistQueue(Bank* bank, Queue(int) queue_regist)
{
	printf("\n");
	console.SetTextColor(FORE_BLUE);
	printf("Registed Client %lld\n", bank->DataBase.count);
	console.SetTextColor(FORE_WHITE);
	printf("Regist   Queue [%lld]: ", queue_regist->m_size);
	Foreach(client, queue_regist, LoopArray, Client)
	{
		printf("%lld | ", client->id);
	}
	printf("\n");
}



void(*requst_print)(const Request*);

void RequstPrint1(const Request* cust)
{
	if (cust->rejected)
		console.SetTextColor(FORE_RED);
	printf("=");
	console.SetTextColor(FORE_WHITE);
}

void RequstPrint2(const Request* cust)
{
	if (cust->rejected)
		console.SetTextColor(FORE_RED);
	printf("ID: %-3d $:%-4d | ", cust->client->id, cust->amount);
	console.SetTextColor(FORE_WHITE);
}

void PrintQueue(Queue(Request) queue, const char* name, WORD color1)
{
	if (!QueueEmpty(Request, queue))
	{
		Request* cust = &QueueFront(Request, queue);
		PrintRequestInfo(cust);
	}
	else printf("\n");
	console.SetTextColor(color1);
	printf(name);
	printf(" [%lld]: ", queue->m_size);
	console.SetTextColor(FORE_WHITE);
	Foreach(cust, queue, LoopArray, Request)
	{
		requst_print(cust);
	}
	printf("\n");
}

void PrintLinkedList(LinkedList(Request) list, const char* name, WORD color1)
{
	if (!LinkedListEmpty(Request, list))
	{
		Request* cust = &LinkedListFront(Request, list);
		PrintRequestInfo(cust);
	}
	else printf("\n");
	printf("Red Color request rejected request from Withdraw Queue\n");
	console.SetTextColor(color1);
	printf(name);
	printf(" [%lld]: ", list->length);
	console.SetTextColor(FORE_WHITE);
	Foreach(cust, list, LinkedList, Request)
	{
		requst_print(cust);
	}
	printf("\n");
}
