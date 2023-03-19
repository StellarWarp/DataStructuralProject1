#pragma once
float avg_durtime = 0;
int cust_num = 0;
void Statistics(Request* request)
{
	cust_num += 1;
	avg_durtime = (avg_durtime * (cust_num - 1) + request->durtime) / cust_num;
}
void StatisticsReset()
{
	avg_durtime = 0;
	cust_num = 0;
}