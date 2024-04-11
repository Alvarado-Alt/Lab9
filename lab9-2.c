#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

#define HASH_SIZE 23

// Fill out this structure
struct HashType
{
	struct RecordType* record;
    struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	return x % HASH_SIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		struct HashType* current = &pHashArray[i];
        printf("Index %d -> ", i);
        while (current != NULL)
        {
            if (current->record != NULL) {
                printf("%d %c %d -> ", current->record->id, current->record->name, current->record->order);
            }
            current = current->next;
        }
        printf("NULL\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);

	int i;
	// Initialize hash table
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * HASH_SIZE);
    for (i = 0; i < HASH_SIZE; ++i)
    {
		// Initialize each bucket head to NULL
        hashTable[i].record = NULL;
        // Initialize next to NULL
		hashTable[i].next = NULL;
    }

    // Insert records into hash table
    for (i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        // Create a new node
        struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
        newNode->record = &pRecords[i];
        newNode->next = hashTable[index].next;

        // Insert at the beginning of the linked list
        hashTable[index].next = newNode;
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free memory
    for (i = 0; i < HASH_SIZE; ++i)
    {
        struct HashType* current = hashTable[i].next;
        while (current != NULL)
        {
            struct HashType* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable);
    free(pRecords);

    return 0;
}