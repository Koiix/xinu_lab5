#include <xinu.h>

status pipdisconnect(did32 devpipe) {
	int32 mask = disable();
	if(isbadpipe(devpipe)){
		return SYSERR;
	}
	struct pipe_t pipe = pipe_table[devtab[devpipe].dvminor];
  if((pipe.state != PIPE_CONNECTED && pipe.state != PIPE_SEMICONNECTED) || (pipe.reader != currpid && pipe.writer != currpid)){
		return SYSERR;
	}
	if(PIP_DEBUG) kprintf("Process: %s is disconnecting from pipe: %d\n", proctab[currpid].prname, devtab[devpipe].dvminor);
	//This process cleans the pipe
	if(pipe.state == PIPE_SEMICONNECTED){
		pipe.state = PIPE_USED;
		pipe.reader = -1;
		pipe.writer = -1;
		pipe.head = pipe.tail = -1;
		semdelete(pipe.to_write);
		semdelete(pipe.to_read);
	}
	if(pipe.state == PIPE_CONNECTED){
		if(pipe.writer == currpid){
			pipe.writer = -1;
		}
		if(pipe.reader == currpid){
			pipe.reader = -1;
		}
		pipe.state = PIPE_SEMICONNECTED;
	}
	// if writer calls disconnect, reader reads until pipe is empty and cleans

	// if reader calls disconnect, writer cleans pipe
	if(PIP_DEBUG)
		kprintf("Pipe: %d is now in %s state\n", devtab[devpipe].dvminor, pipe.state==PIPE_SEMICONNECTED ? "Semi-Connected" : "Used");
	restore(mask);
	return OK;
}
