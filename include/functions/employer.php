<?php

function registerEmployer($company_name, $company_email, $company_website, $company_password, $company_mobile, $company_address, $company_size, $industry_type, $company_about, $email_code) {
    require('include/database/pdo_connect.php');;
    $connection = $pdo->prepare('INSERT into employer (name, email, website, password, mobile, address, size, industry_type, about, email_code) values (:name, :email, :website, :password, :mobile, :address, :size, :industry_type, :about, :email_code)');
    $connection->bindParam(':name' , $company_name);
    $connection->bindParam(':email' , $company_email);
    $connection->bindParam(':website' , $company_website);
    $connection->bindParam(':password' , $company_password);
    $connection->bindParam(':mobile' , $company_mobile);
    $connection->bindParam(':address' , $company_address);
    $connection->bindParam(':size' , $company_size);
    $connection->bindParam(':industry_type' , $industry_type);
    $connection->bindParam(':about' , $company_about);
    $connection->bindParam(':email_code' , $email_code);
    if ($connection->execute()) {
        return true;
    }else {
        return false;
    }
}

if (logged_in()) {

    $session_employer_id = $_SESSION['id'];
    $email = $_SESSION['company_email'];
    $employer_data = employer_data($session_employer_id, $email, 'name' , 'website' , 'size' , 'email', 'industry_type' , 'about' , 'mobile');

}

function uploadLogo($file_temp, $file_ext) {
    require('include/database/pdo_connect.php');
    $file_path = "img/logo/" . substr(md5(time()), 0 , 10) . "." . $file_ext;
    move_uploaded_file($file_temp,$file_path);
    $query = $pdo->prepare('INSERT into employer (logo) values (:logo)');
    $query->bindParam(':logo' , $file_path);
    if ($query->execute()) {
        return true;
    }else {
        return false;
    }
}

function employer_data($employer_id, $email) {
      require('include/database/pdo_connect.php');
      $employer_id = (int)$employer_id;
      $email = htmlentities(strip_tags(trim($email)));

      $func_num_args = func_num_args();
      $func_get_args = func_get_args();


      if ($func_num_args > 1) {
          unset($func_get_args[0]);
          unset($func_get_args[1]);
      }

      $fields =  implode(", " , $func_get_args) ;

      $query = $pdo->prepare("SELECT $fields from employer  where email=:email");
      $query->bindParam(':email' , $email);
      $query->execute();
      $data = $query->fetchAll(PDO::FETCH_ASSOC);
      $data = $data[0];
      return $data;
  }

  function getEmployerJobPost($employer) {
      require('include/database/pdo_connect.php');
      $query = $pdo->prepare('SELECT * FROM jobs WHERE employer = :employer');
      $query->bindParam(':employer' , $employer);
      $query->execute();
      $jobs = $query->fetchAll(PDO::FETCH_ASSOC);
      return $jobs;
  }

  function getApplicationList($job_id) {
      require('include/database/pdo_connect.php');
      $query = $pdo->prepare('SELECT * FROM application WHERE job_id = :job_id');
      $query->bindParam(':job_id' , $job_id);
      $query->execute();
      $jobs = $query->fetchAll(PDO::FETCH_ASSOC);
      return $jobs;
  }

  function getJobseekerDetails($jobseeker_id) {
      require('include/database/pdo_connect.php');
      $query = $pdo->prepare('SELECT * FROM jobseeker WHERE id = :jobseeker_id');
      $query->bindParam(':jobseeker_id' , $jobseeker_id);
      $query->execute();
      $jobs = $query->fetchAll(PDO::FETCH_ASSOC);
      return $jobs;
  }

  function getJobDetails ($id) {
      require('include/database/pdo_connect.php');
      $query = $pdo->prepare('SELECT * FROM jobs WHERE id = :id');
      $query->bindParam(':id' , $id);
          if ($query->execute()) {
            $jobs = $query->fetchAll(PDO::FETCH_ASSOC);
            return $jobs;
          }
  }





 ?>
