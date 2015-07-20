<?
function redirect($url)
{
echo "<meta http-equiv='Refresh' content='0;url=$url' />";
}

function checkIn ($book_id, $user_id)
{
	Global $client;
	
	$command = "COMMAND CHECKIN book_id=$book_id&user_id=$user_id";
	$xml = $client->send($command);
	return (string)$xml->message;
}

function checkOut ($book_id, $user_id)
{
	Global $client;
	
	$command = "COMMAND CHECKOUT book_id=$book_id&user_id=$user_id";
	$xml = $client->send($command);
	return (string)$xml->message;
}


?>