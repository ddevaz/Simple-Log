
#define DATALOG_MAX_BYTES	10000L

#define	DATALOG_MAX_RECORDS	10000L

typedef struct 
{
	char *indeces[DATALOG_MAX_RECORDS];

	char data[DATALOG_MAX_BYTES];

	uint32_t numRecords;

	uint32_t usedBytes;

} Datalog_t;

void datalog_init(Datalog_t * log);

const char * datalog_get_record(Datalog_t * log, uint32_t recordIndex);

void datalog_add_record(Datalog_t * log, const char * record);

void datalog_insert_record(Datalog_t * log, uint32_t recordIndex, char record[]);

uint32_t datalog_get_num_free_bytes(Datalog_t * log);

uint32_t datalog_get_num_free_records(Datalog_t * log);