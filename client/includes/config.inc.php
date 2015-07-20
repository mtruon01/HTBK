<?

//$host = "128.238.64.5"; ///PDC
$host = "127.0.0.1";
$port = "5432";
$dbname = "hchen08";
$dbuser = "hchen08";
$dbpassword = "herman285";

$dbconn = pg_connect( "host=$host port=$port dbname=$dbname user=$dbuser password=$dbpassword" );





?>