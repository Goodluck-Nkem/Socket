#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>

#define SERVER_PORT 50000
#define BUFFSIZE 2048

char play, response[BUFFSIZE];
char server_address[32];

int main(void)
{
	/* Enter Server Address, without trailing line from fgets(), Ctrl+D selects 127.0.0.1 */
	printf("Enter Server Address: ");
	if(fgets(server_address, sizeof(server_address), stdin))
		server_address[strlen(server_address) - 1] = 0;
	else
		strcpy(server_address, "127.0.0.1");

	struct sockaddr_in addr = {.sin_addr.s_addr = inet_addr(server_address),
		.sin_family = AF_INET,
		.sin_port = htons(SERVER_PORT)};
    int br, addrlen = sizeof(addr), conn_sock = socket(AF_INET, SOCK_STREAM, 0);

    int error, flag = fcntl(conn_sock, F_GETFL, 0); /* get current flags */
    fcntl(conn_sock, F_SETFL, flag | O_NONBLOCK);   /* make sure it is a non-blocking socket */
    while ((error = connect(conn_sock, (const struct sockaddr *)&addr, addrlen)))
    {
        sleep(2);
        puts("Retrying connection!");
    }
    printf("Connected to Server at %s:%i...\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    fcntl(conn_sock, F_SETFL, flag & ~O_NONBLOCK); /* make sure it is a blocking socket */

    while (1)
    {
        // puts("\n<<Next loop>>\n");
        /* receive characters, look for "ETX" */
        int recv_done = 0;
        while (!recv_done)
        {
            br = recv(conn_sock, response, BUFFSIZE, 0);
            if (0x03 == response[br - 1])
            {
                br--;
                recv_done = 1;
            }
            write(STDOUT_FILENO, response, br);
        }

        /* send response */
        fgets(response, BUFFSIZE, stdin);
        play = toupper(response[0]);
        send(conn_sock, &play, 1, 0);
        if ('0' == play || 'E' == play)
            break;
    }

    close(conn_sock);
    puts("Game Ends!");
    return 0;
}
