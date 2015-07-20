#ifndef HISTORY_H
#define HISTORY_H

#include <string>

/* Struct hold a History */
class History {
public:
	History( int u_id, int b_id )
		: user_id(u_id), book_id(b_id)
	{}
	History( int id, int u_id, int b_id, const char *r_date )
		: id(id), user_id(u_id), book_id(b_id), date_returned(r_date)
	{}
	int id;
	int user_id;
	int book_id;
	std::string date_returned;
};

#endif
