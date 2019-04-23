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
	pipe.state = PIPE_USED;
	pipe.reader = -1;
	pipe.writer = -1;
	enable(mask);
	return OK;
}
