#ifndef _LINUX_SYSCALL_H
#define _LINUX_SYSCALL_H

#include "syscall_arch.h"

#define LINUX_PROT_READ 0x1
#define LINUX_PROT_WRITE 0x2
#define LINUX_PROT_EXEC 0x4

#define LINUX_MAP_ANONYMOUS 0x20
#define LINUX_MAP_PRIVATE 0x2

#define LINUX_O_RDONLY 00
#define LINUX_AT_FDCWD -100

// networking defines
#define LINUX_AF_INET 2
#define LINUX_SOCK_STREAM 1

#ifndef __scc
#define __scc(X) ((long)(X))
typedef long syscall_arg_t;
#endif

#define __syscall1(n, a) __syscall1(n, __scc(a))
#define __syscall2(n, a, b) __syscall2(n, __scc(a), __scc(b))
#define __syscall3(n, a, b, c) __syscall3(n, __scc(a), __scc(b), __scc(c))
#define __syscall4(n, a, b, c, d)                                              \
  __syscall4(n, __scc(a), __scc(b), __scc(c), __scc(d))
#define __syscall5(n, a, b, c, d, e)                                           \
  __syscall5(n, __scc(a), __scc(b), __scc(c), __scc(d), __scc(e))
#define __syscall6(n, a, b, c, d, e, f)                                        \
  __syscall6(n, __scc(a), __scc(b), __scc(c), __scc(d), __scc(e), __scc(f))
#define __syscall7(n, a, b, c, d, e, f, g)                                     \
  __syscall7(n, __scc(a), __scc(b), __scc(c), __scc(d), __scc(e), __scc(f),    \
             __scc(g))

#define __SYSCALL_NARGS_X(a, b, c, d, e, f, g, h, n, ...) n
#define __SYSCALL_NARGS(...)                                                   \
  __SYSCALL_NARGS_X(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0, )
#define __SYSCALL_CONCAT_X(a, b) a##b
#define __SYSCALL_CONCAT(a, b) __SYSCALL_CONCAT_X(a, b)
#define __SYSCALL_DISP(b, ...)                                                 \
  __SYSCALL_CONCAT(b, __SYSCALL_NARGS(__VA_ARGS__))(__VA_ARGS__)

#define __syscall(...) __SYSCALL_DISP(__syscall, __VA_ARGS__)

static inline int linux_socket(int domain, int type, int protocol) {
  return __syscall(SYS_socket, domain, type, protocol);
}

struct in_addr {
    uint32_t s_addr;
};

struct sockaddr_in {
    unsigned short sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
};

static inline int linux_connect(int socketfd, const struct sockaddr_in* addr, int addrlen){
  return __syscall(SYS_connect, socketfd, addr, addrlen);
}

static inline ptrdiff_t linux_send(int socketfd, const void* buf, size_t len, int flags){
  return __syscall(SYS_sendto, socketfd, buf, len, flags, NULL, 0);
}

static inline ptrdiff_t linux_recv(int socketfd, void* buf, size_t len, int flags){
  return __syscall(SYS_recvfrom, socketfd, buf, len, flags, NULL, NULL);
}

static inline int linux_close(int fd){
  return __syscall(SYS_close, fd);
}

#endif
