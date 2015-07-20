#ifndef BACKEND_POSTGRESQL_H
#define BACKEND_POSTGRESQL_H

//#include <string>
#include <vector>
#include "book.h"
#include "borrow.h"
#include "reserve.h"
#include "review.h"
#include "history.h"
#include "user.h"
#include "genre.h"
#include "my_enum.h"
#include "libpq-fe.h"

class Backend_PostgreSQL {
public:
	Backend_PostgreSQL( const char *db, const char* host, const unsigned port,
		const char *user, const char *pass );
	~Backend_PostgreSQL();


	/**
	  *	Return true if connection to Database is ok otherwise false
	 **/
	bool connectDB();

	/**
	  *	Checks if the connection to the Database is still ok return true if okie return false otherwise
	 **/
	bool checkConnection();

	/**
	  *	Close the connection to the Database
	 **/
	void closeConn();
	
	/** Pass in an User object to verify if the user existed and password is correct with the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE verifyUser( User &user );

	/** Pass in an User object to add user to the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE addUser( User &user );

	/** Pass in a Book object to add book to the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE addBook( Book &book );

	/** Pass in a Genre object to add book genre to the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE addBookGenre( Genre &genre );

	/** Pass in a Reserve object to add reserve to the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE addReserve( Reserve &reserve );

	/** Pass in a Review object to add review to the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE addReview( Review &review );

	/** Pass in an User object to edit user information and update the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE editUser( User &user );

	/** Pass in a Book object to edit book information and update the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE editBook( Book &book );

	/** Pass in an User object to delete user from the Database
	  *	
	  * Return error code to the Frontend
	 **/
	ERR_CODE deleteUser( User &user );

	/** Pass in a Book object to delete book from the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE deleteBook( Book &book );

	/** Pass in a Review object to delete review from the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE deleteReview( Review &review );

	/** Pass in a Reserve object to delete reserve from the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE deleteReserve( const int &reserve_id );

	/** Pass in a book_id object to delete book genre from the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE deleteBookGenre( const int &book_id );

	/** Pass in a History object to delete from borrow and insert to history in the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE checkIn( History &history );

	/** Pass in a borrow object to delete from reserve and insert to borrow in the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE checkOut( Borrow &borrow );

	/** Pass in a Review object to approve a review in the Database
	  *	
	  *	Return error code to the Frontend
	 **/
	ERR_CODE approveReview( Review &review );

	/** Pass in an User object to get user information from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getUserInfo( User &user );

	/** Pass in a Book object to get book information from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getOneBook( Book &book );

	/** Pass in vector of genres and book_id to get one book's genre from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getOneBookGenre( const int &book_id, std::vector<Genre*> &genres );

	/** Pass in vector of users and vector of books, user_id or book_id or non to get history of borrowed books from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getHistory( const int &user_id, const int &book_id, std::vector<User*> &user, std::vector<Book*> &bookk );

	/** Pass in vector of users and vector of, user_id or book_id or non to get reserved books from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getReserved( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books );

	/** Pass in vector of users and vector of books, user_id or book_id or non to get reviews of books from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getReview( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books );

	/** Pass in vector of users and vector of books, user_id or book_id or non to get borrow list of books from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getBorrow( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books );

	/** Pass in vector of users, keyword or non to get list of users from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getUsersByKey( const std::string &keyword, std::vector<User*> &users );

	/** Pass in vector of users and vector of genres keyword or non to get list of books with genres from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getSearchResult( const std::string &keyword, std::vector<Book*> &books, std::vector<Genre*> &genres );

	/** Pass in vector of genres to get list of available genres from the Database
	  *	
	  *	Return error code to the Frontend
	**/
	ERR_CODE getGenre( std::vector<Genre*> &genres );

	/** Pass in query string, and num of tuples to be assigned to
	  *	
	  *	Return result from the query and update num of tuples
	**/
	PGresult *selectQuery( const std::string &query, int &ntuples );

private:
	std::string p_db;
	std::string p_host;
	unsigned p_port;
	std::string p_user;
	std::string p_pass;
	PGconn *p_conn;
};

#endif
