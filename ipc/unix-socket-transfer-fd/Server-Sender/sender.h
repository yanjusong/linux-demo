#ifndef SENDER_H
#define SENDER_H

/*
 * Pass a file descriptor to another process.
 * If fd<0, then -fd is sent back instead as the error status.
 */
int send_fd(int fd, int fd_to_send);

#endif // SENDER_H
