
 <!DOCTYPE html>
 <!-- [if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7" lang=""> <![endif] -->
 <!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8" lang=""> <![endif]-->
 <!--[if IE 8]>         <html class="no-js lt-ie9" lang=""> <![endif]-->
 <!--[if gt IE 8]><!-->
 <html class="no-js" lang="en" ng-app="myApp"> <!--<![endif]-->

  <head>
     <title> <?=$title;?> | Online Recruitment System </title>
     <meta charset="utf-8">
     <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
     <meta name="viewport" content="width=device-width, initial-scale=1">
     <!-- css plugins -->
     <link rel="stylesheet" href="css/bootstrap.min.css">
     <link rel="stylesheet" href="css/yeti-bootstrap-theme.min.css">
     <link rel="stylesheet" type="text/css" href="css/font-awesome.css" />
     <link href="font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
     <link rel="stylesheet" href="css/jquery-ui.min.css">
     <link rel="stylesheet" href="css/jquery-ui.structure.min.css">
     <link rel="stylesheet" href="css/jquery-ui.theme.min.css">
     <link rel="stylesheet" href="css/jquery.datetimepicker.css">

     <!-- Important Owl stylesheet -->
     <link rel="stylesheet" href="css/owl.carousel.css">

     <!-- Default Theme -->
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
                $("#new_question").click(function(){
                    $("#myform").show(1000);
                });
                $("#cancelbtn").click(function(){
                    $("#myform").hide(1000);
                });            
            });
        </script>
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

               <!-- <img src="" class="img-responsive logo" alt="LOGO"> --> <a href="#" class="scrollto" ><h6></br>Online Recruitment System</h6> </a>

             </a>

           </div>

           <div class="collapse navbar-collapse" id = "bs-One-navbar-collapse-1"><!--This creates a collapsed navbar when screen is below 768px-->

               <ul class="nav navbar-nav navbar-right" style="margin-right: 0px;">

                     <?php

                           if (logged_in()) { ?>

                           <li id="my-account" class="dropdown open">
                               <a href="#" class="dropdown-toggle" data-toggle="dropdown"><i class="fa fa-user" expanded="false"></i> Hi <?=$employer_data['name'];?> <b class="caret"></b></a>
                               <ul class="dropdown-menu">
                                   <li>
                                       <a href="employer-dashboard.php"><i class="fa fa-fw fa-dashboard"></i> Dashboard</a>
                                   </li>
                                   <li>
                                       <a href="postjob.php"><i class="fa fa-fw fa-envelope"></i> Post a job</a>
                                   </li>
                                   <li>
                                       <a href="joblist.php"><i class="fa fa-fw fa-envelope"></i> My  job list</a>
                                   </li>
                                   <li>
                                       <a href="#"><i class="fa fa-fw fa-envelope"></i> Inbox</a>
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
