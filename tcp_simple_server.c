#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define PORT 50000
#define ADDRESS "127.0.0.1"
#define BUFFSIZE 1024

char rules1[] = "Welcome to Rock, Paper, Scissors.\n";
char rules2[] = "Enter R, P or S as your option.\n";

char invalid[] = "Invalid Option, ";

char prompt[] = "\x1B[32mChoose your Play:\x1B[0m \x3"; /* color text, indicate end with "ETX" */

char play_map[] = {'R', 'P', 'S'};
char* object_map[] = {"Rock.\n", "Paper.\n", "Scissor.\n"};

char RBS[] = "Rock Beat Scissor, ";
char PBR[] = "Paper Beat Rock, ";
char SBP[] = "Scissor Beat Paper, ";
char computer_tells[] = "Computer Plays ";

char you_win[] = "You Win!\n";
char com_wins[] = "Computer Wins!\n";
char tie[] = "We have a tie, ";
char retry[] = "Retry!\n";

char response[BUFFSIZE];

int main(void)
{
    srand(time(NULL));
    struct sockaddr_in addr = {.sin_addr.s_addr = inet_addr(ADDRESS), .sin_family = AF_INET, .sin_port = htons(PORT)};
    int addrlen = sizeof(addr), server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt, bw, br, error = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    error = bind(server_fd, (const struct sockaddr *)&addr, addrlen);
    error = listen(server_fd, 5);
    printf("Server listening on %s:%i...\n", ADDRESS, PORT);
    int client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
    printf("Accepted Client at %s:%i...\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    bw = send(client_fd, rules1, sizeof(rules1) - 1, 0);
    printf("%i bytes sent!\n", bw);
    bw = send(client_fd, rules2, sizeof(rules2) - 1, 0);
    printf("%i bytes sent!\n", bw);
    int running = 1;
    while (running)
    {
        bw = send(client_fd, prompt, sizeof(prompt) - 1, 0);
        printf("%i bytes sent!\n", bw);
        br = recv(client_fd, response, BUFFSIZE, 0);
        printf("%i bytes received!\n", br);
        
        int com_key = rand() % 3;
        bw = send(client_fd, computer_tells, sizeof(computer_tells) - 1, 0);
        bw += send(client_fd, object_map[com_key], strlen(object_map[com_key]), 0);
        printf("%i bytes sent!\n", bw);

        char your_play = toupper(response[0]);
        char com_play = play_map[com_key];
        switch (your_play)
        {
        case 'R':
            /* Rock Beat Scissor, Paper Beat Rock */
            if ('S' == com_play)
            {
                /* Player Wins */
                bw = send(client_fd, RBS, sizeof(RBS) - 1, 0);
                bw += send(client_fd, you_win, sizeof(you_win) - 1, 0);
                printf("%i bytes sent!\n", bw);
            }
            else if ('P' == com_play)
            {
                /* Computer Wins */
                bw = send(client_fd, PBR, sizeof(PBR) - 1, 0);
                bw += send(client_fd, com_wins, sizeof(com_wins) - 1, 0);
                printf("%i bytes sent!\n", bw);
            }
            else
            {
                /* We have a tie! */
                bw = send(client_fd, tie, sizeof(tie) - 1, 0);
                bw += send(client_fd, retry, sizeof(retry) - 1, 0);
                printf("%i bytes sent!\n", bw);
            }
            break;

        case 'P':
            /* Paper Beat Rock, Scissor Beat Paper */
            if ('R' == com_play)
            {
                /* Player Wins */
                bw = send(client_fd, PBR, sizeof(PBR) - 1, 0);
                bw += send(client_fd, you_win, sizeof(you_win) - 1, 0);
                printf("%i bytes sent!\n", bw);
            }
            else if ('S' == com_play)
            {
                /* Computer Wins */
                bw = send(client_fd, SBP, sizeof(SBP) - 1, 0);
                bw += send(client_fd, com_wins, sizeof(com_wins) - 1, 0);
                printf("%i bytes sent!\n", bw);
            }
            else
            {
                /* We have a tie! */
                bw = send(client_fd, tie, sizeof(tie) - 1, 0);
                bw += send(client_fd, retry, sizeof(retry) - 1, 0);
                printf("%i bytes sent!\n", bw);
            }
            break;

        case 'S':
            /* Scissor Beat Paper, Rock Beat Scissor */
            if ('P' == com_play)
            {
                /* Player Wins */
                bw = send(client_fd, SBP, sizeof(SBP) - 1, 0);
                bw += send(client_fd, you_win, sizeof(you_win) - 1, 0);
                printf("%i bytes sent!\n", bw);
            }
            else if ('R' == com_play)
            {
                /* Computer Wins */
                bw = send(client_fd, RBS, sizeof(RBS) - 1, 0);
                bw += send(client_fd, com_wins, sizeof(com_wins) - 1, 0);
                printf("%i bytes sent!\n", bw);
            }
            else
            {
                /* We have a tie! */
                bw = send(client_fd, tie, sizeof(tie) - 1, 0);
                bw += send(client_fd, retry, sizeof(retry) - 1, 0);
                printf("%i bytes sent!\n", bw);
            }
            break;

        case '0':
        case 'E':
            running = 0;
            break;

        default:
            bw = send(client_fd, invalid, sizeof(invalid) - 1, 0);
            bw += send(client_fd, retry, sizeof(retry) - 1, 0);
            printf("%i bytes sent!\n", bw);
            break;
        }
    }

    close(server_fd);
    close(client_fd);
    puts("Game Ends!");
    return 0;
}
