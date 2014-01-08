df=read.table('ipc.txt')
names(df)=c("time","value")
e=read.table('events')
names(e)=c("value")
events=e$value
t=df$time-df$time[1]
events=events-df$time[1]

pdf("/home/spica/work/ece590/figures/phases.pdf",width=9,height=5)
op=par(mar=par("mar")/1.3)
plot(t/1000000,df$value,pch='.',xlab="Time (S)",main="",cex.lab=2,cex=2,cex.axis=2)
rect(events/1000000,0,events/1000000+1,1.5,col="#00002222",border=F)
dev.off()
