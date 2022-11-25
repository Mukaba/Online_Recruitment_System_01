<!-- Sign in modal for job seeker -->
<div class="modal fade" id="signinseeker" tabindex="-1" role="dialog" aria-labelledby="signinLabel">

    <div id="login-modal" class="modal-dialog" role="document">

        <div class="modal-content con" style="border:none; border-radius:10px;">

            <div class="modal-header">
                

                <!-- <button type="button" class="close" data-dismiss="modal" aria-label="Close" name="button"><span style="color:white" aria-hidden="true">&times;</span></button> -->

                <!-- <img class="img img-responsive center"src="" alt="Smashjob" /> -->

                <h4 class="modal-title text-center" id="signinLabel"><b>Jobseeker Login</b>
                     <button type="button" data-dismiss="modal" style="width:2px; background-color:transparent; position:relative; right:-80px; color:CFD2CF; text-decoration:none; marging:0px; padding:0px;" onmouseover="this.style.color='red'" onmouseout="this.style.color=''"><b>X</b></button>
                </h4> 
                

            </div>

            <div class="modal-body">

                <div class="container-fluid">

                    <div class="row">

                        <div class="col-md-offset-2 col-md-8">

                            <p style="font-size:12px;" id="login-error" class="text-center"></p>

                            <form id="jobseeker-signin-form" class="" action="include/action/jobseekerloginaction.php" method="post" novalidate="" name="loginform">

                                <div class="form-group">
                                    <label for=""><b>Username</b></label>
                                    <input class="form-control" type="text" name="username" id="username" ng-model="user.username" ng-required="true">
                                    <p style="font-size:12px;" id="login-error" class="" ng-show="loginform.username.$invalid && loginform.username.$touched">Please enter a username to sign in</p>
                                </div>

                                <div class="form-group">
                                  <label for=""><b>Password</b></label>
                                  <input class="form-control" type="password" name="password" id="password" ng-model="user.password" ng-required="true">
                                </div>

                                 <button class="btn btn-primary btn-block" id="login-seeker" type="submit" name="login-seeker">Login</button>
                                 
                        </div>

                    </div>

                </div>

            </div>

            <div id="seeker-modal" class="modal-footer text-center">

                 <div class="container-container text-center">
                     <a class="text-center" id="forgot-password" href="recovery.php?recover=username">Forgot Username?</a> <span id="or">or</span> <a class="text-center" id="forgot-password" href="recovery.php?recover=password">Forgot Password?</a>
                 </div>

                  <p class="sign-in-p text-center">
                       In case you are using a public/shared computer we recommend that you logout to prevent any un-authorized access to your account
                  </p>
            </div>

              </form>

        </div>

    </div>

</div>
