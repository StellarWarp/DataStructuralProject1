#pragma once

#define BASIC_MODE 1
#define EXTEND_MODE 2
#define EXTENDED (params.mode == 2)

void InputTip()
{
	console.Clear();
	console.SetTextColor(FORE_WHITE);
	printf("李思贤 3121004736 伏羲班\n");
	console.SetTextColor(FORE_YELLOW);
	printf("\
//////////////////////////////////\n\
[Space] to accept\n\
You can switch Mode with first option\n\
Key [<] and [>] to select parameters\n\
input [number] to modify parameters\n\
[Enter] to apply modification\n\
DONOT input non ascii character（right. left. arrow）\n\
//////////////////////////////////\n");
	console.SetTextColor(FORE_WHITE);
}

typedef struct SimParam
{
	int mode;
	int initial_fund;
	int open_time;
	int max_client;
	int density;
	float max_requst_factor;
	int min_requst;
	int initial_credit;
	int withdraw_queue_count;
	int deposit_queue_count;
	int rand_seed;
	int display_style;
	int reject_strategic;
}SimParam;

SimParam ParamsInput()
{
	static SimParam params =
	{
		.mode = 1,
		.initial_fund = 10000,
		.open_time = 600,
		.max_client = 1000,
		.density = 20,
		.max_requst_factor = 0.2,
		.min_requst = 100,
		.initial_credit = 10000,
		.withdraw_queue_count = 3,
		.deposit_queue_count = 4,
		.rand_seed = 0,
		.display_style = 2,
		.reject_strategic = 1
	};

	enum TYPE{INT, FLOAT};
	typedef struct ParamsInfo
	{
		char* name;
		enum TYPE type;
		int range_min;
		int range_max;
		bool extend;
	}ParamsInfo;
	ParamsInfo params_info[sizeof(params) / sizeof(int)] = {
		{"Mode[1:Basic,2:Extend]",INT,1,2,false},
		{"Initial Fund",INT,0,INT_MAX,false},
		{"Open Time",INT,0,INT_MAX,false},
		{"Max Client",INT,1,INT_MAX,false},
		{"Density",INT,0,100,false},
		{"Max Request Factor",FLOAT,0,1,false},
		{"Min Request",INT,0,INT_MAX,false},
		{"Initial Credit",INT,0,INT_MAX,false},
		{"Withdraw Queue Count",INT,0,INT_MAX,true},
		{"Deposit Queue Count",INT,0,INT_MAX,true},
		{"Random Seed",INT,0,INT_MAX,false},
		{"Display Style[1:simplify,2:detailed]",INT,1,2,false},
		{"Reject Strategic[1:move back,2:wait]",INT,1,2,true}
	};

	//use < >  to select
	//space to accept
	int selected = 0;
	int option_count = sizeof(params) / sizeof(int);

	while (true)
	{
		InputTip();

		if (EXTENDED)
			printf("\n[Extend Mode]\n\n");
		else
			printf("\n[Basic Mode]\n\n");

		for (int i = 0; i < option_count; i++)
		{
			if (params_info[i].extend && !EXTENDED)
				continue;
			if (i == selected)
				console.SetTextColor(FORE_CYAN);
			else
				console.SetTextColor(FORE_WHITE);
			printf("%s: ", params_info[i].name);
			if(params_info[i].type == INT)
				printf("%d", *((int*)&params + i));
			else
				printf("%.4f", *((float*)&params + i));
			printf("\n");
		}
		//inputs

		char c = _getch();
		//< and > to move
		if (c == ',')
		{
			do
			{
				selected--;
				if (selected < 0)
					selected = option_count - 1;
			} while (params_info[selected].extend && !EXTENDED);
		}
		if (c == '.')
		{
			do
			{
				selected++;
				if (selected > option_count - 1)
					selected = 0;
			} while (params_info[selected].extend && !EXTENDED);
		}
		//space to accept
		if (c == ' ')
		{
			break;
		}
		//input value
		if (isalnum(c))
		{
			//take c as the first char
			char str[100];
			str[0] = c;
			console.SetTextColor(FORE_YELLOW);
			printf("\nSet Value : %c", c);
			int i = 1;
			//input to buffer
			while (true)
			{
				c = _getch();
				if (c == '\r')
				{
					str[i] = '\0';
					break;
				}
				if (c == '\b')
				{
					if (i > 0)
					{
						i--;
						printf("\b \b");
					}
				}
				else
				{
					str[i] = c;
					i++;
					printf("%c", c);
				}
			}
			if (params_info[selected].type == INT)
				*((int*)&params + selected) =
				min(max(
					atoi(str),
					params_info[selected].range_min), params_info[selected].range_max);
			else
				*((float*)&params + selected) = 
				min(max(
					atof(str),
					params_info[selected].range_min), params_info[selected].range_max);
		}
	}
	
	srand(params.rand_seed);
	requst_print = params.display_style == 2 ? RequstPrint2 : RequstPrint1;

	return params;
}
