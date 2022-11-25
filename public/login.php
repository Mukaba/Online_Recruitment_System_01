<?php
      require_once('../include/functions/main.php');
      $title = "Login";
      require('../include/layout/header.php');
      $error = array();

      // elseif (isset($_POST['submit-employer'])) {
      //
      // }

      if (isset($_GET['jobseeker'])) { ?>

        <?php

              if (isset($_POST['submit-seeker'])) {

                $username = htmlentities(strip_tags(trim($_POST['username'])));

                $password = htmlentities(strip_tags(trim($_POST['password'])));

                $hashed_password = passwordFetch($username);

                // $password = md5($password);

                  if(empty($username) || $username === ""){

                      $incorrect_username_error= "username cannot be blank";

                  }

                  // elseif (empty($password)) {
                  //
                  //     $password_error = "password must not be blank";
                  //
                  // }

                  elseif (userExist($username) === false  || (password_verify($password,$hashed_password) === false) ) {

                      $userexist_error = "Incorrect Login Credentials. Username/Password is incorrect";

                  }

                  elseif (userActive($username) === false) {

                      $active_error =  "Your Account has not been activated, Please kindly check your email and activate your account by clicking the provided link";

                  }

                  else{

                        $login = loginJobseeker($username,$hashed_password);

                        // if ($login === false) {
                        //
                        //     $username_password_match_error = "Username/Password combination does not match";
                        //
                        // }
                        if (check_user_deactivate($username,$hashed_password) === true) {

                            $user_deactivated = "Sorry, Your Account has been deactivated";

                        }

                        else {

                          $user_id = user_id_from_username($username);

                          $_SESSION['id'] = $user_id;

                          if (change_password_recovered_redirect($username) === true) {

                              redirect('changepassword.php?force');

                          }

                          else {
                            redirect('jobseeker-dashboard.php');
                            print_r(headers_list);
                            exit;

                          }

                        }

                      }

              }

         ?>

<!--
        <img class="page-logo img-responsive center-block "src="img/call.png" alt="" />

        <div class="container jobseeker-login">

            <div class="col-md-offset-3 col-md-6">

              <div id="signin-panel" class="panel panel-default">

                  <div id="sign-head" class="panel-heading">
                    <h3 class="panel-title text-center">Jobseeker Login</h3>
                  </div>

                <div class="panel-body">

                  <form class="" action="login.php?jobseeker" method="post">
                      <p class="text-center" style="color:red;font-size:13px;margin-left:5px;"><?= $userexist_error; ?></p>
                      <p class="text-center" style="color:red;font-size:13px;margin-left:5px;"><?= $user_deactivated; ?></p>
                      <p class="text-center" style="color:red;font-size:13px;margin-left:5px;"><?= $active_error; ?></p>
                      <div class="form-group">
                          <label for=""><b>Username</b></label>
                          <input class="form-control" type="text" name="username" value="">
                          <p style="color:red;font-size:13px;margin-left:5px;"><?= $incorrect_username_error; ?></p>
                      </div>

                      <div class="form-group">
                        <label for=""><b>Password</b></label>
                        <input class="form-control" type="password" name="password" value="">
                      </div>

                      <button class="btn btn-primary btn-block" type="submit" name="submit-seeker">Submit</button>
                  </form>

                </div>

                <div id="sign-footer" class="panel-footer">
                        <form class="" action="login.php?jobseeker" method="post">

                        </form>
                        <p class="forgot-password-p text-center"><a class="forgot-password" href="#">Forgot Password</a></p>
                        <p class="sign-in-p text-center">
                            In case you are using a public/shared computer we recommend that you logout to prevent any un-authorized access to your account
                        </p>
                </div>

              </div>

            </div>

        </div>


      <?php

      }

      elseif (isset($_GET['employer'])) { ?>

        <img class="page-logo img-responsive center-block "src="img/call.png" alt="" />
        <div class="container employer-login">

            <div class="col-md-offset-3 col-md-6">

              <div id="signin-panel" class="panel panel-default">
                <div id="sign-head" class="panel-heading">
                  <h3 class="panel-title text-center">Employer Login</h3>
                </div>
                <div class="panel-body">

                  <form class="" action="index.html" method="post">
                      <div class="form-group">
                          <label for=""><b>Company Email</b></label>
                          <input class="form-control" type="text" name="employer-username" value="">
                      </div>

                      <div class="form-group">
                        <label for=""><b>Password</b></label>
                        <input class="form-control" type="password" name="employer-password" value="">
                      </div>
                  </form>

                </div>

                <div id="sign-footer" class="panel-footer">
                        <form class="" action="" method="post">
                            <button class="btn btn-primary btn-block" type="submit" name="submit-employer">Submit</button>
                        </form>
                        <p class="forgot-password-p text-center"><a class="forgot-password" href="#">Forgot Password</a></p>
                        <p class="sign-in-p text-center">
                            In case you are using a public/shared computer we recommend that you logout to prevent any un-authorized access to your account
                        </p>
                </div>

              </div>

            </div>

        </div> -->


      <?php
      }
      require('../include/layout/page-header-footer.php');
      require('../include/layout/footer.php');
 ?>
