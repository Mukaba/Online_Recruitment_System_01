<?php
      require_once('../include/functions/main.php');
      $title = "Jobseeker Sign up";
      require('../include/layout/header.php');
      $error = array();
      if (!(logged_in())) {
          redirect('index.php');
      }
      error_reporting(2);


      if (isset($_GET['success'])) { ?>

              <div style="margin-bottom:50px" class="container text-center">

                  <div id="reg-success-page-row" class="row">
                      <h2>Thanks. Your registration was succesful. Check your email for your activation code.</h2>
                      <a id="reg-success-page-a" href="index.php">continue to login page</a>
                  </div>

              </div>
      <?php
      }
      else {

          if (isset($_POST['register']) || isset($_POST['register-again'])) {

              $reg_middle_name = htmlentities(strip_tags(trim($_POST['reg_middle_name'])));
              $reg_mobile = htmlentities(strip_tags(trim($_POST['reg_mobile'])));
              $reg_state_of_residence = htmlentities(strip_tags(trim($_POST['reg_state_of_residence'])));
              $reg_lga = htmlentities(strip_tags(trim($_POST['reg_lga'])));
              $reg_specialization = htmlentities(strip_tags(trim($_POST['reg_specialization'])));
              $reg_sex = htmlentities(strip_tags(trim($_POST['reg_sex'])));
              $reg_address = htmlentities(strip_tags(trim($_POST['reg_address'])));
              $reg_state_of_origin = htmlentities(strip_tags(trim($_POST['reg_state_of_origin'])));
              $reg_university = htmlentities(strip_tags(trim($_POST['reg_university'])));
              $reg_degree = htmlentities(strip_tags(trim($_POST['reg_degree'])));
              $reg_field_of_study = htmlentities(strip_tags(trim($_POST['reg_field_of_study'])));
              $reg_year_of_graduation = htmlentities(strip_tags(trim($_POST['reg_year_of_graduation'])));
              $reg_age = htmlentities(strip_tags(trim($_POST['reg_age'])));
              $reg_institution = htmlentities(strip_tags(trim($_POST['reg_institution'])));
              $reg_title = htmlentities(strip_tags(trim($_POST['reg_title'])));
              $reg_year = htmlentities(strip_tags(trim($_POST['reg_year'])));
              $reg_about = htmlentities(strip_tags(trim($_POST['reg_about'])));
              $reg_allow_email  = (isset($_POST['reg_allow_email'])) ? 1 : 0;
              $reg_cv = htmlentities(strip_tags(trim($_POST['reg_cv'])));
              $reg_profile = htmlentities(strip_tags(trim($_POST['reg_profile-pix'])));

                    if (   addJobSeeker($reg_lastname,$reg_firstname,$reg_middlename,$reg_username,$reg_password,$reg_mobile,$reg_address,$reg_email,$reg_specialization,$reg_sex,$reg_state_residence,$reg_city_residence,$reg_state_of_origin,$reg_email_code,$reg_university,$reg_degree,$reg_field_of_study,$reg_year_of_graduation,$reg_age,$reg_title_cert,$reg_cert_institution,$reg_date_cert,$reg_about,$reg_allow_email,$reg_cv,$reg_profile) ) {

                      redirect('jobseeker-signup.php?success');

                  }
                  else {
                      echo "failed";
                  }
          }

      }

 ?>

          <div class="container">

              <div class="row">



                   <div style="margin:30px" class="reg-form">

                     <img class="img-responsive center-block" src="img/call.png" alt="" />

                     <form action="" method="post">

                           <div class="col-xs-12 col-sm-12 col-md-12">

                             <fieldset>

                                 <legend>Personal Details</legend>
                                 <div class="row">

                                   <div class="form-group col-md-4">
                                       <label for="reg_firstname"><b>Firstname</b></label>
                                       <input type="text" name="reg_first_name" id="reg_first_name" value="<?=$user_data['first_name'];?>" class="form-control" required="">
                                       <!-- <span class="glyphicon glyphicon-ok form-control-feedback"></span> -->
                                   </div>

                                   <div class="form-group col-md-4">
                                       <label for="reg_middlename"><b>Middlename</b></label>
                                       <input type="text" name="reg_middle_name" id="reg_middle_name" value="" class="form-control" required="">
                                       <!-- <span class="glyphicon glyphicon-ok form-control-feedback"></span> -->
                                   </div>

                                   <div class="form-group col-md-4">
                                       <label for="reg_lastname"><b>Lastname</b></label>
                                       <input type="text" name="reg_last_name" value="<?=$user_data['last_name'];?>" class="form-control" required="">
                                   </div>

                                 </div>

                                 <div class="row">

                                     <div class="form-group col-md-4">
                                         <label for="reg_mobile"><b>Username</b></label>
                                         <input type="text" name="reg_username" id="reg_username" value="<?=$user_data['username'];?>" class="form-control" required="">
                                     </div>

                                     <div class="form-group col-md-4">
                                         <label for="reg_mobile"><b>Phone number</b></label>
                                         <input type="text" name="reg_mobile" id="reg_mobile" value="" class="form-control" required="">
                                     </div>

                                     <div class="form-group col-md-4 ">
                                         <label for="reg_email"><b>Email Address</b></label>
                                         <input type="text" name="reg_email" id="reg_email" value="<?=$user_data['email'];?>" class="form-control" required="">
                                     </div>

                                 </div>

                                 <div class="row">

                                   <div class="state col-md-3 form-group">
                                      <label for="reg_age"><b>State of Origin</b></label>
                                       <select id="reg_state_of_origin" class="form-control" name="reg_state_of_origin">
                                            <option disabled="" selected="">Select a state</option>
                                       </select>
                                   </div>

                                   <div class="state col-md-3 form-group">
                                      <label for="reg_age"><b>LGA</b></label>
                                       <select id="reg_lga" class="form-control" name="reg_lga">
                                       </select>
                                   </div>

                                   <div class="state col-md-3 form-group">
                                      <label for="reg_age"><b>State of Residence</b></label>
                                       <select id="reg_state_of_residence" class="form-control" name="reg_state_of_residence">
                                            <option disabled="" selected="">Select a state</option>
                                       </select>
                                   </div>

                                   <div class="state col-md-3 form-group">
                                      <label for="reg_age"><b>Gender</b></label>
                                       <select id="reg_sex" class="form-control" name="reg_sex">
                                            <option disabled="" selected="">Gender</option>
                                            <option value="Male">Male</option>
                                            <option value="Female">Female</option>
                                       </select>
                                   </div>

                                 </div>

                                 <div class="form-group">
                                     <label for="reg_address"><b>Address</b></label>
                                     <textarea name="reg_address" id="reg_address" class="form-control" rows="3" cols="40" required=""></textarea>
                                 </div>

                             </fieldset>

                         </div>

                         <div class="col-xs-12 col-sm-12 col-md-12">

                           <fieldset>

                               <legend>Education Details</legend>
                               <div class="col-md-4 form-group">
                                   <label for="reg_type_of_institution"><b>Type of Institution</b></label>
                                    <select id="reg_type_of_institution" class="form-control" name="reg_type_of_institution">
                                         <option disabled="" selected="">Select an option</option>
                                    </select>
                               </div>

                               <div class="col-md-4 form-group">
                                   <label for="reg_university"><b>Institution</b></label>
                                    <select id="reg_university" class="form-control" name="reg_university">
                                    </select>
                               </div>

                               <div class="col-md-4 form-group">
                                   <label for="reg_field"><b>Field of study</b></label>
                                   <input type="text" name="reg_field_of_study" id="reg_field_of_study" value="" class="form-control" required="">
                               </div>

                               <div class="col-md-6 form-group">
                                   <label for="reg_degree"><b>Degree</b></label>
                                   <!-- <input type="text" name="reg_degree" id="reg_degree" value="" class="form-control" required=""> -->
                                   <select class="form-control" name="reg_degree" id="reg_degree">
                                       <option value="B.Sc">B.Sc</option>
                                       <option value="MPhil / PhD">MPhil / PhD</option>
                                       <option value="MBA / MSc">MBA / MSc</option>
                                       <option value="MBBS">MBBS</option>
                                       <!-- <option value="80">Degree</option> -->
                                       <option value="HND">HND</option>
                                       <option value="OND">OND</option>
                                       <option value="N.C.E">N.C.E</option>
                                       <option value="Diploma">Diploma</option>
                                       <option value="High School (S.S.C.E)">High School (S.S.C.E)</option>
                                       <option value="Vocational">Vocational</option>
                                       <option value="Others">Others</option>
                                   </select>

                               </div>

                               <div class="col-md-6 form-group">
                                   <label for="reg_degree"><b>Classification</b></label>
                                   <select name="reg_classification" id="reg_classification" class="form-control">
                                        <option value="None">No classification</option>
                                        <option value="First Class/Distinction">First Class/Distinction</option>
                                        <option value="Second Class Upper/Upper Credit">Second Class Upper/Upper Credit</option>
                                        <option value="Second Class Lower/Lower Credit">Second Class Lower/Lower Credit</option>
                                        <option value="Third Class">Third Class</option>
                                        <option value="Pass">Pass</option>
                                  </select>
                               </div>

                               <div class="col-md-6 form-group">
                                   <label for="reg_specialization"><b>Area of Specialization</b></label>
                                   <!-- <input type="text" name="reg_specialization" id="reg_specialization" value="" class="form-control" required=""> -->
                                   <select name="reg_specialization" id="reg_specialization" class="form-control"><!-- Specialization Dropdown -->
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
                                      <option value="Oil&Gas / Mining / Energy">Oil&amp;Gas / Mining / Energy</option>
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

                               <div class="col-md-6 form-group">
                                   <label for="reg_grad"><b>Year of graduation</b></label>
                                   <!-- <input type="text"  id="reg_year_of_graduation" value=""  required=""> -->
                                   <select class="" name="reg_year_of_graduation" id="reg_year_of_graduation" class="form-control">
                                        <?php
                                            $currentYear = date(Y);
                                            for ($i=1920; $i = $currentYear ; $i++) { ?>

                                                <option value="<?=$i;?>"><?=$i;?></option>

                                        <?php
                                            }

                                         ?>
                                   </select>
                               </div>

                               <div class="form-group">
                                   <label for=""><b>Upload CV</b></label>
                                   <input type="file" name="reg_cv" id="reg_cv" value="">
                               </div>

                           </fieldset>

                           <fieldset>

                               <legend>Certiification</legend>
                               <div class="col-md-4 form-group">
                                   <label for="reg_institue"><b>Institution of Certification</b></label>
                                   <input type="text" name="reg_institution" id="reg_institution" value="" class="form-control" required="">
                               </div>

                               <div class="col-md-4 form-group">
                                   <label for="reg_title_of_cert"><b>Title of certificate</b></label>
                                   <input type="text" name="reg_title" id="reg_title" value="" class="form-control" required="">
                               </div>

                               <div class="col-md-4 form-group">
                                   <label for="reg_year"><b>Year of Certification</b></label>
                                   <input type="text" name="reg_year" id="reg_year" value="" class="form-control" required="">
                               </div>

                           </fieldset>

                           <fieldset>

                               <legend>Others</legend>

                               <div style="margin-bottom:20px" class="profile-pix">
                                 <label for=""><b>Upload Profile Picture</b></label>
                                 <input type="file" name="reg_profile-pix" id="reg_profile-pix" value="">
                               </div>

                               <div class="form-group">
                                   <label for="reg_about"><b>About me</b></label>
                                   <textarea name="reg_about" id="reg_about" class="form-control" rows="10" cols="40" required=""></textarea>

                                   <div class="check">
                                       <input id="input-check" type="checkbox" name="reg_allow_email" id="reg_allow_email" value=""><h6>I would like to receive job notifications via email</h6>
                                   </div>
                               </div>

                           </fieldset>
                         </div>

                         <div style="margin-bottom:20px" class="col-md-12">
                               <button class="btn btn-primary btn-block" type="submit" name="complete" id="complete">Complete Registration</button>
                         </div>

                     </form>

                   </div>

              </div>

          </div>



 <?php
     require('../include/layout/page-header-footer.php');
     require('../include/layout/footer.php');

 ?>
