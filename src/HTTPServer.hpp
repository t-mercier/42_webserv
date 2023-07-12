#pragma once
#include <sys/poll.h>

class HTTPServer {
public:
  HTTPServer();
  void run();

private:
};


/**
 * @brief wait for some event on a file descriptor
 *
 * @param  {struct*} pollfd : array of pollfd struct
 * @param  {nfds_t} nfds    : number of pollfd struct in the array
 * @param  {int} timeout    : max interval to wait for a poll event, in ms
 * @return {int}
 */

int poll(struct pollfd *fds, nfds_t nfds, int timeout);


