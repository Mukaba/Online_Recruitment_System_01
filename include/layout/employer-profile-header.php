<?php


?>

<div class="container-fluid">
    <div style="margin-right:0px;margin-left:0px;" class="row">

      <div class="col-xs-12 col-sm-12 col-md-12 profile-top">

        <div class="dashboard-profile-display">

            <div class="col-xs-5 col-sm-3 col-md-2 img-display">

                <?php
                    if (empty($user_data['profile_image']) === false) { ?>
                        <img class="img-responsive img-thumbnail  profile-pix" src="<?=$user_data['profile_image'];?>" alt="" />
                <?php
              }elseif (empty($user_data['profile_image']) === true) { ?>
                        <img class="img-responsive img-thumbnail  profile-pix" src="img/exa.png" alt="Upload an image" style="border-radius: 10px;" />
                        <!-- <form class="" action="" method="post" enctype="multipart/form-data">
                            <input type="file" name="profile-img" value="">
                            <button type="submit" class="btn btn-primary" name="button">Submit</button>
                        </form> -->
                <?php
                    }
                ?>
                <h6>localhost/ors/<?=$employer_data['name'];?></h6>
            </div>

            <div class="dashboard-details col-xs-6 col-sm-6 col-md-8">
                <h3><?php echo strtoupper($employer_data['name']); ?></h3>
                <label id="email-label" for="">Email:</label><br>
                <h6 class="info"><?php echo strtolower($employer_data['email']); ?><br></h6>
                <label id="phone-label" for="">Phone:</label><br>
                <h6 class="info"><?php echo $employer_data['mobile']; ?></h6><br>
            </div>

        </div>



      </div>


    </div>
</div>
