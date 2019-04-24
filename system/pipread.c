#include <xinu.h>

uint32 pipread(struct dentry *devptr, char* buf, uint32 len) {
	int32 mask = disable();
	if(isbadpipe(devptr->dvnum) || strlen(buf) < len){
		return SYSERR;
	}
	struct pipe_t pipe = pipe_table[devptr->dvminor];

	//process must be the reader
	if(pipe.reader!=currpid)
		return SYSERR;
	if((pipe.state != CONNECTED && pipe.state != PIPE_SEMICONNECTED)){
		return SYSERR;
	}

	int32 numread;

	for(numread = 0; numread < len; i++){

		wait(pipe.to_read);
		pipe.tail = (pipe.tail+1)%PIPE_SIZE;
    *(buf+numread) = pipe.data[pipe.tail];
		signal(pipe.to_write);

		if(no_data(pipe)){
			if(pipe.state == PIPE_SEMICONNECTED){
				pipdisconnect(devptr->dvnum);
			}
			break;
		}
	}

	enable(mask);
	return numread;
}
