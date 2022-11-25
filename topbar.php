<style>
	.logo {
    margin: auto;
    font-size: 20px;
    background: white;
    padding: 7px 11px;
    border-radius: 50% 50%;
    color: #000000b3;
}
 .nav-item.active>.nav-link{
    color :white !important
  }
</style>

<!-- <nav class="navbar navbar-light fixed-top bg-primary" style="padding:0;min-height: 3.5rem">
  <div class="container-fluid mt-2 mb-2">
  	<div class="col-lg-12">
  		<div class="col-md-1 float-left" style="display: flex;">
  		
  		</div>
      <div class="col-md-4 float-left text-white">
        <large><b>E-Learning Realtime Quiz System</b></large>
      </div>
      <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNavDropdown" aria-controls="navbarNavDropdown" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
      </button>
      <div class="collapse navbar-collapse" id="navbarNavDropdown">
        <ul class="navbar-nav">
          <li class="nav-item active">
            <a class="nav-link" href="#">Home <span class="sr-only">(current)</span></a>
          </li>
          <li class="nav-item">
            <a class="nav-link" href="#">Features</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" href="#">Pricing</a>
          </li>
          <li class="nav-item dropdown">
            <a class="nav-link dropdown-toggle" href="#" id="navbarDropdownMenuLink" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
              Dropdown link
            </a>
            <div class="dropdown-menu" aria-labelledby="navbarDropdownMenuLink">
              <a class="dropdown-item" href="#">Action</a>
              <a class="dropdown-item" href="#">Another action</a>
              <a class="dropdown-item" href="#">Something else here</a>
            </div>
          </li>
        </ul>
      </div>
	  	<div class="col-md-2 float-right text-white">
	  		<a href="ajax.php?action=logout" class="text-white"><?php echo $_SESSION['login_name'] ?> <i class="fa fa-power-off"></i></a>
	    </div>
    </div>
  </div>
  
</nav> -->

<nav class="navbar navbar-expand-lg navbar-dark  fixed-top bg-primary">
  <a class="navbar-brand" href="./">E-Learning Realtime Quiz System</b></a>
  <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNavDropdown" aria-controls="navbarNavDropdown" aria-expanded="false" aria-label="Toggle navigation">
    <span class="navbar-toggler-icon"></span>
  </button>
  <div class="collapse navbar-collapse" id="navbarNavDropdown">
    <ul class="navbar-nav">
      <li class="nav-item">
        <a class="nav-link" href="index2.php?page=home">Home <span class="sr-only">(current)</span></a>
      </li>
      <?php if($_SESSION['login_type'] < 3): ?>

      <li class="nav-item">
        <a class="nav-link" href="index2.php?page=quiz_list">Quiz List</a>
      </li>
    <?php else: ?>
      <li class="nav-item">
        <a class="nav-link" href="index2.php?page=join_quiz">Join Quiz</a>
      </li>
      <li class="nav-item">
        <a class="nav-link" href="index2.php?page=history">History</a>
      </li>
    <?php endif; ?>
      <?php if($_SESSION['login_type'] == 1): ?>
      <li class="nav-item">
        <a class="nav-link" href="index2.php?page=users">Users</a>
      </li>
    <?php endif; ?>
      
    </ul>
  </div>
  <div class="text- dropdown mr-4">
        <a href="#" class="text-white dropdown-toggle"  id="account_settings" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false"><?php echo $_SESSION['login_name'] ?> </a>
        <div class="dropdown-menu" aria-labelledby="account_settings" style="left: -2.5em;">
          <a class="dropdown-item" href="javascript:void(0)" id="manage_my_account"><i class="fa fa-cog"></i> Manage Account</a>
          <a class="dropdown-item" href="ajax.php?action=logout"><i class="fa fa-power-off"></i> Logout</a>
        </div>
  </div>
</nav>
<script>
 $('.nav-link').each(function(){
    var _h = location.href
    if(_h.includes($(this).attr('href'))){
      $(this).addClass('active')
      $(this).closest('li').addClass('active')
    }
 })
 $('#manage_my_account').click(function(){
  uni_modal("Manage Account","manage_user.php?id=<?php echo $_SESSION['login_id'] ?>&mtype=own")
 })
</script>