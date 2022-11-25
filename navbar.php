
<style>
	.collapse a{
		text-indent:10px;
	}
</style>
<nav id="sidebar" class='mx-lt-5 bg-dark' >
		
		<div class="sidebar-list">
				<?php if($_SESSION['login_type'] < 3): ?>

				<a href="index2.php?page=home" class="nav-item nav-home"><span class='icon-field'><i class="fa fa-home"></i></span> Home</a>
				<a  class="nav-item nav-quiz_list nav-collapse" data-toggle="collapse" href="#collapseExample" role="button" aria-expanded="false" aria-controls="collapseExample"><span class='icon-field'><i class="fa fa-th-list"></i></span> Quiz <span class="float-right"><i class="fa fa-angle-down"></i></span></a>
				<div class="collapse" id="collapseExample">
					<a href="index2.php?page=quiz_list" class="nav-item nav-quiz_list"> List</a>	
				</div>
				
			<?php elseif($_SESSION['login_type'] == 3): ?>
				<a href="index2.php?page=home" class="nav-item nav-home"><span class='icon-field'><i class="fa fa-home"></i></span> Home</a>
				<a href="index2.php?page=join_quiz" class="nav-item nav-home"><span class='icon-field'><i class="fa fa-sign-in-alt"></i></span> Join Quiz</a>
			<?php endif; ?>
		</div>

</nav>
<script>
	$('.nav_collapse').click(function(){
		console.log($(this).attr('href'))
		$($(this).attr('href')).collapse()
	})
	$('.nav-<?php echo isset($_GET['page']) ? $_GET['page'] : '' ?>').addClass('active')
</script>
