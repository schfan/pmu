df=read.table('/home/spica/work/vexpress/log/ipc/bbench-ipc-a7.txt')
names(df)=c("value")
png("/home/spica/work/vexpress/plot/ipc/bbench_game_a7.png")
plot(seq(1:length(df$value))/10000,df$value,pch='.',xlab="Time (S)",ylab="IPC",main="IPC BBench A7")
dev.off()
