<?
require_once( "includes/header.inc.php" );

displaySearchBox();

if ( isset( $_GET["search"] ) )
{
	if( !preg_match('/^[a-zA-Z0-9 ,\']+$/', $_GET['search_input']) && $_GET["search_input"] != ""  )
	{
		echo "<div class='errormsg'>No special characters allowed!!!</div><br/>";
	}
	else {
		$search_input = pg_escape_string( $_GET["search_input"] );

		$bookAry = Book::searchBookKeyword( $search_input );
		
		if ( count( $bookAry ) == 0 ) {
			echo "<div class='errormsg'>No result!</div><br/>";
		}
		
		foreach( $bookAry as $bookObject )
		{
			displayOneBook( $bookObject );
		}
	}

}

function displayOneBook( $bookObject ) 
{
	?>
	<div class="menubox" >
	
		<div class="menuboxheader" >
			<a href="book.php?book_id=<?=$bookObject->book_id?>" ><?=$bookObject->title?></a>
		</div>
		
		<div class="menuboxcontent" >
			Author: <?=$bookObject->author_name?> <br/>
			ISBN: <?=$bookObject->isbn?> <br/>
			<?
			echo "Genre<Br/>";
			if ( count( $bookObject->genreObjAry ) > 0 ) {
				
				foreach( $bookObject->genreObjAry as $genre )
				{
					echo "&nbsp;&nbsp;&nbsp;". $genre->genre_name . "<br/>";
				}
			}
			else
			{
				echo "&nbsp;&nbsp;&nbsp;Not specify";
			}
			?>
			
		</div>
	</div>
	<?
	
}

function displaySearchBox()
{
	?>
	<div class="menubox"  >
	<div class="menuboxheader" >Search</div>
	<div class="menuboxcontent" >

	<center>
		<form action="search.php" method="get">
			Keyword&nbsp;<input type="input" name="search_input" />
				
				<!--<select name="search_type" >
					<option value="keyword"> Keyword </option>
					<option value="genre"> Genre </option>
					<option value="author"> Author </option>
					<option value="title"> Title </option>
					<option value="isbn"> ISBN# </option>
				</select>-->
			<input type="submit" name="search" value="Search" />
		</form>
	</center>

	</div>
	</div>

	<?
}

require_once( "includes/footer.inc.php" );?>