<?php
    require_once('include/functions/main.php');

    $state = htmlentities(strip_tags(trim($_GET['state'])));
    $remove = htmlentities(strip_tags(trim($_GET['remove'])));
    $user = $user_data['username'];

    if (isset($_GET['state']) && !empty($_GET['state']) && isset($_GET['remove']) && !empty($_GET['remove'])) {

        if ($state == "education") {

          $query = $pdo->prepare('DELETE  FROM education WHERE jobseeker_username = :username AND id = :id');
          $query->bindParam(':username' , $user);
          $query->bindParam(':id' , $remove);
          if ($query->execute()) {
              redirect('profile.php');
          }
          else {
              redirect('jobseeker-dashboard.php');
          }

        }elseif ($state == "certification") {

          $query = $pdo->prepare('DELETE  FROM certification WHERE jobseeker_username = :username AND id = :id');
          $query->bindParam(':username' , $user);
          $query->bindParam(':id' , $remove);
          if ($query->execute()) {
              redirect('profile.php');
          }
          else {
              redirect('jobseeker-dashboard.php');
          }

        }

    }

 ?>
