#ifndef RESERVE_H
#define RESERVE_H

#include <string>

/* Struct hold a Reserve */
class Reserve {
public:
	Reserve( int u_id, int b_id )
		: user_id(u_id), book_id(b_id)
	{}
	Reserve( int id, int u_id, int b_id, const char *r_date )
		: id(id), user_id(u_id), book_id(b_id), reserved_date(r_date)
	{}
	int id;
	int user_id;
	int book_id;
	std::string reserved_date;
};

#endif
