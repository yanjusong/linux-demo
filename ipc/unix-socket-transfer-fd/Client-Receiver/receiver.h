#ifndef RECEIVER_H
#define RECEIVER_H

#include <unistd.h>

/*
 * Receive a file descriptor from a server process.  Also, any data
 * received is passed to (*userfunc)(STDERR_FILENO, buf, nbytes).
 * We have a 2-byte protocol for receiving the fd from send_fd().
 */
int recv_fd(int fd, ssize_t (*userfunc)(int, const void *, size_t));

#endif // RECEIVER_H
