<?php include 'include/database/pdo_connect.php' ?> 
<style>
	#uni_modal .modal-footer{
		display: none;
	}
	#uni_modal .modal-footer.display{
		display: block !important;
	}
	.answer {
	    font-size: 24px;
	    font-weight: bolder;
	    padding: 3px !important;
	}
</style>
<?php 
	$qry = $conn->query("SELECT * FROM quiz_list where id = '".$_GET['qid']."' ")->fetch_array();
	$status = array("pending","waiting","on-going","done");
	?>
<div class="container-fluid">
	<div class="col-md-12 alert alert-primary">	
			<p>Title: <?php echo $qry['title'] ?></p>
			<p>Code: <?php echo md5($qry['id']) ?></p>
			<p>Status: <?php echo ucwords($status[$qry['status']])  ?></p>
			<p>Score: <span id="score"></span></p>
			
		</div>
	<div class="col-md-12">
		<div class="card mr-4">
			<div class="card-header">
				Questions
			</div>
			<div class="card-body">
				<div class="container-fluid">
					<div class="row">
						<p><b>LEGEND :</b> <span class="legend badge badge-primary">&nbsp;</span> Participant selected option.  <span class="legend badge badge-success">&nbsp;</span> Correct Answer.</p>
					</div>
				<?php
					$i = 1;
							$ppoints = 0;
					$qquery = $conn->query("SELECT * FROM questions where qid = '".$_GET['qid']."' order by order_by asc");
					if($qquery->num_rows > 0){
						$qry2 = $conn->query("SELECT * FROM question_opt where question_id in (SELECT id FROM questions where qid = '".$_GET['qid']."' ) ");
						while($row = $qry2->fetch_assoc()){
							$opt[$row['question_id']][] = $row;
							$ans[$row['id']] = 0;
						}
						while($row = $qry2->fetch_assoc()){
							$opt[$row['question_id']][] = $row;
						}
						while($row=$qquery->fetch_array()){
							$q_points[$row['id']] = $row['points'];
						}
						$answers = $conn->query("SELECT * FROM answers where quiz_id = '".$_GET['qid']."' and user_id =".$_GET['uid']);
						while($row=$answers->fetch_array()){
							$ans[$row['question_id']] = $row['option_id'];
							$ppoints += ($q_points[$row['question_id']] * $row['is_right']);
						}
					}
					$qquery = $conn->query("SELECT * FROM questions where qid = '".$_GET['qid']."' order by order_by asc");
					while($row=$qquery->fetch_array()){
						?>
						<div class="card mb-3">
						<div class="card-body">
							Question <?php echo $i++ ?> :<?php echo $row['question'] ?><br>
							<hr>
							<p><small><i><b>Points:<?php echo $row['points'] ?></b></i></small></p>
							<p><b>Options:</b></p>
							<div class="row">
							<?php foreach($opt[$row['id']] as $orow): ?>
							<div class="col-md-6 mb-2">
								<div class="card <?php echo $ans[$row['id']] == $orow['id'] ? "bg-primary" : ($orow['is_right'] ==1 ? "bg-success" : "") ?>">
									<div class="card-body <?php echo $ans[$row['id']] == $orow['id'] ? "text-white" : ($orow['is_right'] ==1 ? "text-white" : "") ?>">
										<p><?php echo $ans[$row['id']] == $orow['id'] ? ($orow['is_right'] == 1 ? "<i class='fa fa-check badge-success p-1  text-white answer'></i>" : "<i class='fa fa-times  badge-danger p-1 text-white answer'></i>"):'' ?> <b><?php echo $orow['option_txt'] ?></b></p>
									</div>
								</div>
							</div>

							<?php endforeach; ?>
							</div>
							
						</div>
						</div>
				<?php
					}
				?>
				</div>
		</div>
	</div>
</div>
</div>
<div class="modal-footer display">
        <button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
  </div>
<script>
	$('#score').html('<?php echo $ppoints ?>')
</script>