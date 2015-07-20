<? require_once( "includes/header.inc.php" );

?>

<div class="menubox" >
	<div class="menuboxheader" >Daily Actions</div>
	
	<div class="menuboxcontent" >
		<a href ="checkout.php">Check Out</a> <br/>
		<a href ="checkin.php">Check In</a> <br/>
	</div>
</div>

<div class="menubox" >
	<div class="menuboxheader" > Book Actions </div>
	
	<div class="menuboxcontent" >
		<a href ="addbook.php">Add New Books</a> <br/>
		<a href ="search.php"> Find Books </a> <br/>
		<a href ="reserve.php">View Reserved List</a> <br/>
		<a href ="borrow.php">View Borrowed List </a> <br/>
		<a href ="history.php">View History List </a> <br/>
		<a href ="approvereview.php"> Approve New Review </a> <br/>
		<a href ="overdue.php"> View Over due List</a> <br/>
		<a href ="budget.php"> Budget</a> <br/>
	</div>
</div>

<?require_once( "includes/footer.inc.php" );