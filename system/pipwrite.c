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

  int32 numwrite;
  for(numwrite = 0; numwrite < len; numwrite++){
    wait(pipe.to_write);
    pipe.head = (pipe.head+1)%PIPE_SIZE;
    pipe.data[pipe.head] = ch;
    signal(pipe.to_read);
  }

  enable(mask);
  return numwrite;
}
