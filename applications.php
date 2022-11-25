<?php
      session_start();
      $title = "Employer";
      require('include/layout/output_buffer.php');
      require('include/database/pdo_connect.php');
      require_once('include/functions/employer.php');
      $applications = getApplicationList($_GET['job']);
      $jobDetails = getJobDetails($_GET['job']);
      // $jobseeker_details = getJobseekerDetails($applications[0]['jobseeker_id']);
      // var_dump($jobseeker_details);
      function logged_in () {
          return (isset($_SESSION['id'])) ? true : false;
      }

      require('include/layout/variables.php');
      require('include/layout/employerheader.php');
      require('include/layout/employer-profile-header.php');
 ?>
<div class="employer-container container-fluid">

  <div class="col-xs-12 col-sm-12">
       <div class="col-md-12 dashboard-main-content">
           <h4 class="content-title text-center">LIST OF PEOPLE WHO APPLIED FOR THE JOB OF <?=strtoupper($jobDetails[0]['title']);?></h4>
       </div>
       <div id="jobs-table" class="table-responsive">
       <table class="table table-striped">
           <thead>
               <tr>
                   <th>S/no</th>
                   <th>Full Name</th>
                   <th>Email</th>
                   <th>Phone no</th>
                   <th>Gender</th>
                   <th>Location</th>
                   <th>Profile Handle</th>
                   <th>Download CV</th>
                   <th>Status</th>
                   <th></th>
               </tr>
           </thead>
           <tbody>

                 <?php
                         $i = 1;
                         foreach ($applications  as $key => $applications) {

                            $jobseeker_details = getJobseekerDetails($applications['jobseeker_id']);
                            // var_dump($jobseeker_details);
                 ?>

                         <tr>
                           <td><?=$i;?></td>
                           <td><?=$jobseeker_details[0]['last_name'] . " " .$jobseeker_details[0]['first_name'] . " " . $jobseeker_details[0]['middle_name']?></td>
                           <td><?=$jobseeker_details[0]['email']?></td>
                           <td><?=$jobseeker_details[0]['mobile']?></td>
                           <td><?=$jobseeker_details[0]['sex']?></td>
                           <td><?=$jobseeker_details[0]['state_of_residence']?></td>
                           <td><a target="_blank" href="localhost:8080/smashjob/<?=$jobseeker_details[0]['username']?>">localhost:8080/smashjob/<?=$jobseeker_details[0]['username']?></a></td>
                           <td><a href="download.php?file=<?=$jobseeker_details[0]['cv']?>">Download</a></td>
                           <td><?=$jobseeker_details[0]['mobile']?></td>
                         </tr>

                 <?php
                         $i++;
                         }
                  ?>

         </tbody>
         </table>
     </div>
</div>

</div>
<div style="height:100vh;">

</div>

 <?php
    require('include/layout/page-header-footer.php');
    require('include/layout/footer.php');
 ?>
