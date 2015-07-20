<?
// Use tomtest.php to test ur code
require_once( "includes/config.inc.php" );
require_once( "includes/HTBKClient.class.php" );

class User
{
	public $user_id;
	public $username;
	public $firstname;
	public $lastname;
	public $group_id;
	public $register_date;
	public $email;
	public $phone;
	public $address;
	public $c;
	
	function __construct( $user_id ) //Constructor
	{
		Global $client;
		$this->c = $client;
		$this->getInfo( $user_id );
	}

	public function getInfo($user_id)
	{
	
		$command = "COMMAND GET USER user_id=$user_id";
		$xml = $this->c->send($command);
		
		echo "<pre>";
		//echo $xml;
		echo "</pre>";
		
		$this->user_id = (string)$xml['id'];
		$this->username = (string)$xml->username;
		$this->firstname = (string)$xml->firstname;
		$this->lastname= (string)$xml->lastname;
		$this->group_id = (string)$xml->group_id;
		$this->register_date = (string)$xml->register_date;
		$this->email = (string)$xml->email;
		$this->address = (string)$xml->address;
		$this->phone = (string)$xml->phone;		
	}
	
	
	public static function userExistById( $user_id )  //return true if exist
	{
		
		
		Global $client;
		$command = "COMMAND GET USER user_id=$user_id";
		$xml = $client->send($command);
		return ($xml->getName() != "error");

	}
	
	
	public static function verifyUser( $username, $plain_password ) // return true if login is good //Password is not md5ed
	{
		Global $client;
		$password = md5($plain_password);

		
		$htmlData = "username=$username&password=$password";
		$command = "COMMAND GET VERIFYUSER $htmlData";
		$xml = $client->send($command);
		return ($xml->getName() != "error");
	}

	public static function addUser($info)
	{ 
		Global $client;
		
		$info["password"] = md5($info["password"]);
		$htmlData = http_build_query($info);
		
		$command = "COMMAND INSERT USER $htmlData";
		$xml = $client->send($command);
		
		if($xml->getName() != "error")
		{
			return "Added user successful!";
		}
		else
		{
			return $xml->message;
		}
	}
}

?>