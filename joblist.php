<?php
      session_start();
      $title = "Employer";
      require('include/layout/output_buffer.php');
      require('include/database/pdo_connect.php');
      require_once('include/functions/employer.php');
      $jobs = getEmployerJobPost($employer_data['name']);

      function logged_in () {
          return (isset($_SESSION['id'])) ? true : false;
      }

      require('include/layout/variables.php');
      require('include/layout/employerheader.php');
      require('include/layout/employer-profile-header.php');
 ?>
<div class="employer-container container-fluid">

  <div class="col-xs-12 col-sm-9">
       <div class="col-md-12 dashboard-main-content">
           <h4 class="content-title text-center">MY JOB LISTING</h4>
       </div>
       <div id="jobs-table" class="table-responsive">
       <table class="table table-striped">
           <thead>
               <tr>
                   <th>S/no</th>
                   <th>Company Name</th>
                   <th>Job Title</th>
                   <th>Date posted</th>
                   <th>Application Deadline</th>
                   <th>Qualification</th>
                   <th>Gender</th>
                   <th>Status</th>
                   <th></th>
               </tr>
           </thead>
           <tbody>

                 <?php
                         $i = 1;
                         foreach ($jobs  as $key => $jobs) {
                 ?>

                         <tr>
                           <td><?=$i;?></td>
                           <td><?=$jobs['employer']?></td>
                           <td><?=$jobs['title']?></td>
                           <td><?=$jobs['date_posted']?></td>
                           <td><?=$jobs['deadline']?></td>
                           <td><?=$jobs['qualification']?></td>
                           <td><?=$jobs['gender']?></td>
                           <td><?=$jobs['status']?></td>
                           <td><a href="applications.php?job=<?=$jobs['id']?>">view</a></td>
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
<div style="height: 55vh;">

</div>

 <?php
    require('include/layout/page-header-footer.php');
    require('include/layout/footer.php');
 ?>
