<?
require_once( "includes/header.inc.php" );

$bookAry = Book::searchBookKeyword( "" );

if ( count( $bookAry ) == 0 ) {
	echo "<div class='errormsg'>No result!</div><br/>";
}
else {
$totalPrice = 0;
?>
<div class="menubox" >
	<div class="menuboxheader" >Budget</div>
	<div class="menuboxcontent" >
	<center>
	<table  border=1 class="tablelist" >
		<tr>
			<th>Title</th>
			<th>ISBN</th>
			<th>Quantity</th>
			<th>Price</th>
		<tr/>
<?
	foreach( $bookAry as $bookObject )
	{
		$totalPrice += $bookObject->price * $bookObject->quantity;
		$price = money_format('%i', $bookObject->price);
		echo "<tr>";
		echo "<td>$bookObject->title</td>";
		echo "<td ALIGN=\"CENTER\">$bookObject->isbn</td>";
		echo "<td ALIGN=\"CENTER\">$bookObject->quantity</td>";
		echo "<td ALIGN=\"RIGHT\">\$$price</td>";
		echo "</tr>";
	}
	$totalPrice = money_format('%i', $totalPrice);
?>
	<tr>
		<td COLSPAN="4" ALIGN="RIGHT">Total Price: $<?=$totalPrice?></td>
	</tr>
	</table>
	</center>
	</div>
	</div>
<?
}
require_once( "includes/footer.inc.php" );
?>