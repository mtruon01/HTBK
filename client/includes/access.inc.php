<?php

if ( isset( $_GET["act"] ) && $_GET["act"] == "logout" )
{
	logout();
}

function login( $username, $password )
{
	
	global $client;
	$password = md5( $password );
	$htmlData = "username=$username&password=$password";
	$command = "COMMAND GET VERIFYUSER $htmlData";
	$xml = $client->send($command);
	
	if ( isset( $xml->message ) )
	{
		return false;
	}
	
	$user = new User( $xml['id'] );
	
	$_SESSION['username'] = $user->username;
	$_SESSION['user_id'] = $user->user_id;
	$_SESSION['firstname'] = $user->firstname;
	$_SESSION['lastname'] = $user->lastname;
	$_SESSION['group_id'] = $user->group_id;
	$_SESSION['register_date'] = $user->register_date;
	$_SESSION['email'] = $user->email;
	$_SESSION['phone'] = $user->phone;
	$_SESSION['address'] = $user->address;
	$_SESSION['loggedIn'] =  true;
	
	return true;
	
}

function logout()
{
	session_unset();
    session_destroy();
}

function isLoggedIn()
{
	if ( isset( $_SESSION['loggedIn'] ) && $_SESSION['loggedIn'] == true )
	{
		return true;
	}
	
	return false;
}

function isAdmin()
{
	if ( !isLoggedIn() )
	{
		return false;
	}
	return $_SESSION['group_id'] == GROUP_ADMIN;
}

function isStaff()
{
	if ( !isLoggedIn() )
	{
		return false;
	}
	return $_SESSION['group_id'] == GROUP_STAFF;
}

function isBorrower()
{
	if ( !isLoggedIn() )
	{
		return false;
	}
	return $_SESSION['group_id'] == GROUP_BORROWER;
}

?>