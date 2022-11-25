      <footer>

          <div class="container">
              <div class="row">
                  <div class="row">
                    <div class="col-xs-12 col-md-3">
                      <ul>
                          <h4>SUPPORT</h4>
                          <li><a href="#">Subscribe for newsletter</a></li>
                          <li><a href="#">Our Business guides</a></li>
                          <li><a href="#">Get Events Notification</a></li>
                          <li><a href="#">Stay updated</a></li>
                          <li><a href="#">About us</a></li>
                      </ul>
                    </div>
                    <div class="col-xs-12 col-md-3">
                      <ul>
                        <h4>HOW DO I ?</h4>
                        <li><a href="#">Cancel my account</a></li>
                        <li><a href="#">Reset my password</a></li>
                        <li><a href="#">Apply for jobs</a></li>
                        <li><a href="#">Contact employer</a></li>
                        <li><a href="inbox.php">Upgrade my account</a></li>
                      </ul>
                    </div>
                    <?php
                        if (logged_in()) {?>

                          <div class="col-xs-12 col-md-3">
                            <ul>
                                <h4>QUICK LINKS</h4>
                                <li><a href="jobseeker-dashboard.php">Dashboard</a></li>
                                <li><a href="profile.php">Profile</a></li>
                                <li><a href="myapplication.php">My Application</a></li>
                                <li><a href="accountsetting.php">Account Settings</a></li>
                                <li><a href="logout.php">Log Out</a></li>
                            </ul>
                          </div>
                    <?php
                        }else {?>

                          <div class="col-xs-12 col-md-3">
                            <ul>
                              <h4>HOW DO I ?</h4>
                              <li><a href="#">Cancel my account</a></li>
                              <li><a href="#">Reset my password</a></li>
                              <li><a href="#">Apply for jobs</a></li>
                              <li><a href="#">Contact employer</a></li>
                              <li><a href="#">Upgrade my account</a></li>
                            </ul>
                          </div>

                    <?php
                        }
                     ?>
                    <div class="col-xs-12 col-sm-3">
                      <h4 class="follow-us">Follow us:</h4>
                      <ul class="icon-ul">
                          <li class="icon-li"><a href="#"><img src="icons/facebook32.png" class="img img-responsive" alt="" /></a></li>
                          <li class="icon-li"><a href="#"><img src="icons/instagram32.png" class="img img-responsive" alt="" /></a></li>
                          <li class="icon-li"><a href="#"><img src="icons/skype32.png" class="img img-responsive" alt="" /></a></li>
                          <li class="icon-li"><a href="#"><img src="icons/twitter32.png" class="img img-responsive" alt="" /></a></li>
                          <li class="icon-li"><a href="#"><img src="icons/linkedin32.png" class="img img-responsive" alt="" /></a></li>
                      </ul>
                    </div>
                  </div>

                  </div>
              </div>
              <hr id="divider">
              <div class="row">
                  <div class="col-md-12">
                      <h5 class="copyright text-center">&copy; Made by K. M. Jean-louis Raoul, ULK </h5>
                  </div>
              </div>
      </footer>

      <?php require('include/layout/script.php');?>

  </body>
</html>
