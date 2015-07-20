//#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "backend_postgresql.h"
#include "my_enum.h"
#include "book.h"
#include "user.h"
#include "genre.h"
//#include "libpq-fe.h"

#define APPROVE 1;
#define FAILED_QUERY -1;

static bool is_number( const std::string &s );

Backend_PostgreSQL::Backend_PostgreSQL( const char *db, const char* host, unsigned port,
	const char *user, const char *pass )
	: p_db(db), p_host(host), p_port(port), p_user(user), p_pass(pass), p_conn(NULL)
{
	connectDB();
}

Backend_PostgreSQL::~Backend_PostgreSQL() {
	closeConn();
}

/* Establish connection to database */
bool Backend_PostgreSQL::connectDB(){
	std::stringstream ss;
	ss 	<< "user="		<< p_user
		<< " password="	<< p_pass
		<< " dbname="	<< p_db
		<< " hostaddr="	<< p_host
		<< " port="		<< p_port;

	// Make a connection to the database
	p_conn = PQconnectdb(ss.str().c_str());

	// Check to see that the backend connection was successfully made
	if(PQstatus(p_conn) != CONNECTION_OK) {
		printf("Connection to database failed\n");
		return false;
		closeConn();
	}
	return true;
}

bool Backend_PostgreSQL::checkConnection() {
	if( PQstatus(p_conn) != CONNECTION_OK ) {
		closeConn();
		return connectDB();
	}
	return true;
}

void Backend_PostgreSQL::closeConn() {
	PQfinish(p_conn);
}

ERR_CODE Backend_PostgreSQL::verifyUser( User &user ){
	if( !user.username.empty() && !user.password.empty() && checkConnection() ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_users\" WHERE "
			<< "username='"			<< user.username
			<< "' AND password='"	<< user.password
			<< "'";
		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) {
			PQclear( res );
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			user.id = atoi(PQgetvalue(res, 0, 0));
			PQclear(res);
			/* possible send to frontend saying verified */
			return QUERY_PASS;
		}
		PQclear(res);
		return QUERY_FAIL;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::addUser( User &user ){
	if( !is_number(user.username) && !user.username.empty() && !user.password.empty() && checkConnection() ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_users\" WHERE "
			<< "username='"			<< user.username
			<< "' AND firstname='"	<< user.firstname
			<< "' AND lastname='"	<< user.lastname
			<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear( res );
		if( nTuples < 0 ) {
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			/* Send user existed to Frontend */
			return EXISTED_IN_DB;
		}
		else {
			ss.str("");
			ss << "INSERT INTO \"HTBK_users\"(username, password, firstname,"
				<< "lastname, group_id, email, phone, address)"		<< " VALUES ('"
				<< user.username	<< "', '"
				<< user.password	<< "', '"
				<< user.firstname	<< "', '"
				<< user.lastname	<< "', '"
				<< user.group_id	<< "', '"
				<< user.email		<< "', '"
				<< user.phone		<< "', '"
				<< user.address
				<< "') RETURNING id";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				printf("INSERT user record failed");
				printf("%s", PQerrorMessage( p_conn) );
				PQclear(res);
				return QUERY_FAIL;
			}
			if( PQntuples(res) >=  1 ) {
				user.id = atoi(PQgetvalue(res, 0, 0));
				PQclear(res);
				/* Send User created successful to Frontend */
				return QUERY_PASS;
			}
			PQclear(res);
		}
		return QUERY_FAIL;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::addBook( Book &book ){
	if( !book.title.empty() && checkConnection() ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_books\" WHERE "
			<< "isbn='"	<< book.isbn
			<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear( res );
		if( nTuples < 0 ) {
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			/* Send Book existed to Frontend */
			return EXISTED_IN_DB;
		}
		else {
			ss.str("");
			ss << "INSERT INTO \"HTBK_books\"(title, author, isbn,"
				<< "quantity, price)"	<< " VALUES ('"
				<< book.title			<< "', '"
				<< book.author	<< "', '"
				<< book.isbn			<< "', '"
				<< book.quantity		<< "', '"
				<< book.price			
				<< "') RETURNING id, title, author, isbn, quantity, price";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				printf("INSERT Book record failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			if( PQntuples(res) >=  1 ) {
				book.id			= atoi(PQgetvalue(res, 0, 0));
				book.title		= PQgetvalue(res, 0, 1);
				book.author		= PQgetvalue(res, 0, 2);
				book.isbn			= PQgetvalue(res, 0, 3);
				book.quantity		= atoi(PQgetvalue(res, 0, 4));
				book.price		= atof(PQgetvalue(res, 0, 5));
				PQclear(res);
				/* Send Book created successful to Frontend */
				return QUERY_PASS;
			}
			PQclear(res);
		}
		return QUERY_FAIL;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::addBookGenre( Genre &genre ) {
	if( checkConnection() && genre.genre_name_id > 0 && genre.book_id > 0 ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_genre\" WHERE "
			<< "book_id='"	<< genre.book_id
			<< "' AND genre_name_id='"  << genre.genre_name_id
			<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) {
			PQclear( res );
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			PQclear(res);
			/* Send Book genre existed to Frontend */
			return EXISTED_IN_DB;
		}
		else {
			PQclear(res);
			ss.str("");
			ss << "INSERT INTO \"HTBK_genre\"(book_id, genre_name_id)"
				<< " VALUES ('"
				<< genre.book_id       << "', '"
				<< genre.genre_name_id
				<< "') RETURNING id";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				printf("INSERT Book genre record failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			if( PQntuples(res) >=  1 ) {
				genre.id    = atoi(PQgetvalue(res, 0, 0));
				PQclear(res);
				/* Send Book genre created successful to Frontend */
				return QUERY_PASS;
			}
			PQclear(res);
		}
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::addReserve( Reserve &reserve ) {
	//INSERT INTO "HTBK_reserve"( user_id, book_id ) VALUES( 3, 1 )
	if( checkConnection() && reserve.user_id > 0 && reserve.book_id > 0 ) {
		std::stringstream ss;
		ss << "SELECT user_id, book_id FROM \"HTBK_reserve\" WHERE user_id=' "
			<< reserve.user_id	<< "' AND book_id=' "
			<< reserve.book_id	<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear(res);
		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			/* Send Reserve Book existed to Frontend */
			return EXISTED_IN_DB;
		}
		else {
			ss.str("");
			ss << "INSERT INTO \"HTBK_reserve\"(user_id, book_id)"
				<< " VALUES ('"
				<< reserve.user_id	<< "', '"
				<< reserve.book_id	
				<< "') RETURNING id, user_id, book_id, reserved_date";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				printf("INSERT reserve record failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			if( PQntuples(res) >=  1 ) {
				reserve.id				= atoi(PQgetvalue(res, 0, 0));
				reserve.user_id			= atoi(PQgetvalue(res, 0, 1));
				reserve.book_id			= atoi(PQgetvalue(res, 0, 2));
				reserve.reserved_date	= PQgetvalue(res, 0, 3);
				PQclear(res);
				/* Send Reserve Book created successful to Frontend */
				return QUERY_PASS;
			}
			PQclear(res);
		}
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::addReview( Review &review ) {
	//INSERT INTO "HTBK_review"( user_id, book_id, rating, review ) VALUES( 3, 1, 1, 'sucks' )
	if( checkConnection() ) {
		std::stringstream ss;
		ss << "SELECT user_id, book_id FROM \"HTBK_review\" WHERE "
			<< "user_id=' "			<< review.user_id
			<< "' AND book_id=' "	<< review.book_id
			<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear(res);
		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			/* Send Review Book existed to Frontend */
			return EXISTED_IN_DB;
		}
		else {
			ss.str("");
			ss << "INSERT INTO \"HTBK_review\"(user_id, book_id, rating, review)"
				<< " VALUES ('"		
				<< review.user_id	<< "', '"
				<< review.book_id	<< "', '"
				<< review.rating	<< "', '"
				<< review.review
				<< "') RETURNING id, user_id, book_id, rating, review, reviewed_date";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				printf("INSERT Review record failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			if( PQntuples(res) >=  1 ) {
				review.id			= atoi(PQgetvalue(res, 0, 0));
				review.user_id		= atoi(PQgetvalue(res, 0, 1));
				review.book_id		= atoi(PQgetvalue(res, 0, 2));
				review.rating		= atoi(PQgetvalue(res, 0, 3));
				review.review		= PQgetvalue(res, 0, 4);
				review.reviewed_date= PQgetvalue(res, 0, 5);
				PQclear(res);
				/* Send Review Book created successful to Frontend */
				return QUERY_PASS;
			}
			PQclear(res);
		}
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::editUser( User &user ) {
	//UPDATE "HTBK_users" SET author='1000' WHERE id='1'
	if( checkConnection() ) {
		if( user.group_id < 0 || user.firstname.empty() || user.lastname.empty() ||
			user.email.empty() || user.phone.empty() || user.address.empty() ) {
				user.display();
				return FIELD_NOT_ENTERED;
		}
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_users\" WHERE "
			<< "id='"	<< user.id	<< "'";
		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		PQclear(res);

		if( nTuples < 0 ) {
			return QUERY_FAIL;
		}
		else if( nTuples >= 1 ) {
			ss.str("");
			ss << "UPDATE \"HTBK_users\" "
				<< "SET firstname ='"	<< user.firstname
				<< "', lastname='" 		<< user.lastname;
			if( !user.password.empty() ) {
				ss << "', password='" 		<< user.password;
			}
			ss << "', group_id='" 		<< user.group_id
				<< "', email='" 		<< user.email
				<< "', phone='" 		<< user.phone
				<< "', address='" 		<< user.address
				<< "' WHERE id='"		<< user.id
				<< "' RETURNING id, firstname, lastname, username, group_id, register_date, email, address, phone";
			res = selectQuery( ss.str(), nTuples );

			if( nTuples < 0 ) {
				PQclear( res );
				return QUERY_FAIL;
			}

			else if( nTuples >= 1 ) {
				user.id				= atoi(PQgetvalue(res, 0, 0));
				user.firstname		= PQgetvalue(res, 0, 1);
				user.lastname		= PQgetvalue(res, 0, 2);
				user.username		= PQgetvalue(res, 0, 3);
				user.group_id       = atoi(PQgetvalue(res, 0, 4));
				user.register_date  = PQgetvalue(res, 0, 5);
				user.email			= PQgetvalue(res, 0, 6);
				user.address		= PQgetvalue(res, 0, 7);
				user.phone			= PQgetvalue(res, 0, 8);

				PQclear(res);
				return QUERY_PASS;
			}
		}
		else return NOT_EXISTED_IN_DB;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::editBook( Book &book ){
	//UPDATE "HTBK_books" SET author='1000' WHERE id='1'
	if( checkConnection() && book.id > 0 ) {
		if( book.title.empty() || book.author.empty() || book.isbn.empty() || book.quantity <= 0 || book.price <= 0 )
			return FIELD_NOT_ENTERED;
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_books\" WHERE "
			<< "id='"	<< book.id	<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear(res);
		if( nTuples < 0 ) {
			return QUERY_FAIL;
		}
		else if( nTuples >= 1 ) {
			ss.str("");
			ss << "UPDATE \"HTBK_books\" "
				<< "SET title ='"	<< book.title
				<< "', author='" 	<< book.author
				<< "', isbn='" 		<< book.isbn
				<< "', quantity='" 	<< book.quantity
				<< "', price='" 	<< book.price
				<< "' WHERE id='"	<< book.id
				<< "' RETURNING id, title, author, isbn, quantity, price";

			res = selectQuery( ss.str(), nTuples );

			if( nTuples < 0 ) {
				PQclear( res );
				return QUERY_FAIL;
			}

			else if( nTuples >= 1 ) {
				book.id			= atoi(PQgetvalue(res, 0, 0));
				book.title		= PQgetvalue(res, 0, 1);
				book.author		= PQgetvalue(res, 0, 2);
				book.isbn		= PQgetvalue(res, 0, 3);
				book.quantity	= atoi(PQgetvalue(res, 0, 4));
				book.price		= atof(PQgetvalue(res, 0, 5));
				PQclear(res);
				return QUERY_PASS;
			}
		}
		else return NOT_EXISTED_IN_DB;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::deleteUser( User &user ){
	//DELETE FROM "HTBK_users" WHERE id='1'
	if( checkConnection() && user.id > 0 ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_users\" WHERE id='"
			<< user.id	<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear( res );
		if( nTuples < 0 ) {
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			ss.str("");
			ss << "DELETE FROM \"HTBK_users\" WHERE id='"
				<< user.id	<< "'";

			PGresult *res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_COMMAND_OK)
			{
				printf("DELETE User failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			PQclear(res);
			/* Send User DELETE successful to Frontend */
			return QUERY_PASS;
		}
		else return NOT_EXISTED_IN_DB;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::deleteBook( Book &book ){
	//DELETE FROM "HTBK_books" WHERE id='1'
	if( checkConnection() && book.id > 0 ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_books\" WHERE id='"
			<< book.id	<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear(res);

		if( nTuples < 0 ) {
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			ss.str("");
			ss << "DELETE FROM \"HTBK_books\" WHERE id='"
				<< book.id	<< "'";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_COMMAND_OK)
			{
				printf("DELETE Book failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			PQclear(res);
			/* Send Book DELETE successful to Frontend */
			return QUERY_PASS;
		}
		else return NOT_EXISTED_IN_DB;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::deleteReview( Review &review ) {
	//DELETE FROM "HTBK_review" WHERE id='1'
	if( checkConnection() && review.id > 0 ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_review\" WHERE id='"
			<< review.id	<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear(res);

		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			ss.str("");
			ss << "DELETE FROM \"HTBK_review\" WHERE id='"
				<< review.id	<< "'";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_COMMAND_OK)
			{
				printf("DELETE Review failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			PQclear(res);
			/* Send Review DELETE successful to Frontend */
			return QUERY_PASS;
		}
		else return NOT_EXISTED_IN_DB;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::deleteReserve( const int &reserve_id ) {
	//DELETE FROM "HTBK_review" WHERE id='1'
	if( checkConnection() && reserve_id > 0 ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_reserve\" WHERE id='"
			<< reserve_id	<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear(res);

		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			ss.str("");
			ss << "DELETE FROM \"HTBK_reserve\" WHERE id='"
				<< reserve_id	<< "'";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_COMMAND_OK)
			{
				printf("DELETE reserve failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			PQclear(res);
			/* Send Reserve DELETE successful to Frontend */
			return QUERY_PASS;
		}
		else return NOT_EXISTED_IN_DB;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::deleteBookGenre( const int &book_id ) {
	//DELETE FROM "HTBK_genre" WHERE book_id='1'
	if( checkConnection() && book_id > 0 ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_genre\" WHERE book_id='"
			<< book_id	<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear(res);

		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			ss.str("");
			ss << "DELETE FROM \"HTBK_genre\" WHERE book_id='"
				<< book_id	<< "'";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_COMMAND_OK)
			{
				printf("DELETE Book Genre failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			PQclear(res);
			/* Send Book Genre DELETE successful to Frontend */
			return QUERY_PASS;
		}
		else return NOT_EXISTED_IN_DB;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::checkIn( History &history ){
	//DELETE FROM "HTBK_borrow" WHERE id='1'
	if( checkConnection() && history.user_id > 0 && history.book_id > 0 ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_borrow\" WHERE user_id='"
			<< history.user_id	<< "' AND book_id='" << history.book_id << "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) {
			PQclear( res );
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			PQclear(res);
			ss.str("");
			ss << "DELETE FROM \"HTBK_borrow\" WHERE user_id='"
				<< history.user_id	<< "' AND book_id='" << history.book_id << "'";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_COMMAND_OK)
			{
				printf("DELETE checkin record failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			PQclear(res);
			ss.str("");
			ss << "INSERT INTO \"HTBK_history\"(user_id, book_id) VALUES ('"
				<< history.user_id		<< "', '"
				<< history.book_id
				<< "') RETURNING id, user_id, book_id, date_returned";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				printf("INSERT History record failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			history.id				= atoi(PQgetvalue(res, 0, 0));
			history.user_id			= atoi(PQgetvalue(res, 0, 1));
			history.book_id			= atoi(PQgetvalue(res, 0, 2));
			history.date_returned	= PQgetvalue(res, 0, 3);
			PQclear(res);
			/* Send Check in created successful to Frontend */
			return QUERY_PASS;
		}
		else return NOT_EXISTED_IN_DB;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::checkOut( Borrow &borrow ){
	//INSERT INTO "HTBK_borrow"(user_id, book_id) VALUES('','')
	//UPDATE "HTBK_books" SET quantity=quantity - 1 WHERE id=1
	if( checkConnection() && borrow.user_id > 0 && borrow.book_id > 0 ) {
		std::stringstream ss;
		ss << "SELECT user_id, book_id FROM \"HTBK_borrow\" WHERE "
			<< "user_id=' "			<< borrow.user_id
			<< "' AND book_id=' "	<< borrow.book_id
			<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear(res);
		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			/* Send Borrow Book existed to Frontend */
			return EXISTED_IN_DB;
		}
		else {
			ss.str("");
			ss << "DELETE FROM \"HTBK_reserve\" WHERE "
				<< "user_id='"			<< borrow.user_id
				<< "' AND book_id='"	<< borrow.book_id << "'";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_COMMAND_OK)
			{
				printf("DELETE Reserve record failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			PQclear(res);
			ss.str("");
			ss << "INSERT INTO \"HTBK_borrow\"(user_id, book_id)"
				<< " VALUES ('"
				<< borrow.user_id	<< "', '"
				<< borrow.book_id
				<< "') RETURNING id, user_id, book_id, borrowed_date";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				printf("INSERT Borrow record failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			borrow.id				= atoi(PQgetvalue(res, 0, 0));
			borrow.user_id			= atoi(PQgetvalue(res, 0, 1));
			borrow.book_id			= atoi(PQgetvalue(res, 0, 2));
			borrow.borrowed_date	= PQgetvalue(res, 0, 3);
			PQclear(res);
			/* Send Borrow Book created successful to Frontend */
			return QUERY_PASS;
		}
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::approveReview( Review &review ) {
	if( checkConnection() && review.id > 0 ) {
		std::stringstream ss;
		ss << "SELECT id FROM \"HTBK_review\" WHERE id='"
			<< review.id	<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		PQclear(res);
		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			ss.str("");
			ss << "UPDATE \"HTBK_review\" "
				<< "SET approve ='"	<<  review.approve
				<< "' WHERE id='"<<  review.id
				<< "' RETURNING id, user_id, book_id, rating, review, reviewed_date, approve";

			res = PQexec(p_conn, ss.str().c_str());

			if(PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				printf("UPDATE Review failed");
				PQclear(res);
				return QUERY_FAIL;
			}
			if( PQntuples(res) >= 1 ) {
				review.id		= atoi(PQgetvalue(res, 0, 0));
				review.user_id	= atoi(PQgetvalue(res, 0, 1));
				review.book_id	= atoi(PQgetvalue(res, 0, 2));
				review.rating	= atoi(PQgetvalue(res, 0, 3));
				review.review	= PQgetvalue(res, 0, 4);
				review.reviewed_date    = PQgetvalue(res, 0, 5);
				review.approve          = atoi(PQgetvalue(res, 0, 6));
				PQclear(res);
				/* Send Review Book created successful to Frontend */
				return QUERY_PASS;
			}
			else return NOT_EXISTED_IN_DB;
		}
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getUserInfo( User &user ){
	if( checkConnection() && (user.id > 0 || !user.username.empty()) ) {
		std::stringstream ss;
		ss << "SELECT id, username, firstname, lastname, group_id, register_date, email, phone, address ";
		if( user.id > 0 )
			ss << "FROM \"HTBK_users\" WHERE id='"<< user.id << "'";
		else
			ss << "FROM \"HTBK_users\" WHERE username='"<< user.username << "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) {
			PQclear( res );
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			user.id				= atoi(PQgetvalue(res, 0, 0));
			user.username		= PQgetvalue(res, 0, 1);
			user.firstname		= PQgetvalue(res, 0, 2);
			user.lastname		= PQgetvalue(res, 0, 3);
			user.group_id		= atoi(PQgetvalue(res, 0, 4));
			user.register_date	= PQgetvalue(res, 0, 5);
			user.email			= PQgetvalue(res, 0, 6);
			user.phone			= PQgetvalue(res, 0, 7);
			user.address		= PQgetvalue(res, 0, 8);
			PQclear(res);
			/* Send getUser successful to Frontend */
			return QUERY_PASS;
		}
		PQclear(res);
		return QUERY_FAIL;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getOneBook( Book &book ){
	//SELECT "HTBK_books".id, title, author, isbn, quantity, price, "HTBK_genre".id, genre_name_id, genre_name
	//FROM "HTBK_books" INNER JOIN "HTBK_genre" ON ("HTBK_books".id = "HTBK_genre".book_id) INNER JOIN "HTBK_genre_groups"
	//ON ( "HTBK_genre".genre_name_id = "HTBK_genre_groups".id )
	if( checkConnection() && (book.id > 0 || !book.isbn.empty()) ) {
		std::stringstream ss;
		ss << "SELECT id, title, author, isbn, quantity, price FROM \"HTBK_books\" ";
		//ss << "SELECT \"HTBK_books\".id AS book_id, title, author, isbn, quantity, price, \"HTBK_genre\".id AS genre_id, genre_name_id, genre_name "
		//	<< "FROM \"HTBK_books\" INNER JOIN \"HTBK_genre\" ON( \"HTBK_books\".id = \"HTBK_genre\".book_id ) "
		//	<< "INNER JOIN \"HTBK_genre_groups\" ON ( \"HTBK_genre\".genre_name_id = \"HTBK_genre_groups\".id ) ";
		if( book.id > 0 )
			ss << "WHERE id='"<< book.id	<< "'";
		else if( !book.isbn.empty() )
			ss << "WHERE isbn='"<< book.isbn	<< "'";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		if( nTuples < 0 ) {
			PQclear( res );
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			book.id			= atoi(PQgetvalue(res, 0, 0));
			book.title		= PQgetvalue(res, 0, 1);
			book.author		= PQgetvalue(res, 0, 2);
			book.isbn		= PQgetvalue(res, 0, 3);
			book.quantity	= atoi(PQgetvalue(res, 0, 4));
			book.price		= atof(PQgetvalue(res, 0, 5));
			PQclear(res);
			return QUERY_PASS;
		}
		PQclear(res);
		return NO_RESULT;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getOneBookGenre( const int &book_id, std::vector<Genre*> &genres ){
	if( checkConnection() && book_id > 0 ) {
		std::stringstream ss;
		//ss << "SELECT id, title, author, isbn, quantity, price FROM \"HTBK_books\" ";
		ss << "SELECT \"HTBK_genre\".id AS genre_id, book_id, genre_name_id, genre_name "
			<< "FROM \"HTBK_genre\" INNER JOIN \"HTBK_genre_groups\" ON ( \"HTBK_genre\".genre_name_id = \"HTBK_genre_groups\".id ) "
			<< "WHERE book_id='"<< book_id	<< "' ORDER BY genre_name";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );
		if( nTuples < 0 ) {
			PQclear( res );
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			for( int i = 0; i < nTuples; i++ ) {
				Genre *oneGenre = new Genre(atoi( PQgetvalue(res, i, 0) ),
					atoi(PQgetvalue(res, i, 1)),
					atoi(PQgetvalue(res, i, 2)),
					PQgetvalue(res, i, 3));

				genres.push_back(oneGenre);
			}
			PQclear(res);
			return QUERY_PASS;
		}
		PQclear(res);
		return NO_RESULT;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getHistory( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books ) {
	//SELECT * FROM \"HTBK_history\" INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_history\".book_id) WHERE user_id=1
	//book_id	date_returned	id	user_id	author	isbn	quantity	id	price	title

	//SELECT username, firstname, lastname, email, title, author, isbn, date_returned FROM "HTBK_history" INNER JOIN "HTBK_books" ON
	//("HTBK_books".id = "HTBK_history".book_id) INNER JOIN "HTBK_users" ON("HTBK_users".id = "HTBK_history".user_id)
	//username	firstname	lastname	email	title	author	date_returned

	//SELECT "HTBK_history".id, user_id, group_id, username, firstname, lastname, register_date, email, phone, address, book_id, title, author, isbn, quantity, price, date_returned FROM "HTBK_history" INNER JOIN "HTBK_books" ON 
	//("HTBK_books".id = "HTBK_history".book_id) INNER JOIN "HTBK_users" ON ("HTBK_users".id = "HTBK_history".user_id)
	if( checkConnection() ) {
		std::stringstream ss;
		if( user_id > 0 ) {
			ss << "SELECT \"HTBK_history\".id, user_id, group_id, username, firstname, lastname, register_date, email, phone, address, "
				<< "book_id, title, author, isbn, quantity, price, date_returned FROM \"HTBK_history\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_history\".book_id)  "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_history\".user_id) "
				<< "WHERE user_id='"	<< user_id		<< "' ORDER BY date_returned DESC";
		}
		else if( book_id > 0 ) {
			ss << "SELECT \"HTBK_history\".id, user_id, group_id, username, firstname, lastname, register_date, email, phone, address, "
				<< "book_id, title, author, isbn, quantity, price, date_returned FROM \"HTBK_history\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_history\".book_id) "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_history\".user_id) "
				<< "WHERE book_id='"	<< book_id	<< "' ORDER BY date_returned DESC";
		}
		else {
			ss << "SELECT \"HTBK_history\".id, user_id, group_id, username, firstname, lastname, register_date, email, phone, address, "
				<< "book_id, title, author, isbn, quantity, price, date_returned FROM \"HTBK_history\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_history\".book_id) "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_history\".user_id) ORDER BY date_returned DESC";
		}

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			for (int i = 0; i < nTuples; i++) {
				History *oneHistory = new History( atoi(PQgetvalue(res, i, 0)),
					atoi(PQgetvalue(res, i, 1)), 
					atoi(PQgetvalue(res, i, 10)),
					PQgetvalue(res, i, 16) );
				User	*oneUser	= new User( atoi(PQgetvalue(res, i, 1)) );
				Book	*oneBook	= new Book( atoi(PQgetvalue(res, i, 10)) );

				oneUser->group_id		= atoi(PQgetvalue(res, i, 2));
				oneUser->username		= PQgetvalue(res, i, 3);
				oneUser->firstname		= PQgetvalue(res, i, 4);
				oneUser->lastname		= PQgetvalue(res, i, 5);
				oneUser->register_date	= PQgetvalue(res, i, 6);
				oneUser->email			= PQgetvalue(res, i, 7);
				oneUser->phone			= PQgetvalue(res, i, 8);
				oneUser->address		= PQgetvalue(res, i, 9);
				oneBook->title			= PQgetvalue(res, i, 11);
				oneBook->author			= PQgetvalue(res, i, 12);
				oneBook->isbn			= PQgetvalue(res, i, 13);
				oneBook->quantity		= atoi(PQgetvalue(res, i, 14));
				oneBook->price			= atof(PQgetvalue(res, i, 15));

				oneUser->history_list.push_back(oneHistory);
				users.push_back(oneUser);
				books.push_back(oneBook);
			}
			PQclear(res);
			return QUERY_PASS;
		}
		PQclear(res);
		return NO_RESULT;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getReserved( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books ) {
	//SELECT * FROM \"HTBK_reserve\" INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_reserve\".book_id) WHERE user_id=1
	/* SELECT * FROM \"HTBK_reserve\" INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_reserve\".book_id) 
	INNER JOIN \"HTBK_users\" ON(\"HTBK_reserve\".user_id = \"HTBK_users\".id) WHERE user_id=3 */
	if( checkConnection() ) {
		std::stringstream ss;
		if( user_id > 0 ) {
			ss << "SELECT \"HTBK_reserve\".id, user_id, group_id, username, firstname, lastname, register_date, "
				<< "email, phone, address, book_id, title, author, isbn, quantity, price, reserved_date FROM \"HTBK_reserve\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_reserve\".book_id)  "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_reserve\".user_id) "
				<< "WHERE user_id='"	<< user_id		<< "' ORDER BY reserved_date DESC";
		}
		else if( book_id > 0 ) {
			ss << "SELECT \"HTBK_reserve\".id, user_id, group_id, username, firstname, lastname, register_date, "
				<< "email, phone, address, book_id, title, author, isbn, quantity, price, reserved_date FROM \"HTBK_reserve\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_reserve\".book_id) "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_reserve\".user_id) "
				<< "WHERE book_id='"	<< book_id	<< "' ORDER BY reserved_date DESC";
		}
		else {
			ss << "SELECT \"HTBK_reserve\".id, user_id, group_id, username, firstname, lastname, register_date, "
				<< "email, phone, address, book_id, title, author, isbn, quantity, price, reserved_date FROM \"HTBK_reserve\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_reserve\".book_id) "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_reserve\".user_id) ORDER BY reserved_date DESC";
		}

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			for (int i = 0; i < nTuples; i++)
			{
				Reserve *oneReserve = new Reserve( atoi(PQgetvalue(res, i, 0)),
					atoi(PQgetvalue(res, i, 1)),
					atoi(PQgetvalue(res, i, 10)),
					PQgetvalue(res, i, 16) );
				User	*oneUser	= new User( atoi(PQgetvalue(res, i, 1)) );
				Book	*oneBook	= new Book( atoi(PQgetvalue(res, i, 10)) );

				oneUser->group_id		= atoi(PQgetvalue(res, i, 2));
				oneUser->username		= PQgetvalue(res, i, 3);
				oneUser->firstname		= PQgetvalue(res, i, 4);
				oneUser->lastname		= PQgetvalue(res, i, 5);
				oneUser->register_date	= PQgetvalue(res, i, 6);
				oneUser->email			= PQgetvalue(res, i, 7);
				oneUser->phone			= PQgetvalue(res, i, 8);
				oneUser->address		= PQgetvalue(res, i, 9);
				oneBook->title			= PQgetvalue(res, i, 11);
				oneBook->author			= PQgetvalue(res, i, 12);
				oneBook->isbn			= PQgetvalue(res, i, 13);
				oneBook->quantity		= atoi(PQgetvalue(res, i, 14));
				oneBook->price			= atof(PQgetvalue(res, i, 15));

				oneUser->reserved_list.push_back(oneReserve);
				users.push_back(oneUser);
				books.push_back(oneBook);
			}
			PQclear(res);
			return QUERY_PASS;
		}
		PQclear(res);
		return NO_RESULT;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getReview( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books ) {
	//SELECT * FROM \"HTBK_review\" INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_review\".book_id) WHERE user_id=1
	if( checkConnection() ) {
		std::stringstream ss;
		if( user_id > 0 ) {
			ss << "SELECT \"HTBK_review\".id, user_id, group_id, username, firstname, lastname, register_date, "
				<< "email, phone, address, book_id, title, author, isbn, quantity, price, "
				<< "review, rating, reviewed_date, approve FROM \"HTBK_review\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_review\".book_id)  "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_review\".user_id) "
				<< "WHERE user_id='"	<< user_id		<< "' ORDER BY reviewed_date DESC";
		}
		else if( book_id > 0 ) {
			ss << "SELECT \"HTBK_review\".id, user_id, group_id, username, firstname, lastname, register_date, "
				<< "email, phone, address, book_id, title, author, isbn, quantity, price, "
				<< "review, rating, reviewed_date, approve FROM \"HTBK_review\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_review\".book_id) "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_review\".user_id) "
				<< "WHERE book_id='"	<< book_id	<< "' ORDER BY reviewed_date DESC";
		}
		else {
			ss << "SELECT \"HTBK_review\".id, user_id, group_id, username, firstname, lastname, register_date, "
				<< "email, phone, address, book_id, title, author, isbn, quantity, price, "
				<< "review, rating, reviewed_date, approve FROM \"HTBK_review\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_review\".book_id) "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_review\".user_id) ORDER BY reviewed_date DESC";
		}

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			for (int i = 0; i < nTuples; i++)
			{
				Review *oneReview = new Review( atoi(PQgetvalue(res, i, 0)),
					atoi(PQgetvalue(res, i, 1)),
					atoi(PQgetvalue(res, i, 10)), 
					PQgetvalue(res, i, 16),
					atoi(PQgetvalue(res, i, 17)),
					PQgetvalue(res, i, 18),
					atoi(PQgetvalue(res, i, 19)) );
				User	*oneUser	= new User( atoi(PQgetvalue(res, i, 1)) );
				Book	*oneBook	= new Book( atoi(PQgetvalue(res, i, 10)) );

				oneUser->group_id		= atoi(PQgetvalue(res, i, 2));
				oneUser->username		= PQgetvalue(res, i, 3);
				oneUser->firstname		= PQgetvalue(res, i, 4);
				oneUser->lastname		= PQgetvalue(res, i, 5);
				oneUser->register_date	= PQgetvalue(res, i, 6);
				oneUser->email			= PQgetvalue(res, i, 7);
				oneUser->phone			= PQgetvalue(res, i, 8);
				oneUser->address		= PQgetvalue(res, i, 9);
				oneBook->title			= PQgetvalue(res, i, 11);
				oneBook->author			= PQgetvalue(res, i, 12);
				oneBook->isbn			= PQgetvalue(res, i, 13);
				oneBook->quantity		= atoi(PQgetvalue(res, i, 14));
				oneBook->price			= atof(PQgetvalue(res, i, 15));

				oneUser->reviewed_list.push_back(oneReview);
				users.push_back(oneUser);
				books.push_back(oneBook);
			}
			PQclear(res);
			return QUERY_PASS;
		}
		PQclear(res);
		return NO_RESULT;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getBorrow( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books ) {
	//SELECT * FROM \"HTBK_borrow\" INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_borrow\".book_id) WHERE user_id =1
	if( checkConnection() ) {
		std::stringstream ss;
		if( user_id > 0 ) {
			ss << "SELECT \"HTBK_borrow\".id, user_id, group_id, username, firstname, lastname, register_date, "
				<< "email, phone, address, book_id, title, author, isbn, quantity, price, borrowed_date, due_date FROM \"HTBK_borrow\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_borrow\".book_id)  "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_borrow\".user_id) "
				<< "WHERE user_id='"	<< user_id		<< "' ORDER BY borrowed_date DESC";
		}
		else if( book_id > 0 ) {
			ss << "SELECT \"HTBK_borrow\".id, user_id, group_id, username, firstname, lastname, register_date, "
				<< "email, phone, address, book_id, title, author, isbn, quantity, price, borrowed_date, due_date FROM \"HTBK_borrow\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_borrow\".book_id) "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_borrow\".user_id) "
				<< "WHERE book_id='"	<< book_id	<< "' ORDER BY borrowed_date DESC";
		}
		else {
			ss << "SELECT \"HTBK_borrow\".id, user_id, group_id, username, firstname, lastname, register_date, "
				<< "email, phone, address, book_id, title, author, isbn, quantity, price, borrowed_date, due_date FROM \"HTBK_borrow\" "
				<< "INNER JOIN \"HTBK_books\" ON (\"HTBK_books\".id = \"HTBK_borrow\".book_id) "
				<< "INNER JOIN \"HTBK_users\" ON(\"HTBK_users\".id = \"HTBK_borrow\".user_id) ORDER BY borrowed_date DESC";
		}

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			for (int i = 0; i < nTuples; i++)
			{
				Borrow *oneBorrow 	= new Borrow( atoi(PQgetvalue(res, i, 0)),
					atoi(PQgetvalue(res, i, 1)),
					atoi(PQgetvalue(res, i, 10)),
					PQgetvalue(res, i, 16),
					PQgetvalue(res, i, 17) );
				User	*oneUser	= new User( atoi(PQgetvalue(res, i, 1)) );
				Book	*oneBook	= new Book( atoi(PQgetvalue(res, i, 10)) );

				oneUser->group_id		= atoi(PQgetvalue(res, i, 2));
				oneUser->username		= PQgetvalue(res, i, 3);
				oneUser->firstname		= PQgetvalue(res, i, 4);
				oneUser->lastname		= PQgetvalue(res, i, 5);
				oneUser->register_date	= PQgetvalue(res, i, 6);
				oneUser->email			= PQgetvalue(res, i, 7);
				oneUser->phone			= PQgetvalue(res, i, 8);
				oneUser->address		= PQgetvalue(res, i, 9);
				oneBook->title			= PQgetvalue(res, i, 11);
				oneBook->author			= PQgetvalue(res, i, 12);
				oneBook->isbn			= PQgetvalue(res, i, 13);
				oneBook->quantity		= atoi(PQgetvalue(res, i, 14));
				oneBook->price			= atof(PQgetvalue(res, i, 15));

				oneUser->borrowed_list.push_back(oneBorrow);
				users.push_back(oneUser);
				books.push_back(oneBook);
			}
			PQclear(res);
			return QUERY_PASS;
		}
		PQclear(res);
		return NO_RESULT;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getUsersByKey( const std::string &keyword, std::vector<User*> &users ) {
	if( checkConnection() ) {
		std::stringstream ss;

		if( keyword.empty() ) {
			ss << "SELECT id, firstname, lastname, username, group_id, email, address, phone "
				<< "FROM \"HTBK_users\" ORDER BY username";
		}
		else {
			ss << "SELECT id, firstname, lastname, username, group_id, email, address, phone "
				<< "FROM \"HTBK_users\""
				<< "WHERE firstname ILIKE '%"		<< keyword << "%'"
				<< " OR lastname ILIKE '%"		<< keyword << "%'"
				<< " OR username ILIKE '%"		<< keyword << "%'"
				<< " OR email ILIKE '%" 	<< keyword << "%'"
				<< " OR address ILIKE '%" 	<< keyword << "%'"
				<< " OR phone ILIKE '%" 	<< keyword << "%' ORDER BY username";
		}

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			for (int i = 0; i < nTuples; i++)
			{
				User    *oneUser    = new User(atoi(PQgetvalue(res, i, 0)));
				oneUser->firstname   = PQgetvalue(res, i, 1);
				oneUser->lastname    = PQgetvalue(res, i, 2);
				oneUser->username    = PQgetvalue(res, i, 3);
				oneUser->group_id    = atoi(PQgetvalue(res, i, 4));
				oneUser->email       = PQgetvalue(res, i, 5);
				oneUser->address     = PQgetvalue(res, i, 6);
				oneUser->phone       = PQgetvalue(res, i, 7);

				users.push_back(oneUser);
			}
			PQclear(res);
			return QUERY_PASS;
		}
		PQclear(res);
		return NO_RESULT;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getSearchResult( const std::string &keyword, std::vector<Book*> &books, std::vector<Genre*> &genres ) {
	//SELECT * FROM "HTBK_books" INNER JOIN "HTBK_genre" ON( "HTBK_books".id = "HTBK_genre".book_id )
	//INNER JOIN "HTBK_genre_groups" ON ( "HTBK_genre".genre_name_id = "HTBK_genre_groups".id )
	//WHERE title ILIKE '%tom%' OR author ILIKE '%tom%' OR isbn ILIKE '%tom%' OR genre_name ILIKE '%tom%'
	if( checkConnection() ) {
		std::stringstream ss;

		if( keyword.empty() ) {
			ss << "SELECT \"HTBK_books\".id, title, author, isbn, quantity, price, \"HTBK_genre\".id, "
				<< "genre_name_id, genre_name FROM \"HTBK_books\" INNER JOIN \"HTBK_genre\" ON( \"HTBK_books\".id = \"HTBK_genre\".book_id ) "
				<< "INNER JOIN \"HTBK_genre_groups\" ON ( \"HTBK_genre\".genre_name_id = \"HTBK_genre_groups\".id ) ORDER BY title";
		}
		else {
			ss << "SELECT \"HTBK_books\".id, title, author, isbn, quantity, price, \"HTBK_genre\".id, "
				<< "genre_name_id, genre_name FROM \"HTBK_books\" INNER JOIN \"HTBK_genre\" ON( \"HTBK_books\".id = \"HTBK_genre\".book_id ) "
				<< "INNER JOIN \"HTBK_genre_groups\" ON ( \"HTBK_genre\".genre_name_id = \"HTBK_genre_groups\".id ) "
				<< "WHERE title ILIKE '%"		<< keyword << "%'"
				<< " OR author ILIKE '%"		<< keyword << "%'"
				<< " OR isbn ILIKE '%"			<< keyword << "%'"
				<< " OR genre_name ILIKE '%" 	<< keyword << "%' ORDER BY title";
		}

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) return QUERY_FAIL;

		else if( nTuples >= 1 ) {
			for (int i = 0; i < nTuples; i++)
			{
				Book	*oneBook	= new Book( atoi(PQgetvalue(res, i, 0)),
					PQgetvalue(res, i, 1),
					PQgetvalue(res, i, 2),
					PQgetvalue(res, i, 3),
					atoi(PQgetvalue(res, i, 4)),
					atoi(PQgetvalue(res, i, 5)) );
				Genre	*oneGenre	= new Genre( atoi(PQgetvalue(res, i, 6)),
					atoi(PQgetvalue(res, i, 0)),
					atoi(PQgetvalue(res, i, 7)),
					PQgetvalue(res, i, 8) );

				books.push_back(oneBook);
				genres.push_back(oneGenre);
			}
			PQclear(res);
			return QUERY_PASS;
		}
		PQclear(res);
		return NO_RESULT;
	}
	return BAD_REQUEST;
}

ERR_CODE Backend_PostgreSQL::getGenre( std::vector<Genre*> &genres ) {
	//SELECT id, genre_name FROM "HTBK_genre_groups"
	if( checkConnection() ) {
		std::stringstream ss;

		ss << "SELECT id, genre_name FROM \"HTBK_genre_groups\" ORDER BY genre_name";

		int nTuples;

		PGresult *res = selectQuery( ss.str(), nTuples );

		if( nTuples < 0 ) {
			PQclear( res );
			return QUERY_FAIL;
		}

		else if( nTuples >= 1 ) {
			for (int i = 0; i < nTuples; i++)
			{
				Genre *oneGenre = new Genre( atoi(PQgetvalue(res, i, 0)), PQgetvalue(res, i, 1) );
				genres.push_back(oneGenre);
			}
			PQclear(res);
			return QUERY_PASS;
		}
		PQclear(res);
		return NO_RESULT;
	}
	return BAD_REQUEST;
}

PGresult *Backend_PostgreSQL::selectQuery( const std::string &query, int &ntuples ) {

	PGresult *res = PQexec( p_conn, query.c_str() );

	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		printf("FETCH ALL failed\n");
		printf("%s\n", PQerrorMessage( p_conn ) );
		PQclear(res);
		ntuples = FAILED_QUERY;
	}
	ntuples = PQntuples(res);
	return res;
}

static bool is_number(const std::string &s) {
	for( size_t i = 0; i < s.length(); ++i )
		if( !isdigit(s[i]) )
			return false;
	return true;
}
