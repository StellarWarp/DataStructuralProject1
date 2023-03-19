#pragma once
#include "Console.h"
#include "Compare.h"
#include "IndexTree.h"
#include "Input.h"
#include "AutoPtr.h"
#include "Allocator.h"
#include "Array.h"
#include "LinkedList.h"


//客户类型
typedef struct Client
{
	uint64_t id;
	int account;//账目
	int credit;//信用额度
}Client;

#define T Client
#include "ArrayTemplate.h"
#include "ArrayCore.h"
#undef T

typedef struct Request
{
	Client* client;
	uint64_t id;
	int amount;
	int arrtime;
	int durtime;
	int required_time;
	bool rejected;
}Request;
#define T Request
#include "ArrayTemplate.h"
#include "ArrayCore.h"
#include "LinkedListTemplate.h"
#include "LinkedListCore.h"
#undef T

typedef struct Bank
{
	int fund;
	int open_time;
	IndexTree DataBase;
}Bank;

#include "Statistics.h"
#include "Display.h"
#include "ParamsInput.h"
#include "BankDataBase.h"


//ramdom func
inline int RangeRand(int rand_range)
{
	int rand_num = rand() % rand_range;
	return rand() % 2 ? rand_num : -rand_num;
}

Request RandomRequest(int time, const Bank* bank, SimParam* params)
{
	int max_requst_factor = params->max_requst_factor;
	int min_requst = params->min_requst;
	int initial_credit = params->initial_credit;
	int requst_rand_range = max(bank->fund * max_requst_factor, min_requst);
	uint64_t id = rand() % params->max_client;
	Client* client = DataBaseFind(&bank->DataBase, id);
	int requst_amount = RangeRand(requst_rand_range);
	//requst_amount must under credit
	if (client)
	{
		if (-(client->account + requst_amount) > client->credit)
			requst_amount = -client->credit - client->account;
	}
	else requst_amount = max(requst_amount, -initial_credit);
	//requst_amount must greater than min_requst
	if (requst_amount >= 0)
		requst_amount = max(requst_amount, min_requst);
	if (requst_amount < 0)
		requst_amount = min(requst_amount, -min_requst);

	int rand_range = 3;//[-3,3]
	int require_time = abs(requst_amount) / 500 + RangeRand(rand_range);
	if (require_time < 5) require_time = 5;
	if (require_time > 60) require_time = 60;
	Request new_cust = {
		.client = client,
		.id = id,
		.amount = requst_amount,
		.arrtime = time,
		.durtime = 0 ,
		.required_time = require_time,
		.rejected = false
	};
	return new_cust;
}


void AcceptRequest(Bank* bank, Request* request, int time)
{
	bank->fund += request->amount;
	request->durtime = time - request->arrtime;
	if (request->client == NULL)//new client
	{
		Client new_client = {
			.id = request->id,
			.account =0,
			.credit = 1000
		};
		if (!DataBaseFind(&bank->DataBase, new_client.id))
			DataBaseAdd(&bank->DataBase, new_client);
	}

	if (request->client->account < 0
		&& request->client->account + request->amount > 0)
		request->client->credit *= 1.5;//if pay back the debt, credit will be increased
	request->client->account += request->amount;
}
//return done
bool ProcessRequest(Bank* bank, Request* request, int time)
{
	request->required_time--;
	if (request->required_time == 0)
	{
		AcceptRequest(bank, request, time);
		return true;
	}
	return false;
}


//input control

enum Operation
{
	NOOP,
	PAUSED,
	SWITCH,
	EXIT
};
enum Operation Input(int* time_gap, Bank* bank)
{
	static bool tree_mode = false;
	static bool paused = false;

	enum Operation op = NOOP;
	int internal_wait_time = 0;
	int internal_wait_count = 1;
	int time_gap_op[] = { 0,5,10,20,30,40,50,60,100,150,200,300,400,500 };
	const int time_gap_op_size = sizeof(time_gap_op) / sizeof(int);
	static int time_gap_op_index = 6;

	//internal wait loop
	for (size_t i = 0; i < internal_wait_count; i++)
	{
		op = NOOP;
		//process input
		if (_kbhit())
		{
			char c = _getch();
			if (c == 'x')
			{
				time_gap_op_index--;
				if (time_gap_op_index < 0) time_gap_op_index = 0;
				*time_gap = time_gap_op[time_gap_op_index];
				break;
			}
			else if (c == 'z')
			{
				time_gap_op_index++;
				if (time_gap_op_index >= time_gap_op_size)
					time_gap_op_index = time_gap_op_size - 1;
				*time_gap = time_gap_op[time_gap_op_index];
				break;
			}
			if (c == 'c')
			{
				paused = !paused;
				break;
			}
			else if (c == 't')
			{
				tree_mode = !tree_mode;
				system("cls");
				if (tree_mode)DataBasePrint(&bank->DataBase);
				paused = tree_mode;
				break;
			}
			//input esc to exit
			else if (c == 27)
			{
				exit(0);
				break;
			}
			//input space to switch
			else if (c == 32)
			{
				op = SWITCH;
				break;
			}
		}
		if (*time_gap < 10)
		{
			internal_wait_time = *time_gap;
			internal_wait_count = 1;
		}
		else
		{
			internal_wait_time = 10;
			internal_wait_count = *time_gap / 10;
		}

		Sleep(internal_wait_time);
	}
	if (paused) op = PAUSED;
	return op;
}

