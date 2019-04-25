#include <xinu.h>

devcall pipputc(struct dentry *devptr, char ch) {
    int32 mask = disable();
    pipid32 devpipe = devptr->dvnum;
    if(isbadpipe(devpipe)){
      if(PIP_DEBUG) PIP_ERR("putc");
      restore(mask);
      return SYSERR;
    }
    struct pipe_t pipe = pipe_table[devpipe];

    //caller must be the writer!!
    if(pipe.writer!=currpid){
      if(PIP_DEBUG) PIP_ERR("putc");
      restore(mask);
      return SYSERR;
    }
    if(pipe.state != PIPE_CONNECTED){
      if(pipe.state == PIPE_SEMICONNECTED){
        pipdisconnect(devpipe);
      }
      if(PIP_DEBUG) PIP_ERR("putc");
      restore(mask);
      return SYSERR;
    }

    wait(pipe.to_write);

    pipe.head = (pipe.head+1)%PIPE_SIZE;
    pipe.data[pipe.head] = ch;

    signal(pipe.to_read);

    if(PIP_DEBUG){
      kprintf("Process: %s put character: %c to pipe %d\n", proctab[currpid].prname, ch, devptr->dvminor);
    }

    restore(mask);
    return OK;
}
