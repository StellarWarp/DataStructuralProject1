#pragma once
#include "BankFunction.h"

//在最短的队列中加入客户
void Distribut(Request request, Queue(Request) queues[], int count)
{
	int min_index = 0;
	for (int i = 1; i < count; i++)
	{
		if (QueueSize(Request, queues[i]) < QueueSize(Request, queues[min_index]))
			min_index = i;
	}
	QueuePush(Request, queues[min_index], request);
}

void BankSimExtend(SimParam param)
{
	AutoMallocBegin();

	Bank bank = { param.initial_fund,param.open_time };
	DataBaseInit(&bank.DataBase);
	StatisticsReset();


	Queue(Client) queue_regist = QueueInit(Client, 4, true);

	int queue_withdraw_count = param.withdraw_queue_count;
	int queue_deposit_count = param.deposit_queue_count;

	Queue(Request)* queues_withdraw =
		AUTO_MALLOC(Queue(Request), queue_withdraw_count);
	Queue(Request)* queues_deposit =
		AUTO_MALLOC(Queue(Request), queue_deposit_count);

	for (int i = 0; i < queue_withdraw_count; i++)
		queues_withdraw[i] = QueueInit(Request, 4, true);
	for (int i = 0; i < queue_deposit_count; i++)
		queues_deposit[i] = QueueInit(Request, 4, true);

	int time = 0;//minutes

	int time_gap = 50;

	//simulate loop
	while (true)
	{
		enum Operation op = Input(&time_gap, &bank);
		if (op == SWITCH) break;
		if (op == PAUSED) continue;

		time += 1;
		bank.open_time -= 1;
		//a day for a cycle
		if (time >= 600)
		{
			time = 0;
			bank.open_time = 600;
			for (int i = 0; i < queue_withdraw_count; i++)
				QueueClear(Request, queues_withdraw[i]);
			for (int i = 0; i < queue_deposit_count; i++)
				QueueClear(Request, queues_deposit[i]);
		}

		//ramdonly add customer
		int queue_length = 0;
		queue_length = max(queue_length, QueueSize(Client, queue_regist));
		for (int i = 0; i < queue_withdraw_count; i++)
			queue_length = max(queue_length, QueueSize(Request, queues_withdraw[i]));
		for (int i = 0; i < queue_deposit_count; i++)
			queue_length = max(queue_length, QueueSize(Request, queues_deposit[i]));
		int p_factor = param.density - queue_length;
		if (rand() % 100 < p_factor)
			for (size_t i = 0; i < 10; i++)
			{
				Request new_cust = RandomRequest(time, &bank, &param);

				if (new_cust.client == NULL)
				{
					int amount = 0;
					Client new_client = { new_cust.id, amount, 10000 };
					QueuePush(Client, queue_regist, new_client);
					continue;
				}

				if (new_cust.amount >= 0)
					Distribut(new_cust, queues_deposit, queue_deposit_count);
				else
					Distribut(new_cust, queues_withdraw, queue_withdraw_count);

			}
		//process regist
		if (!QueueEmpty(Client, queue_regist))
		{
			Client new_client = QueueFront(Client, queue_regist);
			QueuePop(Client, queue_regist);
			if (!DataBaseFind(&bank.DataBase, new_client.id))
			{
				DataBaseAdd(&bank.DataBase, new_client);
				bank.fund += new_client.account;
			}
		}
		//queue_withdraw update logic
		for (int i = 0; i < queue_withdraw_count; i++)
			if (!QueueEmpty(Request, queues_withdraw[i]))
			{
				Request* cust1 = &QueueFront(Request, queues_withdraw[i]);
				//process cust1
				if (-cust1->amount > bank.fund)
				{
					//reject
					cust1->rejected = true;
					if (param.reject_strategic == 1)//move back
					{
						QueuePop(Request, queues_withdraw[i]);
						QueuePush(Request, queues_withdraw[i], *cust1);
					}
				}
				else
				{
					if (ProcessRequest(&bank, cust1, time))
					{
						QueuePop(Request, queues_withdraw[i]);
						Statistics(cust1);
					}
				}
			}
		//queue_deposit update logic
		for (int i = 0; i < queue_deposit_count; i++)
			if (!QueueEmpty(Request, queues_deposit[i]))
			{
				Request* cust2 = &QueueFront(Request, queues_deposit[i]);
				//process cust2
				if (ProcessRequest(&bank, cust2, time))
				{
					//done
					QueuePop(Request, queues_deposit[i]);
					Statistics(cust2);
				}
			}

		//display
		console.Clear();
		PrintDeclaration();
		PrintBankInfo(bank, time, time_gap, op);
		PrintRegistQueue(&bank, queue_regist);
		for (int i = 0; i < queue_withdraw_count; i++)
			PrintQueue(queues_withdraw[i], "Withdraw Queue", FORE_CYAN);
		for (int i = 0; i < queue_deposit_count; i++)
			PrintQueue(queues_deposit[i], "Deposit  Queue", FORE_PURPLE);

	}


	AutoMallocEnd();
}