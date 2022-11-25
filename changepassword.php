<?php
      $title = "Recovery";
      require_once('include/functions/main.php');
      require('include/layout/header.php');

      jobseeker_logged_in_redirect();

      if (isset($_POST['recovery_password'])) {

          $email = htmlentities(strip_tags(trim($_POST['email'])));

          if ($_POST['email'] === "") {

              $email_error = "Please enter an email address";

          }

          elseif (!(emailExist($email))) {

              $email_exist_error = "Sorry, " . $email . "does not exist. Provide a correct email address";

          }

      }

 ?>


    <div class="container">

        <div class="row">

              <form class="" action="" method="post">

                    <div class="col-md-4">

                    </div>

                    <div class="recovery-div col-md-4 form-group">

                      <div style="margin-bottom:30px;"class="">

                        <label for="email">Enter your email address</label> <br>
                        <input type="email" name="email" value="" class="form-control">
                        <?=$email_exist_error;?>


                      </div>

                      <button type="submit" name="recovery_password"  class="btn btn-primary btn-block">Recover Password</button>

                    </div>

                    <div class="col-md-4">

                    </div>

              </form>

        </div>

    </div>


 <?php

 require('include/layout/page-header-footer.php');
 require('include/layout/footer.php');
?>
