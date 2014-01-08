df=read.table('ipc.txt')
names(df)=c("time","value")
t=(df$time-df$time[1])/1000000

pdf("/home/spica/work/ece590/figures/phases_zoomin.pdf",width=9,height=5)
op=par(mar=par("mar")/1.3)
plot(t[t>7.0&t<7.2],df$value[t>7.0&t<7.2],type='s',xlab="Time (S)",main="",cex.lab=2,cex=2,cex.axis=2,ylab="",col="dimgray")
dev.off()
