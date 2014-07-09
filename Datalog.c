// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>

// assertions
#include <assert.h>

#include "Datalog.h"



struct Datalog_t
{
	char *indeces[DATALOG_MAX_RECORDS];

	char data[DATALOG_MAX_BYTES];

	uint32_t numRecords;

	uint32_t usedBytes;

};

// Private Functions
static bool datalog_has_free_bytes(Datalog_t * log, const uint32_t numBytes);

static bool datalog_is_max_records_used(Datalog_t * log);

static uint32_t count_bytes(char data[]);


int main()
{
		Datalog_t myLog;
	datalog_init(&myLog);

	datalog_add_record(&myLog, "String one.");
	datalog_add_record(&myLog, "String two.");
	datalog_add_record(&myLog, "String three.");
	const char *resultString  = datalog_get_record(&myLog, 1);

	uint32_t dataLength = count_bytes(myLog.data);

	datalog_insert_record(&myLog, 2, "This is an inserted record. Long string.");

	for (int i = 0; i < myLog.numRecords; i++)
		printf("index:%d string:%s\n", i, datalog_get_record(&myLog, i));


	while(1);
	return 0;
}

void datalog_init(Datalog_t * log)
{
	assert(( "datalog can't be NULL.", log != NULL ));
	memset(log->data, 0x00, DATALOG_MAX_BYTES);
	log->indeces[0] = &log->data[0];
	log->numRecords = 0;
	log->usedBytes = 0;
}


const char * datalog_get_record(Datalog_t * log, uint32_t recordIndex)
{
	uint32_t length = strlen(log->indeces[recordIndex]) + 1;
	assert(( "record index out of bounds.", recordIndex < log->numRecords ));
	return log->indeces[recordIndex];
};


void datalog_add_record(Datalog_t * log, const char * record)
{
	uint32_t charCount = strlen(record) + 1;

	// perform checks.
	assert (( "Adding record would exceed free space or max num records.", 
			  datalog_has_free_bytes(log, charCount) && !datalog_is_max_records_used(log) ));

	// make the index point to the correct string.
	if (log->numRecords > 0) 
	{
		log->indeces[log->numRecords] = log->indeces[log->numRecords - 1] + charCount;
	}

	// perform the copy of the new record.
	memcpy(log->indeces[log->numRecords], record, charCount);
	
	// update state of datalog.
	log->numRecords += 1;
	log->usedBytes += charCount;
}


void datalog_insert_record(Datalog_t * log, uint32_t recordIndex, char record[])
{
	uint32_t numRecords = log->numRecords;
	uint32_t recordLength = strlen(record) + 1;

	// perform checks.
	assert (( "Adding record would exceed free space or max num records.", 
			  datalog_has_free_bytes(log, recordIndex) && !datalog_is_max_records_used(log) ));

	// inserting a record at the end is the
	// same as adding a record.
	if (recordIndex == numRecords) 
	{
			datalog_add_record(log, record);
			return;
	}

	
	uint32_t bytesUsedFromIndex = count_bytes(log->indeces[recordIndex]);
	memmove(log->indeces[recordIndex] + recordLength, log->indeces[recordIndex],  bytesUsedFromIndex);
	memcpy(log->indeces[recordIndex], record, recordLength);

	for (int i = recordIndex+1; i < log->numRecords+1; i++)
	{
		log->indeces[i] = log->indeces[i-1] + strlen(log->indeces[i-1])+1;
	}

	log->numRecords += 1;
	log->usedBytes += 1;
}


uint32_t datalog_get_num_free_bytes(Datalog_t * log)
{
	return DATALOG_MAX_BYTES - log->usedBytes;
}


uint32_t datalog_get_num_free_records(Datalog_t * log)
{
	return DATALOG_MAX_RECORDS - log->numRecords;
}


static bool datalog_has_free_bytes(Datalog_t * log, const uint32_t numBytes)
{
	// perform checks.
	if (log->usedBytes + numBytes > DATALOG_MAX_BYTES)  
	{
		printf("Not enough space for record.\n");
		return false;
	}

	return true;
}

static bool datalog_is_max_records_used(Datalog_t * log)
{
	if (log->numRecords >= DATALOG_MAX_RECORDS) 
	{
		printf("Max number of records would be exceeded.\n");
		return true;
	}

	return false;
}


static uint32_t count_bytes(char data[])
{
	char prevChar = -1;
	char currChar = -1;
	int i = 0;
	uint32_t bytesCount = 0;
	while (i < DATALOG_MAX_BYTES)
	{
		currChar = data[i];
		if (currChar == 0 && prevChar == 0) break;
		++bytesCount;
		++i;
		prevChar = currChar;
	}

	return bytesCount;
}

