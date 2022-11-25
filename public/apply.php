<?php
    require_once('../include/functions/main.php');
    $title = "Application";
    require('../include/layout/header.php');
    require('../include/layout/profile-header.php');
    $id = $_GET['job'];
    $jobs = getPresentJob($id);
    not_logged_in_redirect();
 ?>

 <section class="job-application-section">
    <div class="row">
        <div class="col-md-2"></div>
        <div style="background-color:white;" class="col-md-8">
            <div class="row">
                <h5>Company: <br>
                    <small><?=$jobs[0]['employer']?></small>
                </h5>
            </div>
            <div class="row">
                <h5>Job Title: <br>
                    <small><?=$jobs[0]['title']?></small>
                </h5>
            </div>
            <div class="row">
                <h5>Job Description: <br>
                    <small><?=$jobs[0]['description']?></small>
                </h5>
            </div>
            <div class="row">
                <h5>Qualification:  <br>
                    <small><?=$jobs[0]['qualification']?></small>
                </h5>
            </div>
            <div class="row">
                <h5>Experience:  <br>
                  <small><?=$jobs[0]['experience']?></small>
                </h5>
            </div>
            <div class="row">
                <h5>Job category:  <br>
                    <small><?=$jobs[0]['category']?></small>
                </h5>
            </div>
            <div class="row">
                <h5>Job Type:  <br>
                    <small><?=$jobs[0]['type']?></small>
                </h5>
            </div>
            <div class="row">
                <h5>Job Location:  <br>
                    <small><?=$jobs[0]['location']?></small>
                </h5>
            </div>
            <div class="row">
                <h5>Application Deadline:  <br>
                    <small><?=$jobs[0]['deadline']?></small>
                </h5>
            </div>
            <div class="row">
                <button type="submit" class="btn btn-primary pull-right" name="apply">Apply</button>
            </div>
        </div>
        <div class="col-md-2"></div>
    </div>
 </section>

 <section class="application-form">

    <div class="col-sm-2"></div>

    <div class="row">
      <div style="padding:0px;" class="col-sm-8">
        <form style="background-color:white;" id="application-form" class="application-form form-group" action="" method="post">

          <div class="form-row row">
            <div class="col-sm-4">
              <label for="">Surname</label>
              <input type="text" name="name" value="<?=$user_data['last_name'];?>" class="form-control">
            </div>
            <div class="col-sm-4">
              <label for="">Firstname</label>
              <input type="text" name="name" value="<?=$user_data['first_name'];?>" class="form-control">
            </div>
            <div class="col-sm-4">
              <label for="">Middlename</label>
              <input type="text" name="name" value="<?=$user_data['middle_name'];?>" class="form-control">
            </div>
          </div>

          <div class="form-row row">
            <div class="col-sm-4">
              <label for="">Email</label>
              <input type="text" name="name" value="<?=$user_data['email'];?>" class="form-control">
            </div>
            <div class="col-sm-4">
              <label for="">Mobile no</label>
              <input type="text" name="name" value="<?=$user_data['mobile'];?>" class="form-control">
            </div>
            <div class="col-sm-4">
              <label for="">Middlename</label>
              <input type="text" name="name" value="<?=$user_data['sex'];?>" class="form-control">
            </div>
          </div>

          <div class="form-row row">
            <div class="col-sm-12">
              <label for="">Upload CV</label>
              <input type="file" name="name" value="">
            </div>
          </div>

          <div class="form-row row">
            <div class="col-sm-12">
              <label for="">About yourself</label>
              <textarea name="name" rows="8" cols="40" class="form-control"></textarea>
            </div>
          </div>

          <div class="form-row row">
            <button type="submit" name="button" class="btn btn-primary pull-right">Submit Application</button>
          </div>

        </form>
      </div>
    </div>

    <div class="col-sm-2"></div>

 </section>

<?php
    require('../include/layout/page-header-footer.php');
    require('../include/layout/footer.php');
?>
