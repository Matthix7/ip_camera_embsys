
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;


#define PORT  12800

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1
#define PIN1  24 /* P1-18 */
#define PIN2  23 /* P1-16 */

#define BUF_SIZE	1024
