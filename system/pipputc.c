#include <xinu.h>

devcall pipputc(struct dentry *devptr, char ch) {
    int32 mask = disable();
    pipid32 devpipe = devptr->dvnum;
    if(isbadpipe(devpipe)){
      return SYSERR;
    }
    pipe_t pipe = pipe_table[devpipe];

    wait(full_sem);

    head = (head+1)%PIPE_SIZE;
    data[head] = ch;

    signal(empty_sem);

    enable(mask);
    return OK;
}
