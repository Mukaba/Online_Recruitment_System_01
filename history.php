<?php include('include/database/pdo_connect.php') ?>
<?php 
	$qquery = $conn->query("SELECT * FROM questions where qid in (SELECT quiz_id FROM answers a  where user_id = ".$_SESSION['login_id'].") order by order_by asc");

		while($row=$qquery->fetch_array()){
			$q_points[$row['id']] = $row['points'];
		}
		$answers = $conn->query("SELECT * FROM answers where user_id = ".$_SESSION['login_id']);
		while($row=$answers->fetch_array()){
			if(!isset($ppoints[$row['quiz_id']]))
			$ppoints[$row['quiz_id']] = 0;
		if(!isset($right[$row['quiz_id']]))
			$right[$row['quiz_id']] = 0;
			$ppoints[$row['quiz_id']] += ($q_points[$row['question_id']] * $row['is_right']);
			if($row['is_right'] == 1)
				$right[$row['quiz_id']] += 1;

		}
?>
<div class="container-fluid">
	<div class="col-lg-12">
		<div class="card">
			<div class="card-body">
				<p class="text-center"><b>My Quiz History</b></p>
				<hr>
				<table class="table table-bordered">
					<colgroup>
						<col width="10%">
						<col width="20%">
						<col width="30%">
						<col width="15%">
						<col width="20%">
						<col width="5%">
					</colgroup>
					<thead>
						<tr>
							<th>#</th>
							<th>Quiz</th>
							<th>Participant</th>
							<th>Correct Item/s</th>
							<th>Score</th>
							<th></th>
						</tr>
					</thead>
					<tbody>
						<?php
						$i = 1;
							$participants = $conn->query("SELECT distinct(a.quiz_id),u.name as uname,q.title as qtitle FROM answers a inner join users u on  u.id = a.user_id inner join quiz_list q on q.id = a.quiz_id where a.user_id = ".$_SESSION['login_id']);
							while($row=$participants->fetch_assoc()):
						?>
						<tr>
							<td><center><?php echo $i++ ?></center></td>
							<td><?php echo ucwords($row['qtitle']) ?></td>
							<td><?php echo ucwords($row['uname']) ?></td>
							<td><center><?php echo $right[$row['quiz_id']] ?></center></td>
							<td><center><?php echo $ppoints[$row['quiz_id']] ?></center></td>
							<td><center><a href="index2.php?page=my_quiz_result&quiz=<?php echo md5($row['quiz_id']) ?>" class="btn btn-sm btn-outline-primary">View</a></center></td>

						</tr>
					<?php endwhile; ?>
					</tbody>
				</table>
			</div>
		</div>
	</div>
</div>

