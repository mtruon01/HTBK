<? require_once( "includes/header.inc.php" );

if ( !(isAdmin() || isStaff()) )
{
	die("You are not allowed here");
}
displaySearchBox();

//if ( isset( $_GET["search_input"] ) && $_GET["search_input"] == "" )
//{
//	echo "Please enter something for the search";
//}
//else if ( isset( $_GET["search_input"] ) )
if ( isset( $_GET["search_input"] ) )
{
	if( !preg_match('/^[a-zA-Z0-9 ,]+$/', $_GET['search_input']) && $_GET["search_input"] != ""  )
	{
		echo "<div class='errormsg'>No special characters allowed!!!</div><br/>";
	}
	else {
		$search_input = pg_escape_string( $_GET["search_input"] );
		$command = "COMMAND LOOKUP USER keyword=$search_input";
		$xml = $client->send( $command );

		if ( isset( $xml->message ) )
		{
			echo "<div class='errormsg'>$xml->message</div><br/>";
		}
		else 
		{
			foreach( $xml as $user ) 
			{
				displayOneUser( $user );
			}
		}
	}
}

function displayOneUser( $user )
{
	
	?>
		<div class="menubox" >
		
			<div class="menuboxheader"><?=$user->username?>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="user.php?user_id=<?=$user->user_id?>">Edit</a></div>
			<div class="menuboxcontent" >
				<table>
					<tr>
						<td>User ID</td><td><?=$user->user_id?></td>
					</tr>
					<tr>
						<td>First Name</td><td><?=$user->firstname?></td>
					</tr>
					<tr>
						<td>Last name</td><td><?=$user->lastname?></td>
					</tr>
					<tr>
						<td>Username</td><td><?=$user->username?></td>
					</tr>
					<tr>
						<td>Email</td><td><?=$user->email?></td>
					</tr>
					<tr>
						<td>Address</td><td><?=$user->address?></td>
					</tr>
					<tr>
						<td>Phone</td><td><?=$user->phone?></td>
					</tr>
					<tr>
						<td>Group</td>
						<td><?
							if ( $user->group_id == 1 ) echo "Admin";
							if ( $user->group_id == 2 ) echo "Staff";
							if ( $user->group_id == 3 ) echo "Borrower";
							?>
						</td>
					</tr>
				</table>
			</div>
		</div>

	<?
}


function displayOneBook( $user ) 
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
	<div class="menuboxheader" >Search User</div>
	<div class="menuboxcontent" >

	<center>
		<form action="searchuser.php" method="get">
			<input type="input" name="search_input" />
			<input type="submit" name="search" value="Search" />
		</form>
	</center>

	</div>
	</div>

	<?
}

require_once( "includes/footer.inc.php" );
