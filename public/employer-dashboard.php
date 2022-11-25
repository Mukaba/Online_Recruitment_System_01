<?php
      session_start();
      $title = "Employer";
      require('../include/layout/output_buffer.php');
      require('../include/database/pdo_connect.php');
      require_once('../include/functions/employer.php');
      function logged_in () {
          return (isset($_SESSION['id'])) ? true : false;
      }
      require('../include/layout/variables.php');
      require('../include/layout/employerheader.php');
 ?>

<div class="jumbotron hero-holder">
    <div class="jumbo-content">
      <div class="p-content text-center">
          <h2 id="p1">Let's help us find the right man for the job</h2>
          <div class="button">
            <a href="postjob.php" class="advertise-job1"><button type="button" class="btn-hide" name="button">Advertise a job</button></a>
            <a href="postjob.php" class="advertise-job2"><button type="button" class="btn-hide" name="button">Find Jobseeker</button></a>
          </div>
      </div>
    </div>
</div>

<section class="main-content">
    <div class="row first-row">
          <div class="col-sm-6">
              <p class="text-justify">
                  Lorem ipsum dolor sit amet, consectetur adipiscing elit. Non est ista, inquam, Piso, magna dissensio. Illi enim inter se dissentiunt. Duo Reges: constructio interrete. Quae in controversiam veniunt, de iis, si placet, disseramus. Hoc etsi multimodis reprehendi potest, tamen accipio, quod dant. Morbo gravissimo affectus, exul, orbus, egens, torqueatur eculeo: quem hunc appellas, Zeno? Lorem ipsum dolor sit amet, consectetur adipiscing elit. Non est ista, inquam, Piso, magna dissensio. Illi enim inter se dissentiunt. Duo Reges: constructio interrete. Quae in controversiam veniunt, de iis, si placet, disseramus. Hoc etsi multimodis reprehendi potest, tamen accipio, quod dant. Morbo gravissimo affectus, exul, orbus, egens, torqueatur eculeo: quem hunc appellas, Zeno?
              </p>
              <button class="btn btn-primary" type="submit" name="button">Post a job</button>
          </div>
          <div class="col-sm-6">
              <img class="img-responsive block-img" src="img/portfolio.jpg" alt="" />
          </div>
    </div>

    <div class="row second-row">
          <div class="col-sm-6">
              <img class="img-responsive block-img" src="img/bg.jpg" alt="" />
          </div>
          <div class="col-sm-6">
              <p class="text-justify">
                  Lorem ipsum dolor sit amet, consectetur adipiscing elit. Non est ista, inquam, Piso, magna dissensio. Illi enim inter se dissentiunt. Duo Reges: constructio interrete. Quae in controversiam veniunt, de iis, si placet, disseramus. Hoc etsi multimodis reprehendi potest, tamen accipio, quod dant. Morbo gravissimo affectus, exul, orbus, egens, torqueatur eculeo: quem hunc appellas, Zeno? Lorem ipsum dolor sit amet, consectetur adipiscing elit. Non est ista, inquam, Piso, magna dissensio. Illi enim inter se dissentiunt. Duo Reges: constructio interrete. Quae in controversiam veniunt, de iis, si placet, disseramus. Hoc etsi multimodis reprehendi potest, tamen accipio, quod dant. Morbo gravissimo affectus, exul, orbus, egens, torqueatur eculeo: quem hunc appellas, Zeno?
              </p>
              <button class="btn btn-primary" type="submit" name="button">Post a job</button>
          </div>
    </div>

    <div class="row third-row">
          <div class="col-sm-6">
              <p class="text-justify">
                  Lorem ipsum dolor sit amet, consectetur adipiscing elit. Non est ista, inquam, Piso, magna dissensio. Illi enim inter se dissentiunt. Duo Reges: constructio interrete. Quae in controversiam veniunt, de iis, si placet, disseramus. Hoc etsi multimodis reprehendi potest, tamen accipio, quod dant. Morbo gravissimo affectus, exul, orbus, egens, torqueatur eculeo: quem hunc appellas, Zeno? Lorem ipsum dolor sit amet, consectetur adipiscing elit. Non est ista, inquam, Piso, magna dissensio. Illi enim inter se dissentiunt. Duo Reges: constructio interrete. Quae in controversiam veniunt, de iis, si placet, disseramus. Hoc etsi multimodis reprehendi potest, tamen accipio, quod dant. Morbo gravissimo affectus, exul, orbus, egens, torqueatur eculeo: quem hunc appellas, Zeno?
              </p>
              <button class="btn btn-primary" type="submit" name="button">Post a job</button>
          </div>
          <div class="col-sm-6">
              <img class="img-responsive block-img" src="img/hire-bg.jpg" alt="" />
          </div>
    </div>

    <div class="row fourth-row">
          <div class="col-sm-6">
              <img class="img-responsive block-img" src="img/counter-bg.jpg" alt="" />
          </div>
          <div class="col-sm-6">
              <p class="text-justify">
                  Lorem ipsum dolor sit amet, consectetur adipiscing elit. Non est ista, inquam, Piso, magna dissensio. Illi enim inter se dissentiunt. Duo Reges: constructio interrete. Quae in controversiam veniunt, de iis, si placet, disseramus. Hoc etsi multimodis reprehendi potest, tamen accipio, quod dant. Morbo gravissimo affectus, exul, orbus, egens, torqueatur eculeo: quem hunc appellas, Zeno? Lorem ipsum dolor sit amet, consectetur adipiscing elit. Non est ista, inquam, Piso, magna dissensio. Illi enim inter se dissentiunt. Duo Reges: constructio interrete. Quae in controversiam veniunt, de iis, si placet, disseramus. Hoc etsi multimodis reprehendi potest, tamen accipio, quod dant. Morbo gravissimo affectus, exul, orbus, egens, torqueatur eculeo: quem hunc appellas, Zeno?
              </p>
              <button class="btn btn-primary" type="submit" name="button">Post a job</button>
          </div>
    </div>
</section>

 <?php
    require('../include/layout/page-header-footer.php');
    require('../include/layout/footer.php');
 ?>
