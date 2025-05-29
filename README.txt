For both unix and Windows

Server
------
Create a TCP Socket
Set Socket Option for Immediate reuse
Bind with a Port
Listen for connection
Accept a connection, get Client fd
send and recv (or read and write)
Server Prompts, Client Responds

Client
------
Create a socket
Connect to a Server, get Server fd
send and recv (or read and write)

A good client should handle when server to refuses connection, or when server is not active

Observation
-----------
sends are fused together.
I noticed server abruptly ended after client closed socket and a write was attempted, I shall investigate this later

Task
----
A simple Rock, Paper, Scissor Game with Scores.
Rock beat Scissor.
Scissor beat Paper.
Paper beat Rock.

Later may add:
- Multiple Client via User Login
- Highscore
- sqlite to store above plus num_login, num_plays, play_details