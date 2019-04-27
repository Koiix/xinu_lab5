/* pipe.h: contains all the macros and definition for the pipe type in XINU */

#define MAXPIPES 10
#define PIPE_SIZE 1024
#define PIP_DEBUG 1

#define no_data(f) ((abs(f->head - f->tail)) == (0))

#define isbadpipe(f)  ( ((f) < PIPELINE0) | ((f) >= PIPELINE0+MAXPIPES) )

#define PIP_ERR(err) kprintf("ERROR: %s\n", err)

enum pipe_state_t {
	PIPE_FREE,
	PIPE_USED,
	PIPE_CONNECTED,
	PIPE_SEMICONNECTED
};

struct pipe_t {
	pipid32 pipid;			    // Pipe ID
	enum pipe_state_t state;	// Pipe state defined by the enum
	pid32 owner; //Process ID of pipe owner, only owner creates/deletes
	pid32 reader;	//Process ID of reading process
	pid32 writer;	//Process ID of writing processs
	sid32 to_read;	//semaphore that tells when buffer is empty
	sid32 to_write;	//semaphore that tells when buffer is full
	char data[PIPE_SIZE];	//Data stored in pipe
	int32 head;	//Head for circular buffer
	int32 tail;	//Tail for circular buffer
};

extern struct pipe_t pipe_table[MAXPIPES];	// Table for all pipes
