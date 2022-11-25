<?php

function postJob (
    $employer, $employer_email, $description, $type, $experience, $date_posted, $deadline, $title, $qualification, $gender, $category,
    $question1,$question1_opt_0,$question1_opt_1,$question1_opt_2,$question1_opt_3,$question1_points,$question1_valid_answer,
    $question2,$question2_opt_0,$question2_opt_1,$question2_opt_2,$question2_opt_3,$question2_points,$question2_valid_answer,
    $question3,$question3_opt_0,$question3_opt_1,$question3_opt_2,$question3_opt_3,$question3_points,$question3_valid_answer
    ) {
    require('include/database/pdo_connect.php');
    $query = $pdo->prepare('INSERT INTO jobs (employer, employer_email, description, type, experience, date_posted, deadline, title, qualification, gender, category,
    question1,question1_answer1,question1_answer2,question1_answer3,question1_answer4,question1_points,question1_answer_correct,
    question2,question2_answer1,question2_answer2,question2_answer3,question2_answer4,question2_points,question2_answer_correct,
    question3,question3_answer1,question3_answer2,question3_answer3,question3_answer4,question3_points,question3_answer_correct
    
    ) 
        VALUES (:employer, :employer_email, :description, :type, :experience, :date_posted, :deadline, :title, :qualification, :gender, :category,
    :question1,:question1_answer0,:question1_answer1,:question1_answer2,:question1_answer3,:question1_points,:question1_answer_correct,
    :question2,:question2_answer0,:question2_answer1,:question2_answer2,:question2_answer3,:question2_points,:question2_answer_correct,
    :question3,:question3_answer0,:question3_answer1,:question3_answer2,:question3_answer3,:question3_points,:question3_answer_correct
    )
        ');
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
    $query->bindParam(':question1' , $question1);
    $query->bindParam(':question1_answer0' , $question1_opt_0);
    $query->bindParam(':question1_answer1' , $question1_opt_1);
    $query->bindParam(':question1_answer2' , $question1_opt_2);
    $query->bindParam(':question1_answer3' , $question1_opt_3);
    $query->bindParam(':question1_points' , $question1_points);
    $query->bindParam(':question1_answer_correct' , $question1_valid_answer);
    $query->bindParam(':question2' , $question2);
    $query->bindParam(':question2_answer0' , $question2_opt_0);
    $query->bindParam(':question2_answer1' , $question2_opt_1);
    $query->bindParam(':question2_answer2' , $question2_opt_2);
    $query->bindParam(':question2_answer3' , $question2_opt_3);
    $query->bindParam(':question2_points' , $question2_points);
    $query->bindParam(':question2_answer_correct' , $question2_valid_answer);
    $query->bindParam(':question3' , $question3);
    $query->bindParam(':question3_answer0' , $question3_opt_0);
    $query->bindParam(':question3_answer1' , $question3_opt_1);
    $query->bindParam(':question3_answer2' , $question3_opt_2);
    $query->bindParam(':question3_answer3' , $question3_opt_3);
    $query->bindParam(':question3_points' , $question3_points);
    $query->bindParam(':question3_answer_correct' , $question1_valid_answer);
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
