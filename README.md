# 目录

## [进程间通信](./ipc)

#### 1.管道

- [1.1管道 -pipe](./ipc/pipe-demo1)

- [1.2管道 -popen](./ipc/pipe-demo2)

- [1.3管道 -双工/重定向/excel](./ipc/pipe-demo3)
- [1.4命名管道 -FIFO](./ipc/fifo-demo1)
  - 只读open要阻塞到某个其他进程为写而打开这个FIFO为止，只写open同理。
  - 若write一个尚无进程为读而打开的FIFO，则产生信号SIGPIPE。若某个FIFO最后的写进程关闭了FIFO，则为该FIFO读进程产生一个文件结束标志。