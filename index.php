<?php
    $title = "Home";
    require_once('include/functions/main.php');
    require('include/layout/modal.php');
    jobseeker_logged_in_redirect();
?>
<!DOCTYPE html>
<html class="no-js" lang="en" ng-app="myApp">
<head>    
  <title> <?=$title;?> |Online Job Recritment System</title>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
  <meta content="width=device-width, initial-scale=1.0" name="viewport">
  <meta content="JobPortal,Online,Freshers" name="keywords">
  <meta content="Connect Jobseekers and Recruiters, By Mukaba Jean-louis" name="description">

<!-- =======================================================================================START PLUGINS=======================================-->
        <!-- css plugins -->
        <link rel="stylesheet" href="css/bootstrap.min.css">
            <link rel="stylesheet" href="css/yeti-bootstrap-theme.min.css">
            <link rel="stylesheet" type="text/css" href="css/font-awesome.css" />
            <link href="font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
            <link rel="stylesheet" href="css/blueimp-gallery.min.css">
            <link href="lib/owlcarousel/assets/owl.carousel.min.css" rel="stylesheet">
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
        <!-- js plugin -->
            <script src="js/angular.min.js"></script>
            <script src="js/app.js"></script>
<!-- =============================================================================================END PLUGINS=============================== -->
<!-- =======================================================================================START STYLE=========================== -->
            <style>
        body {font-family: Arial, Helvetica, sans-serif;}
        * {box-sizing: border-box;}

/* Full-width input fields */
        input[type=email], input[type=password] {
            width: 100%;
            padding: 15px;
            margin: 5px 0 22px 0;
            display: inline-block;
            border: none;
            background: #f1f1f1;
        }

/* Add a background color when the inputs get focus */
        input[type=text]:focus, input[type=password]:focus {
            background-color: #ddd;
            outline: none;
        }

/* Set a style for all buttons */
        button {
            background-color: #4CAF50;
            color: white;
            padding: 14px 20px;
            margin: 8px 0;
            border: none;
            cursor: pointer;
            width: 100%;
            opacity: 0.9;
        }

        button:hover {
            opacity:1;
        }

/* Extra styles for the cancel button */
        .cancelbtn {
            padding: 14px 20px;
            background-color: #f44336;
        }

/* Float cancel and signup buttons and add an equal width */
        .cancelbtn, .signupbtn {
        float: left;
        width: 50%;
        }

/* Add padding to container elements */
        .container {
            padding: 16px;
        }

/* The Modal (background) */

/* Modal Content/Box */
        .modal-content {
            background-color: #fefefe;
            margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */
            border: 1px solid #888;
            width: 80%; /* Could be more or less, depending on screen size */
        }

 /* Style the horizontal ruler */
        hr {
            border: 1px solid #f1f1f1;
            margin-bottom: 25px;
        }
        
/* The Close Button (x) */
        .close {
            position: absolute;
            right: 35px;
            top: 15px;
            font-size: 40px;
            font-weight: bold;
            color: #f1f1f1;
        }

        .close:hover,
        .close:focus {
            color: #f44336;
            cursor: pointer;
        }

/* Clear floats */
        .clearfix::after {
            content: "";
            clear: both;
            display: table;
        }

/* Change styles for cancel button and signup button on extra small screens */
        @media screen and (max-width: 300px) {
            .cancelbtn, .signupbtn {
            width: 100%;
            }
        }
        </style>
<!-- =======================================================================================END STYLE=========================== -->    
</head>

<body ng-cloak>
  <!--=========Header===========-->
  <header id="header">
  <div class="container-fluid">
  <div id="logo" class="pull-left">
        <h1><a href="#intro" class="scrollto">SearchUp <h6>Online Recruitment System</h6> </a></h1>
      </div>
      <nav id="nav-menu-container" id = "bs-One-navbar-collapse-1">
            <?php
                if (!logged_in()) { ?>
                 <ul class="nav-menu">
                    <li><a href="index.php">Home</a></li>
                    <!-- <li><a href="#">Jobs</a></li>  -->
                    <li><a href="#about">About Us</a></li>
                    <li><a href="#company">Companies</a></li> 
                    <li><a href="#contact">Contact</a></li>
                    <li class="menu-has-children" ><a href="">Account</a>
                    <ul>
                        <li class="dropdown">
                            <a href="#" class="dropdown-toggle" data-toggle="dropdown">Sign in<b class="caret"></b></a>
                        <ul class="dropdown-menu">
                            <li><a href="" data-toggle="modal" data-target="#signinseeker" style="color:black;">As Job seeker</a></li>
                            <li><a href="" data-toggle="modal" data-target="#signinemployer" style="color:black;">As Employer</a></li>
                        </ul>

                        <li class="dropdown">
                            <a href="#" class="dropdown-toggle" data-toggle="dropdown">Sign up<b class="caret"></b></a>
                        <ul class="dropdown-menu">
                            <li><a href=""  data-toggle="modal" data-target="#signupseeker" style="color:black;">As Job seeker</a></li>
                            <li><a href="employer-signup.php" style="color:black;">As Employer</a></li>
                        </ul>

                    </li>
                    </ul>
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
                </ul>

            <?php
                }
             ?><!-- #nav-menu-container -->
        </nav>
    </div>
  </header><!-- #header -->

  <!--==========================
    Intro Section
  ============================-->
  <section id="intro">
    <div class="intro-container">
      <div id="introCarousel" class="carousel  slide carousel-fade" data-ride="carousel">

        <div class="carousel-inner">

          <div class="carousel-item active">
            <div class="carousel-background"><img src="img/intro-carousel/1.jpg" alt="" style="width:100vw"></div>
            <div class="carousel-container">
              <div class="carousel-content">
                <h2>Get ahead</h2>
                <h2>Best Job, Best Applicant</h2>
                <a href="#" onclick="document.getElementById('id01').style.display='block'" style="width:auto;" class="btn-get-started scrollto">Get Started</a></br>
    <?php
      require('include/layout/modal.php');
      require('include/layout/page-header.php');
    ?>
              </div>
            </div>
          </div>

          </div>
       
      </div>
    </div>
  </section><!-- #intro -->

  <main id="main">


    <!--==========================
      About Us Section
    ============================-->
    <section id="about">
      <div class="container">

        <header class="section-header">
          <h3>About Us</h3>
            <p><b>SearchUp</b> is a Banchlor Degree final project that connects jobseekers and recruiters by accurately matching candidate profiles to the relevant job openings through an advanced technology. While most job portals only focus on getting candidates to the next job, <b>SearchUp</b> focuses on the entire career growth of candidates. For this this, <b>SearchUp Group</b> composed by K. Mukaba Jean-louis students at <a href="https://ulk.ac.rw">Kigali Independent University</a>, School of Science and Technology, Computer Science department, has launched this project. the goal is to extend the logic and provide not just a job portal but also educative features such as free courses, internship and much more in an earlier future.
</p>
          </header>

        <div class="row about-cols">

          <div class="col-md-4 wow fadeInUp">
            <div class="about-col">
              <div class="img">
                <img src="img/about-mission.jpg" alt="" class="img-fluid">
                <div class="icon"><i class="ion-ios-speedometer-outline"></i></div>
              </div>
              <h2 class="title"><a href="#">Our Mission</a></h2>
        <center>      
        <p>The purpose of this project is to make a place which hold all information about competitive candidates to competitive vacant jobs, We do belief that job information's availability can make a positive difference to employers, and to the society at large.
            SearchUp aims to be the driver of this change.</p>
        </center>
        
            </div>
          </div>

          <div class="col-md-4 wow fadeInUp" data-wow-delay="0.1s">
            <div class="about-col">
              <div class="img">
                <img src="img/about-plan.jpg" alt="" class="img-fluid">
                <div class="icon"><i class="ion-ios-list-outline"></i></div>
              </div>
              <h2 class="title"><a href="#">Our Plan</a></h2>
                <center>
                  <p>
                  As student and entrepreneur, this proposal can to solve an everyday problem, in both professional and educative field.
                </p>
                </center>
            </div>
          </div>

          <div class="col-md-4 wow fadeInUp" data-wow-delay="0.2s">
            <div class="about-col">
              <div class="img">
                <img src="img/about-vision.jpg" alt="" class="img-fluid">
                <div class="icon"><i class="ion-ios-eye-outline"></i></div>
              </div>
              <h2 class="title"><a href="#">Our Vision</a></h2>
              <center>
              <p>
                For thousands of organizations that use archaic way to hire interns, Jobseekers to be hired, we are a pleasant surprise for you both companies and job seekers. <br> <br> <br>
              </p>
              </center>
            </div>
          </div>

        </div>

      </div>
    </section><!-- #about -->   
    <!--==========================
      Clients Section
    ============================-->
    <section id="company" class="wow fadeInUp">
      <div class="container">
        <header class="section-header">
          <h3>Companies</h3>
        </header>
        <div class="owl-carousel clients-carousel">
          <!-- <img src="img/clients/exa-logo.png.png" alt=""> -->
          <img src="img/clients/COGNIZANT.jpg" alt="">
          <img src="img/clients/infosys-logo.png" alt="">
          <img src="img/clients/exa-logo.png" alt="this is eXa's logo">
          <img src="img/clients/TCS.jpg" alt="">
          <img src="img/clients/amazon-LOGO.jpg" alt="">
          <img src="img/clients/cisco.png" alt="">  
        </div>

      </div>
    </section><!-- #clients -->
    <!--==========================
      Clients Section
    ============================-->
    <section id="testimonials" class="section-bg wow fadeInUp">
      <div class="container">

        <header class="section-header">
          <h3>SearchUp Team</h3>
        </header>

        <div class="owl-carousel testimonials-carousel">

          <div class="testimonial-item">
            <img src="img/Raoul2.jpg" class="testimonial-img" alt="">
            <h3>K. Mukaba Jean-louis</h3>
            <h4>Software Developer &amp; Founder</h4>
            <p>
              <img src="img/quote-sign-left.png" class="quote-sign-left" alt="">
              Lorem ipsum dolor sit, amet consectetur adipisicing elit. Corporis corrupti pariatur, sint odit rem placeat quos unde eligendi quaerat, hic obcaecati nobis, reiciendis porro labore aperiam. Commodi laudantium et velit!
              <img src="img/quote-sign-right.png" class="quote-sign-right" alt="">
            </p>
          </div>

          <div class="testimonial-item">
            <img src="img/dann.jpeg" class="testimonial-img" alt="">
            <h3>Daniel Munoka</h3>
            <h4>Web dev &amp; Designer chez eXa</h4>
            <p>
              <img src="img/quote-sign-left.png" class="quote-sign-left" alt="">
              Lorem ipsum dolor sit amet consectetur adipisicing elit. Ea asperiores quia ad minima maiores. Cupiditate labore et, alias optio praesentium qui natus exercitationem quos, nihil nemo maxime, modi illo rem.
              <img src="img/quote-sign-right.png" class="quote-sign-right" alt="">
            </p>
          </div>

          <div class="testimonial-item">
            <img src="img/tych.jpg" class="testimonial-img" alt="">
            <h3>Tych Rockis</h3>
            <h4>Electro-informaticien chez eXa</h4>
            <p>
              <img src="img/quote-sign-left.png" class="quote-sign-left" alt="">
              Lorem ipsum dolor sit amet consectetur adipisicing elit. Inventore culpa, at molestiae voluptas quia dolores asperiores adipisci modi facilis quo numquam rem voluptatibus dolore quam consequatur praesentium error quos cupiditate!
              <img src="img/quote-sign-right.png" class="quote-sign-right" alt="">
            </p>
          </div>

          <div class="testimonial-item">
            <img src="img/aubin.jpeg" class="testimonial-img" alt="">
            <h3>Aubin Simpeze</h3>
            <h4>Web Sec</h4>
            <p>
              <img src="img/quote-sign-left.png" class="quote-sign-left" alt="">
              Lorem ipsum dolor sit amet consectetur adipisicing elit. Culpa illo nulla, beatae dignissimos itaque assumenda, maxime error molestias consequuntur laborum eius nisi, earum magni deserunt at possimus voluptatibus sint? Aut.
              <img src="img/quote-sign-right.png" class="quote-sign-right" alt="">
            </p>
          </div>

          <div class="testimonial-item">
            <img src="img/ttt1.jpeg" class="testimonial-img" alt="">
            <h3>Jean-louis Mukaba</h3>
            <h4>Software Developer</h4>
            <p>
              <img src="img/quote-sign-left.png" class="quote-sign-left" alt="">
              Lorem ipsum, dolor sit amet consectetur adipisicing elit. Excepturi perspiciatis temporibus quis libero blanditiis voluptas iusto maxime, debitis delectus laudantium architecto repudiandae! Magnam, incidunt repellat consequatur architecto facilis non deserunt?
              <img src="img/quote-sign-right.png" class="quote-sign-right" alt="">
            </p>
          </div>

        </div>

      </div>
    </section><!-- #testimonials -->
    <!--==========================
      Contact Section
    ============================-->
    <section id="contact" class="section-bg wow fadeInUp">
      <div class="container">

        <div class="section-header">
          <h3>Contact Us</h3>
          <p>Contact us at the following addresses</p>
        </div>

        <div class="row contact-info">

          <div class="col-md-4">
            <div class="contact-address">
              <i class="ion-ios-location-outline"></i>
              <h3>Address</h3>
              <address>Rwanda-Kigali, Gisozi 334St</address>
            </div>
          </div>

          <div class="col-md-4">
            <div class="contact-phone">
              <i class="ion-ios-telephone-outline"></i>
              <h3>Phone Number</h3>
              <p><a href="tel:+250786200013">+250 7862 000 13</a></p>
            </div>
          </div>

          <div class="col-md-4">
            <div class="contact-email">
              <i class="ion-ios-email-outline"></i>
              <h3>Email</h3>
              <p><a href="mailto:jeanlouismukaba01@yahoo.com.com">jeanlouismukaba01@yahoo.com</a></p>
            </div>
          </div>

        </div>

      </div>
    </section><!-- #contact -->

  </main>
<script>
// Get the modal
// var modal = document.getElementById('id01');

// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}
</script>

<!--register-->
<div id="id02" class="modal">
  <span onclick="document.getElementById('id02').style.display='none'" class="close" title="Close Modal">&times;</span>
  <form class="modal-content" action="/action_page.php">
    <div class="container">
      <h1>Sign Up</h1>
      <p>Please fill in this form to create an account.</p>
      <hr>
      <label for="email"><b>Email</b></label>
      <input type="text" placeholder="Enter Email" name="email" required>

      <label for="psw"><b>Password</b></label>
      <input type="password" placeholder="Enter Password" name="psw" required>

      <label for="psw-repeat"><b>Repeat Password</b></label>
      <input type="password" placeholder="Repeat Password" name="psw-repeat" required>
      
      <div class="clearfix">
        <button type="button" onclick="document.getElementById('id02').style.display='none'" class="cancelbtn">Cancel</button>
        <button type="submit" class="signupbtn">Sign Up</button>
      </div>
    </div>
  </form>
</div>
<script>
// Get the modal
var modal = document.getElementById('id02');

// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}
</script>

    
  <!--==========================
    Footer
  ============================-->
  <footer id="footer">
    <div class="footer-top">
      <div class="container">
        <div class="row">

          <div class="col-lg-3 col-md-6 footer-info">
            <h3>SearchUp</h3>
           <!-- <p>Cras fermentum odio eu feugiat lide par naso tierra. Justo eget nada terra videa magna derita valies darta donna mare fermentum iaculis eu non diam phasellus. Scelerisque felis imperdiet proin fermentum leo. Amet volutpat consequat mauris nunc congue.</p>-->
              <p>Our System is useful for every valueable person for his carrier... </p>
          </div>

          <div class="col-lg-3 col-md-6 footer-links">
            <h4>Useful Links</h4>
            <ul>
              <li><i class="ion-ios-arrow-right"></i> <a href="#">Home</a></li>
              <li><i class="ion-ios-arrow-right"></i> <a href="#">About us</a></li>
              <li><i class="ion-ios-arrow-right"></i> <a href="#">Services</a></li>
              <li><i class="ion-ios-arrow-right"></i> <a href="#">Terms of service</a></li>
              <li><i class="ion-ios-arrow-right"></i> <a href="#">Privacy policy</a></li>
            </ul>
          </div>

          <div class="col-lg-3 col-md-6 footer-contact">
            <h4>Contact Us</h4>
            <p>
              Gisozi 334 Street <br>
              Rwanda, Kigali 002<br>
              Afica <br>
              <strong>Phone:</strong> +250 7862 000 13<br>
              <strong>Email:</strong> jeanlouismukaba01@yahoo.com<br>
            </p>

            <div class="social-links">
              <a href="#" class="twitter"><i class="fa fa-twitter"></i></a>
              <a href="#" class="facebook"><i class="fa fa-facebook"></i></a>
              <a href="#" class="instagram"><i class="fa fa-instagram"></i></a>
              <a href="#" class="google-plus"><i class="fa fa-google-plus"></i></a>
              <a href="#" class="linkedin"><i class="fa fa-linkedin"></i></a>
            </div>

          </div>

          <div class="col-lg-3 col-md-6 footer-newsletter">
            <h4>Our Newsletter</h4>
            <p>Lorem ipsum dolor sit amet, consectetur adipisicing elit. Perferendis possimus earum impedit assumenda, laudantium maiores accusamus ducimus tempora blanditiis labore laboriosam provident culpa alias minima non quibusdam consectetur vitae voluptas?</p>
            <form action="" method="post">
              <input type="email" name="email" placeholder="Enter your Email here"><input type="submit"  value="Subscribe">
            </form>
          </div>

        </div>
      </div>
    </div>

    <div class="container">
      <div class="copyright">
        &copy; Copyright <strong>eXa</strong>. All Rights Reserved
      </div>
      <div class="credits">
       Designed by Us for You
      </div>
    </div>
  </footer><!-- #footer -->

  <a href="#" class="back-to-top"><i class="fa fa-chevron-up"></i></a>

  <!-- JavaScript Libraries -->
  <script src="lib/jquery/jquery.min.js"></script>
  <script src="lib/jquery/jquery-migrate.min.js"></script>
  <script src="lib/bootstrap/js/bootstrap.bundle.min.js"></script>
  <script src="lib/easing/easing.min.js"></script>
  <script src="lib/superfish/hoverIntent.js"></script>
  <script src="lib/superfish/superfish.min.js"></script>
  <script src="lib/wow/wow.min.js"></script>
  <script src="lib/waypoints/waypoints.min.js"></script>
  <script src="lib/counterup/counterup.min.js"></script>
  <script src="lib/owlcarousel/owl.carousel.min.js"></script>
  <script src="lib/isotope/isotope.pkgd.min.js"></script>
  <script src="lib/lightbox/js/lightbox.min.js"></script>
  <script src="lib/touchSwipe/jquery.touchSwipe.min.js"></script>
  <!-- Contact Form JavaScript File -->
  <script src="contactform/contactform.js"></script>

  <!-- Template Main Javascript File -->
  <script src="js/main.js"></script>

<!-- </body> -->
<!-- </html> -->

<?php require('include/layout/indexscript.php'); ?>