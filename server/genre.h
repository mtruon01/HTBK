#ifndef GENRE_H
#define GENRE_H

#include <string>

/* Struct hold a Genre */
class Genre {
public:
	Genre( int b_id, int g_id )
		: book_id(b_id), genre_name_id(g_id)
	{}
	Genre( int id, const char *g_name )
		: id(id), genre_name(g_name)
	{}
	Genre( int id, int b_id, int g_id, const char *g_name )
		: id(id), book_id(b_id), genre_name_id(g_id), genre_name(g_name)
	{}

	int         id;
	int         book_id;
	int         genre_name_id;
	std::string genre_name;
};

#endif
