#include <xinu.h>

did32 pipcreate() {
    int32 mask = disable();
    /* Traverse the list of pipe devices to find one free for use */
    for(int32 minor = 0; minor < MAX_PIPES; minor++){
      if(pipe_tables[minor].pipe_state_t == PIPE_FREE){
        /* Pipe is free for use, so we take the pipe */
        struct pipe_t * pipeptr = &pipe_tables[minor];
        /* find pipe device entry with same dvminor as index in pipe table */
        pipeptr->pipid = PIPELINE0 + minor;
        pipeptr->pipe_state_t = PIPE_USED;
        pipeptr->owner = currpid;
        return pipeptr->pipid;
      }
    }
    enable(mask);
    return SYSERR;
}
