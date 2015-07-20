<? require_once( "includes/header.inc.php" );

echo "<pre>";
//print_r( $_POST );
echo "</pre>";
	
if ( !isLoggedIn() )
{
	die("Please login");
}

if ( isAdmin() || isStaff () )
{
	if ( !isset( $_GET["user_id"] ) || $_GET["user_id"] == "" ) 
	{	
		die("User is not set");
	}
}

if ( isset( $_POST['change_password'] ) )
{
	if ( $_POST['password1'] != $_POST['password2'] )
	{
		echo "Password doesn't match";
	}
	else
	{
		$userInfo["user_id"] = pg_escape_string ( $_POST["user_id"] );
		$userInfo["username"] = pg_escape_string ( $_POST["username"] );
		$userInfo["firstname"] = pg_escape_string ( $_POST["firstname"] );
		$userInfo["lastname"] = pg_escape_string ( $_POST["lastname"] );
		$userInfo["email"] = pg_escape_string ( $_POST["email"] );
		$userInfo["address"] = pg_escape_string ( $_POST["address"] );
		$userInfo["phone"] = pg_escape_string ( $_POST["phone"] );
		$userInfo["group_id"] = pg_escape_string ( $_POST["group_id"] );
		$userInfo["password"] = md5( pg_escape_string ( $_POST["password1"] ));
		
		echo "<pre>";
		//print_r( $userInfo );
		echo "</pre>";
		
		$htmlQuery = http_build_query($userInfo);
						
		$command = "COMMAND UPDATE USER $htmlQuery";
		//echo $command . "<br/><br/>";
		$xml = $client->send( $command );
		//print_r( $xml );
		
		if ( isset( $xml->message ) )
		{
			echo "<div class='errormsg'>$xml->message</div><br/>";
		}
		else
		{
			echo "<div class='errormsg'>Update successful!</div><br/>";
		}
	}
}

if ( isset( $_POST['edit_user'] ) )
{

	$userInfo["user_id"] = pg_escape_string ( $_POST["user_id"] );
	$userInfo["username"] = pg_escape_string ( $_POST["username"] );
	$userInfo["firstname"] = pg_escape_string ( $_POST["firstname"] );
	$userInfo["lastname"] = pg_escape_string ( $_POST["lastname"] );
	$userInfo["email"] = pg_escape_string ( $_POST["email"] );
	$userInfo["address"] = pg_escape_string ( $_POST["address"] );
	$userInfo["phone"] = pg_escape_string ( $_POST["phone"] );
	$userInfo["group_id"] = pg_escape_string ( $_POST["group_id"] );
	
	echo "<pre>";
	//print_r( $userInfo );
	echo "</pre>";
	
	$htmlQuery = http_build_query($userInfo);
					
	$command = "COMMAND UPDATE USER $htmlQuery";
	//echo $command . "<br/><br/>";
	$xml = $client->send( $command );
	//print_r( $xml );
	
	if ( isset( $xml->message ) )
	{
		echo "<div class='errormsg'>$xml->message</div><br/>";
	}
	else
	{
		echo "<div class='errormsg'>Update successful!</div><br/>";
	}
}

if ( isBorrower() )
{
	displayBorrowerForm();
}
else
{
	displayEditUserForm( $_GET["user_id"] );
}

function displayBorrowerForm()
{
	$user_id = pg_escape_string( $_SESSION['user_id'] );
	$user = new User( $user_id );
	displayOneUser( $user );
	?>
	<form action="user.php?user_id=<?=$user->user_id?>" method="post">
	<center>
	
	<div class="menubox"  >
	<div class="menuboxheader" >Change Password </div>
	<div class="menuboxcontent" >
		<table >
			<tr>
				<td> Password: </td>
				<td> <input type = "password" name = "password1"  /> </td>
			</tr>
			<tr>
				<td> Retype Password: </td>
				<td> <input type = "password" name = "password2" /> </td>
			</tr>
			<tr>
				<td colspan = "2"> <center><input type = "submit" value = "Change Password" name = "change_password"/> </center></td>
			</tr>
		</table>
	</div>
	</div>
	
	<div class="menubox"  >
	<div class="menuboxheader" >Edit Info</div>
	<div class="menuboxcontent" >
		<table>
			<tr>
				<td> Email: </td>
				<td> <input type = "text" name = "email" value="<?=$user->email?>" /> </td>
			</tr>

			<tr>
				<td> Phone Number: </td>
				<td> <input type = "text" name = "phone" value="<?=$user->phone?>" /> </td>
			</tr>
			<tr>
				<td colspan = "2"> <center> <input type = "submit" value = "Submit" name = "edit_user"/> </center> </td>
			</tr>
			<input type="hidden" name="user_id" value="<?=$user->user_id?>" />
			<input type="hidden" name="lastname" value="<?=$user->lastname?>" />
			<input type="hidden" name="firstname" value="<?=$user->firstname?>" />
			<input type="hidden" name="address" value="<?=$user->address?>" />
			<input type="hidden" name="group_id" value="<?=$user->group_id?>" />
			<input type="hidden" name="username" value="<?=$user->username?>" />
		</table>
	</div>
	</div>
	
	</center>
	</form>
	
	<?
}

function displayEditUserForm( $user_id )
{
	$user_id = pg_escape_string( $user_id );
	$user = new User( $user_id );
	
	?>
	<div class="menubox"  >
	<div class="menuboxheader" >Edit User</div>
	<div class="menuboxcontent" >
	<center>
	<form action="user.php?user_id=<?=$user->user_id?>" method="post">
		<table >
			<tr>
				<td> UserName: </td>
				<td> <input type = "text" name = "username" value= "<?=$user->username?>" /> </td>
			</tr>
			<tr>
				<td> Group ID: </td>
				<td>
				  <select name = "group_id">
				<?	if ( isStaff() || isAdmin() )
					{
						if ( isAdmin() ) { ?>
							<option value="1" <? if ( $user->group_id == 1) echo "selected" ?> > Admin </option>
						<?}?>
						<option value="2" <? if ( $user->group_id == 2) echo "selected" ?> > Staff </option>
						<option value="3" <? if ( $user->group_id == 3) echo "selected" ?> > Borrower </option>
				<?  }?>	
				  </select>
				</td>
			</tr>
			<tr>
				<td> Email: </td>
				<td> <input type = "text" name = "email" value="<?=$user->email?>" /> </td>
			</tr>
			<tr>
				<td> First Name: </td>
				<td> <input type = "text" name = "firstname" value="<?=$user->firstname?>" /> </td>
			</tr>
			<tr>
				<td> Last Name: </td>
				<td> <input type = "text" name = "lastname" value="<?=$user->lastname?>" /> </td>
			</tr>
			<tr>
				<td> Address: </td>
				<td> <input type = "text" name = "address" value="<?=$user->address?>" /> </td>
			</tr>
			<tr>
				<td> Phone Number: </td>
				<td> <input type = "text" name = "phone" value="<?=$user->phone?>" /> </td>
			</tr>
			<tr>
				<td colspan = "2"> <center> <input type = "submit" value = "Submit" name = "edit_user"/> </center> </td>
			</tr>
			<input type="hidden" name="user_id" value="<?=$user->user_id?>" />
		</table>
	</form>
	</center>
	</div>
	</div>
	<?
}

function displayOneUser( $user )
{
	?>
		<div class="menubox" >
			<div class="menuboxheader" >User Information</div>
			<div class="menuboxcontent" >
				<center>
				<table>
					<? if ( isAdmin() || isStaff()  ){ ?>
					<tr>
						<td><b>User ID</b></td><td><?=$user->user_id?></td>
					</tr>
					<? } ?>
					<tr>
						<td><b>Username</b></td><td><?=$user->username?></td>
					</tr>
					<tr>
						<td><b>First Name</b></td><td><?=$user->firstname?></td>
					</tr>
					<tr>
						<td><b>Last name</b></td><td><?=$user->lastname?></td>
					</tr>
					<tr>
						<td><b>Email</b></td><td><?=$user->email?></td>
					</tr>
					<tr>
						<td><b>Address</b></td><td><?=$user->address?></td>
					</tr>
					<tr>
						<td><b>Phone</b></td><td><?=$user->phone?></td>
					</tr>
					<? if ( isAdmin() || isStaff() ) { ?>
					<tr>
						<td>Group</td>
						<td><?
							if ( $user->group_id == 1 ) echo "Admin";
							if ( $user->group_id == 2 ) echo "Staff";
							if ( $user->group_id == 3 ) echo "Borrower";
							?>
						</td>
					</tr>
					<? } ?>
				</table>
				</center>
			</div>
		</div>

	<?
}

require_once( "includes/footer.inc.php" );