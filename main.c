#include <stdio.h>
#include <stdint.h>

#include "Datalog.h"


int main()
{
	Datalog_t myLog;
	datalog_init(&myLog);

	datalog_add_record(&myLog, "string one");
	datalog_add_record(&myLog, "string two");
	datalog_add_record(&myLog, "string four");

	datalog_insert_record(&myLog, 2, "string three");

	uint32_t i;
	for (i = 0; i < myLog.numRecords; i++)
		printf("index:%d string:%s\n", i, datalog_get_record(&myLog, i));

	return 0;
}

