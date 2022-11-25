<?php
      session_start();
      function userDeactivated($username) {
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT deactivated from jobseeker where username=:username');
          $query->bindParam(':username' , $username);
          $query->bindColumn('deactivated' , $deactivated);
          $query->execute();
          $query->fetchColumn();
          if ($deactivated == 1) {
              return true;
          }else {
            return false;
          }
      }

      function isProfileUpdated($username) {
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT updated_profile from jobseeker where username=:username');
          $query->bindParam(':username' , $username);
          $query->bindColumn('updated_profile' , $updated_profile);
          $query->execute();
          $query->fetchColumn();
          if ($updated_profile == "no") {
              return true;
          }else {
            return false;
          }
      }

      function loginJobseeker($username,$password) {
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT count(id) from jobseeker where username=:username AND password=:password');
          $query->bindParam(':username' , $username);
          $query->bindParam(':password' , $password);
          $query->execute();
          $count = $query->fetchColumn();
          if ($count == "1") {
              return true;
          }else {
              return false;
          }
       }

    //This function checks if user's account has been deacivated
      function check_user_deactivate($username,$password) {
        require('../database/pdo_connect.php');
        $query = $pdo->prepare('SELECT active, deactivated from jobseeker where username=:username and password=:password');
        $query->bindParam(':username' , $username);
        $query->bindParam(':password' , $password);
        $query->bindColumn('active' , $active);
        $query->bindColumn('deactivated' , $deactivated);
        $query->execute();
        $query->fetchColumn();
        if ($active == "1" && $deactivated == "1") {
            return true;
        }else {
            return false;
        }

      }

      function check_password_recovered_status($username) {
        require('../database/pdo_connect.php');
        $query = $pdo->prepare('SELECT password_recovered from jobseeker where username=:username');
        $query->bindParam(':username' , $username);
        $query->bindColumn('password_recovered' , $result);
        $query->execute();
        $query->fetchColumn();
        return $result;
      }

      function change_password_recovered_redirect($username){
        $username =$username;
        $recovery_status = check_password_recovered_status($username);
        if (logged_in() && $recovery_status == "1") {
            return true;
        }else {
            return false;
        }
      }

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

      function passwordFetch($username) {
        require('../database/pdo_connect.php');
        $query = $pdo->prepare('SELECT password from jobseeker where username=:username');
        $query->bindParam(':username', $username);
        $query->bindColumn('password', $password);
        $query->execute();
        $query->fetchColumn();
        return $password;
      }

      function user_id_from_username($username) {
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT id from jobseeker where username =:username');
          $query->bindParam(':username' , $username);
          $query->bindColumn('id' , $user_id);
          $query->execute();
          $query->fetchColumn();
          return $user_id;
      }

      //This function checks if the user's account has been activated.
      // The active field in user table is set = 0 for inactive and = 1 for active. the default is 0 but if user activates account by clicking the link sent to their email, the active field value changes to 1
      function userActive($username){
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT active from jobseeker where username=:username');
          $query->bindParam(':username', $username);
          $query->bindColumn('active', $active);
          $query->execute();
          $query->fetchColumn();
          if ($active == "0") {
              return false;
          }
          else {
            return true;
          }
      }

      function logged_in () {
          return (isset($_SESSION['id'])) ? true : false;
      }

      if (logged_in()) {

          $session_user_id = $_SESSION['id'];
          $username = $_SESSION['username'];
          $user_data = user_data($session_user_id, $username, 'jobseeker.id' , 'jobseeker.username' , 'jobseeker.first_name' , 'jobseeker.last_name' , 'jobseeker.middle_name' ,  'jobseeker.mobile', 'jobseeker.email' , 'jobseeker.sex', 'jobseeker.address' , 'jobseeker.state_of_origin' , 'jobseeker.lga' , 'jobseeker.state_of_residence' , 'jobseeker.specialization' , 'jobseeker.about' , 'education.university' , 'education.degree' , 'education.field_of_study', 'education.year_of_graduation' , 'certification.title' , 'certification.institution' , 'certification.year');

      }



if (isset($_POST['login-seeker'])) {

    try {

      $username = htmlentities(strip_tags(trim($_POST['username'])));
      $password = htmlentities(strip_tags(trim($_POST['password'])));
      $hashed_password = passwordFetch($username);

          if (userExist($username) === false  || (password_verify($password,$hashed_password) === false) ) {
              echo "Incorrect Login Credentials. Username/Password is incorrect";
          }
          elseif (userActive($username) === false) {
              echo  "Your Account has not been activated, Please kindly check your email and activate your account by clicking the provided link";
          }
          else{

              $login = loginJobseeker($username,$hashed_password);

                  if (check_user_deactivate($username,$hashed_password) === true) {
                      echo "Sorry, Your Account has been deactivated";
                  }

                  else {

                    $user_id = user_id_from_username($username);

                    $_SESSION['id'] = $user_id;
                    $_SESSION['username'] = $username;

                    if (change_password_recovered_redirect($username) === true) {

                        echo "Login Successful. You must change your password to conitnue";

                    }
                    elseif (isProfileUpdated($username) === true) {
                        echo "Login Successful. You can now update your profile";
                    }

                  else {

                      echo "Login Successful";

                  }

                }

          }

    } catch (Exception $e) {
        $error = "";
        $error = $e->getMessage();
    }


}
?>
