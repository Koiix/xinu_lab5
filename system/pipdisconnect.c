#include <xinu.h>

status pipdisconnect(did32 devpipe) {
	int32 mask = disable();
	if(isbadpipe(devpipe)){
		return SYSERR;
	}
	pipe_t pipe = pipetable[devtab[devpipe].dvminor];
  if(pipe.state != PIPE_CONNECTED || (pipe.reader != currpid && pipe.writer != currpid)){
		return SYSERR;
	}

	// if writer calls disconnect, reader reads until pipe is empty and cleans
	if(pipe.writer == currpid){
		for(int32 i = 0; i < abs(pipe.head-pipe.tail); i++)
			wait(pipe.to_write);
	}

	// if reader calls disconnect, writer cleans pipe

	pipe.state = PIPE_USED;
	pipe.reader = -1;
	pipe.writer = -1;
	enable(mask);
	return OK;
}
