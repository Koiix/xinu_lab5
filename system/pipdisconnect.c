#include <xinu.h>

status pipdisconnect(did32 devpipe) {
	int32 mask = disable();
	if(isbadpipe(devpipe)){
		return SYSERR;
	}
  if(pipe_table[devpipe] != PIPE_CONNECTED){
		return SYSERR;
	}
	pipe_table[devpipe] = PIPE_USED;
	enable(mask);
	return OK;
}
