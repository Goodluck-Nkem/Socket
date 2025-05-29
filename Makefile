all: server client

server:
	gcc -g tcp_simple_server.c -o tcp_simple_server.prog

client:
	gcc -g tcp_client.c -o tcp_client.prog

clean:
	rm -f ${wildcard *.prog}
	