<?php
      session_start();
      $title = "Post Job";
      require('include/layout/output_buffer.php');
      require('include/database/pdo_connect.php');
      require_once('include/functions/employer.php');
      require_once('include/functions/jobs.php');
      function logged_in () {
          return (isset($_SESSION['id'])) ? true : false;
      }?>
      <?php
      require('include/layout/variables.php');
      require('include/layout/employerheader.php');
      require('include/layout/employer-profile-header.php');

      if (isset($_POST['post_job'])) {
          $employer                 = $employer_data['name'];
          $employer_email           = $employer_data['email'];
          $date_posted              = date('d M Y');
          $title                    = htmlentities(strip_tags(trim($_POST['title'])));
          $description              = htmlentities(strip_tags(trim($_POST['description'])));
          $qualification            = htmlentities(strip_tags(trim($_POST['qualification'])));
          $experience               = htmlentities(strip_tags(trim($_POST['experience'])));
          $type                     = htmlentities(strip_tags(trim($_POST['type'])));
          $gender                   = htmlentities(strip_tags(trim($_POST['gender'])));
          $deadline                 = htmlentities(strip_tags(trim($_POST['deadline'])));
          $category                 = htmlentities(strip_tags(trim($_POST['category'])));
          
            $question1 				= htmlentities(strip_tags(trim($_POST['question1'])));
            $question1_opt_0		= htmlentities(strip_tags(trim($_POST['question1_opt'][0])));
            $question1_opt_1 		= htmlentities(strip_tags(trim($_POST['question1_opt'][1])));
            $question1_opt_2 		= htmlentities(strip_tags(trim($_POST['question1_opt'][2])));
            $question1_opt_3 		= htmlentities(strip_tags(trim($_POST['question1_opt'][3])));
            $question1_points 		= htmlentities(strip_tags(trim($_POST['question1_points'])));
            $question1_valid_answer = htmlentities(strip_tags(trim($_POST['question1_valid_answer'])));
            
            $question2 				= htmlentities(strip_tags(trim($_POST['question2'])));
            $question2_opt_0		= htmlentities(strip_tags(trim($_POST['question2_opt'][0])));
            $question2_opt_1 		= htmlentities(strip_tags(trim($_POST['question2_opt'][1])));
            $question2_opt_2 		= htmlentities(strip_tags(trim($_POST['question2_opt'][2])));
            $question2_opt_3 		= htmlentities(strip_tags(trim($_POST['question2_opt'][3])));
            $question2_points 		= htmlentities(strip_tags(trim($_POST['question2_points'])));
            $question2_valid_answer = htmlentities(strip_tags(trim($_POST['question2_valid_answer'])));
            
            $question3 				= htmlentities(strip_tags(trim($_POST['question3'])));
            $question3_opt_0		= htmlentities(strip_tags(trim($_POST['question3_opt'][0])));
            $question3_opt_1 		= htmlentities(strip_tags(trim($_POST['question3_opt'][1])));
            $question3_opt_2 		= htmlentities(strip_tags(trim($_POST['question3_opt'][2])));
            $question3_opt_3 		= htmlentities(strip_tags(trim($_POST['question3_opt'][3])));
            $question3_points 		= htmlentities(strip_tags(trim($_POST['question3_points'])));
            $question3_valid_answer = htmlentities(strip_tags(trim($_POST['question3_valid_answer'])));

            echo var_dump($_POST);
          if (postJob (
            $employer, $employer_email, $description, $type, $experience, $date_posted, $deadline, $title, $qualification, $gender, $category,
            $question1,$question1_opt_0,$question1_opt_1,$question1_opt_2,$question1_opt_3,$question1_points,$question1_valid_answer,
            $question2,$question2_opt_0,$question2_opt_1,$question2_opt_2,$question2_opt_3,$question2_points,$question2_valid_answer,
            $question1,$question3_opt_0,$question3_opt_1,$question3_opt_2,$question3_opt_3,$question3_points,$question3_valid_answer
            )) {
              redirect('postjob.php?success');
          }else {
              redirect('postjob.php?error');
          }

      }
      
 ?>
     
<?php
if (isset($_GET['success'])) { ?>
  <div class="success">
      <h3 class="post-success">Thanks, <?=$employer_data['name']?>. Your Job has been successfully posted. You will receive notifications whenever anyone applies.</h3>
  </div>
<?php
}elseif (isset($_GET['error'])) {?>
  <div class="success">
      <h3 class="post-error">Ooops!!! Sorry, something went wrong. Please try again</h3>
  </div>
<?php
}
?>

<div class="container job-container">
    <div class="row">
        <div class="col-sm-2"></div>
        <div class="col-sm-8">
          <form class="form-group" action="" method="post" id="post_job">
              <div class="form-group">
                  <label for=""><b>Job Title</b></label>
                  <input type="text" name="title" id="title" value="" class="form-control" placeholder="add a job title here...">
              </div>
              <div class="form-group">
                  <label for=""><b>Job Description</b></label> <br>
                  <small style="color:red">(Describe the resposibilites of this job, required work experience, skills or education.)</small>
                  <textarea name="description" id="description" rows="8" cols="40" class="form-control" placeholder="Type here..."></textarea>
              </div>
              <div class="form-group">
                  <label for=""><b>Application Deadline</b></label>
                  <input type="date" name="deadline" id="deadline" value="" class="form-control">
              </div>
              <div class="form-group">
                  <label for=""><b>Qualification</b></label>
                  <input type="text" name="qualification" id="qualification" value ="" class="form-control" placeholder="Must be good in ...">
              </div>
              <div class="form-group">
                  <label for=""><b>Experience</b></label>
                  <input type="text" name="experience" id="experience" value ="" placeholder="Example: 1year in Marketing" class="form-control" >
              </div>
              <div class="form-group">
                  <label for=""><b>Job Type</b></label>
                  <select class="form-control" name="type" id="type">
                      <option selected="" disabled="">Please choose an option</option>
                      <option value="Full-time">Full-time</option>
                      <option value="Part-time">Part-time</option>
                      <option value="Internship">Internship</option>
                      <option value="Fresh Graduate">Fresh Graduate</option>
                  </select>
              </div>
              <div class="form-group">
                  <label for="reg_specialization"><b>Job Category</b></label>
                  <!-- <input type="text" name="reg_specialization" id="reg_specialization" value="" class="form-control" required=""> -->
                  <select name="category" id="category" class="form-control"><!-- Specialization Dropdown -->
                        <option value="option" selected="" disabled="">Select an option</option>
                        <option value="Accounting / Audit / Tax">Accounting / Audit / Tax</option>
                        <option value="Administration & Office Support">Administration &amp; Office Support</option>
                        <option value="Agriculture/Farming">Agriculture/Farming</option>
                        <option value="Banking / Finance / Insurance">Banking / Finance / Insurance</option>
                        <option value="Building Design/Architecture">Building Design/Architecture</option>
                        <option value="Construction">Construction</option>
                        <option value="Consulting/Business Strategy & Planning">Consulting/Business Strategy &amp; Planning</option>
                        <option value="Creatives(Arts, Design, Fashion)">Creatives(Arts, Design, Fashion)</option>
                        <option value="Customer Service">Customer Service</option>
                        <option value="Education/Teaching/Training">Education/Teaching/Training</option>
                        <option value="Engineering">Engineering</option>
                        <option value="Executive / Top Management">Executive / Top Management</option>
                        <option value="Healthcare / Pharmaceutical">Healthcare / Pharmaceutical</option>
                        <option value="Hospitality / Leisure / Travels">Hospitality / Leisure / Travels</option>
                        <option value="Human Resources">Human Resources</option>
                        <option value="Information Technology">Information Technology</option>
                        <option value="Legal">Legal</option>
                        <option value="Logistics / Transportation">Logistics / Transportation</option>
                        <option value="Manufacturing / Production">Manufacturing / Production</option>
                        <option value="Marketing / Advertising / Communications">Marketing / Advertising / Communications</option>
                        <option value="NGO/Community Services & Dev">NGO/Community Services &amp; Dev</option>
                        <option value="Oil&Gas / Mining / Energy">Oil &amp; Gas / Mining / Energy</option>
                        <option value="Project / Programme Management">Project / Programme Management </option>
                        <option value="QA&QC / HSE">QA&amp;QC / HSE</option>
                        <option value="Real Estate / Property">Real Estate / Property</option>
                        <option value="Research">Research</option>
                        <option value="Sales/Business Development">Sales/Business Development</option>
                        <option value="Supply Chain">Supply Chain / Procurement</option>
                        <option value="Telecommunications">Telecommunications</option>
                        <option value="Vocational Trade and Services">Vocational Trade and Services</option>
                 </select>
               </div>
              <div class="form-group">
                  <label for=""><b>Gender</b></label>
                  <select class="form-control" name="gender" id="gender">
                      <option selected="" disabled="">Please choose an option</option>
                      <option value="Male">Male</option>
                      <option value="Female">Female</option>
                      <option value="All">All</option>
                  </select>
                </div>
              
                                                                <!--============== QUESTONS FORM ==================-->

                             
                <div class="form-group">
                    <label for=""><b>HINT: <i>Test the right Applicant with a Quiz !</i></b></label>
                    
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
                            </
                    <br>
                </div>
                                                                <!-- =============END QUESTIONS FORM============== -->
              <button type="submit" name="post_job" form="post_job" class="btn btn-primary pull-right">Post this job</button>
          </form>
        </div>
        <div class="col-sm-2"></div>
  </div>
</div>



 <?php
    require('include/layout/page-header-footer.php');
    require('include/layout/footer.php');
 ?>
