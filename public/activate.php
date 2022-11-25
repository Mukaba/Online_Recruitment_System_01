<?php
      require_once('../include/functions/main.php');
      $title = "Activation";
      require('../include/layout/header.php');
      logged_in_redirect();

      // if (isset($_POST['to-login'])) {
      //     redirect('login.php?jobseeker');
      // }



      if (isset($_GET['success'])) { ?>

          <<div style="margin-bottom:50px" class="container text-center">

              <div id="activate-success-page-row" class="row">
                  <h2>Congratulations. Your account has been successfully activated.</h2>
                  <a id="activate-success-page-a" href="index.php">continue to login page</a>
              </div>

          </div>


      <?php
      }
      else {

          if (isset($_GET['email']) && isset($_GET['email_code'])) {
              $email = trim($_GET['email']);
              $email_code = trim($_GET['email_code']);

        if (emailExist($email) === false) {
            echo $activate_email_error = "Ooops something went wrong. and we couldn't find that email account";
        }
        elseif (activateAccount($email,$email_code) === false) {
            echo $activate_email_error = "We had problems activating your account";
        }
        else {
            redirect('activate.php?success');
        }
      }
    }
 ?>


 <?php

 require('../include/layout/page-header-footer.php');
 require('../include/layout/footer.php');
?>
