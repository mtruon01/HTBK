#ifndef FRONTEND_XML_H
#define FRONTEND_XML_H
#include <string>
#include "user.h"
#include "book.h"
#include "genre.h"
#include "my_enum.h"
#include "backend_postgresql.h"

class Frontend_XML {
public:
	Frontend_XML();
	~Frontend_XML();


	/** Pass in an User object to verify if the user existed and password is correct with the Database
	  *	
	  *	Return successful with Username and Password or Error in XML format
	 **/
	std::string verifyUser( User &user );


	/** Pass in an User object to add user to the Database
	  *	
	  *	Return successful with user information, inserted user_id or Error in XML format
	 **/
	std::string addUser( User &user );


	/** Pass in a Book object to add book to the Database
	  *	
	  *	Return successful with book information, inserted book_id or Error in XML format
	 **/
	std::string addBook( Book &book );


	/** Pass in a Genre object to add book genre to the Database
	  *	
	  *	Return successful with genre information, inserted genre_id or Error in XML format
	 **/
	std::string addBookGenre( Genre &genre );


	/** Pass in a Reserve object to add reserve to the Database
	  *	
	  *	Return successful with reserve information, inserted reserve_id or Error in XML format
	 **/
	std::string addReserve( Reserve &reserve );


	/** Pass in a Review object to add review to the Database
	  *	
	  *	Return successful with review information, inserted review_id or Error in XML format
	 **/
	std::string addReview( Review &review );


	/** Pass in an User object to edit user information and update the Database
	  *	
	  *	Return successful with updated user information or Error in XML format
	 **/
	std::string editUser( User &user );


	/** Pass in a Book object to edit book information and update the Database
	  *	
	  *	Return successful with updated book information or Error in XML format
	 **/
	std::string editBook( Book &book );


	/** Pass in an User object to delete user from the Database
	  *	
	  *	Return successful or Error in XML format
	 **/
	std::string deleteUser( User &user );


	/** Pass in a Book object to delete book from the Database
	  *	
	  *	Return successful or Error in XML format
	 **/
	std::string deleteBook( Book &book );


	/** Pass in a Review object to delete review from the Database
	  *	
	  *	Return successful or Error in XML format
	 **/
	std::string deleteReview( Review &review );


	/** Pass in a Reserve object to delete reserve from the Database
	  *	
	  *	Return successful or Error in XML format
	 **/
	std::string deleteReserve( const int &reserve_id );


	/** Pass in a book_id object to delete book genre from the Database
	  *	
	  *	Return successful or Error in XML format
	 **/
	std::string deleteBookGenre( const int &book_id );


	/** Pass in a History object to delete from borrow and insert to history in the Database
	  *	
	  *	Return successful or Error in XML format
	 **/
	std::string checkIn( History &history );


	/** Pass in a borrow object to delete from reserve and insert to borrow in the Database
	  *	
	  *	Return successful or Error in XML format
	 **/
	std::string checkOut( Borrow &borrow );


	/** Pass in a Review object to approve a review in the Database
	  *	
	  *	Return successful or Error in XML format
	 **/
	std::string approveReview( Review &review );


	/** Pass in an User object to get user information from the Database
	  *	
	  *	Return successful with all user information or Error in XML format
	**/
	std::string getUserInfo( User &user );


	/** Pass in a Book object to get book information from the Database
	  *	
	  *	Return successful with all book information or Error in XML format
	**/
	std::string getOneBook( Book &book );


	/** Pass in vector of genres and book_id to get one book's genre from the Database
	  *	
	  *	Return successful with all book information with genre or Error in XML format
	**/
	std::string getOneBookGenre( const int &book_id, std::vector<Genre*> &genres );


	/** Pass in vector of users and vector of books, user_id or book_id or non to get history of borrowed books from the Database
	  *	
	  *	Return successful with all book information and user information of the borrowed books or Error in XML format
	**/
	std::string getHistory( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books );


	/** Pass in vector of users and vector of, user_id or book_id or non to get reserved books from the Database
	  *	
	  *	Return successful with all book information and user information of the reserved books or Error in XML format
	**/
	std::string getReserved( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books );


	/** Pass in vector of users and vector of books, user_id or book_id or non to get reviews of books from the Database
	  *	
	  *	Return successful with all book information, user information and review of the books or Error in XML format
	**/
	std::string getReview( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books );


	/** Pass in vector of users and vector of books, user_id or book_id or non to get borrow list of books from the Database
	  *	
	  *	Return successful with all book information, user information of the borrowed books or Error in XML format
	**/
	std::string getBorrow( const int &user_id, const int &book_id, std::vector<User*> &users,  std::vector<Book*> &books );


	/** Pass in vector of users, keyword or non to get list of users from the Database
	  *	
	  *	Return successful with all user(s) information or Error in XML format
	**/
	std::string getUsersByKey( const std::string &keyword, std::vector<User*> &users );


	/** Pass in vector of users and vector of genres keyword or non to get list of books with genres from the Database
	  *	
	  *	Return successful with all book(s) information or Error in XML format
	**/
	std::string getSearchResult( const std::string &keyword, std::vector<Book*> &books, std::vector<Genre*> &genres );


	/** Pass in vector of genres to get list of available genres from the Database
	  *	
	  *	Return successful with all genres information or Error in XML format
	**/
	std::string getGenre( std::vector<Genre*> &genres );


	/** Pass in a string of message for building the error xml code
	  *	
	  *	Return error xml codes
	**/
	static std::string xml_error( const std::string &msg );


	/** Pass in a string of message for building the success xml code
	  *	
	  *	Return success xml codes
	**/
	static std::string xml_success( const std::string &msg );

private:
	ERR_CODE error_type;
	Backend_PostgreSQL *backend;
};

#endif
