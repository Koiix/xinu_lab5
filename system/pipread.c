#include <xinu.h>

uint32 pipread(struct dentry *devptr, char* buf, uint32 len) {
	int32 mask = disable();
	if(isbadpipe(devptr->dvnum)){
		if(PIP_DEBUG) PIP_ERR("read");
		restore(mask);
		return SYSERR;
	}
	struct pipe_t * pipe = pipe_table[devptr->dvminor];

	//process must be the reader
	if(pipe->reader!=currpid){
		if(PIP_DEBUG) PIP_ERR("read");
			restore(mask);
			return SYSERR;
	}
	if((pipe->state != PIPE_CONNECTED && pipe->state != PIPE_SEMICONNECTED)){
		if(PIP_DEBUG) PIP_ERR("read");
		restore(mask);
		return SYSERR;
	}

	int32 numread;

	for(numread = 0; numread < len; numread++){
    *(buf+numread) = pipgetc(devptr);
		// if pipe disconnected after pipgetc, break out of reading
		if(pipe->state != PIPE_SEMICONNECTED && pipe->state != PIPE_CONNECTED)
			break;
	}

	if(PIP_DEBUG){
		char read_str[numread];
		for(int i = 0; i < numread; i++){
			read_str[i] = *(buf+numread);
		}
		kprintf("Process: %s finished reading: %s from pipe %d\n", proctab[currpid].prname, read_str, devptr->dvminor);
	}
	restore(mask);
	return numread;
}
