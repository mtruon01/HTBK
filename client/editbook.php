<?
require_once( "includes/header.inc.php" );

echo "<pre>";
	//print_r( $_POST );
	echo "</pre>";
	
if ( isset( $_POST["edit"] ) )
{
	
	$htmlData = array( 
					'title'=>  		pg_escape_string( $_POST['title'] ),
					'book_id'=>		pg_escape_string( $_POST['book_id'] ),
					'author'=> 		pg_escape_string( $_POST['author'] ),	
					'isbn'=> 		pg_escape_string( $_POST['isbn'] ),
					'quantity'=> 	pg_escape_string( $_POST['quantity'] ),
					'price'=> 		pg_escape_string( $_POST['price'] )
					);
	$htmlQuery = http_build_query($htmlData); 
	
	$error = false;
	
	if ( !isset( $_POST['title'] ) || $_POST['title'] == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Title</div><br/>";
	}
	
	if ( !isset( $_POST['author'] ) || $_POST['author'] == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Author</div><br/>";
	}
	
	if ( !isset( $_POST['isbn'] ) || $_POST['isbn'] == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for ISBN</div><br/>";
	}
	
	if ( !isset( $_POST['quantity'] ) || $_POST['quantity'] == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Quantity</div><br/>";
	}
	
	if ( !isset( $_POST['price'] ) || $_POST['price'] == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Price</div><br/>";
	}
	
	if ( !isset($_POST["genrelist"]) || count( $_POST["genrelist"] ) == 0 )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter atleast one genre</div><br/>";
	}
	
	if ( !$error )
	{
		$xml = $client->send("COMMAND UPDATE BOOK $htmlQuery");
		
		echo "<pre>";
		//print_r( $htmlData );
		echo "</pre>";
		
		if ( isset( $xml->message ) )
		{
			echo "<div class='errormsg'>$xml->message</div><br/>";
		}
		else
		{
			$book_id = pg_escape_string( $_POST['book_id'] );
			
			//Clear out all the genre for this book
			$xml= $client->send("COMMAND DELETE BOOK_GENRE book_id=$book_id");
			
			if ( isset( $_POST['genrelist'] ) )
			{
				foreach( $_POST['genrelist'] as $genre_id )
				{
					$client->send("COMMAND INSERT GENRE book_id=$book_id&genre_name_id=$genre_id" );
				}
			}
			
			echo "<div class='errormsg'>Book updated!</div><br/>";
		}
	}
}


displayEditForm( $_GET['book_id'] );


function displayEditForm( $book_id )
{
	$book_id = pg_escape_string( $book_id );
	$book = new Book( $book_id );
	echo "<pre>";
	//print_r( $book );
	
	foreach( $book->genreObjAry as $genreObj )
	{
			//echo $genreObj->genre_id;
	}
	
	$xml = $book->c->send( "COMMAND LOOKUP GENRE ALL" );
	//print_r( $xml );
	echo "</pre>";
	?>
	<div class="menubox"  >
	<div class="menuboxheader" >Edit Book</div>
	<div class="menuboxcontent" >
	<center>
	<form action="editbook.php?book_id=<?=$book_id?>" method="post">
		<table align="center"  >
			<tr>
				<td>Title:</td>
				<td colspan="3"><input type="text" name="title" value="<?=$book->title?>" /></td>
			</tr>
			<tr>
				<td>Author:</td>
				<td colspan="3"><input type="text" name="author" value="<?=$book->author_name?>" /></td>
			</tr>
			<tr>
				<td>Genre:</td>
				<td>
					<?	$i = 0;
						foreach( $xml as $genre )
						{
							if ( $i == ceil( count( $xml ) / 3 ) || $i == ceil( count( $xml ) * 2/ 3 ) )
							{
								echo "</td><td>";
							}
							?>
							<input type="checkbox" 
							<? 
								foreach( $book->genreObjAry as $genreObj )
								{
									
									if ( (string) $genreObj->genre_id == (string)$genre->genre_id )
									{
										echo " checked ";
									}
								}
							?>
							
							name=genrelist[] value="<?=$genre->genre_id?>" />
							
							<?
							
							echo $genre->genre_name."<br/>";
							$i++;
							
						}
						
					?>
				</td>       
			</tr>
			<tr>
				<td>ISBN:</td>
				<td colspan="3" ><input type="text" name="isbn" value="<?=$book->isbn?>" /></td>  
			</tr>
			<tr>
				<td>Quantity:</td>
				<td colspan="3" ><input type="text" name="quantity" value="<?=$book->quantity?>" /></td>       
			</tr>
			<tr>
				<td>Price:</td>
				<td colspan="3" ><input type="text" name="price" value="<?=$book->price?>" /></td>  
			</tr>
			<tr>
				<td colspan="4" align="center"><input type="submit" value="Submit" name="edit" /></td>
			</tr>
			
		</table>
		<input type="hidden" name="book_id" value="<?=$book->book_id?>" />
	</form>
	</center>
	</div>
	</div>
	<?
}

require_once("includes/footer.inc.php" );