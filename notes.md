##The reactor pattern

is an event handling pattern for handling service requests delivered concurrently to a service handler by one or more inputs. Here, the service handler, being your server, then demultiplexes the incoming requests and dispatches them synchronously to the associated request handlers. We'll use poll() in our reactor implementation, as it is more scalable than select().

      +-------------------+
      |    Reactor        |
      | (main event loop) |
      +---------+---------+
                | 
                |
                | (register handler)
+---------------v---------------+
|         Server (poll)         |
+---------------+---------------+
                | (forward request)
        +-------v------+
         Request Handler
        +--------------+


`FD_ZERO(fd_set *fdset)`: This macro clears (removes all file descriptors from) fdset.
`FD_SET(int fd, fd_set *fdset)`: This macro adds fd to the file descriptor set fdset.
`FD_CLR(int fd, fd_set *fdset)`: This macro removes fd from the file descriptor set fdset.
`FD_ISSET(int fd, fd_set *fdset)`: This macro returns a non-zero value if fd is a member of the file descriptor set fdset, and 0 otherwise.

` poll()` is a function that allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation. In the case of a web server, these file descriptors could represent connections from clients.
Each pollfd structure specifies an array of file descriptors to be examined:
If poll() finds that any of the specified file descriptors is ready for reading,
writing, or has an error condition, it returns.
