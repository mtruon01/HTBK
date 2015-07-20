<?
require_once( "includes/header.inc.php" );

$book_id = pg_escape_string($_GET['book_id']);

if ( !isset( $_GET['book_id'] ) || $_GET['book_id'] =="")
{
	die("No book is selected");
}
else if ( isset( $_GET['act'] ) && $_GET['act'] == "delete" )
{
	$message = Book::delete($book_id);
	die("<div class='errormsg'>$message</div>");
}
else if ( isset( $_GET['act'] ) && $_GET['act'] == "reserve" )
{
	$user_id = pg_escape_string($_SESSION['user_id']);
	$message = Book::reserve($user_id, $book_id);
	echo "<div class='errormsg'>$message</div>";  
}

displayBook( $book_id );
displayReview( $book_id );

function displayBook( $book_id )
{
	$book = new Book( $book_id );
	?>
	<div class="menubox" >
	<div class="menuboxheader" >Book Information</div>
		<div class="menuboxcontent" >
	<table align="center">
		<tr>
			<th>
				<h1 class="title">Title: <?=$book->title?></h1>
			</th>
		</tr>
		<tr>
			<td>Author: <?=$book->author_name?></td>
		</tr>
	
		<tr>
			<td>ISBN: <?=$book->isbn?></td>
		</tr>
		<tr>
			<td>
			<?
			echo "Genre<Br/>";
			if ( count( $book->genreObjAry ) > 0 ) {
				
				foreach( $book->genreObjAry as $genre )
				{
					echo "&nbsp;&nbsp;&nbsp;". $genre->genre_name . "<br/>";
				}
			}
			else
			{
				echo "&nbsp;&nbsp;&nbsp;Not specify";
			}
			?>
			</td>
		</tr>
	</table>
	
	<? if ( isAdmin() || isStaff() ) { ?>
	<a href="book.php?act=delete&book_id=<?=$book->book_id?>" >Delete Book</a><Br/>
	<a href="editbook.php?book_id=<?=$book->book_id?>" >Edit Book</a><Br/>
	<? } ?>
	
	<? if ( isBorrower() ) { ?>
	<a href="review.php?book_id=<?=$book->book_id?>" >Review Book</a><br/>
	<a href="book.php?act=reserve&book_id=<?=$book->book_id?>" >Reserve Book</a>
	<? } ?>
	</div>
	</div>
	
	
	<?
}

function displayReview( $book_id )
{
	Global $client;
	$xml = $client->send( "COMMAND LOOKUP REVIEWED book_id=$book_id" );
	?>
	<div class="menubox" >
	<div class="menuboxheader" >Reviews</div>
		<div class="menuboxcontent" >
	<?
		if ( isset( $xml->message ) )
		{
			echo "<div class='errormsg'>$xml->message</div><br/>";
		}
		else
		{
			$allApprove = 0;
			//echo "<pre>";
			//print_r ( $xml );
			foreach( $xml as $review )
			{
				if ( $review->approve == 1 )
				{		
					echo "Firstname: $review->firstname<br/>";
					echo "Rating: $review->rating<br/>";
					echo "Review: $review->review<br/>";
					echo "<br/>";
					$allApprove++;
				}
				else
					continue;

			}
			if ( $allApprove == 0 )
				echo "<div class='errormsg'>No review is available!</div><br/>";
			//echo "</pre>";
		}
	?>
	
	</div>
	</div>
	<?
}



require_once( "includes/footer.inc.php" );?>
