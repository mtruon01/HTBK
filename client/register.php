<?
require_once( "includes/header.inc.php" );
if(isset($_POST["submit"]))
{
	if(isset($_POST["username"] && $_POST["password"] && $_POST["pwdCheck"] && $_POST["firstname"] && $_POST["lastname"] && $_POST["email"] && $_POST["phone"] && $_POST["address"] && $_POST["city"] && $_POST["zipcode"] && $_POST["state"])
	{
		if(!User::userExistByUsername($_POST["userName"]))
		{
			if($_POST["password"] == $_POST["pwdCheck"])
			{
			
			}
			else
			{
				echo "Re-typed password doesn't match";
			}
		}
		else
		{
			echo "Username is already existed";
		}
	}
	else
	{
		echo "Please return to the Register page and fill in all the information.";
	}
}
else
{
?>

<div class="menubox"  >
<div class="menuboxheader" >Registration</div>
<div class="menuboxcontent" >
<center>
<form action="register.php" method="post">
<table >
	
	<tr>
		<td> UserName: </td>
		<td> <input type = "text" name = "userName" /> </td>
	</tr>
	<tr>
		<td> Password: </td>
		<td> <input type = "password" name = "password" /> </td>
	</tr>
	<tr>
		<td> Re-type Password: </td>
		<td> <input type = "password" name = "pwdCheck" /> </td>
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
		<td> Street Adress: </td>
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
		<td> Email: </td>
		<td> <input type = "text" name = "email" /> </td>
	</tr> 
	<tr>
		<td colspan = "2"> <center> <input type = "submit" value = "Submit" name = "submit"/> </center> </td>
	</tr>
</table>
</form>
</center>
</div>
</div>

<?
}
?>

<?require_once( "includes/footer.inc.php" );?>