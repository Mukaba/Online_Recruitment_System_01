<?php

function logged_in () {
    return (isset($_SESSION['id'])) ? true : false;
}

function getAllDetails($username) {
    require('include/database/pdo_connect.php');
    $query = $pdo->prepare("SELECT * from jobseeker inner join education on jobseeker.username=education.jobseeker_username join certification on jobseeker.username=certification.jobseeker_username where jobseeker.username=:username");
    $query->bindParam(':username' , $username);
    $query->execute();
    $data = $query->fetchAll(PDO::FETCH_ASSOC);
    $data = $data[0];
    return $data;

}

function fetchJobseekerCert($username) {
    require('include/database/pdo_connect.php');
    $query = $pdo->prepare('SELECT certification.id, certification.title, certification.institution , certification.year  from jobseeker inner join certification on jobseeker.username=certification.jobseeker_username where jobseeker.username=:username order by year asc');
    $query->bindParam(':username' , $username);
    $query->bindColumn('title' , $title);
    $query->bindColumn('institution' , $institution);
    $query->bindColumn('year' , $year);
    $query->bindColumn('id' , $id);
    $query->execute();
    $resource = $query->fetchAll(PDO::FETCH_ASSOC);
    return $resource;
}

function fetchJobseekerEducation($username) {
    require('include/database/pdo_connect.php');
    $query = $pdo->prepare('SELECT education.id, education.institution, education.degree , education.field_of_study, education.year_of_graduation from jobseeker inner join education on jobseeker.username=education.jobseeker_username where jobseeker.username=:username order by year_of_graduation asc');
    $query->bindParam(':username' , $username);
    $query->bindColumn('id' , $id);
    $query->bindColumn('degree' , $degree);
    $query->bindColumn('field_of_study' , $field_of_study);
    $query->bindColumn('institution' , $university);
    $query->bindColumn('year_of_graduation' , $year_of_graduation);
    $query->execute();
    $resource = $query->fetchAll(PDO::FETCH_ASSOC);
    return $resource;
}

// jobseeker_logged_in_redirect();

if (isset($_GET['username'])) {

  $username = $_GET['username'];

  $Details = getAllDetails($username);

} else {

    header('location : index.php');

}


 ?>
