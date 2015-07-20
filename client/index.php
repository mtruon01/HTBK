<?require_once( "includes/header.inc.php" );
if ( !isLoggedIn() )
{
	//redirect("login.php");
}
?>

<div class="post">
	<h1 class="title"><a href="#">Welcome to Our Website!</a></h1>
	<p class="byline"><small>Posted on Nov, 2010 by <a href="contact.php">Hi-Tech Book Keeper Team</a></small></p>
	<div class="entry">
		<p><strong>HTBK</strong> is the best library system you can get. </p>
		<p>In this website you get do a lot of stuff such as browsing books, reserving books and rate the books. 
		You will be able to check out book in your library. Other people cannot borrow from from the reserve list unless you are 
		the one that reserved it.<p>
		 
	</div>
</div>

<?

echo "<pre>";
//print_r ( $_SESSION );
echo "</pre>";



require_once( "includes/footer.inc.php" );
?>