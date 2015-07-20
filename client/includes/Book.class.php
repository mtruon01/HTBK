<?
require_once( "includes/config.inc.php" );
require_once( "includes/HTBKClient.class.php" );
 
class Book
{
	public $book_id;
	public $title;
	public $author_name;
	public $isbn;
	public $quantity;
	public $price;
	public $genreObjAry;
	public $c;
		
	function __construct( $book_id )
	{
		Global $client;
		$this->c = $client;
		$this->getInfo($book_id);
	}
		
	public function getInfo($book_id)
	{

		
		$command = "COMMAND GET BOOK book_id=$book_id";
		$xml = $this->c->send($command);
		$this->book_id = (string)$xml['id'];
		$this->title = (string)$xml->title;
		$this->author_name = (string)$xml->author;
		$this->isbn = (string)$xml->isbn;
		$this->quantity = (string)$xml->quantity;
		$this->price = (string)$xml->price;
		if ( isset( $this->book_id ) && $this->book_id  != "" )
		{
			$this->genreObjAry = Genre::getGenreArrayByBookId( $book_id );
		}
	}
	
	public function countBorrowed()
	{
		$command = "COMMAND LOOKUP BORROWED book_id=$this->book_id";
		$xml = $this->c->send($command);
		return count($xml->borrowed);
	}
	
	public function countReserved()
	{
		$command = "COMMAND LOOKUP RESERVED book_id=$this->book_id";
		$xml = $this->c->send($command);
		return count($xml->reserved);
	}
	
	public static function isBorrowedByUser($book_id, $user_id)
	{
		Global $client;
		$command = "COMMAND LOOKUP BORROWED book_id=$book_id";
		$xml = $client->send($command);
				
		foreach ($xml as $user)
		{
			if($user->user_id == $user_id)
			{
				return true;
			}
		}
		return false;
	}
	
	public static function review($book_id, $user_id, $review_text, $rating)
	{
		Global $client;
		
		$review['user_id'] = $user_id;
		$review['book_id'] = $book_id;
		$review['review'] = $review_text;
		$review['rating'] = $rating;
		
		$htmlData = http_build_query($review);
		$command = "COMMAND INSERT REVIEW $htmlData";
		
		$xml = $client->send($command);
		
		if($xml->getName() != "error")
		{
			return "Added review successfully!";
		}
		else
		{
			return $xml->message;
		}
	}
	
	public static function searchBookKeyword( $keyword )
	{	
		Global $client;
		
		$command = "COMMAND LOOKUP BOOK keyword=$keyword";
		$xml = $client->send( $command );

		$bookAry = array();
		
		if ( isset( $xml->message ))
		{
			return $bookAry;
		}
		
		foreach ( $xml as $book )
		{
			$dup = false;
			foreach( $bookAry as $b )
			{
				if ( $b->book_id == $book->book_id )
				{
					$dup = true;
					break;
				}
			}
			if ( !$dup )
			{
				$bookAry[] = new Book( $book->book_id );
			}
		}
	
		return $bookAry;
	}
	
	public static function addBook ($info)
	{
		Global $client;

		$htmlData = http_build_query($info);
		
		$command = "COMMAND INSERT BOOK $htmlData";

		$xml = $client->send($command);
		
		if($xml->getName() != "error")
		{
			$book_id = $xml["id"];
			foreach( $info["genre"] as $genre )
			{
				$command = "COMMAND INSERT GENRE book_id=$book_id&genre_name_id=$genre";
				$client->send($command );
			}
			return "Added book successfully!";
		}
		else
		{
			return (string)$xml->message;
		}
	}
	
	public static function reserve ($user_id, $book_id)
	{
		Global $client;
		
		if( !Book::isBorrowedByUser($book_id, $user_id) )
		{
			$reserve['user_id'] = $user_id;
			$reserve['book_id'] = $book_id;
			$htmlData = http_build_query($reserve);
		
			$command = "COMMAND INSERT RESERVE $htmlData";
			$xml = $client->send($command);
			if ( isset( $xml->message ) )
			{
				return (string)$xml->message;
			}
			else
			{
				return "Reserve succesfull";
			}
		}
		else
		{
			return "You are borrowing the book. Cannot reserve it!";
		}
	}
	
	public static function delete ($book_id)
	{
		Global $client;
		
		$command = "COMMAND DELETE BOOK book_id=$book_id";
		$xml = $client->send($command);
		return (string)$xml->message;
	}
	
	public static function getBookByISBN ($isbn)
	{
		Global $client;
		$command = "COMMAND GET BOOK isbn=$isbn";
		$xml = $client->send($command);
		if($xml->getName() != "error")
		{
			return new Book((string)$xml['id']);
		}
		else
		{
			return "";
		}		
	}
}

?>