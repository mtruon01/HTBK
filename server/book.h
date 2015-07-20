#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>

/* Struct holds one Book's data */
class Book {
public:
	Book( int id, const char *title, const char *authorName, const char *isbn,
		int quantity, double price )
		: id(id), title(title), author(authorName), isbn(isbn),
		quantity(quantity), price(price)
	{}
	Book( const char *title, const char *authorName, const char *isbn,
		int quantity, double price )
		: id(-1), title(title), author(authorName), isbn(isbn), 
		quantity(quantity), price(price)
	{}
	Book( int id, int quantity )
		: id(id), quantity(quantity)
	{}
	Book( int id = -1 )
		: id(id)
	{}
	/*********** Display one Book information if available ********/
	void display() {
		std::cout << std::endl << "id= " << id
			<< std::endl
			<< "title= "	<< title	<< std::endl
			<< "author= "	<< author	<< std::endl
			<< "isbn= "		<< isbn		<< std::endl
			<< "quantity= "	<< quantity << std::endl
			<< "price= "	<< price
			<< std::endl;
	}
	int			id;
	std::string title;
	std::string	author;
	std::string	isbn;
	int			quantity;
	double		price;
};
#endif
