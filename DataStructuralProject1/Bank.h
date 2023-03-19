#pragma once
#include "BankFunction.h"

void BankSim(SimParam param)
{
	AutoMallocBegin();

	Bank bank = { param.initial_fund,param.open_time };
	DataBaseInit(&bank.DataBase);
	StatisticsReset();

	Queue(Client) queue_regist = QueueInit(Client, 4, true);
	Queue(Request) queue_withdraw = QueueInit(Request, 4, true);
	LinkedList(Request) queue_deposit = LinkedListInit(Request, true);

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
			QueueClear(Request, queue_withdraw);
			LinkedListClear(Request, queue_deposit);
		}

		//ramdonly add customer
		int queue_length = max(QueueSize(Request, queue_withdraw), LinkedListSize(Request, queue_deposit));
		queue_length = max(QueueSize(Client, queue_regist), queue_length);
		int p_factor = param.density - queue_length;
		if (rand() % 100 < p_factor)
			for (size_t i = 0; i < 20; i++)
			{
				Request new_cust = RandomRequest(time, &bank, &param);

				if (new_cust.client == NULL)
				{
					int amount = 0;
					Client new_client = { new_cust.id, amount, param.initial_credit };
					QueuePush(Client, queue_regist, new_client);
					continue;
				}

				if (new_cust.amount >= 0)
				{
					LinkedListPushBack(Request, queue_deposit, new_cust);
				}
				else
				{
					QueuePush(Request, queue_withdraw, new_cust);
				}

			}
		//process regist
		if (!QueueEmpty(Client, queue_regist))
		{
			Client new_client = QueueFront(Client, queue_regist);
			QueuePop(Client, queue_regist);
			if (!DataBaseFind(&bank.DataBase, new_client.id))
				DataBaseAdd(&bank.DataBase, new_client);
		}
		//queue 1(queue_withdraw) update logic
		if (!QueueEmpty(Request, queue_withdraw))
		{
			Request cust1 = QueueFront(Request, queue_withdraw);
			QueuePop(Request, queue_withdraw);
			//process cust1
			if (-cust1.amount > bank.fund)
			{
				//reject
				//move to queue 2(queue_deposit)
				cust1.rejected = true;
				LinkedListPushBack(Request, queue_deposit, cust1);
			}
			else
			{
				AcceptRequest(&bank, &cust1, time);
				Statistics(&cust1);
			}
		}
		//queue 2(queue_deposit) update logic
		if (!LinkedListEmpty(Request, queue_deposit))
		{
			Request cust2 = LinkedListFront(Request, queue_deposit);
			LinkedListPopFront(Request, queue_deposit);
			//process cust2
			if (-cust2.amount > bank.fund)//reject
			{
				LinkedListPushBack(Request, queue_deposit, cust2);
			}
			else
			{
				AcceptRequest(&bank, &cust2, time);
				Statistics(&cust2);
				//scan queue 2(queue_deposit) for rejected customers
				LinkedListNode(Request) prev_node = NULL;
				for (Request* cust = LinkedListBegin(Request, queue_deposit);
					cust != LinkedListEnd(Request, queue_deposit);)
				{
					if (-cust->amount > bank.fund) break;
					if (cust->amount <= 0)
					{
						AcceptRequest(&bank, cust, time);
						Statistics(cust);
						cust = LinkedListRemove(Request, queue_deposit, prev_node);
					}
					else
					{
						prev_node = cust;
						cust = LinkedListForward(Request, queue_deposit, cust);
					}
				}
			}

		}
		//display
		console.Clear();
		PrintDeclaration();
		PrintBankInfo(bank, time, time_gap, op);
		PrintRegistQueue(&bank, queue_regist);
		PrintQueue(queue_withdraw, "Withdraw Queue", FORE_CYAN);
		PrintLinkedList(queue_deposit, "Deposit Queue", FORE_PURPLE);
	}


	AutoMallocEnd();
}


