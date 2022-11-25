<?php

function newquestion ($employer, $employer_email, $description, $type, $experience, $date_posted, $deadline, $title, $qualification, $gender, $category) {
    require('include/database/pdo_connect.php');
    $query = $pdo->prepare('INSERT INTO jobs (employer, employer_email, description, type, experience, date_posted, deadline, title, qualification, gender, category) VALUES (:employer, :employer_email, :description, :type, :experience, :date_posted, :deadline, :title, :qualification, :gender, :category)');
    $query->bindParam(':employer' , $employer);
    $query->bindParam(':employer_email' , $employer_email);
    $query->bindParam(':description' , $description);
    $query->bindParam(':type' , $type);
    $query->bindParam(':experience' , $experience);
    $query->bindParam(':date_posted' , $date_posted);
    $query->bindParam(':deadline' , $deadline);
    $query->bindParam(':title' , $title);
    $query->bindParam(':qualification' , $qualification);
    $query->bindParam(':gender' , $gender);
    $query->bindParam(':category' , $category);
    if ($query->execute()) {
        return true;
    }else {
        return false;
    }
}

function redirect($location) {
    header("Location: " . $location);
    exit;
}

function getEmployerJob ($id) {
    require('include/database/pdo_connect.php');
    $query = $pdo->prepare('SELECT employer.name, employer.email, employer.website, employer.address, employer.about, employer.industry_type from employer inner join application on employer.id = application.employer_id where job.id = :id');
    $query->bindParam(':id' , $id);
    $query->bindColumn('name' , $employer_name);
    $query->bindColumn('website' , $employer_website);
    $query->bindColumn('address' , $employer_address);
    $query->bindColumn('about' , $employer_about);
    $query->bindColumn('industry_type' , $employer_industry_type);
    $query->execute();
    $resource = $query->fetchAll(PDO::FETCH_ASSOC);
    return $resource;
}

?>
