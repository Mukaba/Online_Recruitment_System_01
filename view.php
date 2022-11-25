<?php
      $title = "View";
      require_once('include/functions/view.php');

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
     <!-- <link href='https://fonts.googleapis.com/css?family=Lato:400,700|Archivo+Narrow|Crimson+Text:400,700' rel='stylesheet' type='text'/>
     <link href="https://fonts.googleapis.com/css?family=Bree+Serif|Cabin|Copse" rel="stylesheet"> -->

     <!-- css plugins -->
     <link rel="stylesheet" href="css/bootstrap.min.css">
     <link rel="stylesheet" href="css/yeti-bootstrap-theme.min.css">
     <link rel="stylesheet" type="text/css" href="css/font-awesome.css" />
     <link href="font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
     <link rel="stylesheet" href="css/jquery-ui.min.css">
     <link rel="stylesheet" href="css/jquery-ui.structure.min.css">
     <link rel="stylesheet" href="css/jquery-ui.theme.min.css">
     <link rel="stylesheet" href="css/jquery.datetimepicker.css">
     <link rel="stylesheet" href="css/blueimp-gallery.min.css">

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

          <div class="row">
            <div class="navbar-header"><!-- Navbar header holds the webpage logo and the collapse button-->

            <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-One-navbar-collapse-1" aria-expanded="false">
              <span class="sr-only">Toggle navigation</span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
            </button>

              <a href="#" class="navbar-brand">

                <img src="" class="img-responsive logo" alt="Smashjob">

              </a>

            </div>

            <div class="collapse navbar-collapse" id = "bs-One-navbar-collapse-1"><!--This creates a collapsed navbar when screen is below 768px-->

                <ul style="margin-right: 0px;" class="nav navbar-nav navbar-right">

                              <li><a href="index.php">Home</a></li>

                              <li class="dropdown">
                                    <a href="#" class="dropdown-toggle" data-toggle="dropdown">Sign in<b class="caret"></b></a>
                              <ul class="dropdown-menu">
                                  <li><a href="" data-toggle="modal" data-target="#signinseeker">As Job seeker</a></li>
                                  <li><a href="" data-toggle="modal" data-target="#signinemployer">As Employer</a></li>
                              </ul>

                              <li class="dropdown">
                                    <a href="#" class="dropdown-toggle" data-toggle="dropdown">Sign up<b class="caret"></b></a>
                              <ul class="dropdown-menu">
                                  <li><a href=""  data-toggle="modal" data-target="#signupseeker">Sign up</a></li>
                                  <li><a href="employer-signup.php">Sign up as Employer</a></li>
                              </ul>
                            </li>

                </ul>

            </div>
          </div>

      </nav>

 <div class="container-fluid">
     <div style="margin-right:0px;margin-left:0px;" class="row">

       <div class="col-xs-12 col-sm-12 col-md-12 profile-top">

         <div class="dashboard-profile-display">

             <div class="col-xs-5 col-sm-3 col-md-2 img-display">

                 <?php
                     if (empty($Details['profile_image']) === false) { ?>
                         <img class="img-responsive img-thumbnail  profile-pix" src="<?=$Details['profile_image'];?>" alt="" />
                 <?php
                     }elseif (empty($Details['profile_image']) === true) { ?>
                               <img class="img-responsive img-thumbnail  profile-pix" src="http://placehold.it/200x150?text=No+Image" alt="Upload an image" />
                 <?php
                     }
                 ?>
                 <h6>http://localhost/smashjob/<?=$Details['username']; ?></h6>
             </div>

             <div class="dashboard-details col-xs-6 col-sm-6 col-md-8">
                 <h3><?php echo strtoupper($Details['last_name'] . " " . $Details['first_name']); ?></h3>
                 <label id="email-label" for="">Email:</label><br>
                 <h6 class="info"><?php echo strtolower($Details['email']); ?><br></h6>
                 <label id="phone-label" for="">Phone:</label><br>
                 <h6 class="info"><?php echo $Details['mobile']; ?></h6><br>
             </div>

         </div>



       </div>


     </div>
 </div>

  <div class="container profile">
      <div  class="row">

          <div  ng-init="tab=0" class="profile-main col-md-10">

              <fieldset class="edit-profile-fieldset">

                  <legend>Personal Details</legend>

                  <div class="row">
                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Surname:</b></label>
                      <h6><?=strtoupper($Details['last_name'])?></h6>
                    </div>

                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>First name:</b></label>
                      <h6><?=strtoupper($Details['first_name'])?></h6>
                    </div>

                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Middle name:</b></label>
                      <h6><?=strtoupper($Details['middle_name'])?></h6>
                    </div>

                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Email:</b></label>
                      <h6><?=($Details['email'])?></h6>
                    </div>
                  </div>

                  <div class="row">
                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Mobile no:</b></label>
                      <h6><?=($Details['mobile'])?></h6>
                    </div>

                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Sex:</b></label>
                      <h6><?=strtoupper($Details['sex'])?></h6>
                    </div>
                  </div>

                  <div class="clearfix"></div>

              </fieldset>

              <fieldset class="edit-profile-fieldset">
                  <legend id="edu-legend">Educational Background</legend>

                  <?php
                        $Education_resource = fetchJobseekerEducation($Details['username']);

                        if (count($Education_resource) == 1) {
                            unset($Education_resource[0]);
                  ?>

                  <div class="row">
                      <p>No Educational Background Available</p>
                  </div>

                  <?php
                }elseif (count($Education_resource > 1)) {

                        unset($Education_resource[0]);

                        foreach ($Education_resource as $key => $jobseeker_education) {
                        require('include/layout/delete.php');
                  ?>
                  <div class="row">

                    <div class="col-md-4">
                      <label for=""><b>Institution:</b></label>
                      <h6><?=strtoupper($jobseeker_education['institution'])?></h6>
                    </div>

                    <div class="col-md-3">
                      <label for=""><b>Field of study:</b></label>
                      <h6><?=strtoupper($jobseeker_education['field_of_study'])?></h6>
                    </div>

                    <div class="col-md-2">
                      <label for=""><b>Degree:</b></label>
                      <h6><?=strtoupper($jobseeker_education['degree'])?></h6>
                    </div>


                    <div class="col-md-2">
                      <label for=""><b>Year:</b></label>
                      <h6><?=strtoupper($jobseeker_education['year_of_graduation'])?></h6>
                    </div>

                  </div>

                  <div class="clear-float">

                  </div>

                   <?php

                          }
                      }

                   ?>


                  <div ng-show="tab === 2" class="col-md-12 personal-form" ng-cloak>

                    <div class="row">
                      <div class="col-md-4">
                        <label for=""><b>Institution:</b></label>
                        <h6 style="text-transform:uppercase;">{{university}}</h6>
                      </div>

                      <div class="col-md-3">
                        <label for=""><b>Field of study:</b></label>
                        <h6 style="text-transform:uppercase;">{{field_of_study}}</h6>
                      </div>

                      <div class="col-md-2">
                        <label for=""><b>Degree:</b></label>
                        <h6 style="text-transform:uppercase;">{{degree}}</h6>
                      </div>

                      <div class="col-md-2">
                        <label for=""><b>Year of graduation:</b></label>
                        <h6 style="text-transform:uppercase;">{{year_of_graduation}}</h6>
                      </div>
                    </div>

                  </div>
              </fieldset>

              <fieldset class="edit-profile-fieldset">

                  <legend>Certification</legend>

                    <?php
                          $Certificate_resource = fetchJobseekerCert($Details['username']);

                          if (count($Certificate_resource) == 1) {
                              unset($Certificate_resource[0]);
                    ?>

                            <div class="row">
                                <p>You have no certification. Click the <a ng-click="tab=3" href="#edit-certification-form">here</a> to add one</p>
                            </div>

                    <?php
                  }elseif (count($Certificate_resource > 1)) {

                          unset($Certificate_resource[0]);

                          foreach ($Certificate_resource as $key => $jobseeker_cert) {

                    ?>
                            <div class="row">
                              <div class="col-md-5">
                                <label for=""><b>Title:</b></label>
                                <h6><?=strtoupper($jobseeker_cert['title'])?></h6>
                              </div>

                              <div class="col-md-5">
                                <label for=""><b>Institution:</b></label>
                                <h6><?=strtoupper($jobseeker_cert['institution'])?></h6>
                              </div>

                              <div class="col-md-1">
                                <label for=""><b>Year:</b></label>
                                <h6><?=strtoupper($jobseeker_cert['year'])?></h6>
                              </div>

                            </div>

                            <div class="clear-float">

                            </div>

                     <?php
                            }
                        }

                     ?>

              </fieldset>

              <fieldset class="edit-profile-fieldset">
                  <legend>About me</legend>
                  <div class="about-me">

                      <h6 class="about-me-h6 text-jusify"><?=$Details['about'];?></h6>

                  </div>
              </fieldset>


          </div>

          <div class="profile-aside col-md-2">

          </div>


      </div>
  </div>

   <?php
         require('include/layout/page-header-footer.php');
         require('include/layout/footer.php');
    ?>
