#include <xinu.h>

devcall pipgetc(struct dentry *devptr) {
    int32 mask = disable();
    if(isbadpipe(devptr->dvnum)){
      if(PIP_DEBUG) PIP_ERR("getc bad pipe");
      restore(mask);
      return SYSERR;
    }
    struct pipe_t * pipe = pipe_table[devptr->dvminor];
    if((pipe->state != PIPE_CONNECTED && pipe->state != PIPE_SEMICONNECTED) || pipe->reader != currpid){
      if(PIP_DEBUG) PIP_ERR("getc bad connection or bad reader");
      restore(mask);
      return SYSERR;
    }

    char ch;  //char to return

    wait(pipe->to_read);
    pipe->tail = (pipe->tail+1)%PIPE_SIZE;
    ch = pipe->data[pipe->tail];
    signal(pipe->to_write);

    if(no_data(pipe) && pipe->state == PIPE_SEMICONNECTED){
      pipdisconnect(devptr->dvnum);
    }

    if(PIP_DEBUG){
      kprintf("Process: %s got character: %c from pipe %d\n", proctab[currpid].prname, ch, devptr->dvminor);
    }

    restore(mask);
    return ch;
}
