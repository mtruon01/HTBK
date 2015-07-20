<?

require_once( "includes/config.inc.php" );
 
class Genre
{
	public $genre_id;
	public $genre_name;

	function __construct( $g_id, $g_name )
	{
		$this->genre_id = (string)$g_id;
		$this->genre_name = (string)$g_name;
	}
	
	public static function getGenreArrayByBookId( $book_id )
	{
		$command = "COMMAND GET GENRE book_id=$book_id";
		global $client;
		$xml = $client->send($command);
		
		$genreArray = array();
		
		foreach( $xml as $g )
		{
			$genreArray[] = new Genre( (string)$g->genre_name_id, (string)$g->genre_name );
		}
		
		echo "<pre>";
		//print_r ( $xml );
		echo "</pre>";
		
		
		return $genreArray;
	}



}


?>