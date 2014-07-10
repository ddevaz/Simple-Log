#include <stdio.h>
#include <stdint.h>

#include "Datalog.h"


int main()
{
	Datalog_t myLog;
	datalog_init(&myLog);

	datalog_add_record(&myLog, "String one.");
	datalog_add_record(&myLog, "String two.");
	datalog_add_record(&myLog, "String three.");

	datalog_insert_record(&myLog, 2, "This is an inserted record.");

	uint32_t i;
	for (i = 0; i < myLog.numRecords; i++)
		printf("index:%d string:%s\n", i, datalog_get_record(&myLog, i));

	return 0;
}

