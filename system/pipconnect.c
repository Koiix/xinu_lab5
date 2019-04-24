#include <xinu.h>

status pipconnect(did32 devpipe , pid32 writer, pid32 reader) {
  int32 mask = disable();
	if(isbadpipe(devpipe) || reader == writer){
		return SYSERR;
	}
  pipe_t pipe = pipe_table[devtab[devpipe].dvminor];
  if(pipe.state == PIPE_CONNECTED || pipe.state == PIPE_FREE || pipe.state == PIPE_SEMICONNECTED){
    return SYSERR;
  }
  pipe.state = PIPE_CONNECTED;
  pipe.reader = reader;
  pipe.writer = writer;
  pipe.to_read = semcreate(0);  /* Consumer semaphore starts at 0 */
  pipe.to_write = semcreate(PIPE_SIZE); /* Producer semaphore starts at buffer size */

	enable(mask);
	return OK;
}
