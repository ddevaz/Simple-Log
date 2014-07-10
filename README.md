simple-log
==========

A simple char * based data log for embedded devices written in C.
Simple log is designed to work with embedded systems so the library does not use dynamic memory allocation.

**Example Usage**
```C
#include <stdio.h>
#include <stdint.h>
#include "Datalog.h"


static Datalog_t myLog;

int main()
{
	// Initializing.
	datalog_init(&myLog);

	// Adding records.
	datalog_add_record(&myLog, "String one.");
	datalog_add_record(&myLog, "String two.");
	datalog_add_record(&myLog, "String three.");
	
	// Inserting records (no overwrite).
	datalog_insert_record(&myLog, 2, "This is an inserted record.");

	// Displaying records.
	uint32_t i;
	for (i = 0; i < myLog.numRecords; i++)
	{
		printf("index:%d string:%s\n", i, datalog_get_record(&myLog, i));
	}

	return 0;
}

```

... TBC.
