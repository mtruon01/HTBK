all: clean PostgreSQLConnector

PostgreSQLConnector: backend_postgresql.o frontend_xml.o
	g++ -Wall -I/usr/include/postgresql/ -lpq backend_postgresql.o frontend_xml.o main.cpp -o PostgreSQLConnector
	
frontend_xml.o:
	g++ -Wall -c -I/usr/include/postgresql/ -o frontend_xml.o frontend_xml.cpp
	
backend_postgresql.o:
	g++ -Wall -c -I/usr/include/postgresql/ -o backend_postgresql.o backend_postgresql.cpp
	
clean:
	rm -rf *.o PostgreSQLConnector
