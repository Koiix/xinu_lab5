#include <xinu.h>

did32 pipcreate() {
    int32 mask = disable();
    /* Traverse the list of pipe devices to find one free for use */
    for(int32 minor = 0; minor < MAXPIPES; minor++){
      if(pipe_table[minor].state == PIPE_FREE){
        /* Pipe is free for use, so we take the pipe */
        struct pipe_t * pipeptr = &pipe_table[minor];
        /* find pipe device entry with same dvminor as index in pipe table */
        pipeptr->pipid = PIPELINE0 + minor;
        pipeptr->pipe_state_t = PIPE_USED;
        pipeptr->owner = currpid;
        return pipeptr->pipid;
      }
    }
    restore(mask);
    return SYSERR;
}
