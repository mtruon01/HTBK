<? session_start(); // MUST be before any HTML or text is send

// ============= INCLUDES ============

require_once( "includes/config.inc.php" ); //Must go first for mysql connection
require_once( "includes/HTBKClient.class.php" );
$client =  new HTBKClient( "localhost", 33334 );
require_once( "includes/User.class.php" ); 
require_once( "includes/Book.class.php" );
require_once( "includes/Genre.class.php" );
require_once( "includes/constant.php" );
require_once( "includes/access.inc.php" ); 
require_once( "includes/function.php" );

?><link rel="stylesheet" type="text/css" href="includes/htbk.css" /><?  //CSS

// ============= END INCLUDES ================



//How to build query
$htmlData = array( 'user_id'=>'3242',
					'username'=>'tommy',
					'firstname'=>'tom',
					'lastname'=>'tong');

$htmlQuery = http_build_query($htmlData); //user_id=3242&username=tommy&firstname=tom&lastname=tong

// Start the HTBK client connection

//$resultXML = $client->send( "COMMAND GET USERNAME $htmlQuery" );  // This is a example of how you get XML


?>

<title>HTBK - Library System</title> 
<div id="logo">
	<h1><a href="index.php"> HTBK </a></h1>
	<p>Designed By the best people in the world</p>
	<? if ( isLoggedIn() ) { ?>
		<p style="float: right;" >Welcome <?=$_SESSION['firstname']. " " . $_SESSION['lastname']?></p>
	<?}?>
</div>

<div id="menu">
	<ul id="main">
		
		
		<li> <a href ="index.php">Home</a> </li>
		
		<? if ( !isLoggedIn() ) { ?>
			<li> <a href ="login.php">Log In</a> </li>
		<? } ?>
		
       		<li> <a href ="search.php">Search Book</a> </li>
	
		<? if ( isStaff() || isAdmin() ) { ?>
			<li> <a href ="bookmanagement.php">Book Management</a> </li>
			<li> <a href ="usermanagement.php">User Management</a> </li>
			<li> <a href ="user.php?user_id=<?=$_SESSION['user_id']?>">Edit Account</a> </li>
			<li> <a href ="budget.php">Budget</a> </li>
			
        <?}?>

		<? if ( isBorrower() ) { ?>
			<li> <a href ="reserve.php">View Reserved</a> </li>
			<li> <a href ="borrow.php">View Borrowed</a> </li>
			<li> <a href ="history.php">View History</a> </li>
			<li> <a href ="user.php">Account</a> </li>
        <?}?>
			<li> <a href ="contact.php">Contact us</a></li>
		
		<? if ( isLoggedIn() ) {?>
		<li> <a href ="index.php?act=logout">Logout</a></li>
		<?}?>
		
	</ul>
</div>

<div id="banner">&nbsp;</div>

<div id="wrapper">
<!-- start page -->
<div id="page">



