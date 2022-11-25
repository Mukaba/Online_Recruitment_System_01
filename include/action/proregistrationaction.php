<?php
      session_start();
      $title = "registration";
      // require_once('../functions/user.php');
      // require('../include/layout/modal.php');
      $username = $_SESSION['username'];
      $session_user_id = $_SESSION['id'];
      
      function logged_in () {
          return (isset($_SESSION['id'])) ? true : false;
      }

      function updateJobSeeker($username,$reg_middle_name,$reg_mobile,$reg_address,$reg_specialization,$reg_sex,$reg_state_of_residence,$reg_state_of_origin,$reg_lga,$reg_age,$reg_about,$reg_allow_email,$updated_profile) {

              $updated_profile = "yes";
              require('../database/pdo_connect.php');
              $query = $pdo->prepare('UPDATE jobseeker SET middle_name = :middle_name, mobile = :mobile, address = :address, specialization = :specialization, sex = :sex, state_of_residence = :state_of_residence, state_of_origin = :state_of_origin, lga = :lga, age = :age, about = :about, allow_email =:allow_email, updated_profile = :updated_profile WHERE username = :username');

                    $query->bindParam(':username' , $username);
                    $query->bindParam(':middle_name' , $reg_middle_name);
                    $query->bindParam(':mobile' , $reg_mobile);
                    $query->bindParam(':address' , $reg_address);
                    $query->bindParam(':specialization' , $reg_specialization);
                    $query->bindParam(':sex' , $reg_sex);
                    $query->bindParam(':state_of_residence' , $reg_state_of_residence);
                    $query->bindParam(':state_of_origin' , $reg_state_of_origin);
                    $query->bindParam(':lga' , $reg_lga);
                    $query->bindParam(':age' , $reg_age);
                    $query->bindParam(':about' , $reg_about);
                    $query->bindParam(':allow_email' , $reg_allow_email);
                    $query->bindParam(':updated_profile' , $updated_profile);

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

      if (logged_in()) {

          function register_data($session_user_id, $username) {
              require('../database/pdo_connect.php');
              $session_user_id = (int)$session_user_id;
              $username = htmlentities(strip_tags(trim($username)));

              $func_num_args = func_num_args();
              $func_get_args = func_get_args();

              if ($func_num_args > 1) {
                  unset($func_get_args[0]);
                  unset($func_get_args[1]);
              }

              $fields =  implode(", " , $func_get_args) ;
              $query = $pdo->prepare("SELECT $fields from jobseeker where username=:username");
              $query->bindParam(':username' , $username);
              $query->execute();
              $data = $query->fetchAll(PDO::FETCH_ASSOC);
              $data = $data[0];
              return $data;
            }

            $register_data = register_data($session_user_id, $username, 'id' , 'username' , 'first_name' , 'last_name' ,  'jobseeker.email');

      }


      if (isset($_POST['complete'])) {

          $reg_middle_name = htmlentities(strip_tags(trim($_POST['reg_middle_name'])));
          $reg_age = htmlentities(strip_tags(trim($_POST['reg_age'])));
          $reg_mobile = htmlentities(strip_tags(trim($_POST['reg_mobile'])));
          $reg_state_of_origin = htmlentities(strip_tags(trim($_POST['reg_state_of_origin'])));
          $reg_state_of_residence = htmlentities(strip_tags(trim($_POST['reg_state_of_residence'])));
          $reg_sex = htmlentities(strip_tags(trim($_POST['reg_sex'])));
          $reg_lga = htmlentities(strip_tags(trim($_POST['reg_lga'])));
          $reg_address = htmlentities(strip_tags(trim($_POST['reg_address'])));
          $reg_specialization = htmlentities(strip_tags(trim($_POST['reg_specialization'])));
          $reg_type_of_institution = htmlentities(strip_tags(trim($_POST['reg_type_of_institution'])));
          $reg_about = htmlentities(strip_tags(trim($_POST['reg_about'])));
          $reg_allow_email = isset($_POST['reg_allow_email']) ? 1 : 0;
          $reg_title = htmlentities(strip_tags(trim($_POST['reg_title'])));
          $reg_institution = htmlentities(strip_tags(trim($_POST['reg_institution'])));
          $reg_year = htmlentities(strip_tags(trim($_POST['reg_year'])));
          $reg_university = htmlentities(strip_tags(trim($_POST['reg_university'])));
          $reg_degree = htmlentities(strip_tags(trim($_POST['reg_degree'])));
          $reg_field_of_study = htmlentities(strip_tags(trim($_POST['reg_field_of_study'])));
          $reg_year_of_graduation = htmlentities(strip_tags(trim($_POST['reg_year_of_graduation'])));
          $updated_profile = "yes";

          if (updateJobSeeker($username,$reg_middle_name,$reg_mobile,$reg_address,$reg_specialization,$reg_sex,$reg_state_of_residence,$reg_state_of_origin,$reg_lga, $reg_age,$reg_about,$reg_allow_email,$updated_profile) && addJobSeekerCertification($username,$reg_title,$reg_institution,$reg_year) &&  addJobSeekerEducation($username,$reg_type_of_institution,$reg_university,$reg_degree, $reg_field_of_study,$reg_year_of_graduation)) {

              echo "Congratulations!!! Your profile has been fully updated.";

          }else {

              echo "Ooops!!! Something went wrong. Please try again.";

          }


      }
?>
