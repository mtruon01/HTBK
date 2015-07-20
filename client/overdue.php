<? require_once( "includes/header.inc.php" );

if ( isset($_GET['act']) && $_GET['act'] == "del" && isset($_GET['reserve_id']) )
{
	echo "<pre>";
	//print_r( $_GET );
	echo "</pre>";
	$reserve_id = pg_escape_string( $_GET['reserve_id'] );
	$command = "COMMAND DELETE RESERVE reserved_id=$reserve_id";
	$xml = $client->send( $command );
	
	if ( isset($xml->message ) )
	{
		echo "<div class='errormsg'>$xml->message</div><br/>";
	}
	else
	{
		echo "<div class='errormsg'>Delete successful!</div><br/>";
	}
}

displayOverdueReserve();
displayOverdueBorrow();

function displayOverdueReserve( )
{
	global $client;
	$command = "COMMAND LOOKUP RESERVED ALL";
	$xml = $client->send( $command );
	echo "<pre>";
	//print_r( $xml );
	echo "</pre>";
	
	if ( !haveOverLimitForReserve( $xml ) )
	{
		?>
		<div class="menubox" >
			<div class="menuboxheader" >Overdue Reserve List</div>
			<div class="menuboxcontent" >
				<center>None</center>
			</div>
		</div>
		<?
		return;
	}
	
	?>
	<div class="menubox" >
	<div class="menuboxheader" >Overdue Reserve List</div>
	<div class="menuboxcontent" >
	<center>
	<table  border=1 class="tablelist" >
		<tr>
			<th>Title</th>
			<th>User Id</th>
			<th>First Name </th>
			<th>Last Name </th>
			<th>Reserve Date</th>
			<th>Days Overdue</th>
			<th>Remove Reserve</th>
		<tr/>
		<?
		
		
		foreach( $xml as $reserve ) 
		{
			$reserve_date = (string)$reserve->reserved_date;
			$today_date = date( "Y-m-d" );
			$secDiff = (strtotime( $today_date) - strtotime($reserve_date) );
			$dayDiff = floor( $secDiff / 86400 );
			$daysOver = $dayDiff - DAY_RESERVE_LIMIT;
			
			//echo "$today_date - $reserve_date =  $dayDiff<Br/>";
			//echo "<pre>";
			//print_r( $reserve );
			//echo "</pre>";
	
			$r_id = $reserve->reserved_id['id'];
			if ( $dayDiff >= DAY_RESERVE_LIMIT )
			{
				echo "<tr>";
				echo "<td>$reserve->title</td>";
				echo "<td ALIGN=\"CENTER\">$reserve->user_id</td>";
				echo "<td>$reserve->firstname</td>";
				echo "<td>$reserve->lastname</td>";
				echo "<td>$reserve->reserved_date</td>";
				echo "<td ALIGN=\"CENTER\">$daysOver</td>";
				echo "<td ALIGN=\"CENTER\"><a href='overdue.php?act=del&reserve_id=$r_id'>Remove</a></td>";
				echo "</tr>";
			}
		}
		?>

	</table>
	</center>
	</div>
	</div>
	<?
}

function displayOverdueBorrow( )
{
	global $client;
	$command = "COMMAND LOOKUP BORROWED ALL";
	$xml = $client->send( $command );
	echo "<pre>";
	//print_r( $xml );
	echo "</pre>";
	
	if ( !haveOverLimitForBorrow( $xml ) )
	{
		?>
		<div class="menubox" >
			<div class="menuboxheader" >Overdue Borrowed List</div>
			<div class="menuboxcontent" >
				<center>None</center>
			</div>
		</div>
		<?
		return;
	}
	
	?>
	<div class="menubox" >
	<div class="menuboxheader" >Overdue Borrowed List</div>
	<div class="menuboxcontent" >
	<center>
	<table  border=1 class="tablelist" >
		<tr>
			<th>Title</th>
			<th>User Id</th>
			<th>First Name </th>
			<th>Last Name </th>
			<th>Borrowed Date</th>
			<th>Days Overdue</th>
			<th>Amount Owed</th>
		<tr/>
		<?
		
		
		foreach( $xml as $borrowed ) 
		{
			$borrowed_date = (string)$borrowed->borrowed_date;
			$today_date = date( "Y-m-d" );
			$secDiff = (strtotime( $today_date) - strtotime($borrowed_date) );
			$dayDiff = floor( $secDiff / 86400 );
			$daysOver = $dayDiff - DAY_BORROW_LIMIT;
			$owe = money_format('%i', $daysOver * COST_PER_DAY);
			//echo "$today_date - $borrowed_date =  $dayDiff<Br/>";
			
			if ( $dayDiff >= DAY_BORROW_LIMIT )
			{
				echo "<tr>";
				echo "<td>$borrowed->title</td>";
				echo "<td ALIGN=\"CENTER\">$borrowed->user_id</td>";
				echo "<td>$borrowed->firstname</td>";
				echo "<td>$borrowed->lastname</td>";
				echo "<td>$borrowed->borrowed_date</td>";
				echo "<td ALIGN=\"CENTER\">$daysOver</td>";
				echo "<td ALIGN=\"RIGHT\">\$$owe</td>";
				echo "</tr>";
			}
		}
		?>

	</table>
	</center>
	</div>
	</div>
	<?
}

function haveOverLimitForBorrow( $xml )
{
	if ( isset( $xml->message ) )
	{
		return false;
	}
	
	foreach( $xml as $borrowed ) 
	{
		$borrowed_date = (string)$borrowed->borrowed_date;
		$today_date = date( "Y-m-d" );
		$secDiff = (strtotime( $today_date) - strtotime($borrowed_date) );
		$dayDiff = floor( $secDiff / 86400 );
		$daysOver = $dayDiff - DAY_BORROW_LIMIT;
		if ( $dayDiff >= DAY_BORROW_LIMIT )
		{
			return true;
		}
	}
	return false;
}

function haveOverLimitForReserve( $xml )
{
	if ( isset( $xml->message ) )
	{
		return false;
	}
	
	foreach( $xml as $reserve ) 
	{
		$reserve_date = (string)$reserve->reserved_date;
		$today_date = date( "Y-m-d" );
		$secDiff = (strtotime( $today_date) - strtotime($reserve_date) );
		$dayDiff = floor( $secDiff / 86400 );
		$daysOver = $dayDiff - DAY_RESERVE_LIMIT;
		if ( $dayDiff >= DAY_RESERVE_LIMIT )
		{
			return true;
		}
	}
	return false;
}


require_once( "includes/footer.inc.php" );
?>