all: server client script

server: project_server_2.c
	gcc -o server project_server_2.c -lpthread

client: project_client.c
	gcc -o client project_client.c

script: run_tests.sh
	chmod +x run_tests.sh

clean:
	rm -f server
	rm -f client