#include <string>
#include <sstream>
using namespace std;
#include "user.h"
#include "book.h"
#include "genre.h"
#include "my_enum.h"
#include "xml_writer.h"
#include "frontend_xml.h"


#define DB_HOST "127.0.0.1"
#define DB_PORT 5432
#define DB_NAME "hchen08"
#define DB_USER "hchen08"
#define DB_PASS "herman285"

Frontend_XML::Frontend_XML(){
	backend = new Backend_PostgreSQL(DB_NAME, DB_HOST, DB_PORT, DB_USER, DB_PASS);
}

Frontend_XML::~Frontend_XML() {
	delete backend;
}

string Frontend_XML::verifyUser( User &user ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->verifyUser(user);
	if( error_type == QUERY_PASS ) {
		{
			xml::element user_info("user", xml);
			user_info.attr("id", user.id );
			{
				xml::element username("username", xml);
				username.contents(user.username);
			}
			{
				xml::element password("password", xml);
				password.contents(user.password);
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Username or password is incorrect!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::addUser( User &user ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->addUser(user);
	if( error_type == QUERY_PASS ) {
		{
			xml::element user_info("user", xml);
			user_info.attr("id", user.id );
			{
				xml::element username("username", xml);
				username.contents(user.username);
			}
			{
				xml::element password("password", xml);
				password.contents(user.password);
			}
		}
	}
	else if( error_type == EXISTED_IN_DB ) {
		ss << xml_error("User is taken!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to add user!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::addBook( Book &book ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->addBook(book);
	if( error_type == QUERY_PASS ) {
		{
			xml::element book_info("book", xml);
			book_info.attr("id", book.id );
			{
				xml::element title("title", xml);
				title.contents(book.title);
			}
			{
				xml::element author("author", xml);
				author.contents(book.author);
			}
			{
				xml::element isbn("isbn", xml);
				isbn.contents(book.isbn);
			}
			{
				xml::element quantity("quantity", xml);
				quantity.contents(book.quantity);
			}
			{
				xml::element price("price", xml);
				price.contents(book.price);
			}
		}
	}
	else if( error_type == EXISTED_IN_DB ) {
		ss << xml_error("Book existed in database!");
	}
	else if( error_type == QUERY_FAIL ) {
		{
			xml::element error("error", xml);
			{
				xml::element msg("message", xml);
				msg.contents("Unable to add book!");
			}
		}
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::addBookGenre( Genre &genre ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->addBookGenre(genre);
	if( error_type == QUERY_PASS ) {
		{
			xml::element genre_info("genre", xml);
			genre_info.attr("id", genre.id );
			{
				xml::element book_id("book_id", xml);
				book_id.contents(genre.book_id);
			}
			{
				xml::element genre_name_id("genre_name_id", xml);
				genre_name_id.contents(genre.genre_name_id);
			}
			{
				xml::element genre_name("genre_name", xml);
				genre_name.contents(genre.genre_name);
			}
		}
	}
	else if( error_type == EXISTED_IN_DB ) {
		ss << xml_error("Book already assigned with a genre in database!");
	}
	else if( error_type == QUERY_FAIL ) {
		{
			xml::element error("error", xml);
			{
				xml::element msg("message", xml);
				msg.contents("Unable to add book to genre!");
			}
		}
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::addReserve( Reserve &reserve ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->addReserve(reserve);
	if( error_type == QUERY_PASS ) {
		{
			xml::element reserve_info("reserve", xml);
			reserve_info.attr("id", reserve.id );
			{
				xml::element user_id("user_id", xml);
				user_id.contents(reserve.user_id);
			}
			{
				xml::element book_id("book_id", xml);
				book_id.contents(reserve.book_id);
			}
			{
				xml::element reserved_date("reserved_date", xml);
				reserved_date.contents(reserve.reserved_date);
			}
		}
	}
	else if( error_type == EXISTED_IN_DB ) {
		ss << xml_error("Book already reserved!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to reserve book!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::addReview( Review &review ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->addReview(review);
	if( error_type == QUERY_PASS ) {
		{
			xml::element review_info("review", xml);
			review_info.attr("id", review.id );
			{
				xml::element user_id("user_id", xml);
				user_id.contents(review.user_id);
			}
			{
				xml::element book_id("book_id", xml);
				book_id.contents(review.book_id);
			}
			{
				xml::element rating("rating", xml);
				rating.contents(review.rating);
			}
			{
				xml::element text("review", xml);
				text.contents(review.review);
			}
			{
				xml::element reviewed_date("reviewed_date", xml);
				reviewed_date.contents(review.reviewed_date);
			}
		}
	}
	else if( error_type == EXISTED_IN_DB ) {
		ss << xml_error("Review is already existed!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to review book!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::editUser( User &user ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->editUser(user);
	if( error_type == QUERY_PASS ) {
		{
			xml::element user_info("user", xml);
			user_info.attr("id", user.id );
			{
				xml::element username("username", xml);
				username.contents(user.username);
			}
			{
				xml::element firstname("firstname", xml);
				firstname.contents(user.firstname);
			}
			{
				xml::element lastname("lastname", xml);
				lastname.contents(user.lastname);
			}
			{
				xml::element group_id("group_id", xml);
				group_id.contents(user.group_id);
			}
			{
				xml::element register_date("register_date", xml);
				register_date.contents(user.register_date);
			}
			{
				xml::element email("email", xml);
				email.contents(user.email);
			}
			{
				xml::element phone("phone", xml);
				phone.contents(user.phone);
			}
			{
				xml::element address("address", xml);
				address.contents(user.address);
			}
		}
	}
	else if( error_type == FIELD_NOT_ENTERED ) {
		ss << xml_error("All fields need to be entered!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to edit user!");
	}
	else if( error_type == NOT_EXISTED_IN_DB ) {
		ss << xml_error("No such user to edit!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::editBook( Book &book ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->editBook(book);
	if( error_type == QUERY_PASS ) {
		{
			xml::element book_info("book", xml);
			book_info.attr("id", book.id );
			{
				xml::element title("title", xml);
				title.contents(book.title);
			}
			{
				xml::element author("author", xml);
				author.contents(book.author);
			}
			{
				xml::element isbn("isbn", xml);
				isbn.contents(book.isbn);
			}
			{
				xml::element quantity("quantity", xml);
				quantity.contents(book.quantity);
			}
			{
				xml::element price("price", xml);
				price.contents(book.price);
			}
		}
	}
	else if( error_type == FIELD_NOT_ENTERED ) {
		ss << xml_error("All fields need to be entered!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to edit book!");
	}
	else if( error_type == NOT_EXISTED_IN_DB ) {
		ss << xml_error("No such book to edit!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::deleteUser( User &user ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->deleteUser(user);
	if( error_type == QUERY_PASS ) {
		ss << xml_success("Delete user successful!");
	}
	else if( error_type == NOT_EXISTED_IN_DB ) {
		ss << xml_error("User does not exist!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to delete user!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::deleteBook( Book &book ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->deleteBook(book);
	if( error_type == QUERY_PASS ) {
		ss << xml_success("Delete book successful!");
	}
	else if( error_type == NOT_EXISTED_IN_DB ) {
		ss << xml_error("Book does not exist!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to delete book!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::deleteReview( Review &review ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->deleteReview(review);
	if( error_type == QUERY_PASS ) {
		ss << xml_success("Delete review successful!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to delete review!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::deleteReserve( const int &reserve_id ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->deleteReserve(reserve_id);
	if( error_type == QUERY_PASS ) {
		ss << xml_success("Delete reserve successful!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to delete reserve!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::deleteBookGenre( const int &book_id ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->deleteBookGenre(book_id);
	if( error_type == QUERY_PASS ) {
		ss << xml_success("Delete book genre successful!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to delete book genre!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::checkIn( History &history ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->checkIn(history);
	if( error_type == QUERY_PASS ) {
		{
			ss << xml_success("Check in book successful!");
		}
	}
	else if( error_type == NOT_EXISTED_IN_DB ) {
		ss << xml_error("Book not in the borrow list!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to check in!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::checkOut( Borrow &borrow ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->checkOut(borrow);
	if( error_type == QUERY_PASS ) {
		{
			ss << xml_success("Check out book successful!");
		}
	}
	else if( error_type == EXISTED_IN_DB ) {
		ss << xml_error("Book already borrowed!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to check out!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::approveReview( Review &review ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->approveReview(review);
	if( error_type == QUERY_PASS ) {
		ss << xml_success("Approve review successful!");
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to approve review!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getUserInfo( User &user ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getUserInfo(user);
	if( error_type == QUERY_PASS ) {
		{
			xml::element user_info("user", xml);
			user_info.attr("id", user.id );
			{
				xml::element username("username", xml);
				username.contents(user.username);
			}
			{
				xml::element firstname("firstname", xml);
				firstname.contents(user.firstname);
			}
			{
				xml::element lastname("lastname", xml);
				lastname.contents(user.lastname);
			}
			{
				xml::element group_id("group_id", xml);
				group_id.contents(user.group_id);
			}
			{
				xml::element register_date("register_date", xml);
				register_date.contents(user.register_date);
			}
			{
				xml::element email("email", xml);
				email.contents(user.email);
			}
			{
				xml::element phone("phone", xml);
				phone.contents(user.phone);
			}
			{
				xml::element address("address", xml);
				address.contents(user.address);
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		{
			xml::element error("error", xml);
			{
				xml::element msg("message", xml);
				msg.contents("Unable to get User Info!");
			}
		}
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getOneBook( Book &book ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getOneBook(book);
	if( error_type == QUERY_PASS ) {
		{
			xml::element book_info("book", xml);
			book_info.attr("id", book.id );
			{
				xml::element title("title", xml);
				title.contents(book.title);
			}
			{
				xml::element author("author", xml);
				author.contents(book.author);
			}
			{
				xml::element isbn("isbn", xml);
				isbn.contents(book.isbn);
			}
			{
				xml::element quantity("quantity", xml);
				quantity.contents(book.quantity);
			}
			{
				xml::element price("price", xml);
				price.contents(book.price);
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to get book!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getOneBookGenre( const int &book_id, std::vector<Genre*> &genres ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getOneBookGenre(book_id, genres);
	if( error_type == QUERY_PASS ) {
		{
			xml::element genre_list("genre_list", xml);
			genre_list.attr("type", "array" );
			for( size_t i = 0; i < genres.size(); i++ ) {
				{
					xml::element genre("genre", xml);
					{
						xml::element genre_id("genre_id", xml);
						genre_id.attr("id", genres[i]->id );
					}
					{
						xml::element book_id("book_id", xml);
						book_id.contents(genres[i]->book_id);
					}
					{
						xml::element genre_name_id("genre_name_id", xml);
						genre_name_id.contents(genres[i]->genre_name_id);
					}
					{
						xml::element genre_name("genre_name", xml);
						genre_name.contents(genres[i]->genre_name);
					}
				}
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to get book's genre(s)!");
	}
	else if( error_type == NO_RESULT ) {
		ss << xml_error("Book's genre list is empty!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getHistory( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getHistory(user_id, book_id, users, books);
	if( error_type == QUERY_PASS ) {
		{
			xml::element history_list("history_list", xml);
			history_list.attr("type", "array" );
			for( size_t i = 0; i < users.size(); i++ ) {
				/* Returning below fields from database
				* id
				* user_id
				* group_id
				* username
				* firstname
				* lastname
				* register_date
				* email
				* phone
				* address
				* book_id
				* title
				* author
				* isbn
				* quantity
				* price
				* date_returned
				*/
				{
					xml::element history("history", xml);
					{
						xml::element history_id("history_id", xml);
						history_id.attr("id", users[i]->history_list.front()->id );
					}
					{
						xml::element user_id("user_id", xml);
						user_id.contents(users[i]->id);
					}
					{
						xml::element group_id("group_id", xml);
						group_id.contents(users[i]->group_id);
					}
					{
						xml::element username("username", xml);
						username.contents(users[i]->username);
					}
					{
						xml::element firstname("firstname", xml);
						firstname.contents(users[i]->firstname);
					}
					{
						xml::element lastname("lastname", xml);
						lastname.contents(users[i]->lastname);
					}
					{
						xml::element register_date("register_date", xml);
						register_date.contents(users[i]->register_date);
					}
					{
						xml::element email("email", xml);
						email.contents(users[i]->email);
					}
					{
						xml::element phone("phone", xml);
						phone.contents(users[i]->phone);
					}
					{
						xml::element address("address", xml);
						address.contents(users[i]->address);
					}
					{
						xml::element book_id("book_id", xml);
						book_id.contents(books[i]->id);
					}
					{
						xml::element title("title", xml);
						title.contents(books[i]->title);
					}
					{
						xml::element author("author", xml);
						author.contents(books[i]->author);
					}
					{
						xml::element isbn("isbn", xml);
						isbn.contents(books[i]->isbn);
					}
					{
						xml::element quantity("quantity", xml);
						quantity.contents(books[i]->quantity);
					}
					{
						xml::element price("price", xml);
						price.contents(books[i]->price);
					}
					{
						xml::element returned_date("returned_date", xml);
						returned_date.contents(users[i]->history_list.front()->date_returned);
					}
				}
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to get history list!");
	}
	else if( error_type == NO_RESULT ) {
		ss << xml_error("History list is empty!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getReserved( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getReserved(user_id, book_id, users, books);
	if( error_type == QUERY_PASS ) {
		{
			xml::element reserved_list("reserved_list", xml);
			reserved_list.attr("type", "array" );
			for( size_t i = 0; i < users.size(); i++ ) {
				/* Returning below fields from database
				* id
				* user_id
				* group_id
				* username
				* firstname
				* lastname
				* register_date
				* email
				* phone
				* address
				* book_id
				* title
				* author
				* isbn
				* quantity
				* price
				* reserved_date
				*/
				{
					xml::element reserved("reserved", xml);
					{
						xml::element reserved_id("reserved_id", xml);
						reserved_id.attr("id", users[i]->reserved_list.front()->id );
					}
					{
						xml::element user_id("user_id", xml);
						user_id.contents(users[i]->id);
					}
					{
						xml::element group_id("group_id", xml);
						group_id.contents(users[i]->group_id);
					}
					{
						xml::element username("username", xml);
						username.contents(users[i]->username);
					}
					{
						xml::element firstname("firstname", xml);
						firstname.contents(users[i]->firstname);
					}
					{
						xml::element lastname("lastname", xml);
						lastname.contents(users[i]->lastname);
					}
					{
						xml::element register_date("register_date", xml);
						register_date.contents(users[i]->register_date);
					}
					{
						xml::element email("email", xml);
						email.contents(users[i]->email);
					}
					{
						xml::element phone("phone", xml);
						phone.contents(users[i]->phone);
					}
					{
						xml::element address("address", xml);
						address.contents(users[i]->address);
					}
					{
						xml::element book_id("book_id", xml);
						book_id.contents(books[i]->id);
					}
					{
						xml::element title("title", xml);
						title.contents(books[i]->title);
					}
					{
						xml::element author("author", xml);
						author.contents(books[i]->author);
					}
					{
						xml::element isbn("isbn", xml);
						isbn.contents(books[i]->isbn);
					}
					{
						xml::element quantity("quantity", xml);
						quantity.contents(books[i]->quantity);
					}
					{
						xml::element price("price", xml);
						price.contents(books[i]->price);
					}
					{
						xml::element reserved_date("reserved_date", xml);
						reserved_date.contents(users[i]->reserved_list.front()->reserved_date);
					}
				}
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to get reserve list!");
	}
	else if( error_type == NO_RESULT ) {
		ss << xml_error("Reserve list is empty!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getReview( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getReview(user_id, book_id, users, books);
	if( error_type == QUERY_PASS ) {
		{
			xml::element reviewed_list("reviewed_list", xml);
			reviewed_list.attr("type", "array" );
			for( size_t i = 0; i < users.size(); i++ ) {
				/* Returning below fields from database
				* id
				* user_id
				* group_id
				* username
				* firstname
				* lastname
				* register_date
				* email
				* phone
				* address
				* book_id
				* title
				* author
				* isbn
				* quantity
				* price
				* review, rating, reviewed_date
				*/
				{
					xml::element reviewed("reviewed", xml);
					{
						xml::element reviewed_id("reviewed_id", xml);
						reviewed_id.attr("id", users[i]->reviewed_list.front()->id );
					}
					{
						xml::element user_id("user_id", xml);
						user_id.contents(users[i]->id);
					}
					{
						xml::element group_id("group_id", xml);
						group_id.contents(users[i]->group_id);
					}
					{
						xml::element username("username", xml);
						username.contents(users[i]->username);
					}
					{
						xml::element firstname("firstname", xml);
						firstname.contents(users[i]->firstname);
					}
					{
						xml::element lastname("lastname", xml);
						lastname.contents(users[i]->lastname);
					}
					{
						xml::element register_date("register_date", xml);
						register_date.contents(users[i]->register_date);
					}
					{
						xml::element email("email", xml);
						email.contents(users[i]->email);
					}
					{
						xml::element phone("phone", xml);
						phone.contents(users[i]->phone);
					}
					{
						xml::element address("address", xml);
						address.contents(users[i]->address);
					}
					{
						xml::element book_id("book_id", xml);
						book_id.contents(books[i]->id);
					}
					{
						xml::element title("title", xml);
						title.contents(books[i]->title);
					}
					{
						xml::element author("author", xml);
						author.contents(books[i]->author);
					}
					{
						xml::element isbn("isbn", xml);
						isbn.contents(books[i]->isbn);
					}
					{
						xml::element quantity("quantity", xml);
						quantity.contents(books[i]->quantity);
					}
					{
						xml::element price("price", xml);
						price.contents(books[i]->price);
					}
					{
						xml::element review("review", xml);
						review.contents(users[i]->reviewed_list.front()->review);
					}
					{
						xml::element rating("rating", xml);
						rating.contents(users[i]->reviewed_list.front()->rating);
					}
					{
						xml::element reviewed_date("reviewed_date", xml);
						reviewed_date.contents(users[i]->reviewed_list.front()->reviewed_date);
					}
					{
						xml::element approve("approve", xml);
						approve.contents(users[i]->reviewed_list.front()->approve);
					}
				}
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to get review list!");
	}
	else if( error_type == NO_RESULT ) {
		ss << xml_error("No review is available!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getBorrow( const int &user_id, const int &book_id, std::vector<User*> &users, std::vector<Book*> &books ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getBorrow(user_id, book_id, users, books);
	if( error_type == QUERY_PASS ) {
		{
			xml::element borrowed_list("borrowed_list", xml);
			borrowed_list.attr("type", "array" );
			for( size_t i = 0; i < users.size(); i++ ) {
				/* Returning below fields from database
				* id
				* user_id
				* group_id
				* username
				* firstname
				* lastname
				* register_date
				* email
				* phone
				* address
				* book_id
				* title
				* author
				* isbn
				* quantity
				* price
				* borrowed_date
				*/
				{
					xml::element borrowed("borrowed", xml);
					{
						xml::element borrowed_id("borrowed_id", xml);
						borrowed_id.attr("id", users[i]->borrowed_list.front()->id );
					}
					{
						xml::element user_id("user_id", xml);
						user_id.contents(users[i]->id);
					}
					{
						xml::element group_id("group_id", xml);
						group_id.contents(users[i]->group_id);
					}
					{
						xml::element username("username", xml);
						username.contents(users[i]->username);
					}
					{
						xml::element firstname("firstname", xml);
						firstname.contents(users[i]->firstname);
					}
					{
						xml::element lastname("lastname", xml);
						lastname.contents(users[i]->lastname);
					}
					{
						xml::element register_date("register_date", xml);
						register_date.contents(users[i]->register_date);
					}
					{
						xml::element email("email", xml);
						email.contents(users[i]->email);
					}
					{
						xml::element phone("phone", xml);
						phone.contents(users[i]->phone);
					}
					{
						xml::element address("address", xml);
						address.contents(users[i]->address);
					}
					{
						xml::element book_id("book_id", xml);
						book_id.contents(books[i]->id);
					}
					{
						xml::element title("title", xml);
						title.contents(books[i]->title);
					}
					{
						xml::element author("author", xml);
						author.contents(books[i]->author);
					}
					{
						xml::element isbn("isbn", xml);
						isbn.contents(books[i]->isbn);
					}
					{
						xml::element quantity("quantity", xml);
						quantity.contents(books[i]->quantity);
					}
					{
						xml::element price("price", xml);
						price.contents(books[i]->price);
					}
					{
						xml::element borrowed_date("borrowed_date", xml);
						borrowed_date.contents(users[i]->borrowed_list.front()->borrowed_date);
					}
					{
						xml::element borrowed_date("due_date", xml);
						borrowed_date.contents(users[i]->borrowed_list.front()->due_date);
					}
				}
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to get borrow list!");
	}
	else if( error_type == NO_RESULT ) {
		ss << xml_error("Borrow list is empty!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getUsersByKey( const std::string &keyword, std::vector<User*> &users ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getUsersByKey(keyword, users);
	if( error_type == QUERY_PASS ) {
		{
			xml::element book_list("user_list", xml);
			book_list.attr("type", "array" );
			for( size_t i = 0; i < users.size(); i++ ) {
				/* Returning below fields from database
				* book_id
				* title
				* author
				* isbn
				* quantity
				* price
				* genre_name_id
				* genre_name
				*/
				{
					xml::element user("user", xml);
					{
						xml::element user_id("user_id", xml);
						user_id.contents(users[i]->id);
					}
					{
						xml::element firstname("firstname", xml);
						firstname.contents(users[i]->firstname);
					}
					{
						xml::element lastname("lastname", xml);
						lastname.contents(users[i]->lastname);
					}
					{
						xml::element username("username", xml);
						username.contents(users[i]->username);
					}
					{
						xml::element email("group_id", xml);
						email.contents(users[i]->group_id);
					}
					{
						xml::element email("email", xml);
						email.contents(users[i]->email);
					}
					{
						xml::element address("address", xml);
						address.contents(users[i]->address);
					}
					{
						xml::element phone("phone", xml);
						phone.contents(users[i]->phone);
					}
				}
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to get user list!");
	}
	else if( error_type == NO_RESULT ) {
		ss << xml_error("No user found!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getSearchResult( const std::string &keyword, std::vector<Book*> &books, std::vector<Genre*> &genres ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getSearchResult(keyword, books, genres);
	if( error_type == QUERY_PASS ) {
		{
			xml::element book_list("book_list", xml);
			book_list.attr("type", "array" );
			for( size_t i = 0; i < books.size(); i++ ) {
				/* Returning below fields from database
				* book_id
				* title
				* author
				* isbn
				* quantity
				* price
				* genre_name_id
				* genre_name
				*/
				{
					xml::element book("book", xml);
					{
						xml::element book_id("book_id", xml);
						book_id.contents(books[i]->id);
					}
					{
						xml::element title("title", xml);
						title.contents(books[i]->title);
					}
					{
						xml::element author("author", xml);
						author.contents(books[i]->author);
					}
					{
						xml::element isbn("isbn", xml);
						isbn.contents(books[i]->isbn);
					}
					{
						xml::element quantity("quantity", xml);
						quantity.contents(books[i]->quantity);
					}
					{
						xml::element price("price", xml);
						price.contents(books[i]->price);
					}
					{
						xml::element genre_name_id("genre_name_id", xml);
						genre_name_id.contents(genres[i]->genre_name_id);
					}
					{
						xml::element genre_name("genre_name", xml);
						genre_name.contents(genres[i]->genre_name);
					}
				}
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to get search list!");
	}
	else if( error_type == NO_RESULT ) {
		ss << xml_error("Search result is empty!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::getGenre( std::vector<Genre*> &genres ) {
	stringstream ss;
	xml::writer xml(ss);
	error_type = backend->getGenre(genres);
	if( error_type == QUERY_PASS ) {
		{
			xml::element genre_list("genre_list", xml);
			genre_list.attr("type", "array" );
			for( size_t i = 0; i < genres.size(); i++ ) {
				/* Returning below fields from database
				* id
				* genre_name
				*/
				{
					xml::element genre("genre", xml);
					{
						xml::element genre_id("genre_id", xml);
						genre_id.contents(genres[i]->id);
					}
					{
						xml::element genre_name("genre_name", xml);
						genre_name.contents(genres[i]->genre_name);
					}
				}
			}
		}
	}
	else if( error_type == QUERY_FAIL ) {
		ss << xml_error("Unable to get genre list!");
	}
	else if( error_type == NO_RESULT ) {
		ss << xml_error("Genre list is empty!");
	}
	else {
		ss << xml_error("Bad request!");
	}
	return ss.str();
}

string Frontend_XML::xml_error( const string &msg ) {
	stringstream ss;
	xml::writer xml(ss);
	{
		xml::element error("error", xml);
		{
			xml::element err_msg("message", xml);
			err_msg.contents(msg);
		}
	}
	return ss.str();
}

string Frontend_XML::xml_success( const string &msg ) {
	stringstream ss;
	xml::writer xml(ss);
	{
		xml::element success("success", xml);
		{
			xml::element success_msg("message", xml);
			success_msg.contents(msg);
		}
	}
	return ss.str();
}
