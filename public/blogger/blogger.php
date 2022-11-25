<?php
      $title = "Blogger";
      require_once('../include/functions/main.php');
      require('../include/layout/header.php');
 ?>



      <div class="container-fluid">

          <div class="row">

            <?php   require('../include/layout/profile-header.php'); ?>

                <div style="padding:40px" class="container">

                    <div class="row">

                      <section class="write-blog col-md-7">

                          <form class="" action="" method="post">
                              <div class="form-group">
                                <label for="blog-title">Title:</label>
                                <input type="text" name="blog-title" value="" class="form-control">

                                <label for="">Body</label>
                                <textarea name="name" rows="20" cols="100" class="form-control"></textarea>

                                <input type="file" name="name" value="">

                                <button type="submit" class="btn btn-primary btn-block" name="button">Post</button>
                              </div>
                          </form>

                      </section>

                      <aside class="edit-blog col-md-5">
                          <div class="">
                              Blogs by you
                          </div>
                      </aside>

                    </div>

                </div>

          </div>


      </div>



 <?php
       require('../include/layout/page-header-footer.php');
       require('../include/layout/footer.php');
  ?>
