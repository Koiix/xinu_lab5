/* pipe.h: contains all the macros and definition for the pipe type in XINU */

#define MAXPIPES 10
#define PIPE_SIZE 1024


#define isbadpipe(f)  ( ((f) < NDEVS - Npip) | ((f) >= NDEVS) )

enum pipe_state_t {
	PIPE_FREE,
	PIPE_USED,
	PIPE_CONNECTED,
	PIPE_READ,
	PIPE_WRITE
};

struct pipe_t {
	pipid32 pipid;			    // Pipe ID
	enum pipe_state_t state;	// Pipe state defined by the enum
	pid32 owner; //Process ID of pipe owner, only owner creates/deletes
	pid32 reader;	//Process ID of reading process
	pid32 writer;	//Process ID of writing process
	sid32 r_lock;	//Semaphore for reader to wait on
	sid32 w_lock;  //Semaphore for writer to wait on
	sid32 empty_sem;	//semaphore that tells when buffer is empty
	sid32 full_sem;	//semaphore that tells when buffer is full
	char[PIPE_SIZE] data;	//Data stored in pipe
	int32 head;	//Head for circular buffer
	int32 tail;	//Tail for circular buffer
};

extern struct pipe_t pipe_table[MAXPIPES];	// Table for all pipes
