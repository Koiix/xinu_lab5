#include <xinu.h>

devcall pipgetc(struct dentry *devptr) {
    int32 mask = disable();
    if(isbadpipe(devptr->dvnum)){
      return SYSERR;
    }
    struct pipe_t pipe = pipe_table[devptr->dvnum];
    if((pipe.state != PIPE_CONNECTED && pipe.state != PIPE_SEMICONNECTED) || pipe.reader != currpid){
      return SYSERR;
    }

    char ch;  //char to return

    wait(pipe.to_read);
    pipe.tail = (pipe.tail+1)%PIPE_SIZE;
    ch = pipe.data[pipe.tail];
    signal(pipe.to_write);

    if(no_data(pipe) && pipe.state == PIPE_SEMICONNECTED){
      pipdisconnect(devptr->dvnum);
    }

    restore(mask);
    return ch;
}
