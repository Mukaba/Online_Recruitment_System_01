<?php

      function employerDeactivated($email) {
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT deactivated from employer where email=:email');
          $query->bindParam(':email' , $email);
          $query->bindColumn('deactivated' , $deactivated);
          $query->execute();
          $query->fetchColumn();
          if ($deactivated == 1) {
              return true;
          }else {
            return false;
          }
      }

      function loginEmployer($email,$password) {
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT count(id) from employer where email=:email AND password=:password');
          $query->bindParam(':email' , $email);
          $query->bindParam(':password' , $password);
          $query->execute();
          $count = $query->fetchColumn();
          if ($count == 1) {
              return true;
          }else {
              return false;
          }
       }

    //This function checks if user's account has been desactivated
      function check_employer_deactivate($email,$password) {
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT active, deactivated FROM employer WHERE email=:email AND password=:password');
          $query->bindParam(':email' , $email);
          $query->bindParam(':password' , $password);
          $query->bindColumn('active' , $active);
          $query->bindColumn('deactivated' , $deactivated);
          $query->execute();
          $query->fetchColumn();
          if ($active == 1 && $deactivated == 1) {
              return true;
          }else {
              return false;
          }
      }

      function check_password_recovered_status($email) {
        require('../database/pdo_connect.php');
        $query = $pdo->prepare('SELECT password_recovered from employer where email=:email');
        $query->bindParam(':email' , $email);
        $query->bindColumn('password_recovered' , $result);
        $query->execute();
        $query->fetchColumn();
        return $result;
      }

      function change_password_recovered_redirect($email){
        $email = $email;
        $recovery_status = check_password_recovered_status($email);
        if (logged_in() && $recovery_status == 1) {
            return true;
        }else {
            return false;
        }
      }

      //This function checks if the provided username exists in the database
      function employerExist ($email) {
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT count(id) from employer where email=:email');
          $query->bindParam(':email', $email);
          $query->execute();
          $count = $query->fetchColumn();
          if ($count == 1) {
              return true;
          }
          else {
              return false;
          }
      }

      function passwordFetch($email) {
        require('../database/pdo_connect.php');
        $query = $pdo->prepare('SELECT password from employer where email=:email');
        $query->bindParam(':email', $email);
        $query->bindColumn('password', $password);
        $query->execute();
        $query->fetchColumn();
        return $password;
      }

      function employer_id_from_email($email) {
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT id from employer where email =:email');
          $query->bindParam(':email' , $email);
          $query->bindColumn('id' , $employer_id);
          $query->execute();
          $query->fetchColumn();
          return $employer_id;
      }

      //This function checks if the user's account has been activated.
      // The active field in user table is set = 0 for inactive and = 1 for active. the default is 0 but if user activates account by clicking the link sent to their email, the active field value changes to 1
      function employerActive($email){
          require('../database/pdo_connect.php');
          $query = $pdo->prepare('SELECT active from employer where email =:email');
          $query->bindParam(':email', $email);
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

      function employer_data($employer_id, $email) {
            require('../include/database/pdo_connect.php');
            $employer_id = (int)$employer_id;
            $email = htmlentities(strip_tags(trim($email)));

            $func_num_args = func_num_args();
            $func_get_args = func_get_args();


            if ($func_num_args > 1) {
                unset($func_get_args[0]);
                unset($func_get_args[1]);
            }

            $fields =  implode(", " , $func_get_args) ;

            $query = $pdo->prepare("SELECT $fields from employer  where email=:email");
            $query->bindParam(':email' , $email);
            $query->execute();
            $data = $query->fetchAll(PDO::FETCH_ASSOC);
            $data = $data[0];
            return $data;
        }

      if (logged_in()) {

          $session_employer_id = $_SESSION['id'];
          $email = $_SESSION['employer_email'];
          $employer_data = employer_data($session_employer_id, $email, 'name' , 'website' , 'size' , 'industry_type' , 'about');

      }



if (isset($_POST['login-employer'])) {

    try {

      $email = htmlentities(strip_tags(trim($_POST['email'])));
      $password = htmlentities(strip_tags(trim($_POST['password'])));
      $hashed_password = passwordFetch($email);

          if (employerExist($email) === false  || (password_verify($password,$hashed_password) === false) ) {
              echo "Incorrect Login Credentials. Username/Password is incorrect";
          }
          elseif (employerActive($email) === false) {
              echo  "Your Account has not been activated, Please kindly check your email and activate your account by clicking the provided link";
          }
          else{

              $login = loginEmployer($email,$hashed_password);

                  if (check_employer_deactivate($email,$hashed_password) === true) {

                      echo "Sorry, Your Account has been deactivated";

                  }else {
                      session_start();
                      $employer_id = employer_id_from_email($email);

                      $_SESSION['id'] = $employer_id;
                      $_SESSION['company_email'] = $email;

                    if (change_password_recovered_redirect($email) === true) {

                        echo "Login Successful. You must change your password to conitnue";

                    }else {

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
