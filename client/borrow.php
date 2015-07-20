<? 
require_once( "includes/header.inc.php" );
if ( !isLoggedIn() )
{
	die("<div class='errormsg'>You need to log in to view this page</div>");
}

else if( isAdmin() || isStaff() )
{
	if ( isset($_GET['viewByUser']) )
	{
		$user_id = pg_escape_string($_GET['user_id']);

		if( !isset($user_id) || $user_id=="")
		{
			echo "<div class='errormsg'>Please enter User ID</div>";
		}
		else
		{
			$xml = $client->send( "COMMAND LOOKUP BORROWED user_id=$user_id" );
		}
	}
	
	else if ( isset($_GET['viewByBook']) )
	{
		$isbn = pg_escape_string($_GET['isbn']);
		
		if( !isset($isbn) || $isbn=="")
		{
			echo "<div class='errormsg'>Please enter Book ISBN</div>";
		}
		else
		{
			$book = Book::getBookByISBN ($isbn);
			$xml = $client->send( "COMMAND LOOKUP BORROWED book_id=$book->book_id" );
		}
	}
	
	else if ( isset($_GET['viewAll']) )
	{
		$xml = $client->send( "COMMAND LOOKUP BORROWED ALL" );
	}
	
?>

<div class="menubox" >
<div class="menuboxheader" >Borrowed List</div>
	
<div class="menuboxcontent" >

<td>
	<form align= "center" action="borrow.php" method="get">
	<tr>User ID</tr>
	<tr><input type="input" name="user_id" /> </tr>
	<tr><input type="submit" name="viewByUser" value="View" /> </tr>
	</form>
</td>
<td>
	<form align= "center" action="borrow.php" method="get">
	<tr>Book ISBN</tr>
	<tr><input type="input" name="isbn" /> </tr>
	<tr><input type="submit" name="viewByBook" value="View" /> </tr>
	</form>
</td>
<td> 
	<form align= "center" action="borrow.php" method="get">
	<tr><input type="submit" name="viewAll" value="View All" /> </tr>
</form> </td>
	
<?
	if( isset($xml) )
	{
		if ( $xml->getName()== "error")
		{
			echo "<div class='errormsg'>$xml->message</div>";
		}
		else
		{
?>		
<td>
<table align="center" border="1" class="tablelist" >
	<tr>
		<th>Title</th>
		<th>User Id</th>
		<th> First Name </th>
		<th> Last Name </th>
		<th>Borrowed Date</th>
	<tr/>
	<?
	foreach( $xml as $r ) 
	{
		echo "<tr>";
		echo "<td>$r->title</td>";
		echo "<td>$r->user_id</td>";
		echo "<td>$r->firstname</td>";
		echo "<td>$r->lastname</td>";
		echo "<td>$r->borrowed_date</td>";
		echo "</tr>";
	}
	?>

</table>
</td>

<? }} ?>

</div>
</div>

<?
}

else
{
	$user_id = pg_escape_string($_SESSION['user_id']);
	$xml = $client->send( "COMMAND LOOKUP BORROWED user_id=$user_id" );
	
	if($xml->getName() == "error")
	{
		echo "<div class='errormsg'>$xml->message</div>";
	}
	else
	{
?>
<div class="menubox" >
<div class="menuboxheader" >Borrowed List</div>
	
<div class="menuboxcontent" >
<td>
<table align="center" border=1>
	<tr>
		<td>Title</td>
		<td>Borrowed Date</td>
	<tr/>
	<?
	foreach( $xml as $r ) 
	{
		echo "<tr>";
		echo "<td>$r->title</td>";
		echo "<td>$r->borrowed_date</td>";
		echo "</tr>";
	}
	?>

</table>
</td>
	
<?
	}
}

require_once( "includes/footer.inc.php" );
?>
