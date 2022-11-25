<?php
      require_once('include/functions/main.php');
      $title = "Dashboard";
      require('include/layout/header.php');
      require('include/layout/profile-header.php');
      $jobs = getJobsAvailable();
      not_logged_in_redirect();

 ?>

<!-- container-fluid -->
<div class="container-fluid">

    <!-- dashboard-lower-row -->
     <div id="dashboard-lower-row" style="margin-right:0px;margin-left:0px;" class="row">
         <div class="col-xs-12 col-sm-9">
              <div class="col-md-12 dashboard-main-content">
                  <h4 class="content-title text-center">RECENT JOB LISTINGS</h4>
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
                                  <td><a href="apply.php?job=<?=$jobs['id']?>">Apply</a></td>
                                </tr>

                        <?php
                                $i++;
                                }
                         ?>

                </tbody>
                </table>
            </div>
      </div>

       <div class="col-xs-12 col-sm-3">
            <form id="search-jobs-form" class="form-group searchform" action="include/action/searchjobs.php" method="post">
                <div class="input-group dashboard-input-group">
                    <input class="form-control" type="search" name="search" value="" id="search-jobs" placeholder="Use keywords. E.g: Accountant,  Human Resource"><span class="input-group-addon"><a href=""><i class="glyphicon glyphicon-search"></i></a></span>
                </div>
            </form>

           <div id="my-application" class="panel panel-default">
               <div id="my-application-heading" class="panel-heading">
                 <h3  id="my-application-inner" class="panel-title">My Application</h3>
               </div>
               <div class="panel-body">

                  <?php
                      $jobseeker_id = $user_data['id'];
                      $application_count = countApplication($jobseeker_id);
                      $check_reviewed_application = checkApplication($jobseeker_id);
                      $unreviewed_application = ($application_count - $check_reviewed_application);

                      if ($application_count == "0") { ?>
                        <h6>You have no application</h6>
                  <?php
                      }elseif($application_count == "1") { ?>
                          <h6>You have <a href=""><?=$application_count;?></a> application</h6>
                          <h6>Reviewed applications : <a href="#"><?=$check_reviewed_application;?></a></h6>
                          <h6>Non-reviewed applications : <a href="#"><?=$unreviewed_application;?></a></h6>
                        <?php
                      }elseif($application_count > "1") { ?>
                          <h6>You have <a href="#"><?=$application_count;?></a> applications</h6>
                          <h6>Reviewed applications : <a href="#"><?=$check_reviewed_application;?></a></h6>
                          <h6>Non-reviewed applications : <a href="#"><?=$unreviewed_application;?></a></h6>
                  <?php
                      }
                  ?>
               </div>
           </div>

           <div class="panel-group" id="accordion">

               <div class="panel panel-default">
                   <div class="panel-heading panel-clr">
                     <div class="panel-title">
                         <a href="#first" data-toggle="collapse" data-parent="#accordion"><i class="glyphicon glyphicon-plus"></i>Accounting / Audit / Tax</a>
                     </div>
                   </div>
                   <div class="panel-collapse collapse" id="first">
                       <div class="panel-body">
                           <?php
                                $jobs = getJobs('Accounting / Audit / Tax');
                                foreach ($jobs as $key => $jobs) { ?>
                                    <ul>
                                        <li class="job-li">
                                            <a href="apply.php?job=<?=$jobs['id'];?>">
                                              <?php echo 'Company : ' . $jobs['employer']; ?></a><br>
                                              <?php echo 'Job Title : ' .  $jobs['title']; ?>
                                        </li>
                                    </ul>
                            <?php
                                }
                            ?>
                       </div>
                   </div>
               </div>

               <div class="panel panel-default">
                   <div class="panel-heading panel-clr">
                     <div class="panel-title">
                         <a href="#second" data-toggle="collapse" data-parent="#accordion"><i class="glyphicon glyphicon-plus"></i>Banking / Finance / Insurance</a>
                     </div>
                   </div>
                   <div class="panel-collapse collapse" id="second">
                     <div class="panel-body">
                         <?php
                              $jobs = getJobs('Banking / Finance / Insurance');
                              foreach ($jobs as $key => $jobs) { ?>
                                  <ul>
                                      <li class="job-li">
                                          <a href="apply.php?job=<?=$jobs['id'];?>">
                                            <?php echo 'Company : ' . $jobs['employer']; ?></a><br>
                                            <?php echo 'Job Title : ' .  $jobs['title']; ?>
                                      </li>
                                  </ul>
                          <?php
                              }
                          ?>
                     </div>
                   </div>
               </div>

             <div class="panel panel-default">
                   <div class="panel-heading panel-clr">
                     <div class="panel-title">
                         <a href="#third" data-toggle="collapse" data-parent="#accordion"><i class="glyphicon glyphicon-plus"></i>Engineering</a>
                     </div>
                   </div>
                   <div class="panel-collapse collapse" id="third">
                       <div class="panel-body">
                         <?php
                              $jobs = getJobs('Engineering');
                              foreach ($jobs as $key => $jobs) { ?>
                                  <ul>
                                      <li class="job-li">
                                          <a href="apply.php?job=<?=$jobs['id'];?>">
                                            <?php echo 'Company : ' . $jobs['employer']; ?></a><br>
                                            <?php echo 'Job Title : ' .  $jobs['title']; ?>
                                      </li>
                                  </ul>
                          <?php
                              }
                          ?>
                       </div>
                   </div>
               </div>

               <div class="panel panel-default">
                   <div class="panel-heading panel-clr">
                     <div class="panel-title">
                         <a href="#fourth" data-toggle="collapse" data-parent="#accordion"><i class="glyphicon glyphicon-plus"></i>Information Technology</a>
                     </div>
                   </div>
                   <div class="panel-collapse collapse" id="fourth">
                     <div class="panel-body">
                       <?php
                            $jobs = getJobs('Information Technology');
                            foreach ($jobs as $key => $jobs) { ?>
                                <ul>
                                    <li class="job-li">
                                        <a href="apply.php?job=<?=$jobs['id'];?>">
                                          <?php echo 'Company : ' . $jobs['employer']; ?></a><br>
                                          <?php echo 'Job Title : ' .  $jobs['title']; ?>
                                    </li>
                                </ul>
                        <?php
                            }
                        ?>
                     </div>
                   </div>
               </div>

               <div class="panel panel-default">
                   <div class="panel-heading panel-clr">
                     <div class="panel-title">
                         <a href="#fifth" data-toggle="collapse" data-parent="#accordion"><i class="glyphicon glyphicon-plus"></i>Marketing / Advertising / Communications</a>
                     </div>
                   </div>
                   <div class="panel-collapse collapse" id="fifth">
                       <div class="panel-body">
                         <?php
                              $jobs = getJobs('Marketing / Advertising / Communications');
                              foreach ($jobs as $key => $jobs) { ?>
                                  <ul>
                                      <li class="job-li">
                                          <a href="apply.php?job=<?=$jobs['id'];?>">
                                            <?php echo 'Company : ' . $jobs['employer']; ?></a><br>
                                            <?php echo 'Job Title : ' .  $jobs['title']; ?>
                                      </li>
                                  </ul>
                          <?php
                              }
                          ?>
                       </div>
                   </div>
               </div>

               <div class="panel panel-default">
                   <div class="panel-heading panel-clr">
                     <div class="panel-title">
                         <a href="#sixth" data-toggle="collapse" data-parent="#accordion"><i class="glyphicon glyphicon-plus"></i>Oil &amp; Gas</a>
                     </div>
                   </div>
                   <div class="panel-collapse collapse" id="sixth">
                       <div class="panel-body">
                         <?php
                              $jobs = getJobs('Oil & Gas');
                              foreach ($jobs as $key => $jobs) { ?>
                                  <ul>
                                      <li class="job-li">
                                          <a href="apply.php?job=<?=$jobs['id'];?>">
                                            <?php echo 'Company : ' . $jobs['employer']; ?></a><br>
                                            <?php echo 'Job Title : ' .  $jobs['title']; ?>
                                      </li>
                                  </ul>
                          <?php
                              }
                          ?>
                       </div>
                   </div>
               </div>

               <div class="panel panel-default">
                   <div class="panel-heading panel-clr">
                     <div class="panel-title">
                         <a href="#seven" data-toggle="collapse" data-parent="#accordion"><i class="glyphicon glyphicon-plus"></i>Real Estate / Property</a>
                     </div>
                   </div>
                   <div class="panel-collapse collapse" id="seven">
                       <div class="panel-body">
                         <?php
                              $jobs = getJobs('Real Estate / Property');
                              foreach ($jobs as $key => $jobs) { ?>
                                  <ul>
                                      <li class="job-li">
                                          <a href="apply.php?job=<?=$jobs['id'];?>">
                                            <?php echo 'Company : ' . $jobs['employer']; ?></a><br>
                                            <?php echo 'Job Title : ' .  $jobs['title']; ?>
                                      </li>
                                  </ul>
                          <?php
                              }
                          ?>
                          
                       </div>
                   </div>
               </div>

           </div>

         </div>
         
     </div>
    
 </div>
      <section>
      <?php
              require('include/layout/about.php');
          ?>
      </section>
<section class="career-tips text-center">
        <div style="margin-right:0px;margin-left:0px;" class="rw"> <!--class="row"-->
            <h2>GET CAREER TIPS FROM EXPERTS</h2>

            <hr>

              <div class="col-sm-3 first-tip">
                    <h4 class="career-title">How to be the best employee</h4>
                        <img class="blog-img img-responsive img-thumbnail center-block" src="img/1.jpg" alt="" />
                        <p>
                            Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.<br><a class="" href="#"><br>read more</a>
                        </p>

              </div>

              <div class="col-sm-3 second-tip">
                  <h4 class="career-title">Best way to build your career</h4>
                      <img class="blog-img img-responsive img-thumbnail center-block" src="img/keyboard.jpg" alt="" />
                      <p>
                          Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.<br><a class="" href="#"><br>read more</a>
                      </p>

              </div>



              <!-- <hr> -->

              <div class="col-sm-3 third-tip">
                        <h4 class="career-title">Become effectively productive</h4>
                        <img class="blog-img img-responsive img-thumbnail center-block" src="img/man.jpg" alt="" />
                        <p>
                            Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.<br><a class="" href="#"><br>read more</a>
                        </p>

              </div>


              <div class="col-sm-3 third-tip">
                    <h4 class="career-title">Knowing the right job</h4>
                        <img class="blog-img img-responsive img-thumbnail center-block" src="img/job.jpg" alt="" />
                <p>
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.<br><a class="" href="#"><br>read more</a>
                </p>

              </div>


        </div>
</section>
<!-- Carreer tips ends -->

<!-- Top jobs starts -->
<!-- <section class="top-jobs">

    <div class="container">
        <h2 class="text-center">TOP JOBS</h2>
        <div class="row">
            <div id="blueimp-gallery" class="blueimp-gallery blueimp-gallery-controls">
            <div class="slides"></div>
                <h3 class="title"></h3>
                <a class="prev">‹</a>
                <a class="next">›</a>
                <a class="close">×</a>
                <a class="play-pause"></a>
                <ol class="indicator"></ol>
            </div>

            <div id="links">
                <div class="row">
                    <a href="img/gtb.jpg" title="Job Title: " class="jobs-logo col-md-3">
                        <img class="img-logo img-responsive center-block" src="img/gtb.jpg" alt="" />
                    </a>
                    <a href="img/chevron.jpg" title="Bank Manager" class="jobs-logo col-md-3">
                        <img class="img-logo img-responsive center-block"src="img/chevron.jpg" alt="" />
                    </a>
                    <a href="img/coke.jpg" title="Job Title: Sales Representative" class="jobs-logo col-md-3">
                        <img class="img-logo img-responsive center-block"src="img/coke.jpg" alt="" />
                    </a>
                    <a href="img/microsoft.jpg" title="job title" class="jobs-logo col-md-3">
                        <img class="img-logo img-responsive center-block"src="img/microsoft.jpg" alt="" />
                    </a>
                </div>
                <div class="row">
                    <a href="img/first.png" title="job title" class="jobs-logo col-md-3">
                        <img class="img-logo img-responsive center-block" src="img/first.png" alt="" />
                    </a>
                    <a href="img/uber.png" title="Job Title: Chief Operation Officer" class="jobs-logo col-md-3">
                        <img class="img-logo img-responsive center-block"src="img/uber.png" alt="" />
                    </a>
                    <a href="img/samsung.jpg" title="Banana" class="jobs-logo col-md-3">
                        <img class="img-logo img-responsive center-block"src="img/samsung.jpg" alt="" />
                    </a>
                    <a href="img/toyota.jpg" title="Banana" class="jobs-logo col-md-3">
                        <img class="img-logo img-responsive center-block"src="img/toyota.jpg" alt="" />
                    </a>
                </div>
            </div>
            
        </div>
    </div>

</section> -->
<!-- Top jobs ends -->

<!-- testimonal starts -->
<section class="testimonal">
  <div class="inner">
    <h2 class="text-center">TESTIMONIAL</h2>

    <div class="row">
      <div class="col-md-4 testimonal-col">
        <blockquote>
          <p>"I never taught i could get a job so easily without moving a leg. I was shocked to have received a job interview invitation few weeks after my application"</p>
          <cite>
            <a href="https://twitter.com/tim_kos">K. Mukaba Jean-louis </a> - Congo, Kinshasa. <br>
            <img src="img/Raoul.jpg" class="img-responsive img-thumbnail testimonal-img pull-right">
          </cite>
        </blockquote>
      </div>

      <div class="col-md-4 testimonal-col">
        <blockquote>
          <p>"It's a huge privilege to have found this website. I recently got a job applying via smashjob.ng . I am grateful for the good efforts."</p>
          <cite>
            <a href="https://twitter.com/tim_kos">K. Mukaba Jean-louis</a> - Us, NewYork. <br>
            <img src="img/Raoul.jpg" class="img-responsive img-thumbnail testimonal-img pull-right">
          </cite>
        </blockquote>
      </div>

      <div class="col-md-4 testimonal-col">
        <blockquote>
          <p>"I never taught i could get a job so easily without moving a leg. I was shocked to have received a job interview invitation few weeks after my application"</p>
          <cite>
            <a href="https://twitter.com/tim_kos">K. Mukaba Jean-louis </a> - Kigali, Rwanda. <br>
            <img src="img/Raoul.jpg" class="img-responsive img-thumbnail testimonal-img pull-right">
          </cite>
        </blockquote>
      </div>
    </div>

  </div>
</section>
<!-- testimonal ends -->


 <?php
       require('include/layout/page-header-footer.php');
       require('include/layout/footer2.php');
  ?>
