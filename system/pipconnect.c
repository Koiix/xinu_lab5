#include <xinu.h>

status pipconnect(did32 devpipe , pid32 writer, pid32 reader) {
  int32 mask = disable();
	if(isbadpipe(devpipe) || reader == writer){
		return SYSERR;
	}
  pipe_t pipe = pipe_table[devtab[devpipe].dvminor];
  if(pipe.state == PIPE_CONNECTED || pipe.state == PIPE_FREE){
    return SYSERR;
  }
  pipe_table[devpipe].state = PIPE_CONNECTED;
  pipe_table[devpipe].reader = reader;
  pipe_table[devpipe].writer = writer;

	enable(mask);
	return OK;
}
