#include <xinu.h>

status pipdelete(did32 devpipe) {
	int32 mask = disable();
	/* check if id is actually the id of a pipe device */
  if(isbadpipe(devpipe)){
		return SYSERR;
	}
	pipe_t pipe = pipe_table[devtab[devpipe].dvminor]; //extracts pipe_t
	if(pipe.state == PIPE_FREE || pipe.owner != currpid){
		return SYSERR
	}
	if(pipe.state == PIPE_CONNECTED){
		pipdisconnect(devpipe);
	}
	pipe.owner = -1;
	semdelete(pipe.empty_sem);
	semdelete(pipe.full_sem);
	pipe.head = pipe.tail = -1;
	pipe.state = PIPE_FREE;

	enable(mask);
	return OK;
}
