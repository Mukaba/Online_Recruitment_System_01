<?php
      session_start();
      $title = "Post Job";
      require('../include/layout/output_buffer.php');
      require('../include/database/pdo_connect.php');
      require_once('../include/functions/employer.php');
      require_once('../include/functions/jobs.php');
      function logged_in () {
          return (isset($_SESSION['id'])) ? true : false;
      }
      require('../include/layout/variables.php');
      require('../include/layout/employerheader.php');

      if (isset($_POST['post_job'])) {
          $employer = $employer_data['name'];
          $employer_email = $employer_data['email'];
          $date_posted = date('d M Y');
          $title = htmlentities(strip_tags(trim($_POST['title'])));
          $description = htmlentities(strip_tags(trim($_POST['description'])));
          $qualification = htmlentities(strip_tags(trim($_POST['qualification'])));
          $experience = htmlentities(strip_tags(trim($_POST['experience'])));
          $type = htmlentities(strip_tags(trim($_POST['type'])));
          $gender = htmlentities(strip_tags(trim($_POST['gender'])));
          $deadline = htmlentities(strip_tags(trim($_POST['deadline'])));
          $category = htmlentities(strip_tags(trim($_POST['category'])));

          if (postJob ($employer, $employer_email, $description, $type, $experience, $date_posted, $deadline, $title, $qualification, $gender, $category)) {
              redirect('postjob.php?success');
          }else {
              redirect('postjob.php?error');
          }

      }
 ?>
<div class="jumbotron">
    Lorem ispumLorem ispumLorem ispum Lorem ispum Lorem ispum Lorem ispum Lorem ispumLorem ispumLorem ispum Lorem ispum Lorem ispum Lorem ispum Lorem ispumLorem ispumLorem ispum Lorem ispum Lorem ispum Lorem ispum
</div>
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
          <form class="form-group" action="" method="post">
              <div class="form-group">
                  <label for=""><b>Job Title</b></label>
                  <input type="text" name="title" id="title" value="" class="form-control">
              </div>
              <div class="form-group">
                  <label for=""><b>Job Description</b></label> <br>
                  <small style="color:red">(Describe the resposibilites of this job, required work experience, skills or education.)</small>
                  <textarea name="description" id="description" rows="8" cols="40" class="form-control"></textarea>
              </div>
              <div class="form-group">
                  <label for=""><b>Application Deadline</b></label>
                  <input type="text" name="deadline" id="deadline" value="" class="form-control">
              </div>
              <div class="form-group">
                  <label for=""><b>Qualification</b></label>
                  <input type="text" name="qualification" id="qualification" value ="" class="form-control">
              </div>
              <div class="form-group">
                  <label for=""><b>Experience</b></label>
                  <input type="text" name="experience" id="experience" value ="" placeholder="Example: 1year in Marketing" class="form-control">
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
              <button type="submit" name="post_job" class="btn btn-primary pull-right">Post this job</button>
          </form>
        </div>
        <div class="col-sm-2"></div>
  </div>
</div>

 <?php
    require('../include/layout/page-header-footer.php');
    require('../include/layout/footer.php');
 ?>
