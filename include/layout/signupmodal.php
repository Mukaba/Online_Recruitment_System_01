
   <!-- Sign up modal -->
    <div class="modal fade" id="signup" tabindex="-1" role="dialog" aria-labelledby="signupLabel">

        <div class="modal-dialog" role="document">

            <div class="modal-content">

                <div class="modal-header">

                    <button type="button" class="close" data-dismiss="modal" aria-label="Close" name="button"><span style="color:white" aria-hidden="true">&times;</span></button>

                    <img class="img img-responsive center"src="" alt="Smashjob" />

                    <h4 class="modal-title text-center" id="signupLabel"><b>Account Information</b></h4>

                </div>

                <div class="modal-body">

                    <div class="container-fluid">

                        <div class="row">

                              <form action="" method="post">

                                  <div class="col-md-6">

                                      <div class="form-group has-feedback">
                                          <label for="reg_firstname"><b>Firstname</b></label>
                                          <input type="text" name="reg_firstname" id="reg_firstname" value="" class="form-control">
                                      </div>

                                      <div class="form-group">
                                          <label for="reg_lastname"><b>Lastname</b></label>
                                          <input type="text" name="reg_lastname" id="reg_lastname" value="" class="form-control">
                                      </div>


                                      <div class="form-group">
                                          <label for="reg_email"><b>Email Address</b></label>
                                          <input type="text" name="reg_email" id="reg_email" value="" class="form-control" required="">
                                     </div>


                                  </div>

                                  <div class="col-md-6">

                                      <div class="form-group">
                                          <label for="reg_username"><b>Username</b></label>
                                          <input type="text" name="reg_username" id="reg_username" value="" class="form-control">
                                      </div>

                                      <div class="form-group">
                                          <label for="reg_password"><b>Password</b></label>
                                          <input type="text" name="reg_password" id="reg_password" value="" class="form-control">
                                      </div>

                                      <div class="form-group">
                                          <label for="reg_confirm_password"><b>Confirm Password</b></label>
                                          <input type="text" name="reg_confirm_password" id="reg_confirm_password" value="" class="form-control" required="">
                                      </div>

                                  </div>

                        </div>

                    </div>

                </div>

                <div class="modal-footer">

                  <button class="btn btn-primary btn-block" type="submit" name="register">Register</button>

              </form>

                </div>

            </div>

        </div>

    </div><!-- Sign up modal ends-->
