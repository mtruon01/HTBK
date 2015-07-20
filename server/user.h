#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <iostream>
#include "borrow.h"
#include "history.h"
#include "reserve.h"
#include "review.h"

/* Struct hold a user and information of History, Reserve, Borrow, Review lists */
class User {
public:
	User( const char *u_name, const char *u_pass, const char *f_name, const char *l_name,
		int g_id, const char *r_date, const char *email, const char *phone, const char *address )
		: id(-1), username(u_name), password(u_pass), firstname(f_name), lastname(l_name), group_id(g_id),
		register_date(r_date), email(email), phone(phone), address(address)
	{}
	User( const char *n, const char *p)
		: username(n), password(p)
	{}
	User(int i, const char *n): id(i), username(n)
	{}
	User(int i=-1): id(i)
	{}
	/*********** Display one User information if available ********/
	void display() {
		std::cout << std::endl	<< "id= "	<< id
			<< std::endl
			<< "username= "			<< username			<< std::endl
			<< "password= "			<< password			<< std::endl
			<< "firstname= "		<< firstname		<< std::endl
			<< "lastname= "			<< lastname			<< std::endl
			<< "group_id= "			<< group_id			<< std::endl
			<< "register_date= "	<< register_date	<< std::endl
			<< "email= "			<< email			<< std::endl
			<< "phone= "			<< phone			<< std::endl
			<< "address= "			<< address			<< std::endl
			<< std::endl;
	}
	int 					id;
	std::string			username;
	std::string			password;
	std::string			firstname;
	std::string			lastname;
	int					group_id;
	std::string			register_date;
	std::string			email;
	std::string			phone;
	std::string			address;
	std::vector<History*>	history_list;
	std::vector<Reserve*>	reserved_list;
	std::vector<Review*>	reviewed_list;
	std::vector<Borrow*>	borrowed_list;
};

#endif
