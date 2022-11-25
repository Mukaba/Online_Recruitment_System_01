<?php
    require_once('../include/functions/main.php');
    if (isset($_GET['remove'])) {
        $certification_id = $_GET['remove'];
        $user = $user_data['username'];

        $query = $pdo->prepare('DELETE * FROM certification WHERE jobseeker_username = :username AND id = :id');
        $query->bindParam(':username' , $user);
        $query->bindParam(':id' , $certification_id);
        if ($query->execute()) {
            redirect('profile.php');
        }
        else {
            redirect('jobseeker-dashboard.php');
        }
    }

 ?>
