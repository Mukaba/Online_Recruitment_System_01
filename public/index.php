<?php
    $title = "Home";
    require_once('../include/functions/main.php');
    require('../include/layout/modal.php');
    jobseeker_logged_in_redirect();
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
      <link href="https://fonts.googleapis.com/css?family=Bree+Serif|Cabin|Copse" rel="stylesheet">

      <!-- css plugins -->
      <link rel="stylesheet" href="css/bootstrap.min.css">
      <link rel="stylesheet" href="css/yeti-bootstrap-theme.min.css">
      <link rel="stylesheet" type="text/css" href="css/font-awesome.css" />
      <link href="font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
      <link rel="stylesheet" href="css/blueimp-gallery.min.css">
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

<nav class="navbar navbar-default navbar-static-top navbar-fixed-top">

    <div class="navbar-header"><!-- Navbar header holds the webpage logo and the collapse button-->

        <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-One-navbar-collapse-1" aria-expanded="false">
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>

        <a href="index.php" class="navbar-brand">
            <img src="" class="img-responsive logo" alt="Smashjob">
        </a>

    </div>

    <div class="collapse navbar-collapse" id = "bs-One-navbar-collapse-1"><!--This creates a collapsed navbar when screen is below 768px-->

        <ul class="nav navbar-nav navbar-right">
            <?php
                if (!logged_in()) { ?>
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
            <?php
                }
                if (logged_in()) { ?>
                    <li id="my-account" class="dropdown">
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown"><i class="fa fa-user"></i> Hi <?=$user_data['first_name'];?> <b class="caret"></b></a>

                        <ul class="dropdown-menu">

                            <li>
                                <a href="jobseeker-dashboard.php"><i class="fa fa-fw fa-dashboard"></i> Dashboard</a>
                            </li>
                            <li>
                                <a href="profile.php"><i class="fa fa-fw fa-user"></i> Profile</a>
                            </li>
                            <li>
                                <a href="#"><i class="fa fa-fw fa-lock"></i> Change Password</a>
                            </li>
                            <li>
                                <a href="#"><i class="fa fa-fw fa-envelope"></i> Inbox</a>
                            </li>
                            <li>
                                <a href="#"><i class="fa fa-fw fa-book"></i> Blog</a>
                            </li>
                            <li>
                                <a href="#"><i class="fa fa-fw fa-gear"></i> Account Settings</a>
                            </li>
                            <li class="divider"></li>
                            <li>
                                <a href="logout.php"><i class="fa fa-fw fa-power-off"></i> Log Out</a>
                            </li>

                        </ul>

                    </li>

            <?php
                }
             ?>

        </ul>

    </div>

</nav>

<?php
      require('../include/layout/modal.php');
      require('../include/layout/page-header.php');
 ?>

<div id="landing-bottom" class="container">

    <div class="row">

        <div id="upload-cv" class="col-md-12 upload-cv">

            <h3 class="midtext text-center">Get the job in the bag anywhere in Nigeria.</h3>

            <div class="img-text text-center">

                <a class="home-a" href="#"><i class="glyphicon glyphicon-cloud-upload"></i></a> <br>

                <form style="margin-bottom:20px;" class="" action="" method="post">
                      <button class="btn btn-primary landing-button" type="submit" name="button">Upload Resume</button>
                </form>

            </div>

        </div>

    </div>

</div>

<?php require('../include/layout/script.php'); ?>
