#include <xinu.h>

uint32 pipwrite(struct dentry *devptr, char* buf, uint32 len) {
  int32 mask = disable();
  pipid32 devpip = devptr->dvnum;
  if(isbadpipe(devpip) || strlen(buf) < len){
    return SYSERR;
  }
  struct pipe_t pipe = pipe_table[devpip];

  //process must be the writer
  if(pipe.writer != currpid)
    return SYSERR;

  if(pipe.state != CONNECTED){
    if(pipe.state==PIPE_SEMICONNECTED){
      pipdisconnect(devptr->dvnum);
    }
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
