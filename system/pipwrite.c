#include <xinu.h>

uint32 pipwrite(struct dentry *devptr, char* buf, uint32 len) {
  int32 mask = disable();
  pipid32 devpip = devptr->dvnum;
  if(isbadpipe(devpip) || strlen(buf) < len){
    return SYSERR;
  }
  pipe_t pipe = pipe_table[devpip];
  if(pipe.state != CONNECTED || pipe.writer != currpid){
    return SYSERR;
  }

  wait(w_lock);

  int32 numwrite;
  for(numwrite = 0; numwrite < len; numwrite++){
    if(pipputc() == SYSERR){
      return SYSERR;
    }
  }

  signal(w_lock);
  enable(mask);
  return OK;
}
