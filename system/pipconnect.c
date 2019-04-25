#include <xinu.h>

status pipconnect(did32 devpipe , pid32 writer, pid32 reader) {
  int32 mask = disable();
	if(isbadpipe(devpipe) || isbadpid(reader) || isbadpid(writer) || reader == writer){
    if(PIP_DEBUG) PIP_ERR("connect");
    restore(mask);
    return SYSERR;
	}
  struct pipe_t pipe = pipe_table[devtab[devpipe].dvminor];
  if(pipe.state == PIPE_CONNECTED || pipe.state == PIPE_FREE || pipe.state == PIPE_SEMICONNECTED){
    if(PIP_DEBUG) PIP_ERR("connect");
    restore(mask);
    return SYSERR;
  }
  if(PIP_DEBUG){
    kprintf("connecting writer: %s to reader: %s using pipe: %d\n", proctab[writer].prname, proctab[reader].prname,devtab[devpipe].dvminor);
  }
  pipe.state = PIPE_CONNECTED;
  pipe.reader = reader;
  pipe.writer = writer;
  pipe.to_read = semcreate(0);  /* Consumer semaphore starts at 0 */
  pipe.to_write = semcreate(PIPE_SIZE); /* Producer semaphore starts at buffer size */

	restore(mask);
	return OK;
}
