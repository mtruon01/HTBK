<? require_once( "includes/header.inc.php" );


if ( isset( $_POST['approve'] ) || isset( $_POST['deny'] ) )
{
	//print_r ( $_POST );
	$review_id = $_POST['reviewed_id'];
	
	if ( isset( $_POST['approve'] ) )
	{
		$command = "COMMAND UPDATE APPROVE_REVIEW review_id=$review_id";
	}
	else if ( isset( $_POST['deny'] ) )
	{
		$command = "COMMAND DELETE REVIEW reviewed_id=$review_id";
	}
	$xml = $client->send( $command );
	
	if ( $xml->message )
	{
		echo "<div class='errormsg'>$xml->message</div><br/>";
	}
	else
	{
		echo "<div class='errormsg'>Error!</div><br/>";
	}
}


$xml = $client->send( "COMMAND LOOKUP REVIEWED ALL" );

$haveNewReview = false;
foreach( $xml as $review )
{
	if ( $review->approve == 0 )
	{
		$haveNewReview = true;
	}
}

?>
<div class="menubox" >
	<div class="menuboxheader" >Reviews</div>
	<div class="menuboxcontent" > <center>
		
<?

if ( $xml->getName() == "error" )
{
	echo "<div class='errormsg'>$xml->message</div><br/>";
}
else
{
	?>
	<table border="1" class="tablelist" >
		<tr>
			<th>Title</th>
			<th>User Id</th>
			<th>Name</th>
			<th>Rating</th>
			<th>Review</th>
			<th>Review Date</th>
			<th>Approve/Deny</th>
		</tr>
		<?
		foreach( $xml as $review )
		{
			if ( $review->approve != 0 )
			{
				continue;
			}
			echo "<tr>";
			echo "<td>$review->title</td>";
			echo "<td>$review->user_id</td>";
			echo "<td>$review->firstname</td>";
			echo "<td>$review->rating</td>";
			echo "<td>$review->review</td>";
			echo "<td>$review->reviewed_date</td>";
			$review_id = (string)$review->reviewed_id['id'];
			?><td>
			<form action="approvereview.php" method="post" >
				<input type="hidden" name="reviewed_id" value="<?=$review_id?>" />
				<input type='submit' name='approve' value='Approve' />
				<input type='submit' name='deny' value='Deny' />
			</form></td>
			<?
			echo "<tr>";
		}
		?>
	</table>
	
	<?
}

?>
</center>
</div></div>

<?

require_once( "includes/footer.inc.php" );