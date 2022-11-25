<!DOCTYPE html>
<html class="no-js" lang="en" ng-app="myApp">

 <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href='https://fonts.googleapis.com/css?family=Lato:400,700|Archivo+Narrow|Crimson+Text:400,700' rel='stylesheet' type='text'/>
    <link href="https://fonts.googleapis.com/css?family=Bree+Serif|Cabin|Copse" rel="stylesheet">
<!-- ======================================================================START PLUGINS========================================= -->
<!-- css plugins -->
    <link rel="stylesheet" href="css/yeti-bootstrap-theme.min.css">
    <link rel="stylesheet" type="text/css" href="css/font-awesome.css" />
    <link href="font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link rel="stylesheet" href="css/jquery-ui.min.css">
    <link rel="stylesheet" href="css/jquery-ui.structure.min.css">
    <link rel="stylesheet" href="css/jquery-ui.theme.min.css">
    <link rel="stylesheet" href="css/jquery.datetimepicker.css">
    <link rel="stylesheet" href="css/blueimp-gallery.min.css">
<!-- Favicons -->
    <link href="img/favicon.png" rel="icon">
    <link href="img/apple-touch-icon.png" rel="apple-touch-icon">
<!-- Google Fonts -->
    <link href="https://fonts.googleapis.com/css?family=Open+Sans:300,300i,400,400i,700,700i|Montserrat:300,400,500,700" rel="stylesheet">
<!-- Bootstrap CSS File -->
     <link href="lib/bootstrap/css/bootstrap.min.css" rel="stylesheet">
     <link rel="stylesheet" href="css/bootstrap.min.css">
     <link rel="stylesheet" href="css/yeti-bootstrap-theme.min.css">
<!-- Libraries CSS Files -->
    <link href="lib/font-awesome/css/font-awesome.min.css" rel="stylesheet">
    <link href="lib/animate/animate.min.css" rel="stylesheet">
    <link href="lib/ionicons/css/ionicons.min.css" rel="stylesheet">
    <link href="lib/owlcarousel/assets/owl.carousel.min.css" rel="stylesheet">
    <link href="lib/lightbox/css/lightbox.min.css" rel="stylesheet">
<!-- Custom/Main Stylesheet File -->
    <link rel="stylesheet" href="css/main.css">
    <link href="css/style.css" rel="stylesheet">
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
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js"></script>
        <script>
            $(document).ready(function(){
                $("#access_test").click(function(){
                    $("#test_form").show(1000);
                });
                $('body').click(function(){
                    $('#test_for').hide();
                });            
            });
        </script>
<!-- =============================================================================================END PLUGINS=============================== -->
<title> <?=$title;?> | Online Recruitment System</title>

 </head>

 <body>

      <nav class="navbar navbar-inverse navbar-static-top">

          <div class="row">
            <div class="navbar-header"><!-- Navbar header holds the webpage logo and the collapse button-->

            <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-One-navbar-collapse-1" aria-expanded="false">
              <span class="sr-only">Toggle navigation</span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
            </button>

              <a href="#" class="navbar-brand" style="right: 30vw; position:relative; top:3vh;">Online Recruitment System</a>

            </div>

            <div class="collapse navbar-collapse" id = "bs-One-navbar-collapse-1"><!--This creates a collapsed navbar when screen is below 768px-->

                <ul style="margin-right: 0px;" class="nav navbar-nav navbar-right">

                      <?php
                            if (!logged_in()) { ?>
                              <li><a href="index.php">Home</a></li>

                              <li class="dropdown">
                                    <a href="#" class="dropdown-toggle" data-toggle="dropdown">Sign in<b class="caret"></b></a>
                              <ul class="dropdown-menu">
                                  <li><a href="/include/layout/modal.php" data-toggle="modal" data-target="#signinseeker">As Job seeker</a></li>
                                  <li><a href="/include/layout/modal.php" data-toggle="modal" data-target="#signinemployer">As Employer</a></li>
                              </ul>

                              <li class="dropdown">
                                    <a href="#" class="dropdown-toggle" data-toggle="dropdown">Sign up<b class="caret"></b></a>
                              <ul class="dropdown-menu" >
                                  <li><a href=""  data-toggle="modal" data-target="#signupseeker">Sign up as Jobseeker</a></li>
                                  <li><a href="employer-signup.php">Sign up as Employer</a></li>
                              </ul>
                            </li>

                      <?php
                            }

                            if (logged_in()) { ?>
                            <li id="my-account" class="dropdown" style="left: 30vw; position:relative; top:-4vh;">
                                <a href="#" class="dropdown-toggle nav-link" data-toggle="dropdown"><i class="fa fa-user"></i> Hi   <?=$user_data['first_name'];?> <b class="caret"></b></a>
                                <ul class="dropdown-menu" style="position:absolute">
                                    <li>
                                        <a href="jobseeker-dashboard.php" class="dropdown-item"><i class="fa fa-fw fa-dashboard"></i> Dashboard</a>
                                    </li>
                                    <li>
                                        <a href="profile.php" class="dropdown-item"><i class="fa fa-fw fa-user"></i> Profile</a>
                                    </li>
                                    <li>
                                        <a href="myapplication.php" class="dropdown-item"><i class="fa fa-fw fa-book"></i> My Application</a>
                                    </li>
                                    <li>
                                        <a href="#" class="dropdown-item"><i class="fa fa-fw fa-envelope"></i> Inbox</a>
                                    </li>
                                    <li>
                                        <a href="blog.php" class="dropdown-item"><i class="fa fa-fw fa-book"></i> Blog</a>
                                    </li>
                                    <li>
                                        <a href="accountsetting.php" class="dropdown-item"><i class="fa fa-fw fa-gear"></i> Account Settings</a>
                                    </li>
                                    <li class="divider"></li>
                                    <li>
                                        <a href="logout.php" class="dropdown-item"><i class="fa fa-fw fa-power-off"></i> Log Out</a>
                                    </li>
                                </ul>
                            </li>


                      <?php
                            }
                       ?>

                </ul>

            </div>
          </div>

      </nav>
