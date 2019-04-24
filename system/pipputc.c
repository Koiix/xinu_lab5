#include <xinu.h>

devcall pipputc(struct dentry *devptr, char ch) {
    int32 mask = disable();
    pipid32 devpipe = devptr->dvnum;
    if(isbadpipe(devpipe)){
      return SYSERR;
    }
    struct pipe_t pipe = pipe_table[devpipe];
    if(pipe.writer!=currpid)
      return SYSERR;
    if(pipe.state != PIPE_CONNECTED){
      if(pipe.state == PIPE_SEMICONNECTED){
        pipdisconnect(devpipe);
      }
      return SYSERR;
    }

    wait(pipe.to_write);

    pipe.head = (pipe.head+1)%PIPE_SIZE;
    pipe.data[pipe.head] = ch;

    signal(pipe.to_read);

    restore(mask);
    return OK;
}
