<?php
      require_once('../include/functions/main.php');
      $title = "Account Setting";
      require('../include/layout/header.php');
      require('../include/layout/profile-header.php');
      not_logged_in_redirect();
      if (isset($_POST['submit'])) {
          $username = $user_data['username'];
          $old_password = htmlentities(strip_tags(trim($_POST['old_password'])));
          $new_password = htmlentities(strip_tags(trim($_POST['new_password'])));
          $hashed_password = password_hash($new_password , PASSWORD_DEFAULT);
          if (changePassword($username,$old_password,$hashed_password)) {
              redirect('accountsetting.php?success');
          }else {
              redirect('accountsetting.php?error');
          }
      }
 ?>
<?php
    if (isset($_GET['success'])) { ?>
      <div class="success">
          <h3 class="post-success">Thanks, <?=$user_data['first_name']?>. Your Password change was successful.</h3>
      </div>
    <?php
    }elseif (isset($_GET['error'])) {?>
      <div class="success">
          <h3 class="post-error">Ooops!!! Sorry, something went wrong. Please try again</h3>
      </div>
<?php
    } 
?>

 <!-- Container -->
<div class="container">
  <!-- row -->
    <div class="row">
        <div class="col-md-2"></div>
        <div class="account-setting col-md-8">
            <h3 class="account-setting-header text-center">CHANGE PASSWORD</h3>
            <div class="row">
              <form class="" action="" method="post" novalidate="">
                  <div class="account-setting-details col-md-4 form-group">
                      <label for=""><b>Enter old password</b></label>
                      <input type="password" name="old-password" value="" class="form-control">
                  </div>
                  <div class="account-setting-details col-md-4 form-group">
                      <label for=""><b>Enter new password</b></label>
                      <input type="password" name="new-password" value="" class="form-control">
                  </div>
                  <div class="account-setting-details col-md-4 form-group">
                      <label for=""><b>Confirm new password</b></label>
                      <input type="password" name="confirm-new-password" value="" class="form-control">
                  </div>
                  <button type="submit" name="submit" class="btn btn-primary pull-right">Change Password</button>
              </form>
            </div>
        </div>
        <div class="col-md-2"></div>
    </div><!-- row ends -->
</div><!-- Container ends -->

 <?php

 require('../include/layout/page-header-footer.php');
 require('../include/layout/footer.php');
?>
