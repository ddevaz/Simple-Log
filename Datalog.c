// Add proper description.





// std includes
#include <stdint.h>

#include <string.h>

#include <stdbool.h>

#include <assert.h>

// library includes
#include "Datalog.h"

#include <stdio.h>

// Private Prototypes
static bool datalog_has_free_bytes(Datalog_t * log, const uint32_t numBytes);

static bool datalog_is_max_records_used(Datalog_t * log);

static uint32_t count_bytes(char data[]);

static uint32_t datalog_num_bytes_from_index(const Datalog_t * log, const uint32_t recordIndex);



void datalog_init(Datalog_t * log)
{
	assert("datalog can't be NULL." && log != NULL );
	memset(log->data, 0x00, DATALOG_MAX_BYTES);
	log->indeces[0] = &log->data[0];
	log->numRecords = 0;
	log->usedBytes = 0;
}


const char * datalog_get_record(Datalog_t * log, uint32_t recordIndex)
{
	assert( "record index out of bounds." && recordIndex < log->numRecords );
	return log->indeces[recordIndex];
};


void datalog_add_record(Datalog_t * log, const char * record)
{
	uint32_t charCount = strlen(record) + 1;

	// perform checks.
	assert ( "Adding record would exceed free space or max num records." &&
			  datalog_has_free_bytes(log, charCount) && !datalog_is_max_records_used(log) );

	// make the index point to the correct string.
	if (log->numRecords > 0)
	{
		log->indeces[log->numRecords] = log->indeces[log->numRecords - 1] + strlen(log->indeces[log->numRecords - 1]) + 1;
	}

	// perform the copy of the new record.
	memcpy(log->indeces[log->numRecords], record, charCount);

	// update state of datalog.
	log->numRecords += 1;
	log->usedBytes += charCount;
}


void datalog_delete_record(Datalog_t * log, const uint32_t recordIndex)
{
	assert("Record Index out of bounds" && recordIndex < log->numRecords);
	char *dataEndIndex = log->indeces[log->numRecords - 1] + strlen(log->indeces[log->numRecords-1]) + 1;
	char *dataStartIndex = log->indeces[recordIndex];
	uint32_t  numberOfBytesToMove = dataEndIndex - dataStartIndex;
	printf("number of bytes to move: %u\n countbytes:%u\n", numberOfBytesToMove, count_bytes(log->indeces[recordIndex]));

}


void datalog_insert_record(Datalog_t * log, uint32_t recordIndex, char record[])
{
	uint32_t numRecords = log->numRecords;
	uint32_t recordLength = strlen(record) + 1;

	// perform checks.
	assert( "Adding record must not exceed free space."
               && datalog_has_free_bytes(log, recordIndex) );
        assert( "Adding record must not exceed max number of records"
               && !datalog_is_max_records_used(log) );

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

	uint32_t i;
	for (i = recordIndex+1; i < log->numRecords+1; i++)
	{
		log->indeces[i] = log->indeces[i-1] + strlen(log->indeces[i-1])+1;
	}

	log->numRecords += 1;
	log->usedBytes += recordLength;
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
	return log->usedBytes + numBytes <= DATALOG_MAX_BYTES;
}

static bool datalog_is_max_records_used(Datalog_t * log)
{
	return log->numRecords >= DATALOG_MAX_RECORDS;
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



