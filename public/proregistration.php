<?php

session_start();
$title = "registration";
require_once('../include/functions/user.php');
require('../include/layout/modal.php');
$username = $_SESSION['username'];
$session_user_id = $_SESSION['id'];

if (logged_in()) {

    function register_data($session_user_id, $username) {
        require('../include/database/pdo_connect.php');
        $session_user_id = (int)$session_user_id;
        $username = htmlentities(strip_tags(trim($username)));

        $func_num_args = func_num_args();
        $func_get_args = func_get_args();

        if ($func_num_args > 1) {
            unset($func_get_args[0]);
            unset($func_get_args[1]);
        }

        $fields =  implode(", " , $func_get_args) ;
        $query = $pdo->prepare("SELECT $fields from jobseeker where username=:username");
        $query->bindParam(':username' , $username);
        $query->execute();
        $data = $query->fetchAll(PDO::FETCH_ASSOC);
        $data = $data[0];
        return $data;
      }

      $register_data = register_data($session_user_id, $username, 'id' , 'username' , 'first_name' , 'last_name' ,  'jobseeker.email');

}


 ?>
<!DOCTYPE html>
<!--[if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7" lang=""> <![endif]-->
<!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8" lang=""> <![endif]-->
<!--[if IE 8]>         <html class="no-js lt-ie9" lang=""> <![endif]-->
<!--[if gt IE 8]><!-->
<html class="no-js" lang="en" ng-app="myApp"> <!--<![endif]-->

   <head>

      <meta charset="utf-8">
      <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <link href='https://fonts.googleapis.com/css?family=Roboto:400,400italic,500,500italic,700,900|Slabo+27px|Lato|Merriweather:400,700,300,900|Bitter:400,700' rel='stylesheet' type='text/css'>
      <link href='https://fonts.googleapis.com/css?family=Lato:400,700|Archivo+Narrow|Crimson+Text:400,700' rel='stylesheet' type='text'/>

      <!-- css plugins -->
      <link rel="stylesheet" href="css/bootstrap.min.css">
      <link rel="stylesheet" href="css/yeti-bootstrap-theme.min.css">
      <link rel="stylesheet" type="text/css" href="css/font-awesome.css" />
      <link href="font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
      <link rel="stylesheet" href="css/jquery-ui.min.css">
      <link rel="stylesheet" href="css/jquery-ui.structure.min.css">
      <link rel="stylesheet" href="css/jquery-ui.theme.min.css">

      <!-- Important Owl stylesheet -->
      <link rel="stylesheet" href="css/owl.carousel.css">
      <link rel="stylesheet" href="css/owl.theme.css">
      <link rel="stylesheet" href="css/owl.transitions.css">


      <!-- <link rel="stylesheet" href="css/normalize.css"> -->
      <!-- custom css -->
      <link rel="stylesheet" href="css/main.css">

      <!-- js plugin -->
      <script src="js/angular.min.js"></script>
      <script src="js/app.js"></script>

      <title> <?=$title;?> | Smashjob </title>

   </head>

 <body>

	 <!--[if lt IE 8]>
        <p class="browserupgrade">You are using an <strong>outdated</strong> browser. Please <a href="http://browsehappy.com/">upgrade your browser</a> to improve your experience.</p>
    	<![endif]-->

<nav class="navbar navbar-inverse navbar-static-top">

    <div class="navbar-header"><!-- Navbar header holds the webpage logo and the collapse button-->

        <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-One-navbar-collapse-1" aria-expanded="false">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
        </button>

        <a href="#" class="navbar-brand">
            <img src="" class="img-responsive logo" alt="LOGO">
        </a>
    </div>

    <div class="collapse navbar-collapse" id = "bs-One-navbar-collapse-1"><!--This creates a collapsed navbar when screen is below 768px-->
        <ul class="nav navbar-nav navbar-right">
            <!-- <li><a href="index.php">Home</a></li> -->
        </ul>
    </div>

</nav>

 <div class="container register-container">

     <div class="row">

          <div style="margin:30px" class="reg-form">

             <img class="img-responsive center-block" src="img/call.png" alt="" />



            <form id="pro-registration-form" action="../include/action/proregistrationaction.php" method="post" novalidate="">

                  <div class="col-xs-12 col-sm-12 col-md-12">

                    <fieldset>

                        <legend>Personal Details</legend>
                        <div class="row">

                          <div class="form-group col-md-4">
                              <label for="reg_firstname"><b>Firstname</b></label>
                              <input type="text" name="reg_first_name" id="reg_first_name" value="<?=$register_data['first_name'];?>" class="form-control" required="">
                              <!-- <span class="glyphicon glyphicon-ok form-control-feedback"></span> -->
                          </div>

                          <div class="form-group col-md-4">
                              <label for="reg_middlename"><b>Middlename</b></label>
                              <input type="text" name="reg_middle_name" id="reg_middle_name" value="" class="form-control" required="">
                              <!-- <span class="glyphicon glyphicon-ok form-control-feedback"></span> -->
                          </div>

                          <div class="form-group col-md-4">
                              <label for="reg_lastname"><b>Lastname</b></label>
                              <input type="text" name="reg_last_name" value="<?=$register_data['last_name'];?>" class="form-control" required="">
                          </div>

                        </div>

                        <div class="row">

                            <div class="form-group col-md-3">
                                <label for="reg_mobile"><b>Username</b></label>
                                <input type="text" name="reg_username" id="reg_username" value="<?=$register_data['username'];?>" class="form-control" required="">
                            </div>

                            <div class="form-group col-md-3">
                                <label for="reg_mobile"><b>Phone number</b></label>
                                <input type="text" name="reg_mobile" id="reg_mobile" value="" class="form-control" required="">
                            </div>

                            <div class="form-group col-md-3">
                                <label for="reg_email"><b>Email Address</b></label>
                                <input type="text" name="reg_email" id="reg_email" value="<?=$register_data['email'];?>" class="form-control" required="">
                            </div>

                            <div class="form-group col-md-3">
                                <label for="reg_mobile"><b>Date of Birth</b></label>
                                <input type="text" name="reg_age" id="reg_age" value="" class="form-control" required="">
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
                              <option value="B.Eng">B.Eng</option>
                              <option value="B.Tech">B.Tech</option>
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
                           <label for=""><b>Year of graduation</b></label>
                           <input type="text" name="reg_year_of_graduation" id="reg_year_of_graduation" value="" class="form-control">
                       </div>

                      <div class="col-md-12 form-group">
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
                        <input type="file" name="reg_profile_pix" id="reg_profile_pix" value="">
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
                      <button class="btn btn-primary btn-block" type="submit" name="complete" id="complete">Complete Your Registration</button>
                </div>

            </form>

            <p id="form-notice" class="text-center"></p>

          </div>

     </div>

 </div>




 <?php
       require('../include/layout/page-header-footer.php');
       require('../include/layout/footer.php');
  ?>

  <script type="text/javascript">
  $('#pro-registration-form').validate({

      rules: {
          reg_middle_name: {
              required: true,
          },
          reg_age: {
              required: true,
          },
          reg_mobile: {
              required: true,
          },
          reg_address: {
              required: true,
          },
          reg_type_of_institution: {
              required: true,
          },
          reg_university: {
              required: true,
          },
          reg_field_of_study: {
              required: true,
          },
          reg_degree: {
              required: true,
          },
          reg_classification: {
              required: true,
          },
          reg_specialization: {
              required: true,
          },
          reg_year_of_graduation: {
              required: true,
          },
      },
      messages: {
          reg_middle_name: {
              required: 'This field is required',
          },
          reg_age: {
              required: 'This field is required',
          },
          reg_mobile: {
              required: 'This field is required',
          },
          reg_address: {
              required: 'This field is required',
          },
          reg_type_of_institution: {
              required: 'This field is required',
          },
          reg_university: {
              required: 'This field is required',
          },
          reg_field_of_study: {
              required: 'This field is required',
          },
          reg_degree: {
              required: 'This field is required',
          },
          reg_classification: {
              required: 'This field is required',
          },
          reg_specialization: {
              required: 'This field is required',
          },
          reg_year_of_graduation: {
              required: 'This field is required',
          },
      },
  });
  </script>
