<? 
require_once( "includes/header.inc.php" );

if ( isset( $_POST["login"] ) )
{
	$username = pg_escape_string ( $_POST["username"] );
	$password = pg_escape_string ( $_POST["password"] );
	
	$error = false;
	if ( !isset($username) || $username=="")
	{
		$error = true;
		echo "<div class='errormsg'>Please enter username</div>";
	}
	
	if ( !isset($password) || $password=="")
	{
		$error = true;
		echo "<div class='errormsg'>Please enter password</div>";
	}
	if ( !$error && login( $username, $password ) )
	{
		redirect("index.php");
	}
	else if (!$error)
	{
		echo "<div class='errormsg'>The combination of username and password does not exist.</div><br/>";
		loginForm();
	}
	else
	{
		echo "<br/>";
		loginForm();
	}
}
else
{
	loginForm();
}

// if( isset($_POST["login"]))
// {
	// $username = pg_escape_string ( $_POST["username"] );
	// $password = pg_escape_string ( $_POST["password"] );
	// echo $username . $password;
	// $good = User::loginGood($username, $password);
	// if ( $good )
	// {
		// echo "<font color='red'> Login Successful </font></br>";
	// }
	// else
	// {
		// echo "<font color='red'> Login Failed </font></br>";
	// }
// }

function loginForm()
{
?>

<div class="menubox"  >
<div class="menuboxheader" >Login</div>
<div class="menuboxcontent" >
<center>
    <form action="login.php"  method="post">
        <table >
            <tr>
                <td> Username: </td>
                <td> <input type = "input" name = "username" /> </td>
            </tr>
            <tr>
                <td> Password: </td>
                <td> <input type = "password" name = "password" /> </td>
            </tr>
            <tr>
                <td colspan = "2"> <center> <input type = "submit" name="login" value = "Submit" /> </center> </td>
            </tr>
        </table>
    </form>
</center>
</body>
</div>
</div>

<?
}

require_once( "includes/footer.inc.php" );
?>