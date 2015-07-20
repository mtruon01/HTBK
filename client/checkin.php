<?
require_once( "includes/header.inc.php" );
if ( !isAdmin() && !isStaff() )
{
	die("<div class='errormsg'> You don't have the authorization to do this function </div></br>");
}

if ( isset($_GET['checkin']) )
{
	$isbn = pg_escape_string($_GET['isbn']);
	$user_id = pg_escape_string($_GET['user_id']);
	
	$error = false;
	if( !isset($isbn) || $isbn=="")
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for ISBN</div><br/>";
	}
	
	if ( !isset( $user_id ) || $user_id == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for User ID</div><br/>";
	}
	
	if(!$error)
	{
		$book = Book::getBookByISBN($isbn);
		if ($book == "")
		{
			$error = true;
			echo "<div class='errormsg'>There is no book with this ISBN</div><br/>";
		}
		
		if( !User::userExistById( $user_id ) )
		{
			$error = true;
			echo "<div class='errormsg'>User ID does not exist</div><br/>";
		}
		
		if (!$error)
		{
			$message = checkIn($book->book_id, $user_id);
			echo "<div class='errormsg'>$message</div><br/>";
		}
	}	
}

?>

<div class="menubox"  >
<div class="menuboxheader" >Check In</div>
<div class="menuboxcontent" >
<center>
<form action="checkin.php" method="get">
	<table align="center">
    	<tr>
        	<tr>
			<td> ISBN </td>
            <td> <input type="text" name="isbn" /> </td>
			</tr>
			<tr>
            <td> User ID </td>
            <td> <input type="text" name="user_id"  /> </td>
			</tr>
			<tr>
			<td colspan='2' align="center"> <input type="Submit" name="checkin" value="Check In"  /> </td>
			</tr>
        </tr>
    </table>
</form>
</center>
</div>
</div>

<?
require_once( "includes/footer.inc.php" );
?>