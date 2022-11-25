<?php

if (isset($_POST['submit2'])) {

	$date_posted 			= date('d M Y');
	$question 				= htmlentities(strip_tags(trim($_POST['question'])));
	$question_opt[0]		= htmlentities(strip_tags(trim($_POST['question_opt[0]'])));
	$question_opt[1] 		= htmlentities(strip_tags(trim($_POST['question_opt[1]'])));
	$question_opt[2] 		= htmlentities(strip_tags(trim($_POST['question_opt[2]'])));
	$question_opt[3] 		= htmlentities(strip_tags(trim($_POST['question_opt[3]'])));
	$points 				= htmlentities(strip_tags(trim($_POST['points'])));
	$valid_answer 			= htmlentities(strip_tags(trim($_POST['valid_answer'])));

	require('include/database/pdo_connect.php');
    $query = $pdo->prepare('INSERT INTO questions (questions, points, correct_answer) VALUES (:questions, :points, :valid_answer)');
    $query->bindParam(':questions' , $question);
    $query->bindParam(':points' , $points);
    $query->bindParam(':valid_answer' , $valid_answer);

	// echo var_dump($question_opt);
	
	
    if ($query->execute()) {

        // return true;
		echo 'Success';
		$query_id = $pdo->prepare('SELECT MAX(id) FROM questions ');
		$question_id = $query_id->execute();


		
		foreach($question_opt As $oklm_){
			$query_ = $pdo->prepare('INSERT INTO questions (question_id, answer) VALUES (:question_id, :answer)');
			$query_->bindParam(':question_id' , $question_id);
			$query_->bindParam(':answer' , $oklm_);
			$query_->execute();
		}

    }else {
        // return false;
		echo 'error';
    }
}
	// if (postJob ($employer, $employer_email, $description, $type, $experience, $date_posted, $deadline, $title, $qualification, $gender, $category)) {
	// 	redirect('postjob.php?success');
	// }else {
	// 	redirect('postjob.php?error');
	// }

// }

?>

<div class="container-fluid">

	<button class="btn btn-primary bt-sm" id="new_question"><i class="fa fa-plus"></i>	Add Question</button>
		
	<br><br><br>
	<div class="row">
		<!-- ==============Questions du test================= -->
		<div class="" id="myform" style="display:none">
		<form id="question-frm" method="post">
			<div class="modal-header">  <h5 class="modal-title">Please fulfil question provide right answers</h5>  </div>
				<div class="modal-body">
					<div class="container-fluid">
							<!-- ===== -->
							<div class="questions_item">
                                                                
								<div class="row form-group">
									<label>Question 1</label>
									<!-- <input type="hidden" name="qid" value="17">
									<input type="hidden" name="id" value=""> -->
									<textarea rows="3" name="question1" required="required" class="form-control" control-id="ControlID-1"></textarea>
								</div>
									<label>Options:</label>
								<div class="row form-group">
									<div class="col-md-6">
										<textarea rows="2" name="question1_opt[0]" required="" class="form-control" control-id="ControlID-3"></textarea>
										<label> <small> Answer 1</small></label>
										<br>
										<textarea rows="2" name="question1_opt[1]" required="" class="form-control" control-id="ControlID-5"></textarea>
										<label><small> Answer 2</small></label>
									</div>
									<div class="col-md-6">
										<textarea rows="2" name="question1_opt[2]" required="" class="form-control" control-id="ControlID-7"></textarea>
										<label><small> Answer 3</small></label>
										<br>
										<textarea rows="2" name="question1_opt[3]" required="" class="form-control" control-id="ControlID-9"></textarea>
										<label> <small> Answer 4</small></label>
									</div>
								</div>
								<div class="row form-group">
									<div class="col-md-6">
										<label>Question Points</label>
										<input type="number" name="question1_points" class="form-control text-right" value="" control-id="ControlID-2">
									</div>
									<div class="col-md-6">
										<label>Correct Answer</label>
										<select  name="question1_valid_answer" required="" class="form-control text-right" value="" control-id="ControlID-4">
											<option value="1">Answer 1</option>
											<option value="2">Answer 2</option>
											<option value="3">Answer 3</option>
											<option value="4">Answer 4</option>
										</select>
									</div>
								</div>
                            </div>
								
                             <div class="questions_item">
                                                                
								<div class="row form-group">
									<label>Question 2</label>
									<!-- <input type="hidden" name="qid" value="17">
									<input type="hidden" name="id" value=""> -->
									<textarea rows="3" name="question2" required="required" class="form-control" control-id="ControlID-1"></textarea>
								</div>
									<label>Options:</label>
								<div class="row form-group">
									<div class="col-md-6">
										<textarea rows="2" name="question2_opt[0]" required="" class="form-control" control-id="ControlID-3"></textarea>
										<label> <small> Answer 1</small></label>
										<br>
										<textarea rows="2" name="question2_opt[1]" required="" class="form-control" control-id="ControlID-5"></textarea>
										<label><small> Answer 2</small></label>
									</div>
									<div class="col-md-6">
										<textarea rows="2" name="question2_opt[2]" required="" class="form-control" control-id="ControlID-7"></textarea>
										<label><small> Answer 3</small></label>
										<br>
										<textarea rows="2" name="question2_opt[3]" required="" class="form-control" control-id="ControlID-9"></textarea>
										<label> <small> Answer 4</small></label>
									</div>
								</div>
								<div class="row form-group">
									<div class="col-md-6">
										<label>Question Points</label>
										<input type="number" name="question2_points" class="form-control text-right" value="" control-id="ControlID-2">
									</div>
									<div class="col-md-6">
										<label>Correct Answer</label>
										<select  name="question2_valid_answer" required="" class="form-control text-right" value="" control-id="ControlID-4">
											<option value="1">Answer 1</option>
											<option value="2">Answer 2</option>
											<option value="3">Answer 3</option>
											<option value="4">Answer 4</option>
										</select>
									</div>
								</div>
                            </div>
                            
                            <div class="questions_item">
                                                                
                                <div class="row form-group">
                                    <label>Question 3</label>
                                    <!-- <input type="hidden" name="qid" value="17">
                                    <input type="hidden" name="id" value=""> -->
                                    <textarea rows="3" name="question3" required="required" class="form-control" control-id="ControlID-1"></textarea>
                                </div>
                                    <label>Options:</label>
                                <div class="row form-group">
                                    <div class="col-md-6">
                                        <textarea rows="2" name="question3_opt[0]" required="" class="form-control" control-id="ControlID-3"></textarea>
                                        <label> <small> Answer 1</small></label>
                                        <br>
                                        <textarea rows="2" name="question3_opt[1]" required="" class="form-control" control-id="ControlID-5"></textarea>
                                        <label><small> Answer 2</small></label>
                                    </div>
                                    <div class="col-md-6">
                                        <textarea rows="2" name="question3_opt[2]" required="" class="form-control" control-id="ControlID-7"></textarea>
                                        <label><small> Answer 3</small></label>
                                        <br>
                                        <textarea rows="2" name="question3_opt[3]" required="" class="form-control" control-id="ControlID-9"></textarea>
                                        <label> <small> Answer 4</small></label>
                                    </div>
                                </div>
                                <div class="row form-group">
                                    <div class="col-md-6">
                                        <label>Question Points</label>
                                        <input type="number" name="question3_points" class="form-control text-right" value="" control-id="ControlID-2">
                                    </div>
                                    <div class="col-md-6">
                                        <label>Correct Answer</label>
                                        <select  name="question3_valid_answer" required="" class="form-control text-right" value="" control-id="ControlID-4">
                                            <option value="1">Answer 1</option>
                                            <option value="2">Answer 2</option>
                                            <option value="3">Answer 3</option>
                                            <option value="4">Answer 4</option>
                                        </select>
                                    </div>
                                </div>
                            </div>
							<!-- ===== -->
							
					</div>
				</div>
				<div class="modal-footer">
					<button type="submit" class="btn btn-primary bt-sm" id="submit2">Save</button>
					<button type="reset" class="btn btn-secondary" id="cancelbtn" data-dismiss="modal">Cancel</button>
				</div>
			</form>
		</div>
		
	</div>
</div>