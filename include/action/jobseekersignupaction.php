<?php
//This function checks if the provided username exists in the database
function userExist ($username) {
    require('../database/pdo_connect.php');
    $query = $pdo->prepare('SELECT count(id) from jobseeker where username=:username');
    $query->bindParam(':username', $username);
    $query->execute();
    $count = $query->fetchColumn();
    if ($count == 1) {
        return true;
    }
    else {
        return false;
    }
}

function emailExist($email) {
      require('../database/pdo_connect.php');
      $query = $pdo->prepare('SELECT count(id) from jobseeker where email=:email');
      $query->bindParam(':email' , $email);
      $query->execute();
      $count = $query->fetchColumn();
      if ($count == 1) {
          return true;
      }
      else {
          return false;
      }
  }


  function registerJobseeker($reg_lastname, $reg_firstname, $reg_username, $reg_password, $reg_email, $reg_email_code) {
      require('../database/pdo_connect.php');
      $query = $pdo->prepare('INSERT into jobseeker (last_name, first_name, username, password, email, email_code) values (:lastname, :firstname, :username, :password, :email, :email_code) ');
      $query->bindParam(':lastname' , $reg_lastname);
      $query->bindParam(':firstname' , $reg_firstname);
      $query->bindParam(':username' , $reg_username);
      $query->bindParam(':password' , $reg_password);
      $query->bindParam(':email' , $reg_email);
      $query->bindParam(':email_code' , $reg_email_code);

      if ($query->execute()) {
          return true;
      }
      else {
          return false;
      }

  }

  function addJobSeekerCertification($username,$reg_title,$reg_institution,$reg_year) {
    require('../database/pdo_connect.php');
    $query = $pdo->prepare('INSERT into certification
      (jobseeker_username, title, institution, year)
      values (:jobseeker_username,:title, :institution, :year)');

          $query->bindParam(':jobseeker_username' , $username);
          $query->bindParam(':title' , $reg_title);
          $query->bindParam(':institution' , $reg_institution);
          $query->bindParam(':year' , $reg_year);

              if ($query->execute()) {
                  return true;
              }
              else {
                  return false;
              }
    }

  function addJobSeekerEducation($username,$reg_type_of_institution,$reg_institution,$reg_degree, $reg_field_of_study,$reg_year_of_graduation) {

          require('../database/pdo_connect.php');
          $query = $pdo->prepare('INSERT into education
            (jobseeker_username,type_of_institution,institution, degree, field_of_study, year_of_graduation) values (:jobseeker_username,:type_of_institution,:institution, :degree, :field_of_study, :year_of_graduation)');

                $query->bindParam(':jobseeker_username' , $username);
                $query->bindParam(':type_of_institution' , $reg_type_of_institution);
                $query->bindParam(':institution' , $reg_institution);
                $query->bindParam(':degree' , $reg_degree);
                $query->bindParam(':field_of_study' , $reg_field_of_study);
                $query->bindParam(':year_of_graduation' , $reg_year_of_graduation);

                    if ($query->execute()) {
                        return true;
                    }
                    else {
                        return false;
                    }
      }


  function sendMail($to, $subject, $body) {
      mail($to, $subject, $body, 'noreply@smashjob.ng');
  }

  //registers jobseeker
  if (isset($_POST['register-jobseeker'])) {

      $reg_firstname = htmlentities(strip_tags(addslashes(trim($_POST['reg_first_name']))));
      $reg_lastname = htmlentities(strip_tags(trim($_POST['reg_last_name'])));
      $reg_username = htmlentities(strip_tags(addslashes(trim($_POST['reg_username']))));
      $reg_password = htmlentities(strip_tags(addslashes(trim($_POST['reg_password']))));
      $reg_confirm_password = htmlentities(strip_tags(addslashes(trim($_POST['reg_confirm_password']))));
      $reg_email = htmlentities(strip_tags(addslashes(trim($_POST['reg_email']))));
      $reg_password = password_hash($reg_password, PASSWORD_DEFAULT);
      $reg_email_code = md5($reg_lastname.microtime());

      $reg_title="";
      $reg_institution="";
      $reg_year="";
      $reg_type_of_institution="";
      $reg_institution = "";
      $reg_degree="";
      $reg_field_of_study="";
      $reg_year_of_graduation="";

          if (userExist($reg_username)) {
              echo "Sorry " . $reg_username . " already exist";
          }
          elseif (emailExist($reg_email)) {
              echo "Sorry an account has already been registered with the email: " . $reg_email;
          }
          elseif (registerJobseeker($reg_lastname, $reg_firstname, $reg_username, $reg_password, $reg_email,$reg_email_code) && addJobSeekerEducation($reg_username,$reg_type_of_institution,$reg_institution,$reg_degree, $reg_field_of_study,$reg_year_of_graduation)&& addJobSeekerCertification($reg_username,$reg_title,$reg_institution,$reg_year)) {

          $subject = 'Account activation';

          $body = "Hello " .$reg_firstname. ", \n\n\n\n
          Copy the url below and paste in your browser to activate your account\n\n\n\n
          localhost:8080/smashjob/activate.php?email=".$reg_email."&email_code=".$reg_email_code." \n\n\n\n
          -smashJob.ng" ;

            sendMail($reg_email, $subject, $body);

            echo "Thanks, " .$reg_firstname. " Your registration was succesful. Your activation code has been sent to the email provided. Kindly check your mail";
          }

          //localhost/smashjob/public/activate.php?email=profchydon@gmail.com&email_code=48a3bf8944c168e9ab04bfab333ad855

  }




 ?>
