<?php
    require('include/layout/output_buffer.php');
    require('include/database/pdo_connect.php');
    require_once('include/functions/user.php');
    require_once('include/functions/employer.php');
    require('include/layout/variables.php');



    session_start();



    // if (logged_in() === true) {
    //   $session_user_id = $_SESSION['id'];
    //   $user_data = user_data($session_user_id, 'id' , 'username' , 'password' , 'first_name' , 'last_name' , 'mobile', 'email' , 'allow_email' , 'image');
    //   if (userActive($user_data['username']) === true && userDeactivated($user_data['username']) === true) {
    //       session_destroy();
    //       redirect('index.php');
    //   }
    // }


    if (logged_in()) {

        $session_user_id = $_SESSION['id'];
        $username = $_SESSION['username'];
        $user_data = user_data($session_user_id, $username, 'jobseeker.id' , 'jobseeker.username' , 'jobseeker.first_name' , 'jobseeker.last_name' , 'jobseeker.middle_name' ,  'jobseeker.mobile', 'jobseeker.email' , 'jobseeker.sex',  'jobseeker.age', 'jobseeker.address' , 'jobseeker.state_of_origin' , 'jobseeker.lga' , 'jobseeker.state_of_residence' , 'jobseeker.specialization' , 'jobseeker.about' , 'jobseeker.profile_image', 'jobseeker.cv', 'education.institution' , 'education.degree' , 'education.field_of_study', 'education.year_of_graduation' , 'certification.title' , 'certification.institution' , 'certification.year');

    }


 ?>
