#ifndef BORROW_H
#define BORROW_H

#include <string>

/* Struct hold a Borrow */
class Borrow {
public:
	Borrow( int u_id, int b_id )
		: user_id(u_id), book_id(b_id)
	{}
	Borrow( int id, int u_id, int b_id, const char *b_date, const char *d_date )
		: id(id), user_id(u_id), book_id(b_id), borrowed_date(b_date), due_date(d_date)
	{}
	int id;
	int user_id;
	int book_id;
	std::string borrowed_date;
	std::string due_date;
};

#endif
