<? require_once( "includes/header.inc.php" );

if ( !isLoggedIn() )
{
	die("Please log in to access this function");
}
else if ( isset($_POST['review']) && isset($_POST['book_id']) )
{
	$book_id = pg_escape_string ( $_POST['book_id'] );
	$user_id = pg_escape_string ( $_SESSION['user_id'] );
	$rating = pg_escape_string ( $_POST['rating'] );
	$review_text = pg_escape_string ( $_POST['review_text'] );

	$message = Book::review($book_id, $user_id, $review_text, $rating);
	echo "<div class='errormsg'>$message</div>";
}
else if ( !isset( $_GET['book_id'] ) )
{
	die("No Book selected");
}

else
{
	//Display book info here
	$book = new Book ($_GET['book_id']);
	echo $book->title;
?>
<form action="review.php" method="post" >
	Rating:
	<input type="radio" name="rating" value="1" /> 1
	<input type="radio" name="rating" value="2" /> 2
	<input type="radio" name="rating" value="3" checked /> 3
	<input type="radio" name="rating" value="4" /> 4
	<input type="radio" name="rating" value="5" /> 5
	<br/>
	<textarea name ="review_text" rows=5 cols=30 ></textarea>
	<br/>
	<input type="hidden" name="book_id" value="<?=$_GET['book_id']?>" />
	<input type="submit" name="review" value="Submit Review" /> 
</form>

<?
}

require_once( "includes/footer.inc.php" );
?>