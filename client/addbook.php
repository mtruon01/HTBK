<?
require_once( "includes/header.inc.php" );

if ( !isAdmin() && !isStaff() )
{
	echo "<div class='errormsg'> You don't have the authorization to do this function </div></br>";
}
else if( isset($_POST["submit"]) )
{	
	$title = pg_escape_string ( $_POST["title"] );
	$author = pg_escape_string ( $_POST["author"] );
	$isbn = pg_escape_string ( $_POST["isbn"] );
	$quantity = pg_escape_string ( $_POST["quantity"] );
	$price = pg_escape_string ( $_POST["price"] );
	
	$error = false;
	
	if ( !isset( $title ) || $title == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Title</div><br/>";
	}
	
	if ( !isset( $author ) || $author == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Author</div><br/>";
	}
	
	if ( !isset( $isbn ) || $isbn == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for ISBN</div><br/>";
	}
	
	if ( !isset( $quantity ) || $quantity == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Quantity</div><br/>";
	}
	
	if ( !isset( $price ) || $price == "" )
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
		$info["title"] = $title;
		$info["author"] = $author;
		$info["isbn"] = $isbn;
		$info["quantity"] = $quantity;
		$info["price"] = $price;
		$info["genre"] = array();
		
		foreach( $_POST["genrelist"] as $genre_id )
		{
			$info["genre"][] = pg_escape_string ($genre_id);
		}
		$message = Book::addBook($info);
		
		echo "<div class='errormsg'>$message</div>";
	}
	else
	{
		addBookForm();
	}
}
else
{
	addBookForm();
}

function addBookForm ()
{
Global $client;
$xml = $client->send( "COMMAND LOOKUP GENRE ALL" );


?>
<div class="menubox"  >
<div class="menuboxheader" >Adding Book</div>
<div class="menuboxcontent" >
<center>
<form action="addbook.php" method="post">
	<table align="center"  >
    	<tr>
        	<td>Title:</td>
            <td colspan="3"><input type="text" name="title" /></td>
        </tr>
        <tr>
        	<td>Author:</td>
            <td colspan="3"><input type="text" name="author" /></td>
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
						?><input type="checkbox" name=genrelist[] value="<?=$genre->genre_id?>" /><?
						echo $genre->genre_name."<br/>";
						$i++;
						
					}
					
				?>
			</td>       
        </tr>
        <tr>
         	<td>ISBN:</td>
            <td colspan="3" ><input type="text" name="isbn" /></td>  
        </tr>
        <tr>
         	<td>Quantity:</td>
            <td colspan="3" ><input type="text" name="quantity" /></td>       
        </tr>
        <tr>
         	<td>Price:</td>
            <td colspan="3" ><input type="text" name="price" /></td>  
        </tr>
        <tr>
        	<td colspan="4" align="center"><input type="submit" value="Submit" name="submit" /></td>
        </tr>
    </table>
</form>
</center>
</div>
</div>
<?
}

require_once( "includes/footer.inc.php" );
?>