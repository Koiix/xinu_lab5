#include <xinu.h>

uint32 pipread(struct dentry *devptr, char* buf, uint32 len) {
	int32 mask = disable();
	if(isbadpipe(devptr->dvnum) || strlen(buf) < len){
		return SYSERR;
	}
	pipe_t pipe = pipe_table[devptr->dvminor];
	if(pipe.state != CONNECTED || pipe.reader!=currpid){
		return SYSERR;
	}
	wait(r_lock);

	int32 numread;
	char ch;

	for(numread = 0; numread < len; i++){
		if((ch = pipgetc())==SYSERR){
			signal(r_lock);
			enable(mask);
			return SYSERR;
		}
		*(buf+numread) = ch;
	}

	signal(r_lock);
	enable(mask);
	return i;
}
