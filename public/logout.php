<?php
      require_once('../include/functions/main.php');
      session_start();
      session_destroy();
      redirect('index.php');
 ?>
