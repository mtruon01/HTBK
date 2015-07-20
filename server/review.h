#ifndef REVIEW_H
#define REVIEW_H

#include <string>

/* Struct hold a Review */
class Review {
public:
	Review( int id, int approve = 1 )
		: id(id), approve(approve)
	{}
	Review( int u_id, int b_id, const char *review, int rating )
		: user_id(u_id), book_id(b_id), review(review), rating(rating)
	{}
	Review( int id, int u_id, int b_id, const char *review, int rating, const char *r_date, int approve )
		: id(id), user_id(u_id), book_id(b_id), review(review), rating(rating), 
		reviewed_date(r_date), approve(approve)
	{}
	int id;
	int user_id;
	int book_id;
	std::string review;
	int rating;
	std::string reviewed_date;
	int approve;
};

#endif
