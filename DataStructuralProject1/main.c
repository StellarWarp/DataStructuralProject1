#include "Bank.h"
#include "BankExtend.h"

int main()
{
	ConsoleInit();
	AutoPtrInit();

	while (true)
	{
		SimParam params = ParamsInput();
		if (params.mode == BASIC_MODE)
			BankSim(params);
		else
			BankSimExtend(params);
	}

	AutoPtrReleaseAll();
	ConsoleOff();
}


