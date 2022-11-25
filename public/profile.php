<?php
      $title = "Profile";
      require_once('../include/functions/main.php');
      require('../include/layout/header.php');
      require('../include/layout/profile-header.php');

      $username = $user_data['username'];

      if (isset($_POST['update-personal'])) {

            $first_name = htmlentities(strip_tags(trim($_POST['first_name'])));
            $last_name = htmlentities(strip_tags(trim($_POST['last_name'])));
            $middle_name = htmlentities(strip_tags(trim($_POST['middle_name'])));
            $email = htmlentities(strip_tags(trim($_POST['email'])));
            $mobile = htmlentities(strip_tags(trim($_POST['mobile'])));


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

            if (updateJobseekerPersonalDetails($last_name, $first_name, $middle_name, $email, $mobile , $username)) {
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
            $degree = htmlentities(strip_tags(trim($_POST['degree'])));
            $field_of_study = htmlentities(strip_tags(trim($_POST['field_of_study'])));
            $year_of_graduation = htmlentities(strip_tags(trim($_POST['year_of_graduation'])));

            $query = $pdo->prepare('INSERT into education (jobseeker_username, university, degree, field_of_study, year_of_graduation) values (:jobseeker_username , :university, :degree, :field_of_study, :year_of_graduation)');

            $query->bindParam(':jobseeker_username' , $username);
            $query->bindParam(':university' , $university);
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


 ?>
  <div class="container">
      <div  class="row">

          <div  ng-init="tab=0" style="margin-top:10px;" class="col-md-9">

              <fieldset class="edit-profile-fieldset">

                  <legend>Personal Details <a href="" onclick="$('#edit-personal-form').animatescroll({scrollSpeed:3000,easing:'easeInOutBack'});" ng-click="tab=1" class="legend-a pull-right">Edit</a></legend>

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
                            <select class="form-control" name="">
                                <option selected="" disabled="" value="<?=strtoupper($user_data['sex'])?>"><?=strtoupper($user_data['sex'])?></option>
                                <!-- <option value="option">Female</option> -->
                            </select>
                          </div>

                        </div>
                      </div>
                      <button class="btn btn-primary form-btn" type="submit" name="update-personal">Update</button>
                  </form>
                </div>


              </fieldset>

              <fieldset class="edit-profile-fieldset">
                  <legend id="edu-legend">Educational Background <a href="" onclick="$('#edit-institution-form').animatescroll({scrollSpeed:2000,easing:'easeOutBounce'});" id="edu-legend-a"  ng-click="tab=2" class="legend-a pull-right">Add</a></legend>

                  <?php

                        $Education_resource = fetchJobseekerEducation($user_data['username']);

                        foreach ($Education_resource as $key => $jobseeker_education) { ?>

                            <div class="row">

                              <div class="col-md-4">
                                <label for=""><b>Institution:</b></label>
                                <h6><?=strtoupper($jobseeker_education['university'])?></h6>
                              </div>

                              <div class="col-md-2">
                                <label for=""><b>Degree:</b></label>
                                <h6><?=strtoupper($jobseeker_education['degree'])?></h6>
                              </div>

                              <div class="col-md-4">
                                <label for=""><b>Field of study:</b></label>
                                <h6><?=strtoupper($jobseeker_education['field_of_study'])?></h6>
                              </div>

                              <div class="col-md-2">
                                <label for=""><b>Year:</b></label>
                                <h6><?=strtoupper($jobseeker_education['year_of_graduation'])?></h6>
                              </div>

                            </div>

                  <?php
                        }
                   ?>


                  <div ng-show="tab === 2" class="col-md-12 personal-form" ng-cloak>

                    <div class="row">
                      <div class="col-md-4">
                        <label for=""><b>Institution:</b></label>
                        <h6 style="text-transform:uppercase;">{{university}}</h6>
                      </div>

                      <div class="col-md-2">
                        <label for=""><b>Degree:</b></label>
                        <h6 style="text-transform:uppercase;">{{degree}}</h6>
                      </div>

                      <div class="col-md-4">
                        <label for=""><b>Field of study:</b></label>
                        <h6 style="text-transform:uppercase;">{{field_of_study}}</h6>
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
                            <select ng-model="type_of_institution" class="form-control" id="type_of_institution" name="type_of_institution">
                                <option selected="" disabled="">Please select an option</option>
                            </select>
                          </div>

                          <div class="col-sm-6">
                            <label for=""><b>Institution:</b></label>
                            <select ng-model="university" class="form-control" id="university" name="university">
                            </select>
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
                              <input ng-model="field_of_study" type="text" class="form-control" name="field_of_study" value="">
                            </div>

                            <div class="col-sm-4">
                              <label for=""><b>Year:</b></label>
                              <select ng-model="year_of_graduation" class="form-control" name="year_of_graduation">
                              <option  selected="" disabled="" value="<?=$user_data['year_of_graduation']?>">YYYY</option>
                                <?php
                                  $currentYear = date(Y) + 20;
                                  for ($i=1940; $i < $currentYear; $i++) { ?>
                                      <option value=<?=$i?>><?=$i?></option>
                                  <?php
                                  }
                               ?>
                              </select>
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

                  <form class="" action="" method="post">

                    <?php
                          $Certificate_resource = fetchJobseekerCert($user_data['username']);

                          if (count($Certificate_resource) == 1) {
                              unset($Certificate_resource[0]);
                          ?>

                            <div class="row">
                                <p>You have no certification. Click the <a ng-click="tab=3" href="#edit-certification-form">here</a> to add one</p>
                            </div>

                    <?php
                  }elseif (count($Certificate_resource) > 1)

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
                                <h6><a id="remove-cert" href="remove.php?remove=<?=$jobseeker_cert['id'];?>"><button class="remove-cert" type="submit" name="remove-cert">Remove</button></a></h6>
                              </div>
                            </div>

                            <div class="clear-float">

                            </div>

                          </form>


                     <?php

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
                        <!-- <input type="text" name="year" value="" placeholder="Year" ng-model="year" class="form-control"> -->
                        <select ng-model="year" class="form-control" name="year">
                        <option  selected="" disabled="" value="">YYYY</option>
                          <?php
                            $currentYear = date(Y) + 20;
                            for ($i=1940; $i < $currentYear; $i++) { ?>
                                <option value=<?=$i?>><?=$i?></option>
                            <?php
                            }
                         ?>
                        </select>
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

          <div style="margin-top:10px;" class="col-md-3">
              <div class="panel panel-default">
                  <div class="panel-body">

                    <button type="button" name="button" class="btn btn-primary">Upload CV</button>
                  </div>
              </div>
          </div>


      </div>
  </div>

   <?php
         require('../include/layout/page-header-footer.php');
         require('../include/layout/footer.php');
    ?>
