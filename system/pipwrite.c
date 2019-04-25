#include <xinu.h>

uint32 pipwrite(struct dentry *devptr, char* buf, uint32 len) {
  int32 mask = disable();
  if(isbadpipe(devptr->dvnum) || strlen(buf) < len){
    if(PIP_DEBUG) PIP_ERR("write");
    restore(mask);
    return SYSERR;
  }
  struct pipe_t pipe = pipe_table[devptr->dvminor];

  //caller must be the writer
  if(pipe.writer != currpid){
    if(PIP_DEBUG) PIP_ERR("write");
    restore(mask);
    return SYSERR;
  }

  if(pipe.state != PIPE_CONNECTED){
    if(pipe.state==PIPE_SEMICONNECTED){
      pipdisconnect(devptr->dvnum);
    }
    if(PIP_DEBUG) PIP_ERR("write");
    restore(mask);
    return SYSERR;
  }

  int32 numwrite;

  for(numwrite = 0; numwrite < len; numwrite++){
    pipputc(devptr, *(buf+numwrite));
    //if pipe disconnected after pipputc, then break
    if(pipe.state != PIPE_SEMICONNECTED && pipe.state != PIPE_CONNECTED)
			break;
  }

  if(PIP_DEBUG){
		char write_str[numwrite];
		for(int i = 0; i < numwrite; i++){
			write_str[i] = *(buf+numwrite);
		}
		kprintf("Process: %s finished writing: %s to pipe %d\n", proctab[currpid].prname, write_str, devptr->dvminor);
	}

  restore(mask);
  return numwrite;
}
