#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>				/* for getenv */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>		/* Internet domain header */
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

#include "backend_postgresql.h"
#include "frontend_xml.h"
#include "book.h"
#include "user.h"
#include "review.h"
#include "libpq-fe.h"
#include "history.h"
#include "genre.h"

//---------------------------------------------------------------------------------//
//------------------------------- CONSTANTS ---------------------------------------//
//---------------------------------------------------------------------------------//

#define SERVER_PORT	33334
#define DB_HOST "127.0.0.1"
#define DB_PORT 5432
#define DB_NAME "hchen08"
#define DB_USER "hchen08"
#define DB_PASS "herman285"

const char HEX2DEC[256] =  {
	/*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
	/* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

	/* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

	/* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

	/* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};

//---------------------------------------------------------------------------------//
//------------------------------- MISC FUNCTIONS-----------------------------------//
//---------------------------------------------------------------------------------//

void stringSplit( string str, const string& delim, vector<string>& results ) {
	results.clear();
	size_t cutAt;
	while( (cutAt = str.find_first_of(delim)) != str.npos )
	{
		if (cutAt > 0)
		{
			results.push_back(str.substr(0,cutAt));
		}
		str = str.substr(cutAt+1);
	}
	if(str.length() > 0)
	{
		results.push_back(str);
	}
}

string urlDecode(const string & sSrc) {
	// Note from RFC1630: "Sequences which start with a percent
	// sign but are not followed by two hexadecimal characters
	// (0-9, A-F) are reserved for future extension"

	const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
	const int SRC_LEN = sSrc.length();
	const unsigned char * const SRC_END = pSrc + SRC_LEN;
	// last decodable '%' 
	const unsigned char * const SRC_LAST_DEC = SRC_END - 2;

	char * const pStart = new char[SRC_LEN];
	char * pEnd = pStart;

	while (pSrc < SRC_LAST_DEC)
	{
		if (*pSrc == '%')
		{
			char dec1, dec2;
			if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
				&& -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
			{
				*pEnd++ = (dec1 << 4) + dec2;
				pSrc += 3;
				continue;
			}
		}

		*pEnd++ = *pSrc++;
	}

	// the last 2- chars
	while (pSrc < SRC_END)
		*pEnd++ = *pSrc++;

	std::string sResult(pStart, pEnd);
	delete [] pStart;
	replace(sResult.begin(), sResult.end(), '+', ' ');
	return sResult;
}

string getValueFromQuery( const string& queryStr, const string& item ) {
	string query = urlDecode( queryStr );
	vector<string> pairs, onePair;
	stringSplit( query, "&", pairs );
	for( size_t i = 0; i < pairs.size(); i++ )
	{
		stringSplit( pairs[i], "=", onePair );
		if ( onePair[0] == item && onePair.size() == 2 )
		{
			return urlDecode( onePair[1] );
		}
	}
	return "";
}

bool found( const string& str, const string& item ){
	if ( str.find( item ) != string::npos ) {
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------//
//------------------------------- OTHER FUNCTIONS----------------------------------//
//---------------------------------------------------------------------------------//

string getOneStrFromClient( int ns ) {
	char buf[500];
	int k;

	//cout << "START READING\n";
	k = read(ns, buf, sizeof(buf) - 1);
	//cout << "STOP READING\n";
	//cout << "K: " << k;
	buf[k] = '\0';	

	//Convert to string
	string str( buf );  
	//cout << str << endl;
	return str;
}

string getStrFromClient( int ns ) {

	string str;
	str = getOneStrFromClient( ns );

	while ( !found( str, "\0" ) ) {

		str += getOneStrFromClient( ns );
	}
	return str;
}

void sendToClient( int ns, string str )
{
	write( ns, str.c_str() , strlen( str.c_str() ) );
}


string parseCommand( string fromClientString, Frontend_XML *frontend )
{
	string command,query,action;
	vector<string> stringVector;

	//cout << urlDecode( fromClientString ) << endl;

	stringSplit( fromClientString, "+", stringVector );

	if ( stringVector.size() < 3 || stringVector[0] != "COMMAND" )
	{
		return "FAILED";
	}

	else if ( stringVector[0] == "CHECK" )
	{
		return "OK";
	}

	command = stringVector[1];

	if ( command == "GET" )
	{
		action = stringVector[2];
		query = stringVector[3];

		if ( action == "USER" )
		{
			string user_id = getValueFromQuery( query, "user_id" );
			User u( atoi(user_id.c_str()) );
			return frontend->getUserInfo( u );
		}

		else if ( action == "BOOK" )
		{
			string isbn = getValueFromQuery( query, "isbn" );
			Book b(atoi(getValueFromQuery( query, "book_id" ).c_str()));
			b.isbn = isbn;
			return frontend->getOneBook( b );
		}

		else if ( action == "VERIFYUSER" )
		{
			string username = getValueFromQuery( query, "username" );
			string password = getValueFromQuery( query, "password" );
			User u( username.c_str(), password.c_str() );
			cout << u.username << u.password << endl;
			return frontend->verifyUser( u );
		}
		else if ( action == "GENRE" )
		{
			vector<Genre*> genres;
			int book_id = atoi(getValueFromQuery( query, "book_id" ).c_str());
			return frontend->getOneBookGenre( book_id, genres );
		}
	}

	else if ( command == "INSERT" )
	{
		action = stringVector[2];
		query = stringVector[3];

		if ( action == "USER" )
		{
			User u;
			u.username = getValueFromQuery( query, "username" );
			u.password = getValueFromQuery( query, "password" );
			u.firstname = getValueFromQuery( query, "firstname" );
			u.lastname = getValueFromQuery( query, "lastname" );
			u.group_id = atoi( getValueFromQuery( query, "group_id" ).c_str() );
			u.email = getValueFromQuery( query, "email" );
			u.phone = getValueFromQuery( query, "phone" );
			u.address = getValueFromQuery( query, "address" );
			return frontend->addUser( u );
		}

		else if ( action == "BOOK" )
		{
			Book b;
			b.title = getValueFromQuery( query, "title" );
			b.author = getValueFromQuery( query, "author" );
			b.isbn = getValueFromQuery( query, "isbn" );
			b.quantity = atoi( getValueFromQuery( query, "quantity" ).c_str() );
			b.price = atof( getValueFromQuery( query, "price" ).c_str() );
			//cout << b.title << " " << b.author << " " << b.isbn << " " << b.quantity << " " << b.price << endl;

			return frontend->addBook( b );
		}

		else if ( action == "RESERVE" )
		{
			string user_id = getValueFromQuery( query, "user_id" );
			string book_id = getValueFromQuery( query, "book_id" );
			Reserve r( atoi( user_id.c_str()) , atoi( book_id.c_str()));
			cout << r.user_id << r.book_id << endl;

			return frontend->addReserve( r );
		}
		else if ( action == "REVIEW" )
		{
			int user_id =  atoi( getValueFromQuery( query, "user_id" ).c_str());
			int book_id =  atoi( getValueFromQuery( query, "book_id" ).c_str());
			string reviewString = getValueFromQuery( query, "review" );
			int rating =  atoi( getValueFromQuery( query, "rating" ).c_str());
			//cout << urlDecode( query ) << endl;
			Review review( user_id, book_id, reviewString.c_str(), rating );
			//cout << review.user_id << " " << review.book_id << " " << review.review << " " << review.rating << endl;

			return frontend->addReview( review );
		}

		else if ( action == "GENRE" )
		{
			int book_id =  atoi( getValueFromQuery( query, "book_id" ).c_str());
			int genre_name_id =  atoi( getValueFromQuery( query, "genre_name_id" ).c_str());

			Genre g( book_id, genre_name_id );

			return frontend->addBookGenre( g );
		}
	}
	else if ( command == "UPDATE" )
	{
		action = stringVector[2];
		query = stringVector[3];

		if ( action == "BOOK" )
		{
			Book b;
			b.id		= atoi( getValueFromQuery( query, "book_id" ).c_str() );
			b.title		= getValueFromQuery( query, "title" );
			b.author	= getValueFromQuery( query, "author" );
			b.isbn		= getValueFromQuery( query, "isbn" );
			b.quantity	= atoi( getValueFromQuery( query, "quantity" ).c_str() );
			b.price		= atoi( getValueFromQuery( query, "price" ).c_str() );

			return frontend->editBook( b );
		}
		else if ( action == "APPROVE_REVIEW" )
		{
			Review review = ( atoi(getValueFromQuery( query, "review_id" ).c_str()) );

			return frontend->approveReview( review );
		}
		else if ( action == "USER" )
		{
			User user		= ( atoi(getValueFromQuery( query, "user_id" ).c_str()) );
			user.username	= getValueFromQuery( query, "username" );
			user.password	= getValueFromQuery( query, "password" );
			user.group_id	= atoi( getValueFromQuery( query, "group_id" ).c_str() );
			user.firstname	= getValueFromQuery( query, "firstname" );
			user.lastname	= getValueFromQuery( query, "lastname" );
			user.email		= getValueFromQuery( query, "email" );
			user.address	= getValueFromQuery( query, "address" );
			user.phone		= getValueFromQuery( query, "phone" );

			return frontend->editUser( user );
		}
	}
	else if ( command == "LOOKUP" )
	{
		action = stringVector[2];
		query = stringVector[3];

		if ( action == "RESERVED" )
		{
			vector<User*> users;
			vector<Book*> books;
			if( query == "ALL" )
				return frontend->getReserved( 0, 0, users, books );
			int user_id =  atoi( getValueFromQuery( query, "user_id" ).c_str());
			int book_id =  atoi( getValueFromQuery( query, "book_id" ).c_str());

			return frontend->getReserved( user_id, book_id, users, books );
		}

		else if ( action == "BORROWED" )
		{
			vector<User*> users;
			vector<Book*> books;
			if( query == "ALL" )
				return frontend->getBorrow( 0, 0, users, books );
			int user_id =  atoi( getValueFromQuery( query, "user_id" ).c_str());
			int book_id =  atoi( getValueFromQuery( query, "book_id" ).c_str());

			return frontend->getBorrow( user_id, book_id, users, books );
		}

		else if ( action == "HISTORY" )
		{
			vector<User*> users;
			vector<Book*> books;
			if( query == "ALL" )
				return frontend->getHistory( 0, 0, users, books );
			int user_id =  atoi( getValueFromQuery( query, "user_id" ).c_str());
			int book_id =  atoi( getValueFromQuery( query, "book_id" ).c_str());

			return frontend->getHistory( user_id, book_id, users, books );
		}

		else if ( action == "REVIEWED" )
		{
			vector<User*> users;
			vector<Book*> books;
			if( query == "ALL" )
				return frontend->getReview( 0, 0, users, books );
			int user_id =  atoi( getValueFromQuery( query, "user_id" ).c_str());
			int book_id =  atoi( getValueFromQuery( query, "book_id" ).c_str());

			return frontend->getReview( user_id, book_id, users, books );
		}
		else if ( action == "BOOK" )
		{
			vector<Book*> books;
			vector<Genre*> genres;

			string keyword =  getValueFromQuery( query, "keyword" );
			//int book_id =  atoi( getValueFromQuery( query, "book_id" ).c_str());

			return frontend->getSearchResult( keyword, books, genres );
		}
		else if ( action == "GENRE" )
		{
			vector<Genre*> genres;
			if( query == "ALL" ) {
				return frontend->getGenre( genres );
			}
		}
		else if ( action == "USER" )
		{
			vector<User*> users;

			string keyword =  getValueFromQuery( query, "keyword" );

			return frontend->getUsersByKey( keyword, users );
		}

	}

	else if ( command == "CHECKIN" )
	{
		query = stringVector[2];
		int user_id =  atoi( getValueFromQuery( query, "user_id" ).c_str());
		int book_id =  atoi( getValueFromQuery( query, "book_id" ).c_str());

		History h( user_id, book_id ); 
		//cout << h.user_id << " " << h.book_id << endl;
		return frontend->checkIn( h );
		//check in remove from borrow insert in history
		//check out delete from reserve insert into borrow
	}

	else if ( command == "CHECKOUT" )
	{
		query = stringVector[2];
		int user_id =  atoi( getValueFromQuery( query, "user_id" ).c_str());
		int book_id =  atoi( getValueFromQuery( query, "book_id" ).c_str());

		Borrow b( user_id, book_id );
		//cout << b.user_id << " " << b.book_id << " " << endl;
		return frontend->checkOut( b );
		//check in remove from borrow insert in history
		//check out delete from reserve insert into borrow
	}
	else if ( command == "DELETE" )
	{
		action = stringVector[2];
		query = stringVector[3];

		if ( action == "BOOK" )
		{
			Book b( atoi( getValueFromQuery( query, "book_id" ).c_str()) );
			//cout << b.id << endl;
			return frontend->deleteBook( b );
		}
		else if ( action == "USER" )
		{
			User u( atoi( getValueFromQuery( query, "user_id" ).c_str()) );

			return frontend->deleteUser( u );
		}
		else if ( action == "REVIEW" )
		{
			Review r( atoi( getValueFromQuery( query, "reviewed_id" ).c_str()) );

			return frontend->deleteReview( r );
		}
		else if ( action == "RESERVE" )
		{
			int reserve_id = atoi( getValueFromQuery( query, "reserved_id" ).c_str());

			return frontend->deleteReserve( reserve_id );
		}
		else if ( action == "BOOK_GENRE" )
		{
			int book_id = atoi( getValueFromQuery( query, "book_id" ).c_str());

			return frontend->deleteBookGenre( book_id );
		}
	}

	//delete backend;
	//delete frontend;
	return Frontend_XML::xml_error( "Bad Request!" );
}

void* oneClientWork( void* pns ) {

	int ns = *((int*) pns);

	cout << endl << endl << "====== OPEN THREAD [ pthread_self(): " << pthread_self() << " ] [ NS: " << ns  << " ] ======" << endl;

	string clientMsg, sendMsg;

	// FROM CLIENT
	clientMsg = getStrFromClient(  ns );
	cout<< "Client: " << clientMsg << endl;

	//Backend_PostgreSQL *backend = new Backend_PostgreSQL(DB_NAME, DB_HOST, DB_PORT, DB_USER, DB_PASS);
	Frontend_XML *frontend = new Frontend_XML();
	string xml = parseCommand( clientMsg, frontend );
	//delete backend;
	delete frontend;

	//TO CLIENT
	sendMsg = "This message is from server. You said: ";
	sendMsg += clientMsg;
	sendMsg = xml;
	sendToClient( ns, sendMsg );
	cout << endl << "Server: " << sendMsg << endl;

	close(ns);  //Close client

	cout << "====== CLOSE THREAD [ pthread_self(): " << pthread_self() << " ] [ NS: " << ns << " ] ======" << endl;
	return NULL;
}

//---------------------------------------------------------------------------------//
//------------------------------- SERVER OBJECT -----------------------------------//
//---------------------------------------------------------------------------------//

class Server
{
public:

	Server( int port ) : server_port( port ) {};

	int soc;
	int server_port;
	struct sockaddr_in self;
	struct sockaddr_in peer;
	socklen_t peer_len;

	string getIP() {
		char Buf [ 200 ] ;
		struct hostent * Host;
		gethostname ( Buf , 200 ) ;
		Host = ( struct hostent * ) gethostbyname ( Buf ) ;
		string ip = inet_ntoa(*((struct in_addr *)Host->h_addr));
		return ip;
	}

	int getPort()
	{
		return server_port;
	}

	void initServer()
	{
		int val = 1;
		self.sin_family = AF_INET;
		self.sin_port = htons( this->server_port );
		peer.sin_family = AF_INET;  
		peer_len = sizeof(peer);


		// Set up listening socket soc
		soc = socket(AF_INET, SOCK_STREAM, 0);

		// Exit if error
		if (soc < 0) {	perror("server:socket"); exit(1); }  

		// Exit if not reusable
		if ( setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &val, sizeof val) != 0 ) { perror("server:setsockopt"); exit(1); }

		// Exit if not blindable
		if (bind(soc, (struct sockaddr *)&self, sizeof(self)) == -1) 
		{	perror("server:bind"); close(soc); exit(1); }

		//Start listening
		listen(soc, 2000);

		cout << "STARTING SERVER:\n";
		cout << "My IP is: " << this->getIP() << ":" << this->getPort() <<  endl;

	}

	void run()
	{
		this->initServer();

		while ( 1 )
		{
			pthread_t tid;
			int* ns, *rc;

			// Accept new connection
			ns = new int ( accept( this->soc, (struct sockaddr *)&this->peer, &this->peer_len) );

			// Create thread
			rc = new int ( pthread_create( &tid, NULL, &oneClientWork, (void *)ns ) );
			if ( *rc != 0 ) {
				cout << "ERRRRRRRRRRRRRRRRRRRRRRRR " << *rc << endl;
			}

			pthread_detach( tid );
			delete rc;

		}
		close( soc ); //Close server
	}
};

int main()
{
	Server server( SERVER_PORT );
	server.run();
	exit(0);
}
