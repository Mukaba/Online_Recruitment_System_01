<!-- Sign in modal for employer -->
<div class="modal fade" id="signinemployer" tabindex="-1" role="dialog" aria-labelledby="signinLabel">

    <div class="modal-dialog" role="document">

         <div class="modal-content con" style="border:none; border-radius:10px;">

             <div class="modal-header">


                  <h4 class="modal-title text-center" id="signinLabel"><b>Employer Login</b><button type="button" data-dismiss="modal" style="width:2px; background-color:transparent; position:relative; right:-80px; color:CFD2CF; text-decoration:none; marging:0px; padding:0px;" onmouseover="this.style.color='red'" onmouseout="this.style.color=''"><b>X</b></button></h4>

             </div>

             <div class="modal-body">

                 <div class="container-fluid">

                     <div class="row">
                          <p style="font-size:12px;color:red;" id="employer-login-error" class="text-center"></p>

                          <div class="col-md-offset-2 col-md-8">

                            <form id="employer-signin-form" class="" action="include/action/employerloginaction.php" method="post">
                                <div class="form-group">
                                    <label for=""><b>Company Email Address</b></label>
                                    <input class="form-control" type="text" name="email" id="email" value="">
                                </div>

                                <div class="form-group">
                                  <label for=""><b>Password</b></label>
                                  <input class="form-control" type="password" name="password" id="password" value="">
                                </div>

                                <button class="btn btn-primary btn-block" id="login-employer" type="submit" name="login-employer">Log in</button>
                            </form>

                          </div>

                     </div>

                 </div>

             </div>

             <div id="seeker-modal" class="modal-footer text-center">

                  <div class="container-container text-center">
                      <a class="text-center" id="forgot-password" href="recovery.php?recover=password">Forgot Password?</a>
                  </div>

                   <p class="sign-in-p text-center">
                        In case you are using a public/shared computer we recommend that you logout to prevent any un-authorized access to your account
                   </p>
             </div>


         </div>

    </div>

</div><!-- Sign in modal ends -->
