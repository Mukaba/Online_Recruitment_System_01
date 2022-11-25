<?php
    function getJobs($category) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT id, employer, title FROM jobs WHERE category = :category');
        $query->bindParam(':category' , $category);
        $query->execute();
        $jobs = $query->fetchAll(PDO::FETCH_ASSOC);
        return $jobs;
    }

    function countApplication($jobseeker_id) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT id FROM application WHERE jobseeker_id = :jobseeker_id');
        $query->bindParam(':jobseeker_id' , $jobseeker_id);
        $query->execute();
        $count = $query->fetchAll(PDO::FETCH_ASSOC);
        return count($count);
    }

    function checkApplication($jobseeker_id) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT id FROM application WHERE jobseeker_id = :jobseeker_id AND reviewed = "1"');
        $query->bindParam(':jobseeker_id' , $jobseeker_id);
        $query->execute();
        $query->execute();
        $count = $query->fetchAll(PDO::FETCH_ASSOC);
        return count($count);
    }

    function applyForJob ($jobseeker_id ,  $job_id, $cv, $date_applied, $additional_info) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('INSERT into application (jobseeker_id , job_id, cv, date_applied, additional_info) values (:jobseeker_id , :job_id, :cv, :date_applied, :additional_info)');
        $query->bindParam(':jobseeker_id' , $jobseeker_id);
        $query->bindParam(':job_id' , $job_id);
        $query->bindParam(':cv' , $cv);
        $query->bindParam(':date_applied' , $date_applied);
        $query->bindParam(':additional_info' , $additional_info);
        if ($query->execute()) {
            return true;
        }else {
            return false;
        }
    }

    function redirect($location) {
        header("Location: " . $location);
        exit;
    }

    function getJobsAvailable () {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT * FROM jobs ORDER BY date_posted ASC');
            if ($query->execute()) {
              $jobs = $query->fetchAll(PDO::FETCH_ASSOC);
              return $jobs;
            }
    }

    function getPresentJob ($id) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT * FROM jobs WHERE id = :id');
        $query->bindParam(':id' , $id);
            if ($query->execute()) {
              $jobs = $query->fetchAll(PDO::FETCH_ASSOC);
              return $jobs;
            }
    }

    function getJobEmployer ($id) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT website, email, address, industry_type, about FROM jobs WHERE id = :id');
        $query->bindParam(':id' , $id);
            if ($query->execute()) {
              $jobs = $query->fetchAll(PDO::FETCH_ASSOC);
              return $jobs;
            }
    }

    function logged_in () {
        return (isset($_SESSION['id'])) ? true : false;
    }

    function sendMail($to, $subject, $body) {
        mail($to, $subject, $body, 'From: noreply@smashJob.ng');
    }

    function logged_in_redirect () {
          if (logged_in()) {
              redirect('index.php');
          }
    }

    function not_logged_in_redirect () {
          if (!(isset($_SESSION['id']))) {
              redirect('index.php');
          }
    }

    function jobseeker_logged_in_redirect () {
          if (logged_in()) {
              redirect('jobseeker-dashboard.php');
          }
    }

    function activateAccount($email,$email_code) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT count(id) FROM jobseeker WHERE email=:email AND email_code=:email_code AND active = 0');
        $query->bindParam(':email', $email);
        $query->bindParam(':email_code', $email_code);
        $query->execute();
        $count = $query->fetchColumn();
        if ($count == "1") {
            $query = $pdo->prepare('UPDATE jobseeker SET active=1 WHERE email=:email');
            $query->bindParam(':email',$email);
            $query->execute();
            return true;
        }
        else {
            return false;
        }
    }

    //  This function gathers loggedin user's data
    function user_data($user_id, $username) {
          require('include/database/pdo_connect.php');
          $user_id = (int)$user_id;
          $username = htmlentities(strip_tags(trim($username)));

          $func_num_args = func_num_args();
          $func_get_args = func_get_args();


          if ($func_num_args > 1) {
              unset($func_get_args[0]);
              unset($func_get_args[1]);
          }

          $fields =  implode(", " , $func_get_args) ;

          $query = $pdo->prepare("SELECT $fields from jobseeker inner join education on jobseeker.username=education.jobseeker_username join certification on jobseeker.username=certification.jobseeker_username where jobseeker.username=:username");
          $query->bindParam(':username' , $username);
          $query->execute();
          $data = $query->fetchAll(PDO::FETCH_ASSOC);
          $data = $data[0];
          return $data;
      }

      function user_id_from_email ($email) {
          require('include/database/pdo_connect.php');
          $query = $pdo->prepare('SELECT id from jobseeker where email=:email');
          $query->bindParam(':email' , $email);
          $query->bindColumn('user_id' , $id);
          $query->execute();
          $query->fetchColumn();
          return $id;
      }

      function changePassword ($username,$password,$newpassword) {
            require('include/database/pdo_connect.php');
            $query = $pdo->prepare('UPDATE jobseeker set password=:newpassword where username=:username and password=:password');
            $query->bindParam(':newpassword' , $newpassword);
            $query->bindParam(':username' , $username);
            $query->bindParam(':password' , $password);
            if ($query->execute()) {
                return true;
            }
            else {
                return false;
            }
      }

      function changePasswordForRecovery ($username,$newpassword) {
            require('include/database/pdo_connect.php');
            $query = $pdo->prepare('UPDATE jobseeker set password=:newpassword where username=:username');
            $query->bindParam(':newpassword' , $newpassword);
            $query->bindParam(':username' , $username);
            if ($query->execute()) {
                return true;
            }
            else {
                return false;
            }
      }

      function haveRecoveredPassword($email) {
          require('include/database/pdo_connect.php');
          $query = $pdo->prepare('UPDATE jobseeker SET password_recovered = 1 WHERE email=:email');
          $query->bindParam(':email' , $email);
          if ($query->execute()) {
              return true;
          }
          else {
              return false;
          }
      }

      function recover_user_data($user_id, $username) {
            require('include/database/pdo_connect.php');
            $user_id = (int)$user_id;
            $username = htmlentities(strip_tags(trim($username)));

            $func_num_args = func_num_args();
            $func_get_args = func_get_args();


            if ($func_num_args > 1) {
                unset($func_get_args[0]);
                unset($func_get_args[1]);
            }

            $fields =  implode(", " , $func_get_args) ;

            $query = $pdo->prepare("SELECT $fields from jobseeker inner join education on jobseeker.username=education.jobseeker_username join certification on jobseeker.username=certification.jobseeker_username where jobseeker.username=:username");
            $query->bindParam(':username' , $username);
            $query->execute();
            $data = $query->fetchAll(PDO::FETCH_ASSOC);
            // $data = $data[0];
            return $data;
        }


      function detailRecovery ($recover_mode,$email) {
          require('include/database/pdo_connect.php');
          $getDetail = $pdo->prepare('SELECT id, username , password,  first_name FROM jobseeker WHERE email = :email');
          $getDetail->bindParam(':email' , $email);
          $getDetail->bindColumn('id' , $id);
          $getDetail->bindColumn('username' , $username);
          $getDetail->bindColumn('password' , $password);
          $getDetail->bindColumn('first_name' , $first_name);
          $getDetail->execute();
          $recover_user_data = $getDetail->fetchAll(PDO::FETCH_ASSOC);

          if ($recover_mode == 'username') {
            sendMail($email, 'Username recovery', "Hello " . $recover_user_data[0]['first_name'] . ",\n\nYour username is " . $recover_user_data[0]['username'] . "\n\n -test");
              redirect('recovery.php?success');
              // echo $recover_user_data[0]['username'];
              return true;
          }
          elseif ($recover_mode == 'password') {
              $generated_password = substr(rand(999, 999999), 0, 8 );
              $hashed_generated_password = password_hash($generated_password , PASSWORD_DEFAULT);
              if (changePasswordForRecovery($recover_user_data[0]['username'], $hashed_generated_password)) {
                  haveRecoveredPassword($email);
                  sendMail($email, 'Password recovery', "Hello " . $recover_user_data[0]['username'] . ",\n\nYour new password is " . $generated_password . "\n\n-test");
                  redirect('recovery.php?success');
                  return true;
              }else {
                return false;
              }
          }
      }

      function removeJobseekerCert($username,$id) {
          require('include/database/pdo_connect.php');
          $query = $pdo->prepare('DELETE * from certification where username=:username and id =:id');
          // $query->bindParam(':username' , $username);
          $query->bindParam(':id' , $id);
          $query->bindParam(':username' , $username);
          if ($query->execute()) {
              return true;
          }
          else {
              return false;
          }
      }

      function fetchJobseekerCert($username) {
          require('include/database/pdo_connect.php');
          $query = $pdo->prepare('SELECT certification.id, certification.title, certification.institution , certification.year  from jobseeker inner join certification on jobseeker.username=certification.jobseeker_username where jobseeker.username=:username order by year asc');
          $query->bindParam(':username' , $username);
          $query->bindColumn('title' , $title);
          $query->bindColumn('institution' , $institution);
          $query->bindColumn('year' , $year);
          $query->bindColumn('id' , $id);
          $query->execute();
          $resource = $query->fetchAll(PDO::FETCH_ASSOC);
          return $resource;
      }

      function fetchJobseekerEducation($username) {
          require('include/database/pdo_connect.php');
          $query = $pdo->prepare('SELECT education.id, education.institution, education.degree , education.field_of_study, education.year_of_graduation from jobseeker inner join education on jobseeker.username=education.jobseeker_username where jobseeker.username=:username order by year_of_graduation asc');
          $query->bindParam(':username' , $username);
          $query->bindColumn('id' , $id);
          $query->bindColumn('degree' , $degree);
          $query->bindColumn('field_of_study' , $field_of_study);
          $query->bindColumn('institution' , $university);
          $query->bindColumn('year_of_graduation' , $year_of_graduation);
          $query->execute();
          $resource = $query->fetchAll(PDO::FETCH_ASSOC);
          return $resource;
      }

      function userDeactivated($username) {
          require('include/database/pdo_connect.php');
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

      function mail_users($subject, $body) {
          require('include/database/pdo_connect.php');
          $query = $pdo->query('SELECT email,first_name from user where allow_email=1 and active=1 and deactivate=0');
          $count = $query->fetchAll(PDO::FETCH_ASSOC);
          foreach ($count as $key => $user) {
              // echo $to = $user['email'];
              // echo "<br>";
              sendMail($to, $subject, "Hello " . $user['first_name'] . ",\n\n" . $body);
          }
      }

      function protectPage() {
          if (!logged_in()) {
              redirect('index.php');
          }
          $not_logged_in_message = "Sorry you have to be log in to change your password";
          return $not_logged_in_message;
      }


    function loginJobseeker($username,$password) {
      require('include/database/pdo_connect.php');
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
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT active, deactivate from jobseeker where username=:username && password=:password');
        $query->bindParam(':username' , $username);
        $query->bindParam(':password' , $password);
        $query->bindColumn('active' , $active);
        $query->bindColumn('deactivate' , $deactivated);
        $query->execute();
        $query->fetchColumn();
        if ($active == "1" && $deactivated == "1") {
            return true;
        }else {
            return false;
        }

      }

      function check_password_recovered_status($username) {
        require('include/database/pdo_connect.php');
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


    function usernameExist($username) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT count(id) from jobseeker where username =:username');
        $query->bindParam(':username' , $username);
        $query->execute();
        $count = $query->fetchColumn();
        if ($count == "1") {
            return true;
        }
        else {
            return false;
        }
    }



    function checkPassword () {
        require('include/database/pdo_connect.php');
    }

    function user_id_from_username($username) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT id from jobseeker where username =:username');
        $query->bindParam(':username' , $username);
        $query->bindColumn('id' , $user_id);
        $query->execute();
        $query->fetchColumn();
        return $user_id;
    }

    function emailExist($email) {
          require('include/database/pdo_connect.php');
          $query = $pdo->prepare('SELECT count(id) from jobseeker where email=:email');
          $query->bindParam(':email' , $email);
          $query->execute();
          $count = $query->fetchColumn();
          if ($count == "1") {
              return true;
          }
          else {
              return false;
          }
      }

      //This function checks if the provided username exists in the database
      function userExist ($username) {
          require('include/database/pdo_connect.php');
          $query = $pdo->prepare('SELECT count(id) from jobseeker where username=:username');
          $query->bindParam(':username', $username);
          $query->execute();
          $count = $query->fetchColumn();
          if ($count == "1") {
              return true;
          }
          else {
              return false;
          }
      }

      //This function checks whether the password supplied on log in is correct
      function passwordExist($password) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT count(id) from jobseeker where password=:password');
        $query->bindParam(':password', $password);
        $query->execute();
        $count = $query->fetchColumn();
        if ($count == "1") {
            return true;
        }
        else {
            return false;
        }
      }

      function passwordFetch($username) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT password from jobseeker where username=:username');
        $query->bindParam(':username', $username);
        $query->bindColumn('password', $password);
        $query->execute();
        $query->fetchColumn();
        return $password;
      }

      // function passwordVerify($password) {
      //
      // }

      //This function checks if the user's account has been activated.
      // The active field in user table is set = 0 for inactive and = 1 for active. the default is 0 but if user activates account by clicking the link sent to their email, the active field value changes to 1
      function userActive($username){
          require('include/database/pdo_connect.php');
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

      function change_profile_image($id, $file_temp, $file_ext) {
          require('include/database/pdo_connect.php');
          $file_path = "img/profile/" . substr(md5(time()), 0 , 10) . "." . $file_ext;
          move_uploaded_file($file_temp,$file_path);
          $query = $pdo->prepare('UPDATE jobseeker SET profile_image =:image WHERE id=:id');
          $query->bindParam(':image' , $file_path);
          $query->bindParam(':id' , $id);
          if ($query->execute()) {
              return true;
          }else {
              return false;
          }
      }

      function change_cv($id, $file_temp, $file_ext) {
          require('include/database/pdo_connect.php');
          $file_path = "img/cv/" . substr(md5(time()), 0 , 10) . "." . $file_ext;
          move_uploaded_file($file_temp,$file_path);
          $query = $pdo->prepare('UPDATE jobseeker SET cv =:image WHERE id=:id');
          $query->bindParam(':image' , $file_path);
          $query->bindParam(':id' , $id);
          if ($query->execute()) {
              return true;
          }else {
              return false;
          }
      }

      function isProfileImageUploaded($username) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT profile_image from jobseeker where username=:username');
        $query->bindParam(':username', $username);
        $query->bindColumn('profile_image', $profile_image);
        $query->execute();
        $query->fetchColumn();
        if ($profile_image == "") {
            return false;
        }
        else {
          return true;
        }
      }

      function isCvUploaded($username) {
        require('include/database/pdo_connect.php');
        $query = $pdo->prepare('SELECT cv from jobseeker where username=:username');
        $query->bindParam(':username', $username);
        $query->bindColumn('cv', $cv);
        $query->execute();
        $query->fetchColumn();
        if ($cv == "") {
            return false;
        }
        else {
          return true;
        }
      }

        function addJobSeekerCertification($username,$reg_title,$reg_institution,$reg_year) {
          require('database/pdo_connect.php');
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

                require('database/pdo_connect.php');
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

        function registerJobseeker($reg_lastname, $reg_firstname, $reg_username, $reg_password, $reg_email, $reg_email_code) {

            require('include/database/pdo_connect.php');

            $query = $pdo->prepare('INSERT into jobseeker
              (last_name, first_name, username, password, email, email_code)
              values (:lastname, :firstname, :username, :password, :email, :email_code) ');

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

        function updateJobseekerPersonalDetails($last_name, $first_name, $middle_name, $email, $mobile, $username, $sex) {

            require('include/database/pdo_connect.php');

            $query = $pdo->prepare('UPDATE jobseeker set last_name=:last_name , first_name=:first_name, middle_name=:middle_name, email=:email, mobile=:mobile, sex = :sex WHERE username=:username');

            $query->bindParam(':last_name' , $last_name);
            $query->bindParam(':first_name' , $first_name);
            $query->bindParam(':middle_name' , $middle_name);
            $query->bindParam(':email' , $email);
            $query->bindParam(':mobile' , $mobile);
            $query->bindParam(':username' , $username);
            $query->bindParam(':sex' , $sex);

            if ($query->execute()) {
                return true;
            }
            else {
                return false;
            }

        }

 ?>
