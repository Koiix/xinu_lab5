#include <xinu.h>

status pipdelete(did32 devpipe) {
	int32 mask = disable();
	/* check if id is actually the id of a pipe device */
  if(isbadpipe(devpipe)){
		if(PIP_DEBUG) PIP_ERR("delete");
		restore(mask);
		return SYSERR;
	}
	struct pipe_t * pipe = &pipe_table[devtab[devpipe].dvminor]; //extracts pipe_t
	if(pipe->state == PIPE_FREE || pipe->owner != currpid){
		if(PIP_DEBUG) PIP_ERR("delete");
		restore(mask);
		return SYSERR;
	}
	if(PIP_DEBUG){
		kprintf("Deleting pipe: %d with owner: %s\n", devtab[devpipe].dvminor, proctab[currpid].prname);
	}
	pipe->pipid = -1;
	pipe->owner = pipe->reader = pipe->writer= -1;
	semdelete(pipe->to_read);
	semdelete(pipe->to_write);
	pipe->head = pipe->tail = -1;
	pipe->state = PIPE_FREE;

	restore(mask);
	return OK;
}
