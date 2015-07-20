<?

class HTBKClient {

	public $ip;
	public $port;
	public $endline = "\0";
	public $byteToRecieve = 1000000 ;
	public $timeout = 30; //seconds
	
	function __Construct( $ip, $port )  //Constructor
	{
		$this->ip = $ip;
		$this->port = $port;
		/*
		
		$fp = fsockopen ($this->ip, $this->port, $errno, $errstr, $this->timeout);
		
		if (!$fp) {
			fclose ($fp);
			die( "Server is down, try connecting to a different server" );
		}
		fputs ($fp, urlencode("CHECK CHECK CHECK") . $this->endline );
		$result = fgets ($fp,$this->byteToRecieve);
		//echo $result;
		fclose ($fp);
		*/
	}
	
	public function send( $str, $isRaw = false )
	{
		$str = urlencode ( $str );  //So it will send nicely
		$fp = fsockopen ($this->ip, $this->port, $errno, $errstr, $this->timeout);
		if (!$fp) {
			return "$errstr ($errno)<br>\n";
		}
		else {
		
			fwrite ($fp, $str . $this->endline );
			$result = fgets ($fp, $this->byteToRecieve);
			fclose ($fp);
		}

		if ( $isRaw )
		{
			return $result;
		}
		return simplexml_load_string($result);
	}
}




?>
