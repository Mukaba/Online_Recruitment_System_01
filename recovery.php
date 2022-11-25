<?php
      $title = "Recovery";
      require_once('include/functions/main.php');
      require('include/layout/header.php');
      require('include/layout/modal.php');

      jobseeker_logged_in_redirect();

      $recover = array('username' , 'password');

          if (isset($_GET['recover']) == true && in_array($_GET['recover'] , $recover) == true) {

              $recover_mode = htmlentities(strip_tags($_GET['recover']));

                if (isset($_POST['recover_password'])) {

                    $recover_email = htmlentities(strip_tags(trim($_POST['email'])));

                      if (empty($_POST['email'])) {

                          $email_error = "Please enter an email address";

                      }

                      elseif (!(emailExist($recover_email))) {

                          $email_exist_error = "Sorry, " . $recover_email . " does not exist. Provide a correct email address";

                      }
                      else {
                            if (detailRecovery($recover_mode,$recover_email)) {
                                redirect('recovery.php?success');
                            }
                            else {
                                echo "string";
                            }
                      }

                }elseif (isset($_POST['recover_username'])) {

                    $recover_email = htmlentities(strip_tags(trim($_POST['email'])));

                      if ((empty($_POST['email']))) {

                          $email_error = "Please enter an email address";

                      }

                      elseif (!(emailExist($recover_email))) {

                          $email_exist_error = "Sorry, " . $recover_email . " does not exist. Provide a correct email address";

                      }
                      else {
                            if (detailRecovery($recover_mode,$recover_email)) {
                                redirect('recovery.php?success');
                            }
                            else {
                                echo "string";
                            }
                      }

                }

          }
          else {
              redirect('index.php');
          }
 ?>
 
    <div class="container">

        <div class="row">

              <form class="" id="recovery-form" action="" method="post">

                  <?php

                      if ($recover_mode == "password") { ?>

                          <div class="col-md-4"></div>

                          <div class="recovery-div col-md-4 form-group">

                            <div style="margin-bottom:20px;"class="">

                              <label for="email">Enter your email address</label> <br>
                              <input type="email" name="email" value="" class="form-control">
                              <p class="emailerror">
                                  <?=$email_exist_error;?>
                              </p>

                            </div>

                            <button type="submit" name="recover_password"  class="btn btn-primary btn-block">Recover Password</button>

                          </div>

                          <div class="col-md-4"></div>


                  <?php
                      }elseif ($recover_mode = "username") { ?>

                        <div class="col-md-4"></div>

                        <div class="recovery-div col-md-4 form-group">

                          <div style="margin-bottom:20px;"class="">

                            <label for="email">Enter your email address</label> <br>
                            <input type="email" name="email" value="" class="form-control">
                            <p class="emailerror">
                                <?=$email_exist_error;?>
                            </p>

                          </div>

                          <button type="submit" name="recover_username"  class="btn btn-primary btn-block">Recover Username</button>

                        </div>

                        <div class="col-md-4"></div>


                  <?php
                      }
                  ?>


              </form>

        </div>

    </div>


 <?php

 require('include/layout/page-header-footer.php');
 require('include/layout/footer.php');
?>
