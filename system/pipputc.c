#include <xinu.h>

devcall pipputc(struct dentry *devptr, char ch) {
    int32 mask = disable();
    pipid32 devpipe = devptr->dvnum;
    if(isbadpipe(devpipe)){
      return SYSERR;
    }
    pipe_t pipe = pipe_table[devpipe];

    wait(pipe.to_write);

    pipe.head = (pipe.head+1)%PIPE_SIZE;
    pipe.data[pipe.head] = ch;

    signal(pipe.to_read);

    enable(mask);
    return OK;
}
