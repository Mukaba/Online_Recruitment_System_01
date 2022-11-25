<?php
      $title = "Profile";
      require_once('include/functions/main.php');
      require('include/layout/header.php');
      require('include/layout/profile-header.php');
      not_logged_in_redirect();

      $upload_error = "";
      $username = $user_data['username'];

      $check_if_profile_image_is_uploaded = isProfileImageUploaded($user_data['username']);
      $check_if_cv_is_uploaded = isCvUploaded($user_data['username']);

      if (isset($_POST['update-personal'])) {

            $first_name = htmlentities(strip_tags(addslashes(trim($_POST['first_name']))));
            $last_name = htmlentities(strip_tags(addslashes(trim($_POST['last_name']))));
            $middle_name = htmlentities(strip_tags(addslashes(trim($_POST['middle_name']))));
            $email = htmlentities(strip_tags(addslashes(trim($_POST['email']))));
            $mobile = htmlentities(strip_tags(addslashes(trim($_POST['mobile']))));
            $sex = htmlentities(strip_tags(addslashes(trim($_POST['sex']))));


            if ($first_name === "") {
                $first_name = $user_data['first_name'];
            }
            else {
                $first_name = htmlentities(strip_tags(trim($_POST['first_name'])));
            }

            if ($last_name === "") {
                $last_name = $user_data['last_name'];
            }
            else {
                $last_name = htmlentities(strip_tags(trim($_POST['last_name'])));
            }

            if ($middle_name === "") {
                $middle_name = $user_data['middle_name'];
            }
            else {
                $middle_name = htmlentities(strip_tags(trim($_POST['middle_name'])));
            }

            if ($email === "") {
                $email = $user_data['email'];
            }
            else {
                $email = htmlentities(strip_tags(trim($_POST['email'])));
            }

            if ($mobile === "") {
                $mobile = $user_data['mobile'];
            }
            else {
                $mobile = htmlentities(strip_tags(trim($_POST['mobile'])));
            }
            if ($sex === "") {
                $sex = $user_data['sex'];
            }
            else {
                $sex = htmlentities(strip_tags(trim($_POST['sex'])));
            }

            if (updateJobseekerPersonalDetails($last_name, $first_name, $middle_name, $email, $mobile , $username , $sex)) {
                  header("Refresh: 0");
            }
            else {
                  echo "Update failed";
            }
      }

      if (isset($_POST['update-about'])) {

        $about = htmlentities(strip_tags(trim($_POST['about'])));
        $query = $pdo->prepare('UPDATE jobseeker set about=:about WHERE username=:username');
        $query->bindParam(':about' , $about);
        $query->bindParam(':username' , $username);

            if ($query->execute()) {

                header("Refresh: 0");

            }
            else {
                echo "failed";
            }

      }

      if (isset($_POST['update-certification'])) {

            $title = htmlentities(strip_tags(trim($_POST['title'])));
            $institution = htmlentities(strip_tags(trim($_POST['institution'])));
            $year = htmlentities(strip_tags(trim($_POST['year'])));

            $query = $pdo->prepare('INSERT into certification (jobseeker_username, title, institution, year) values (:jobseeker_username , :title, :institution, :year)');

            $query->bindParam(':jobseeker_username' , $username);
            $query->bindParam(':title' , $title);
            $query->bindParam(':institution' , $institution);
            $query->bindParam(':year' , $year);

                if ($query->execute()) {
                    header("Refresh: 0");
                }
                else {
                    echo "failed";
                }
      }

      if (isset($_POST['update-education'])) {

            $university = htmlentities(strip_tags(trim($_POST['university'])));
            $type_of_institution = htmlentities(strip_tags(trim($_POST['type_of_institution'])));
            $degree = htmlentities(strip_tags(trim($_POST['degree'])));
            $field_of_study = htmlentities(strip_tags(trim($_POST['field_of_study'])));
            $year_of_graduation = htmlentities(strip_tags(trim($_POST['year_of_graduation'])));

            $query = $pdo->prepare('INSERT into education (jobseeker_username, type_of_institution , institution, degree, field_of_study, year_of_graduation) values (:jobseeker_username , :type_of_institution, :institution, :degree, :field_of_study, :year_of_graduation)');

            $query->bindParam(':jobseeker_username' , $username);
            $query->bindParam(':type_of_institution' , $type_of_institution);
            $query->bindParam(':institution' , $university);
            $query->bindParam(':degree' , $degree);
            $query->bindParam(':field_of_study' , $field_of_study);
            $query->bindParam(':year_of_graduation' , $year_of_graduation);

                if ($query->execute()) {
                    header("Refresh: 0");
                }
                else {
                    echo "failed";
                }
      }

      if (isset($_POST['continue-upload'])) {

        if (isset($_FILES['profile-image'])) {

            if (empty($_FILES['profile-image']['name'])) {
                echo "Pls choose a file";
            }
            else {

                $allowed   = array('jpeg' , 'jpg' ,'png');
                $file_name = $_FILES['profile-image']['name'];
                $file_ext = explode('.' , $file_name);

                //  With STRICT error reporting on we cant directly change the return value of a function, so we store the return value first in a variable and the manipulate it

                $file_ext = strtolower(end($file_ext));

                $file_temp  = $_FILES['profile-image']['tmp_name'];

                if (in_array($file_ext, $allowed)) {
                    change_profile_image($user_data['id'], $file_temp, $file_ext);
                    header("Refresh:0");
                }
                else {
                  $upload_error =  "unacceptable file format. Only " . " " . implode(', ' , $allowed) . " are allowed";
                }


            }

        }

      }

      if (isset($_POST['continue-cv'])) {

        if (isset($_FILES['cv'])) {

            if (empty($_FILES['cv']['name'])) {
                echo "Pls choose a file";
            }
            else {

                $allowed   = array('pdf' , 'docx' ,'doc' , 'txt');
                $file_name = $_FILES['cv']['name'];
                $file_ext = explode('.' , $file_name);

                //  With STRICT error reporting on we cant directly change the return value of a function, so we store the return value first in a variable and the manipulate it

                $file_ext = strtolower(end($file_ext));

                $file_temp  = $_FILES['cv']['tmp_name'];

                if (in_array($file_ext, $allowed)) {
                    change_cv($user_data['id'], $file_temp, $file_ext);
                    // header("Refresh:0");
                }
                else {
                  $upload_error =  "unacceptable file format. Only " . " " . implode(', ' , $allowed) . " are allowed";
                }


            }

        }

      }
 ?>


  <div class="container profile">
      <div  class="row">

          <div  ng-init="tab=0" class="profile-main col-md-10">

              <fieldset class="edit-profile-fieldset">

                  <legend>Personal Details <a href="" ng-click="tab=1" class="legend-a pull-right">Edit</a></legend>

                  <div class="row">
                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Surname:</b></label>
                      <h6><?=strtoupper($user_data['last_name'])?></h6>
                    </div>

                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>First name:</b></label>
                      <h6><?=strtoupper($user_data['first_name'])?></h6>
                    </div>

                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Middle name:</b></label>
                      <h6><?=strtoupper($user_data['middle_name'])?></h6>
                    </div>

                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Email:</b></label>
                      <h6><?=($user_data['email'])?></h6>
                    </div>
                  </div>

                  <div class="row">
                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Mobile no:</b></label>
                      <h6><?=($user_data['mobile'])?></h6>
                    </div>

                    <div class="col-sm-3 col-md-3">
                      <label for=""><b>Sex:</b></label>
                      <h6><?=strtoupper($user_data['sex'])?></h6>
                    </div>
                  </div>

                  <div class="clearfix"></div>



                <div ng-show="tab === 1" id="personal" class="col-md-12 personal-form" ng-cloak>
                  <form id="edit-personal-form" class="form-group" action="" method="post">

                      <div class="form-group">

                        <div class="row">
                          <div class="col-sm-4 col-md-4">
                            <label for=""><b>Surname:</b></label>
                            <input type="text" name="last_name" value="" ng-model="surname" class="form-control">
                          </div>

                          <div class="col-sm-4 col-md-4">
                            <label for=""><b>First name:</b></label>
                            <input type="text" name="first_name" value="" class="form-control">
                          </div>

                          <div class="col-sm-4 col-md-4">
                            <label for=""><b>Middle name:</b></label>
                            <input type="text" name="middle_name" value="" class="form-control">
                          </div>
                        </div>

                      </div>

                      <div class="form-group">
                        <div class="row">
                          <div class="col-sm-4 col-md-4">
                            <label for=""><b>Email:</b></label>
                            <input type="text" name="email" value="" class="form-control">
                          </div>

                          <div class="col-sm-4 col-md-4">
                            <label for=""><b>Mobile no:</b></label>
                            <input type="text" name="mobile" value="" class="form-control">
                          </div>

                          <div class="col-sm-4 col-md-4">
                            <label for=""><b>Sex:</b></label>
                            <select class="form-control" name="sex">
                                <option selected="" disabled="" value="<?=strtoupper($user_data['sex'])?>"><?=strtoupper($user_data['sex'])?></option>
                                <option value="Male">Male</option>
                                <option value="Female">Female</option>
                            </select>
                          </div>

                        </div>
                      </div>
                      <button class="btn btn-primary form-btn" type="submit" name="update-personal">Update</button>
                  </form>
                </div>


              </fieldset>

              <fieldset class="edit-profile-fieldset">
                  <legend id="edu-legend">Educational Background <a href="" id="edu-legend-a" ng-click="tab=2" class="legend-a pull-right">Add</a></legend>

                  <?php
                        $Education_resource = fetchJobseekerEducation($user_data['username']);

                        if (count($Education_resource) == 1) {
                            unset($Education_resource[0]);
                  ?>

                  <div class="row">
                      <p>You have no Education history. Click the <a ng-click="tab=2" href="#edit-institution-form">here</a> to add one</p>
                  </div>

                  <?php
                }elseif (count($Education_resource) > 1) {

                        unset($Education_resource[0]);

                        foreach ($Education_resource as $key => $jobseeker_education) {
                        require('include/layout/delete.php');
                  ?>
                  <div class="row">

                    <div class="col-md-4">
                      <label for=""><b>Institution:</b></label>
                      <h6><?=strtoupper($jobseeker_education['institution'])?></h6>
                    </div>

                    <div class="col-md-3">
                      <label for=""><b>Field of study:</b></label>
                      <h6><?=strtoupper($jobseeker_education['field_of_study'])?></h6>
                    </div>

                    <div class="col-md-2">
                      <label for=""><b>Degree:</b></label>
                      <h6><?=strtoupper($jobseeker_education['degree'])?></h6>
                    </div>


                    <div class="col-md-2">
                      <label for=""><b>Year:</b></label>
                      <h6><?=strtoupper($jobseeker_education['year_of_graduation'])?></h6>
                    </div>

                    <div class="col-md-1">
                      <label for=""><b></b></label>
                      <h6><a id="remove-edu" href="" data-toggle="modal" data-target="#delete"><button class="remove-edu" type="button" name="">remove</button></a></h6>
                    </div>

                  </div>

                  <div class="clear-float">

                  </div>

                   <?php

                          }
                      }

                   ?>


                  <div ng-show="tab === 2" class="col-md-12 personal-form" ng-cloak>

                    <div class="row">
                      <div class="col-md-4">
                        <label for=""><b>Institution:</b></label>
                        <h6 style="text-transform:uppercase;">{{university}}</h6>
                      </div>

                      <div class="col-md-3">
                        <label for=""><b>Field of study:</b></label>
                        <h6 style="text-transform:uppercase;">{{field_of_study}}</h6>
                      </div>

                      <div class="col-md-2">
                        <label for=""><b>Degree:</b></label>
                        <h6 style="text-transform:uppercase;">{{degree}}</h6>
                      </div>

                      <div class="col-md-2">
                        <label for=""><b>Year of graduation:</b></label>
                        <h6 style="text-transform:uppercase;">{{year_of_graduation}}</h6>
                      </div>
                    </div>

                    <form id="edit-institution-form" class="form-group" action="" method="post" ng-cloak>

                        <a href="" class="close-form pull-right" id="close-institution">x</a>

                        <div class="row">

                          <div class="col-sm-6 edit-institution-row">
                            <label for=""><b>Type of Institution:</b></label>
                            <!-- <select ng-model="type_of_institution" class="form-control" id="type_of_institution" name="type_of_institution">
                                <option selected="" disabled="">Please select an option</option>
                            </select> -->
                            <input ng-model="type_of_institution" type="text " class="form-control" id="type_of_institution" name="field_of_study" value="">
                          </div>

                          <div class="col-sm-6">
                            <label for=""><b>Institution:</b></label>
                            <!-- <select ng-model="university" class="form-control" id="university" name="university">
                            </select> -->
                            <input ng-model="university" type="text " class="form-control" id="university" name="university" value="">
                          </div>

                        </div>

                        <div class="row">

                            <div class="col-sm-4 edit-institution-row">
                              <label for=""><b>Degree:</b></label>
                              <select ng-model="degree" class="form-control" name="degree">
                                  <option value="B.Sc">B.Sc</option>
                                  <option value="B.Eng">B.Eng</option>
                                  <option value="B.Tech">B.Tech</option>
                                  <option value="MPhil / PhD">MPhil / PhD</option>
                                  <option value="MBA / MSc">MBA / MSc</option>
                                  <option value="MBBS">MBBS</option>
                                  <option value="HND">HND</option>
                                  <option value="OND">OND</option>
                                  <option value="N.C.E">N.C.E</option>
                                  <option value="Diploma">Diploma</option>
                                  <option value="High School (S.S.C.E)">High School (S.S.C.E)</option>
                                  <option value="Vocational">Vocational</option>
                                  <option value="Others">Others</option>`
                              </select>
                            </div>

                            <div class="col-sm-4">
                              <label for=""><b>Field of study:</b></label>
                              <!-- <select ng-model="field_of_study" class="form-control" name="field_of_study">
                                  <option value="option">option</option>
                              </select> -->
                              <input ng-model="field_of_study" type="text " class="form-control" name="field_of_study" value="">
                            </div>

                            <div class="col-sm-4">
                              <label for=""><b>Year:</b></label>
                              <input type="date" ng-model="year_of_graduation" class="form-control" name="year_of_graduation">
                              
                            </div>

                        </div>

                        <div class="row">
                              <button class="btn btn-primary form-btn" type="submit" name="update-education">Update</button>
                        </div>

                    </form>
                  </div>
              </fieldset>

              <fieldset class="edit-profile-fieldset">

                  <legend>Certification <a href="#edit-certification-form" ng-click="tab=3" class="legend-a pull-right">Add</a></legend>

                    <?php
                          $Certificate_resource = fetchJobseekerCert($user_data['username']);

                          if (count($Certificate_resource) == 1) {
                              unset($Certificate_resource[0]);
                    ?>

                            <div class="row">
                                <p>You have no certification. Click the <a ng-click="tab=3" href="#edit-certification-form">here</a> to add one</p>
                            </div>

                    <?php
                  }elseif (count($Certificate_resource) > 1) {

                          unset($Certificate_resource[0]);

                          foreach ($Certificate_resource as $key => $jobseeker_cert) {

                    ?>
                            <div class="row">
                              <div class="col-md-5">
                                <label for=""><b>Title:</b></label>
                                <h6><?=strtoupper($jobseeker_cert['title'])?></h6>
                              </div>

                              <div class="col-md-5">
                                <label for=""><b>Institution:</b></label>
                                <h6><?=strtoupper($jobseeker_cert['institution'])?></h6>
                              </div>

                              <div class="col-md-1">
                                <label for=""><b>Year:</b></label>
                                <h6><?=strtoupper($jobseeker_cert['year'])?></h6>
                              </div>

                              <div class="col-md-1">
                                <label for=""><b></b></label>
                                  <h6><a id="remove-cert" href="remove.php?state=certification&remove=<?=$jobseeker_cert['id'];?>"><button class="remove-cert" type="submit" name="remove-cert">Remove</button></a></h6>
                              </div>
                            </div>

                            <div class="clear-float">

                            </div>

                     <?php
                            }
                        }

                     ?>

                  <div ng-show="tab === 3" class="row" ng-cloak>

                    <div class="col-md-5">
                      <label for=""><b>Title:</b></label>
                      <h6 style="text-transform:uppercase;">{{title}}</h6>
                    </div>

                    <div class="col-md-5">
                      <label for=""><b>Institution:</b></label>
                      <h6 style="text-transform:uppercase;">{{institution}}</h6>
                    </div>

                    <div class="col-md-1">
                      <label for=""><b>Year:</b></label>
                      <h6 style="text-transform:uppercase;">{{year}}</h6>
                    </div>

                  </div>



                  <div ng-show="tab === 3" class="col-md-12 personal-form" ng-cloak>
                    <form id="edit-certification-form" class="form-group" action="" method="post">

                      <div class="col-sm-4 col-md-4">
                        <label for=""><b>Title:</b></label>
                        <input type="text" name="title" value="" ng-model="title" placeholder="Title" class="form-control">
                      </div>

                      <div class="col-sm-4 col-md-4">
                        <label for=""><b>Institution:</b></label>
                        <input type="text" name="institution" value="" ng-model="institution" placeholder="Institution" class="form-control">
                      </div>

                      <div class="col-sm-4 col-md-4">
                        <label for=""><b>Year:</b></label>
                        <input type="date" name="year" value="" placeholder="Year" ng-model="year" class="form-control">
                        
                      </div>

                      <button class="btn btn-primary form-btn" type="submit" name="update-certification">Update</button>

                    </form>
                  </div>
              </fieldset>

              <fieldset class="edit-profile-fieldset">
                  <legend>About me <a href="" ng-click="tab=4" class="legend-a pull-right">Edit</a></legend>
                  <div class="about-me">

                      <h6 class="about-me-h6 text-jusify"><?=$user_data['about'];?></h6>

                      <div ng-show="tab === 4" class="col-md-12 personal-form" ng-cloak>

                        <form id="edit-about-form" class="" action="" method="post">
                          <label for=""><b>About me</b></label>
                          <textarea class="form-control" name="about" rows="8" cols="107"></textarea>
                          <button class="btn btn-primary form-btn" type="submit" name="update-about">Update</button>

                        </form>

                      </div>

                  </div>
              </fieldset>


          </div>

          <div class="profile-aside col-md-2">
              <!-- <h4>Upload an image for your profile</h4> -->
              <form class="" action="" method="post" enctype="multipart/form-data">
                <?php

                    if ($check_if_profile_image_is_uploaded) { ?>

                      <label id="input-label" for="upload-profile-pix">Change Profile Image</label>
                      <span id="label_span"><?=$upload_error;?></span>
                      <input type="file" id="upload-profile-pix" name="profile-image"> <br>
                      <button type="submit" class="btn btn-primary" id="continue-upload" name="continue-upload">Change</button>

                <?php
                    }else { ?>
                      <label id="input-label" for="upload-profile-pix">Upload Profile Image</label>
                      <span id="label_span"><?=$upload_error;?></span>
                      <input type="file" id="upload-profile-pix" name="profile-image"> <br>
                      <button type="submit" class="btn btn-primary" id="continue-upload" name="continue-upload">Upload</button>
                <?php
                    }
                 ?>


              </form>

              <form class="" action="" method="post" enctype="multipart/form-data">
                <?php

                    if ($check_if_cv_is_uploaded) { ?>

                      <label id="input-label-cv" for="upload-cv">Update your resume..</label>
                      <span id="label_span-cv"><?=$upload_error;?></span>
                      <input type="file" id="upload-profile-cv" name="cv"> <br>
                      <button type="submit" class="btn btn-primary" id="continue-cv" name="continue-cv">Upload</button>

                <?php
                    }else { ?>
                      <label id="input-label-cv" for="upload-profile-cv">Upload your resume..</label>
                      <span id="label_span-cv"><?=$upload_error;?></span>
                      <input type="file" id="upload-profile-cv" name="cv"> <br>
                      <button type="submit" class="btn btn-primary" id="continue-cv" name="continue-cv">Upload</button>
                <?php
                    }
                 ?>

              </form>
          </div>


      </div>
  </div>

   <?php
         require('include/layout/page-header-footer.php');
         require('include/layout/footer.php');
    ?>

    <script type="text/javascript">
        $(document).ready(function() {
            $('#upload-profile-pix').on("change" , function(e){
                var filename = e.target.value.split('\\').pop();
                $('#input-label').css('display' , 'none');
                $('#label_span').text(filename).css('background-color' , 'transparent');
                $('#continue-upload').css('display' , 'block');
            });

            $('#upload-profile-cv').on("change" , function(e){
                var filename = e.target.value.split('\\').pop();
                $('#input-label-cv').css('display' , 'none');
                $('#label_span-cv').text(filename).css('background-color' , 'transparent');
                $('#continue-cv').css('display' , 'block');
            });
        });
    </script>
