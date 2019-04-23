#include <xinu.h>

devcall pipgetc(struct dentry *devptr) {
    int32 mask = disable();
    if(isbadpipe(devptr->dvnum)){
      return SYSERR;
    }
    pipe_t pipe = pipe_table[devptr->dvnum];
    if(pipe.state != CONNECTED || pipe.reader!=currpid){
      return SYSERR;
    }

    char ch;  //char to return

    wait(empty_sem);

    tail = (tail+1)%PIPE_SIZE;
    ch = data[tail];

    signal(full_sem);

    enable(mask);
    return OK;
}
