<?
//Commands will be in the following format
//COMMAND REALCOMMAND ACTION QUERY
//Query look like this: user_id=5&name=tommy

/* ==================== GETTER ==================== */

// verify user ( WORKING )
COMMAND GET VERIFYUSER query 
	mandatory query: username, md5( password )

//get one user info  ( WORKING )
COMMAND GET USER query
	mandatory query: user_id

//get one book info ( WORKING )
COMMAND GET BOOK query 
	mandatory query: book_id
		or
	mandatory query: isbn
	
////get list of genre
//COMMAND GET GENRE query
//	mandatory query: book_id

/* ============== LOOKUP STUFF ====================== */

//Get all the possible genre
COMMAND LOOKUP GENRE ALL

COMMAND LOOKUP RESERVED query
	mandatory query: book_id
		or
	mandatory queyr: user_id


COMMAND LOOKUP BORROWED query
	mandatory query: book_id
		or
	mandatory query: user_id


COMMAND LOOKUP HISTORY query
	mandatory query: book_id
		or
	mandatory query: user_id
	
COMMAND LOOKUP REVIEWED query
	mandatory query: book_id
		or
	mandatory query: user_id	
		

COMMAND LOOKUP BOOK query
	mandatory query: keyword
	
COMMAND LOOKUP USER query
	mandatory query: keyword

/* ========================== INSERT ================= */
// ( WORKING )
COMMAND INSERT BOOK query
	Mandatory query names: title, author_name, isbn, price, quantity

COMMAND INSERT GENRE query
	Mandatory query names: book_id, genre_name_id
	
//( WORKING )
COMMAND INSERT USER query 
	mandatory query: username, md5( password ), group_id
	optional query: email, phone, address

// ( WORKING )
COMMAND INSERT RESERVE  query
	mandatory query: user_id, book_id
	
	
// ( WORKING )
COMMAND INSERT REVIEW query
	mandatory query: user_id, book_id, review, rating
	
	
/* =================== CHECK IN/OUT ==================== */
//CHECKING IN ( WORKING )
COMMAND CHECKIN query
	mandatory query: user_id, book_id
	
//CHECKING OUT ( WORKING )
COMMAND CHECKOUT query
	mandatory query: user_id, book_id

/* ======================= UPDATE ====================== */

COMMAND UPDATE APPROVE_REVIEW
	mandatory query: review_id
	
COMMAND UPDATE BOOK query
	mandatory query: book_id, title, author_name, isbn, price, quantity
	
COMMAND UPDATE USER query
	mandatory query: user_id, group_id, email, firstname, lastname, address, phone

/* ======================= DELETE  ====================== */

COMMAND DELETE BOOK_GENRE query //Delete all the genre for that book
	mandatory query: book_id
	
COMMAND DELETE BOOK query  //DELETE the book
	mandatory query: book_id 
	
COMMAND DELETE REVIEW query
	mandatory query: reviewed_id
	
COMMAND DELETE RESERVE query
	mandatory query: reserved_id
	
COMMAND DELETE USER query
	mandatory query: user_id

?>
