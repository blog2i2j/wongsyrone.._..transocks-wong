//
// Created by wong on 10/24/18.
//

#ifndef TRANSOCKS_WONG_UTIL_H
#define TRANSOCKS_WONG_UTIL_H

#include "mem-allocator.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h> /* inet_pton, inet_ntop */
#include <sys/types.h>
#include <sys/socket.h>  /* socket(), bind(),listen(), accept(),getsockopt() */
#include <linux/if.h>                         /* For IFNAMSIZ. Silly Travis CI Ubuntu kernel */
#include <linux/netfilter_ipv4.h>             /* SO_ORIGINAL_DST */
#include <linux/netfilter_ipv6/ip6_tables.h>  /* IP6T_SO_ORIGINAL_DST */

#include <event2/util.h>


#if defined(__GNUC__) || defined(__clang__)
#define TRANSOCKS_ATTR(s) __attribute__((s))
#else
#define TRANSOCKS_ATTR(s)
#endif

#define TRANSOCKS_ALWAYS_INLINE TRANSOCKS_ATTR(always_inline) inline


#ifndef SO_ORIGINAL_DST
#warning using custom SO_ORIGINAL_DST
#define SO_ORIGINAL_DST  80
#endif

#ifndef IP6T_SO_ORIGINAL_DST
#warning using custom IP6T_SO_ORIGINAL_DST
#define IP6T_SO_ORIGINAL_DST  80
#endif

#define TRANSOCKS_INET_PORTSTRLEN               (5 + 1)
/* '[' + INET6_ADDRSTRLEN + ']' + ':' + "65535" + NUL */
#define TRANSOCKS_INET_ADDRPORTSTRLEN           (1 + INET6_ADDRSTRLEN + 1 + 1 + TRANSOCKS_INET_PORTSTRLEN + 1)

#define TRANSOCKS_SIZEOF_ARRAY(arr)             (sizeof(arr) / sizeof(arr[0]))
#define TRANSOCKS_FOREACH(ptr, array)           for (ptr = array; ptr < array + TRANSOCKS_SIZEOF_ARRAY(array); ptr++)
#define TRANSOCKS_FOREACH_REVERSE(ptr, array)   for (ptr = array + TRANSOCKS_SIZEOF_ARRAY(array) - 1; ptr >= array; ptr--)
#define TRANSOCKS_UNUSED(obj)                   ((void)(obj))

#define TRANSOCKS_CHKBIT(val, flag)             (((val) & (flag)) == (flag))
#define TRANSOCKS_SETBIT(val, flag)             ((val) |= (flag))
#define TRANSOCKS_CLRBIT(val, flag)             ((val) &= ~(flag))
#define TRANSOCKS_TOGGLEBIT(val, flag)          ((val) ^= (flag))
#define TRANSOCKS_BUFSIZE                       (4096)
#define TRANSOCKS_IS_RETRIABLE(err)             ((err) == EAGAIN || (err) == EWOULDBLOCK || (err) == EINTR)

#define TRANSOCKS_FREE(free_fn, ptr)     \
    do {                                 \
        if ((ptr) != NULL) {             \
            free_fn(ptr);                \
            (ptr) = NULL;                \
        }                                \
    } while (0)

#define TRANSOCKS_CLOSE(fd)                 \
    do {                                    \
        if ((fd) >= 0) {                    \
            close(fd);  \
            (fd) = -1;                      \
        }                                   \
    } while (0)

#define TRANSOCKS_SHUTDOWN(fd, how)             shutdown(fd, how)

#define TRANSOCKS_IS_INVALID_FD(fd) ((fd) == -1)

enum {
    GETOPT_VAL_LISTENERADDRPORT,
    GETOPT_VAL_SOCKS5ADDRPORT,
    GETOPT_VAL_PUMPMETHOD,
    GETOPT_VAL_HELP
};

void generate_sockaddr_port_str(char *, size_t, const struct sockaddr *, socklen_t);

int apply_tcp_keepalive(int);

int apply_ipv6only(int, int);

int apply_tcp_nodelay(int);

int createpipe(int *readfd, int *writefd);

int setnonblocking(int, bool);

int getorigdst(int, struct sockaddr_storage *, socklen_t *);

bool validateAddrPort(struct sockaddr_storage *);

int transocks_parse_sockaddr_port(const char *str, struct sockaddr *sa, socklen_t *actualSockAddrLen);

void print_help(void);

#endif //TRANSOCKS_WONG_UTIL_H
