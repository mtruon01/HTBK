<?
require_once( "includes/header.inc.php" );

if ( !isAdmin() && !isStaff() )
{
	echo "<div class='errormsg'> You don't have the authorization to do this function </div></br>";
}
else if( isset($_POST["addUser"]) )
{	
	$username = pg_escape_string ( $_POST["username"] );
	$password = pg_escape_string ( $_POST["password"] );
	$password2 = pg_escape_string ( $_POST["password2"] );
	$firstname = pg_escape_string ( $_POST["firstname"] );
	$lastname = pg_escape_string ( $_POST["lastname"] );
	$email = pg_escape_string ( $_POST["email"] );
	$address = pg_escape_string ( $_POST["address"] );
	$phone = pg_escape_string ( $_POST["phone"] );
	$city = pg_escape_string ( $_POST["city"] );
	$state = pg_escape_string ( $_POST["state"] );
	$zipcode = pg_escape_string ( $_POST["zipcode"] );
	$groupID = pg_escape_string ( $_POST["group_id"] );
	
	$error = false;
	if ( !isset( $username ) || $username == "" )  //Make sure username is not empty
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for the username</div>";
	}
	
	if ( !isset( $password ) || $password == "" ) 
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for the password</div>";
	}
	
	if ( !isset( $password2 ) || $password2 == "" )
	{
		$error = true;
		echo "<div class='errormsg'>Please re-type the password</div>";
	}
	
	if($password != $password2)
	{
		$error = true;
		echo "<div class='errormsg'>Re-typed password doesn't match</div>";
	}
	
	if ( !isset( $firstname ) || $firstname == "" ) 
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for First Name</div>";
	}
	
	if ( !isset( $lastname ) || $lastname == "" ) 
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Last Name</div>";
	}
	
	if ( !isset( $email ) || $email == "" ) 
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Email</div>";
	}

	if ( !isset( $address ) || $address == "" ) 
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Address</div>";
	}

	if ( !isset( $city ) || $city == "" ) 
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for City</div>";
	}

	if ( !isset( $zipcode ) || $zipcode == "" ) 
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Zipcode</div>";
	}

	if ( !isset( $phone ) || $phone == "" ) 
	{
		$error = true;
		echo "<div class='errormsg'>Please enter something for Phone</div>";
	}

	if ( !$error ) //if no error
	{
		$fullAddress = $address . ", " . $city . ", " . $state . " " . $zipcode;

		$info["username"] = $username;
		$info["password"] = $password;
		$info["firstname"] = $firstname;
		$info["lastname"] = $lastname;
		$info["address"] = $fullAddress;
		$info["email"] = $email;
		$info["group_id"] = $groupID;
		$info["phone"] = $phone;
		
		$message = User::addUser($info);
		echo "<div class='errormsg'>$message</div>";
	}
	else
	{
		echo "<br/>";
		displayAddUserForm(); 
	}
}
else
{	
	displayAddUserForm();  
	// The forum still need fixing - there are bugs and it is not finished
	// Add group types to form:
	//  Make sure admins can add staff users and admin users
	//  Staff can only add borrower
}


function displayAddUserForm()
{
?>
	<div class="menubox"  >
	<div class="menuboxheader" >Adding User</div>
	<div class="menuboxcontent" >
	<center>
	<form action="adduser.php" method="post">
	<table >
		
		<tr>
			<td> UserName: </td>
			<td> <input type = "text" name = "username" /> </td>
		</tr>
		<tr>
			<td> Password: </td>
			<td> <input type = "password" name = "password" /> </td>
		</tr>
		<tr>
			<td> Re-type Password: </td>
			<td> <input type = "password" name = "password2" /> </td>
		</tr>
		<tr>
			<td> Group ID: </td>
			<td>
			  <select name = "group_id">
<?
	if ( isAdmin() )
	{
?>	
				<option value="1"> Admin </option>
				<option value="2"> Staff </option>
				<option value="3"> Borrower </option>
<?
	}
	else if ( isStaff() )
	{
?>
				<option value="3"> Borrower </option>
<?
	}
?>	
			  </select>
			</td>
		</tr>
		<tr>
			<td> Email: </td>
			<td> <input type = "text" name = "email" /> </td>
		</tr>
		<tr>
			<td> First Name: </td>
			<td> <input type = "text" name = "firstname" /> </td>
		</tr>
		<tr>
			<td> Last Name: </td>
			<td> <input type = "text" name = "lastname" /> </td>
		</tr>
		<tr>
			<td> Street Address: </td>
			<td> <input type = "text" name = "address" /> </td>
		</tr>
		<tr>
			<td> City: </td>
			<td> <input type = "text" name = "city" /> </td>
		</tr>
		<tr>
			<td> State: </td>
			<td>
			  <select name = "state">
				<option value="Alabama"> Alabama </option>
				<option value="Alaska"> Alaska </option>
				<option value="Arizona"> Arizona </option>
				<option value="Arkansas"> Arkansas </option>
				<option value="California"> California </option>
				<option value="Colorado"> Colorado </option>
				<option value="Connecticut"> Connecticut </option>
				<option value="Delaware"> Delaware </option>
				<option value="Florida"> Florida </option>
				<option value="Georgia"> Georgia </option>
				<option value="Hawaii"> Hawaii </option>
				<option value="Idaho"> Idaho </option>
				<option value="Illinois"> Illinois </option>
				<option value="Indiana"> Indiana </option>
				<option value="Iowa"> Iowa </option>
				<option value="Kansas"> Kansas </option>
				<option value="Kentucky"> Kentucky </option>
				<option value="Louisiana"> Louisiana </option>
				<option value="Maine"> Maine </option>
				<option value="Maryland"> Maryland </option>
				<option value="Massachusetts"> Massachusetts </option>
				<option value="Michigan"> Michigan </option>
				<option value="Minnesota"> Minnesota </option>
				<option value="Mississippi"> Mississippi </option>
				<option value="Missouri"> Missouri </option>
				<option value="Montana"> Montana </option>
				<option value="Nebraska"> Nebraska </option>
				<option value="Nevada"> Nevada </option>
				<option value="New Hampshire"> New Hampshire </option>
				<option value="New Jersey"> New Jersey </option>
				<option value="New Mexico"> New Mexico </option>
				<option value="New York"> New York </option>
				<option value="North Carolina"> North Carolina </option>
				<option value="North Dakota"> North Dakota </option>
				<option value="Ohio"> Ohio </option>
				<option value="Oklahoma"> Oklahoma </option>
				<option value="Oregon"> Oregon </option>
				<option value="Pennsylvania"> Pennsylvania </option>
				<option value="Rhode Island"> Rhode Island </option>
				<option value="South Carolina"> South Carolina </option>
				<option value="South Dakota"> South Dakota </option>
				<option value="Tennessee"> Tennessee </option>
				<option value="Texas"> Texas </option>
				<option value="Utah"> Utah </option>
				<option value="Vermont"> Vermont </option>
				<option value="Virginia"> Virginia </option>
				<option value="Washington"> Washington </option>
				<option value="West Virginia"> West Virginia </option>
				<option value="Wisconsin"> Wisconsin </option>
				<option value="Wyoming"> Wyoming </option>
			  </select>
			</td>
		</tr>
		<tr>
			<td> Zipcode: </td>
			<td> <input type = "text" name = "zipcode" maxlength="5" /> </td>
		</tr>
		<tr>
			<td> Phone Number: </td>
			<td> <input type = "text" name = "phone" /> </td>
		</tr>
		<tr>
			<td colspan = "2"> <center> <input type = "submit" value = "Submit" name = "addUser"/> </center> </td>
		</tr>
	</table>
	</form>
	</center>
	</div>
	</div>

	<?
}

require_once( "includes/footer.inc.php" );?>
